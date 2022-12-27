# http/1.1 server example in C++

Task
Implement a basic HTTP server that supports HTTP/1.1 in C++.
Requirements
1. Run on Linux
2. Do not use any third party networking library, i.e. use only Linux system API.
3. Handle at least 10,000 concurrent connections.
4. Serve at least 100,000 requests per second on a modern personal computer.
5. Serve a simple one or two pages website for demonstration.
6. You may want to skip some trivial features like Multipart data if time is not enough, but
   you need to state clearly what features are supported.
# benchmarking tool:AB

```bash
apt-get install -y apache2-utils
```