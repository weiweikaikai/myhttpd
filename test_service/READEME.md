http_load
 
1)�����
     �������ǳ�С��ѹ��100k��http_load�Բ��и��õķ�ʽ���У����Բ���web���������������븺�ء���
     ������ͬ�ڴ����ѹ�����Թ��ߣ���������һ����һ�Ľ������У�һ�㲻��ѿͻ������������Կ��Բ���
     HTTPS�����վ����


2)����ȡ
     ���Ե�http://www.acme.com/software/http_load/http_load-12mar2006.tar.gz���ء�

3)����װ
     [root@localhost software]# tar -zxvf http_load-12mar2006.tar.tar #��ѹ
     [root@localhost software]# cd http_load-12mar2006
     [root@localhost http_load-12mar2006]# mkdir -p /usr/local/man/man1  #��װʱ�ᱨ��û��
     ǰ�ߵ�Ŀ¼�����Խ���Ŀ¼/usr/local/man/man1
     [root@localhost http_load-12mar2006]# make && make install  #��װ
     [root@localhost http_load-12mar2006]# whereis http_load       #�鿴����װĿ¼
     http_load: /usr/local/bin/http_load
       
4)��ʹ��
     �����ʽ��http_load  -p �������ʽ�����  -s ����ʱ��  ��Ҫ���ʵ�URL list�ļ�
                 http_load  -r ÿ���ӷ���Ƶ��   -s�ܼƵķ���ʱ��  ��Ҫ���ʵ�url list�ļ�
     -p �������ʽ�������
     -s ����ʱ�䣻
     -r ÿ���ӵķ���Ƶ��
     -f �ܼƵķ��ʴ���
   
      ע�⣺ֱ�Ӹ�url�ǲ��еı����Ǹ���һ��url���ļ����ļ���ʽ��ÿһ����һ��url���м䲻Ҫ���ֿ��з���
      �ᱬ ��һ�´���

   
 [root@weikailinux http_load-09Mar2016]# http_load -p 500 -s 60 url.txt    


���ػ��صĲ���
cat url.txt
http://127.0.0.1:8080

     ���ؽ��  
��һ�β��ԣ�     
12521 fetches, 223 max parallel, 4.92075e+06 bytes, in 60.1343 seconds
393 mean bytes/connection
208.217 fetches/sec, 81829.3 bytes/sec
msecs/connect: 167.161 mean, 7098.23 max, 0.153 min
msecs/first-response: 84.1203 mean, 5608.72 max, 4.95 min
HTTP response codes:
  code 200 -- 12521
     
      
  �ڶ��β��ԣ�   
13970 fetches, 260 max parallel, 5.48667e+06 bytes, in 60.0107 seconds
392.747 mean bytes/connection
232.792 fetches/sec, 91428.3 bytes/sec
msecs/connect: 214.462 mean, 7106.34 max, 0.165 min
msecs/first-response: 88.8 mean, 10207.1 max, 4.277 min
9 bad byte counts
HTTP response codes:
  code 200 -- 13961   
�����β��ԣ�
14350 fetches, 242 max parallel, 5.63876e+06 bytes, in 60.0043 seconds
392.945 mean bytes/connection
239.149 fetches/sec, 93972.6 bytes/sec
msecs/connect: 187.165 mean, 7181.41 max, 0.16 min
msecs/first-response: 71.2224 mean, 19696.8 max, 3.132 min
2 bad byte counts
HTTP response codes:
  code 200 -- 14348

���Ĵβ��ԣ�
14816 fetches, 264 max parallel, 5.81994e+06 bytes, in 60.0778 seconds
392.814 mean bytes/connection
246.614 fetches/sec, 96873.4 bytes/sec
msecs/connect: 179.753 mean, 7089.27 max, 0.15 min
msecs/first-response: 87.9884 mean, 16756.5 max, 4.844 min
7 bad byte counts
HTTP response codes:
  code 200 -- 14809




      �������
         12521 fetches, 223 max parallel, 4.92075e+06 bytes, in 60.1343 seconds
       ˵��������Ĳ�����������12521���������Ĳ�����������223���ܼƴ����������4.92075e+06bytes��
       ���е�ʱ����60.1343 ��


         393 mean bytes/connection
        ˵��ÿһ����ƽ�������������4.92075e+06 /393

         208.217 fetches/sec, 81829.3 bytes/sec
    
        ˵��ÿ�����Ӧ����Ϊ208.217��ÿ�봫�ݵ�����Ϊ 81829.3 bytes/sec

          msecs/connect: 167.161 mean, 7098.23 max, 0.153 min
    
        ˵��ÿ���ӵ�ƽ����Ӧʱ����167.161 msecs��������Ӧʱ��7098.23 msecs����С����Ӧʱ��
        0.153 min


        msecs/first-response: 63.5362 mean, 81.624 max, 57.803 min

            HTTP response codes:
              code 200 -- 12521
        ˵������Ӧҳ������ͣ����403�����͹��࣬�ǿ���Ҫע���Ƿ�ϵͳ������ƿ����

        ����˵��:
       
        һ����ע����ָ����fetches/sec��msecs/connect�����Ƿֱ��Ӧ�ĳ�������ָ�����QPS-ÿ����
       Ӧ
         
        �û�����response time��ÿ������Ӧ�û�ʱ�䡣���ԵĽ����ҪҲ�ǿ�������ֵ����Ȼ����������ָ��
       ��
       ������ɶ����ܵķ��������ǻ���Ҫ�Է�������cpu��men���з��������ܵó����ۣ�
