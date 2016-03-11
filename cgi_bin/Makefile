FIND_PATH=-I/home/weikai/test/doing/weikai_httpd/sql_connect  -I/home/weikai/test/doing/weikai_httpd/sql_connect/mysql_lib/include   -L/usr/lib64/mysql  -lmysqlclient

.PHONY:all
all: debug_cgi math_cgi show_student_cgi register_cgi
debug_cgi:debug_cgi.c
	gcc -o $@ $^
math_cgi:math_cgi.c
	gcc -o $@ $^
show_student_cgi:show_student_cgi.cpp /home/weikai/test/doing/weikai_httpd/sql_connect/sql_connect.cpp 
	g++ -o $@ $^ $(FIND_PATH) -g

register_cgi:register_cgi.cpp /home/weikai/test/doing/weikai_httpd/sql_connect/sql_connect.cpp 
	g++ -o $@ $^ $(FIND_PATH) -g


.PHONY:output
output:
	cp debug_cgi ../output/htdocs/cgi_bin 
	cp math_cgi ../output/htdocs/cgi_bin 
	cp show_student_cgi ../output/htdocs/cgi_bin
	cp register_cgi ../output/htdocs/cgi_bin

.PHONY:clean
clean:
	rm -rf debug_cgi math_cgi show_student_cgi register_cgi
