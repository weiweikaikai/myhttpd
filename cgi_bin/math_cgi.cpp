#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include"BigData.h"

void math_add(char *const data_string)//data1=XXX&data2=YYY;
{
	if(!data_string){
		return;
	}
	char *data1 = NULL;
	char *data2 = NULL;
	char *start = data_string;
	while(*start != '\0'){
		if(*start == '=' && data1 == NULL){
			data1 = start+1;
			start++;
			continue;
		}
		if(*start == '&'){
			*start = '\0';
		}
		if(*start == '=' && data1 != NULL){
			data2 = start+1;
			break;
		}
		start++;
	}
	
     BigData str1(data1);
	 BigData str2(data2);
	 BigData str3=str1+str2;
	 std::cout<<"<p>math [add] result : "<<str3<<"</p>\n";
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
	printf("<head>MATH</head>\n");
	printf("<body>\n");
	strcpy(method, getenv("REQUEST_METHOD"));
	if( strcasecmp("GET", method) == 0 ){
		strcpy(query_string, getenv("QUERY_STRING"));
		math_add(query_string);//data1=XXX&data2=YYY;
	}else if( strcasecmp("POST", method) == 0 ){
		content_length = atoi(getenv("CONTENT_LENGTH"));
		int i = 0; 
		for(; i < content_length; i++ ){
			read(0, &post_data[i], 1);
		}
		post_data[i] = '\0';
		math_add(post_data);//data1=XXX&data2=YYY;
	}else{
		//DO NOTHING
		return 1;
	}

	printf("</body>\n");
	printf("</html>\n");
}
