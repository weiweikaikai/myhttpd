#include "httpd.h"


int sckServer_init(const char*ip,short port, int *listenfd)
{
	int 	ret = 0;
	int mylistenfd;
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr(ip);
	
		
	mylistenfd = socket(PF_INET, SOCK_STREAM, 0);
	if (mylistenfd < 0)
	{
		ret = errno ;
		printf("func socket() err:%d \n", ret);
		return ret;
	}
		

	int on = 1;
	ret = setsockopt(mylistenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
	if (ret < 0)
	{
		ret = errno ;
		printf("func setsockopt() err:%d \n", ret);
		return ret;
	}
	

	ret = bind(mylistenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if (ret < 0)
	{
		ret = errno ;
		printf("func bind() err:%d \n", ret);
		return ret;
	}
		
	ret = listen(mylistenfd, SOMAXCONN);
	if (ret < 0)
	{
		ret = errno ;
		printf("func listen() err:%d \n", ret);
		return ret;
	}
		
	*listenfd = mylistenfd;

	return 0;
}

/**
 * accept_timeout - 带超时的accept
 * @fd: 套接字
 * @addr: 输出参数，返回对方地址
 * @wait_seconds: 等待超时秒数，如果为0表示正常模式
 * 成功（未超时）返回已连接套接字，超时返回-1并且errno = ETIMEDOUT
 */
int accept_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds)
{
	int ret=0;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	if (wait_seconds > 0)
	{
		fd_set accept_fdset;
		struct timeval timeout;
		FD_ZERO(&accept_fdset);
		FD_SET(fd, &accept_fdset);
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		do
		{
			ret = select(fd + 1, &accept_fdset, NULL, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret == -1)
			return -1;
		else if (ret == 0)
		{
			errno = ETIMEDOUT;
			return -1;
		}
	}

	//一但检测出 有select事件发生，表示对等方完成了三次握手，客户端有新连接建立
	//此时再调用accept将不会堵塞
	if (addr != NULL)
		ret = accept(fd, (struct sockaddr*)addr, &addrlen); //返回已连接套接字
	else
		ret = accept(fd, NULL, NULL);
		if (ret == -1)
		{
			ret = errno;
			printf("func accept() err:%d \n", ret);
			return ret;
		}
	return ret;
}

int sckServer_accept(int listenfd, int *connfd,  int timeout)
{
	int	ret = 0;
    struct sockaddr_in peeraddr;
	memset(&peeraddr,0,sizeof(peeraddr));
	ret = accept_timeout(listenfd, &peeraddr, (unsigned int) timeout);
	if (ret < 0)
	{
		if (ret == -1 && errno == ETIMEDOUT)
		{
			ret=-1;
			printf("func accept_timeout() timeout err:%d \n", ret);
			return ret;
		}
		else
		{
			ret = errno;
			printf("func accept_timeout() err:%d \n", ret);
			return ret;
		}
	}	
	*connfd = ret;
	printf("a client online ip= %s port= %d\n ", inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
	return 0;
}
void usage(const char *proc)
{
	printf("Usage : %s [PORT]\n", proc);
}

static void not_found(int client)
{
}
void print_debug(const char * msg)
{
#ifdef _DEBUG_
	printf("%s\n", msg);
#endif
}

static void bad_request(int client)
{
	print_debug("enter our fault...\n");
	char buf[1024];
	sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>your enter message is a bad request</p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}


void print_log(const char *fun, int line, int err_no,  const char *err_str)
{
	printf("[%s: %d] [%d] [%s]\n", fun, line, err_no, err_str);
}

void clear_header(int client)
{
	char buf[1024];
	memset(buf, '\0', sizeof(buf));
	int ret = 0;
	do{
		ret = get_line(client, buf, sizeof(buf));
	}while(ret > 0 && strcmp(buf, "\n") != 0 );
}

//return num char, success
//return <= 0, failed
int get_line(int sock, char *buf, int max_len)
{
	if( !buf || max_len < 0){
		return -1;
	}
	int i = 0;
	int n = 0;
	char c = '\0';
	while( i < max_len-1 && c != '\n' ){
		n = recv(sock, &c, 1, 0);
		if(n > 0){//success
			if( c == '\r' ){
				n = recv(sock, &c, 1,MSG_PEEK);
				if( n > 0 && c == '\n' ){//windows
					recv(sock, &c, 1, 0);//delete
				}else{
					c = '\n';
				}
			}
			buf[i++] = c;
		}else{//failed
			c = '\n';
		}
	}
	buf[i] = '\0';
	return i;
}

void echo_error_to_client(int client, int error_code)
{
	switch(error_code){
		case 400://request error
			bad_request(client);
			break;
		case 404://not found
			not_found(client);
			break;
		case 500://server error
 	        //server_error(client);
			break;
		case 503://server unavailable
           //server_unavailable(client);
			break;
		default:
          //default_error(client);
			break;
	}
}

void echo_html(int client, const char *path, unsigned int file_size)
{
	if( !path ){
		return;
	}
	int in_fd = open(path, O_RDONLY);
	if(in_fd < 0){
		print_debug("open index.html error");
		//echo_error_to_client();
		return;
	}
	print_debug("open index.html success");
	char echo_line[1024];
	memset(echo_line, '\0', sizeof(echo_line));
	strncpy(echo_line, HTTP_VERSION, strlen(HTTP_VERSION)+1);
	strcat(echo_line, " 200 OK");
	strcat(echo_line, "\r\n\r\n");
	send(client, echo_line,strlen(echo_line), 0);
	print_debug("send echo head success");
	if( sendfile(client, in_fd, NULL, file_size) < 0 ){
		print_debug("send_file error");
		//echo_error_to_client();
		close(in_fd);
		return;
	}
	print_debug("sendfile success");
	close(in_fd);
}

void exe_cgi(int sock_client, const char *path, const char *method,const char *query_string)
{
	print_debug("enter cgi\n");
	char buf[_COMM_SIZE_];
	int numchars = 0;
	int content_length = -1;
	//pipe
	int cgi_input[2] = {0, 0};
	int cgi_output[2] = {0, 0};
	//child proc
	pid_t id;

	print_debug(method);

	if(strcasecmp(method, "GET") == 0){//GET
		clear_header(sock_client);
	}else{//POST
		do{
			memset(buf, '\0', sizeof(buf));
			numchars = get_line(sock_client, buf, sizeof(buf));
			if(strncasecmp(buf, "Content-Length:", strlen("Content-Length:")) == 0){
				content_length = atoi(&buf[16]);
			}
		}while(numchars > 0 && strcmp(buf, "\n") != 0);
		if( content_length == -1 ){
			//echo_error_to_client();
			return;
		}
	}

	memset(buf, '\0', sizeof(buf));
	strcpy(buf, HTTP_VERSION);
	strcat(buf, " 200 OK\r\n\r\n");
	send(sock_client, buf, strlen(buf), 0);

	if( pipe(cgi_input) == -1 ){//pipe error
		//echo_error_to_client();
		return;
	}
	if( pipe(cgi_output) == -1 ){
		close(cgi_input[0]);
		close(cgi_input[1]);
		//echo_error_to_client();
		return;
	}

	if( (id = fork()) < 0){//fork error
		close(cgi_input[0]);
		close(cgi_input[1]);
		close(cgi_output[0]);
		close(cgi_output[1]);
		//echo_error_to_client();
		return;
	}else if( id == 0 ){//child
		char query_env[_COMM_SIZE_/10];
		char method_env[_COMM_SIZE_];
		char content_len_env[_COMM_SIZE_];
		memset(method_env, '\0', sizeof(method_env));
		memset(query_env, '\0', sizeof(query_env));
		memset(content_len_env, '\0', sizeof(content_len_env));

		close(cgi_input[1]);
		close(cgi_output[0]);
		//redir
		dup2(cgi_input[0], 0);
		dup2(cgi_output[1], 1);

		sprintf(method_env, "REQUEST_METHOD=%s", method);
		putenv(method_env);
		if(strcasecmp("GET", method) == 0){//POST
			sprintf(query_env, "QUERY_STRING=%s", query_string);
			putenv(query_env);
		}else{//POST
			sprintf(content_len_env, "CONTENT_LENGTH=%d", content_length);
			putenv(content_len_env);
		}

		execl(path, path, NULL);
		exit(1);
	}else{//father
		close(cgi_input[0]);
		close(cgi_output[1]);
		int i = 0;
		char c = '\0';
		if(strcasecmp("POST", method) == 0){
			for(; i < content_length; i++ ){
				recv(sock_client, &c, 1, 0);
				write(cgi_input[1], &c, 1);
			}
		}
		while( read(cgi_output[0], &c, 1) > 0 ){
			send(sock_client, &c, 1, 0);
		}
		close(cgi_input[1]);
		close(cgi_output[0]);

		waitpid(id, NULL, 0);
	}
}

//GET && POST
//void *accept_request(void *arg)
int accept_request(int arg)
{

	print_debug("get a new connect...\n");
	pthread_detach(pthread_self());//detach
	//int sock_client = (int)arg;
	int sock_client = arg;
	
	int  cgi = 0;
	char *query_string = NULL;
	char method[_COMM_SIZE_/10];
	char url[_COMM_SIZE_];
	char buffer[_COMM_SIZE_];
	char path[_COMM_SIZE_];
	memset(method, '\0', sizeof(method));
	memset(url, '\0', sizeof(url));
	memset(buffer, '\0', sizeof(buffer));
	memset(path, '\0', sizeof(path));


	if(get_line(sock_client, buffer, sizeof(buffer)) < 0){
		//echo_error_to_client();
		return -1;
	}

	int i = 0; 
	int j = 0;//buffer line index
	while( !isspace(buffer[j]) &&\
			i < sizeof(method)-1 &&\
			j < sizeof(buffer)){
		method[i] = buffer[j];
		i++, j++;
	}

	if( strcasecmp(method, "GET") && strcasecmp(method, "POST")){
		//echo_error_to_client();
		return NULL;
	}

	//clear space point useful url start
	while( isspace(buffer[j]) &&\
			j < sizeof(buffer)){
		j++;
	}

	//get url
	i = 0;
	while(!isspace(buffer[j]) &&\
			i < sizeof(url)-1 &&\
			j < sizeof(buffer)){
		url[i] = buffer[j];
		i++;j++;
	}

	print_debug(method);
	print_debug(url);

	if(strcasecmp(method, "POST") == 0){
		cgi = 1;
	}

	if(strcasecmp(method, "GET") == 0){
		query_string = url;
		while( *query_string != '?' && *query_string != '\0'){
			query_string++;
		}
		if( *query_string == '?' ){//url = /XXX/XXX + arg
			*query_string = '\0';
			query_string++;
			cgi = 1;
		}
	}
	sprintf(path, "htdocs%s", url);
	if(path[strlen(path)-1] == '/'){
		strcat(path, MAIN_PAGE);
	}

	print_debug(path);

	struct stat st;
	if( stat(path, &st) < 0 ){ //failed, does not exist
		print_debug("miss cgi");
		clear_header(sock_client);
		//echo_error_to_client();
	}else{//file exist!
		if(S_ISDIR(st.st_mode)){
			strcat(path, "/");
			strcat(path, MAIN_PAGE);
		}else if(st.st_mode & S_IXUSR ||\
				 st.st_mode & S_IXGRP ||\
				 st.st_mode & S_IXOTH){
			cgi = 1;
		}else{
			//do nothing
		}
		if(cgi){
			exe_cgi(sock_client, path, method, query_string);
		}else{
			clear_header(sock_client);
			print_debug("begin enter our echo_html");
			echo_html(sock_client, path, st.st_size);
		}
	}
	close(sock_client);
	return -1;
}

//if success return sock
//else exit process
int start(const char *ip,short port)
{
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == -1){
		print_log(__FUNCTION__, __LINE__, errno, strerror(errno));
		exit(1);
	}

	//reuse port
	int flag = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port   = htons(port);//host -> net
	local.sin_addr.s_addr   = inet_addr(ip);
	socklen_t len = sizeof(local);

	if(bind(listen_sock, (struct sockaddr*)&local, len) == -1){
		print_log(__FUNCTION__, __LINE__, errno, strerror(errno));
		exit(2);
	}

	if(listen(listen_sock, _BACK_LOG_) == -1){
		print_log(__FUNCTION__, __LINE__, errno, strerror(errno));
		exit(3);
	}

	return listen_sock; //sucess
}

//./httpd port
int main(int argc, char *argv[])
{
	if(argc != 3){
		usage(argv[0]);
		exit(1);
	}
	//daemon();
	int port = atoi(argv[2]);
	const char *ip = argv[1];

	int sock =0;
	int ret =sckServer_init(ip,port,&sock);//listen socket
if(ret != 0)
{
     printf("fun sckserver_init err :\n");
	 return ret;
}
	while(1){
		int wait_second=10;
		int new_sock =0;
		ret=sckServer_accept(sock, &new_sock,wait_second);
		if( ret !=0 ){//accept error
			continue;
		}
		pthread_t new_thread;
   //ret=	pthread_create(&new_thread, NULL, accept_request, (void*)new_sock);
     ret=accept_request(new_sock);
                 if(ret < 0)
                    {
                  close(new_sock);
                    break;
                     }
	 }
             close(sock);
	return 0;
}






