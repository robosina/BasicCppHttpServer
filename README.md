# Task Requirements(http/1.1 server) 

Implement a basic HTTP server that supports HTTP/1.1 in C++.
Requirements
1. Run on Linux
2. Do not use any third party networking library, i.e. use only Linux system API.
3. **Handle at least 10,000 concurrent connections.**
4. **Serve at least 100,000 requests per second on a modern personal computer.**
5. Serve a simple one or two pages website for demonstration.
6. You may want to skip some trivial features like Multipart data if time is not enough, but
   you need to state clearly what features are supported.

# main concept
Via using epoll and thread pool, we can handle 10,000 concurrent connections and serve 100,000 requests per second
on a modern personal computer, which is called c10k problem.

# benchmarking tool:AB

```bash
apt-get install -y apache2-utils
```

# usage

```bash 
ab -n 100000 -c 10000 http://localhost:8080/index.html
```

# Result on my computer

```bash
$ ab -n 100000 -c 10000 http://localhost:8080/index.html
This is ApacheBench, Version 2.3 <$Revision: 1843412 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 10000 requests
Completed 20000 requests
Completed 30000 requests
Completed 40000 requests
Completed 50000 requests
Completed 60000 requests
Completed 70000 requests
Completed 80000 requests
Completed 90000 requests
Completed 100000 requests
Finished 100000 requests


Server Software:        
Server Hostname:        localhost
Server Port:            8080

Document Path:          /index.html
Document Length:        162 bytes

Concurrency Level:      10000
Time taken for tests:   6.803 seconds
Complete requests:      100000
Failed requests:        0
Total transferred:      19900000 bytes
HTML transferred:       16200000 bytes
Requests per second:    14700.10 [#/sec] (mean)
Time per request:       680.268 [ms] (mean)
Time per request:       0.068 [ms] (mean, across all concurrent requests)
Transfer rate:          2856.76 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  323 206.8    288    1517
Processing:   144  328  93.9    327     642
Waiting:        0  106  54.8    100     565
Total:        281  651 205.3    630    1850

Percentage of the requests served within a certain time (ms)
  50%    630
  66%    645
  75%    649
  80%    664
  90%    722
  95%    763
  98%   1642
  99%   1660
 100%   1850 (longest request)
```