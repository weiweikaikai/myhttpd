#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include "sql_connect.h" //sql_connect.h
#include<hiredis/hiredis.h>
const std::string _remote_ip = "127.0.0.1";
const std::string _remote_user = "root";
const std::string _remote_passwd = "123456";
const std::string _remote_db   = "test";

int main()
{
	int content_length = -1;
	char method[1024];
	char query_string[1024];
	char post_data[4096];
	memset(method, '\0', sizeof(method));
	memset(query_string, '\0', sizeof(query_string));
	memset(post_data, '\0', sizeof(post_data));

	std::cout<<"<html>"<<std::endl;
	std::cout<<"<head>show student</head>"<<std::endl;
	std::cout<<"<body>"<<std::endl;
//	strcpy(method, getenv("REQUEST_METHOD"));
//	if( strcasecmp("GET", method) == 0 ){
	//	strcpy(query_string, getenv("QUERY_STRING"));
    	const std::string _host = _remote_ip;
    	const std::string _user = _remote_user;
    	const std::string _passwd = _remote_passwd;
    	const std::string _db   = _remote_db;

    	std::string _sql_data[1024][5];
    	std::string header[5];
    	int curr_row = -1;

		//sql_connecter _conn();
    	sql_connecter conn(_host, _user, _passwd, _db);
    	bool ret=conn.begin_connect();
    	conn.select_sql(header,_sql_data, curr_row);

		std::cout<<"<table border=\"1\">"<<std::endl;
		std::cout<<"<tr>"<<std::endl;
    	for(int i = 0; i<5; i++){
    		std::cout<<"<th>"<<header[i]<<"</th>"<<std::endl;
    	}
		std::cout<<"</tr>"<<std::endl;
    
    	for(int i = 0; i<curr_row; i++){
			std::cout<<"<tr>"<<std::endl;
    		for(int j=0; j<5; j++){
    			std::cout<<"<td>"<<_sql_data[i][j]<<"</td>"<<std::endl;;
    		}
			std::cout<<"</tr>"<<std::endl;
    	}
		std::cout<<"</table>"<<std::endl;
     	std::cout<<"</body>"<<std::endl;

	
       redisContext *redisconn = redisConnect("127.0.0.1",6379);
       if(redisconn != NULL && redisconn->err)
       {
	   	std::cout<<"redis connect err ["<<redisconn->err<<"] please open redis!!!!!"<<std::endl;
       return 0;
       }


       redisReply *reply = (redisReply*)redisCommand(redisconn,"set foo 1234");
        freeReplyObject(reply);

        reply = (redisReply*)redisCommand(redisconn,"get foo");
	    std::cout<<"<p>hello i am redis "<<reply->str<<"</p>"<<std::endl;
         freeReplyObject(reply);

       redisFree(redisconn);
	std::cout<<"</html>"<<std::endl;


}
