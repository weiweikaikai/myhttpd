http_load
 
1)、简介
     这个程序非常小解压后100k，http_load以并行复用的方式运行，用以测试web服务器的吞吐量与负载。但
     是它不同于大多数压力测试工具，它可以以一个单一的进程运行，一般不会把客户机搞死。可以可以测试
     HTTPS类的网站请求。


2)、获取
     可以到http://www.acme.com/software/http_load/http_load-12mar2006.tar.gz下载。

3)、安装
     [root@localhost software]# tar -zxvf http_load-12mar2006.tar.tar #解压
     [root@localhost software]# cd http_load-12mar2006
     [root@localhost http_load-12mar2006]# mkdir -p /usr/local/man/man1  #安装时会报出没有
     前边的目录，所以建立目录/usr/local/man/man1
     [root@localhost http_load-12mar2006]# make && make install  #安装
     [root@localhost http_load-12mar2006]# whereis http_load       #查看程序安装目录
     http_load: /usr/local/bin/http_load
       
4)、使用
     命令格式：http_load  -p 并发访问进程数  -s 访问时间  需要访问的URL list文件
                 http_load  -r 每秒钟访问频率   -s总计的访问时间  需要放问的url list文件
     -p 并发访问进程数；
     -s 访问时间；
     -r 每秒钟的访问频率
     -f 总计的访问次数
   
      注意：直接跟url是不行的必须是跟的一个url的文件，文件格式是每一行有一个url，中间不要出现空行否则
      会爆 出一下错误：

   
 [root@weikailinux http_load-09Mar2016]# http_load -p 500 -s 60 url.txt    


本地环回的测试
cat url.txt
http://127.0.0.1:8080

     返回结果  
第一次测试：     
12521 fetches, 223 max parallel, 4.92075e+06 bytes, in 60.1343 seconds
393 mean bytes/connection
208.217 fetches/sec, 81829.3 bytes/sec
msecs/connect: 167.161 mean, 7098.23 max, 0.153 min
msecs/first-response: 84.1203 mean, 5608.72 max, 4.95 min
HTTP response codes:
  code 200 -- 12521
     
      
  第二次测试：   
13970 fetches, 260 max parallel, 5.48667e+06 bytes, in 60.0107 seconds
392.747 mean bytes/connection
232.792 fetches/sec, 91428.3 bytes/sec
msecs/connect: 214.462 mean, 7106.34 max, 0.165 min
msecs/first-response: 88.8 mean, 10207.1 max, 4.277 min
9 bad byte counts
HTTP response codes:
  code 200 -- 13961   
第三次测试：
14350 fetches, 242 max parallel, 5.63876e+06 bytes, in 60.0043 seconds
392.945 mean bytes/connection
239.149 fetches/sec, 93972.6 bytes/sec
msecs/connect: 187.165 mean, 7181.41 max, 0.16 min
msecs/first-response: 71.2224 mean, 19696.8 max, 3.132 min
2 bad byte counts
HTTP response codes:
  code 200 -- 14348

第四次测试：
14816 fetches, 264 max parallel, 5.81994e+06 bytes, in 60.0778 seconds
392.814 mean bytes/connection
246.614 fetches/sec, 96873.4 bytes/sec
msecs/connect: 179.753 mean, 7089.27 max, 0.15 min
msecs/first-response: 87.9884 mean, 16756.5 max, 4.844 min
7 bad byte counts
HTTP response codes:
  code 200 -- 14809




      结果含义
         12521 fetches, 223 max parallel, 4.92075e+06 bytes, in 60.1343 seconds
       说明在上面的测试中运行了12521个请求，最大的并发进程数是223，总计传输的数据是4.92075e+06bytes，
       运行的时间是60.1343 秒


         393 mean bytes/connection
        说明每一连接平均传输的数据量4.92075e+06 /393

         208.217 fetches/sec, 81829.3 bytes/sec
    
        说明每秒的响应请求为208.217，每秒传递的数据为 81829.3 bytes/sec

          msecs/connect: 167.161 mean, 7098.23 max, 0.153 min
    
        说明每连接的平均响应时间是167.161 msecs，最大的响应时间7098.23 msecs，最小的响应时间
        0.153 min


        msecs/first-response: 63.5362 mean, 81.624 max, 57.803 min

            HTTP response codes:
              code 200 -- 12521
        说明打开响应页面的类型，如果403的类型过多，那可能要注意是否系统遇到了瓶颈。

        特殊说明:
       
        一般会关注到的指标是fetches/sec、msecs/connect，它们分别对应的常用性能指标参数QPS-每秒响
       应
         
        用户数和response time，每连接响应用户时间。测试的结果主要也是看这两个值。当然仅有这两个指标
       并
       不能完成对性能的分析，我们还需要对服务器的cpu、men进行分析，才能得出结论；
