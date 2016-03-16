#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "sql_connect.h" //sql_connect.h

const std::string _remote_ip = "127.0.0.1";
const std::string _remote_user = "root";
const std::string _remote_passwd = "123456";
const std::string _remote_db   = "test";



void insert_message(char*data_string)
{

	    char name[64];
		char age[64];
		char school[64];
		char hobby[64];
		memset(name,'\0', sizeof(name));
		memset(age,'\0', sizeof(age));
		memset(school,'\0', sizeof(school));
 		memset(hobby,'\0', sizeof(hobby));        

//	char data_string[]="name=wk&age=12&school=qb&hobby=do";
         char *start = data_string;
		while(*start != '\0')
		{                   
			if(*start == '=' || *start =='&')
			{
				*start=' ';
			}
			++start;
		}

       sscanf(data_string,"%*s %s %*s %s %*s %s %*s %s",name,age,school,hobby);
      
	   char str[1024];
	   memset(str,'\0',sizeof(str));
    sprintf(str,"'%s','%s','%s','%s'",name,age,school,hobby);
	
	std::string insert_data =str; 
	//std::string insert_data = "'qq', 12, 'wxyz', 'read'";
   	const std::string _host = _remote_ip;
   	const std::string _user = _remote_user;
   	const std::string _passwd = _remote_passwd;
   	const std::string _db   = _remote_db;
   	sql_connecter conn(_host, _user, _passwd, _db);
   	conn.begin_connect();
 conn.insert_sql(insert_data);

}

int main()
{
	int content_length = -1;
	char method[1024];
	char query_string[1024];
	char post_data[4096];
	memset(method, '\0', sizeof(method));
	memset(query_string, '\0', sizeof(query_string));
	memset(post_data, '\0', sizeof(post_data));

	printf("<html>\n");
        std::cout<<"<html>"<<std::endl;
	std::cout<<"<head>insert success</head>"<<std::endl;
	strcpy(method, getenv("REQUEST_METHOD"));
	if( strcasecmp("GET", method) == 0 ){
		strcpy(query_string, getenv("QUERY_STRING"));
		insert_message(query_string);//data1=XXX&data2=YYY;
	}else if( strcasecmp("POST", method) == 0 ){
		content_length = atoi(getenv("CONTENT_LENGTH"));
		int i = 0; 
		for(; i < content_length; i++ ){
			read(0, &post_data[i], 1);
		}
		post_data[i] = '\0';
	std::cout<<"<p>post_data"<<post_data<<"</p>\n";
		insert_message(post_data);//data1=XXX&data2=YYY;
	}else{
		//DO NOTHING
		return 1;
	}


    std::cout<<"</body>"<<std::endl;
	std::cout<<"</html>"<<std::endl;
}
