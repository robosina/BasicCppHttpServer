# Requirements(http/1.1 server) 

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

Upon compilation, you will find a file named `index.html`, which is served by the web server,
and a file called `benchmark.sh`, which can be used to benchmark the active server,
just run it, you do not need to provide any input.

```bash
./benchmark.sh
```

# Result on my computer

```bash
$ ab -n 100000 -c 10000 http://localhost:8080/index.html
This is ApacheBench, Version 2.3 <$Revision: 1843412 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 30000 requests
Completed 60000 requests
Completed 90000 requests
Completed 120000 requests
Completed 150000 requests
Completed 180000 requests
Completed 210000 requests
Completed 240000 requests
Completed 270000 requests
Completed 300000 requests
Finished 300000 requests


Server Software:        
Server Hostname:        localhost
Server Port:            8081

Document Path:          /index.html
Document Length:        162 bytes

Concurrency Level:      10000
Time taken for tests:   18.542 seconds
Complete requests:      300000
Failed requests:        0
Total transferred:      59700000 bytes
HTML transferred:       48600000 bytes
Requests per second:    16179.17 [#/sec] (mean)
Time per request:       618.079 [ms] (mean)
Time per request:       0.062 [ms] (mean, across all concurrent requests)
Transfer rate:          3144.19 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  288 134.4    275    3486
Processing:   109  321  82.2    319     629
Waiting:        0  103  44.8    100     615
Total:        298  608 123.7    595    3693

Percentage of the requests served within a certain time (ms)
  50%    595
  66%    618
  75%    639
  80%    649
  90%    668
  95%    702
  98%    763
  99%    794
 100%   3693 (longest request)

```