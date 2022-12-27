#include <HttpServer/http.hpp>

int main() {

  HTTP::Server server;

  server.handle = [](const HTTP::Request& req, HTTP::Response res) {
    std::vector<HTTP::Header> headers;
    headers.emplace_back("Server", "Basic HTTP Server");

    res.set_headers(headers);
    res.set_content("Hi there!");
    res.send();
    res.close();
  };

  server.listen(8080);

  return 0;
}