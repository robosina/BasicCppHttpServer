#include <HttpServer/http.hpp>
#include <spdlog/spdlog.h>
int main() {

  HTTP::Server server;

  server.handle = [](const HTTP::Request &req, HTTP::Response res) {
      std::vector<HTTP::Header> headers;
      headers.emplace_back("Server", "Basic HTTP Server");

      res.set_headers(headers);
      res.set_content("Hi there!");
      res.send();
      res.close();
  };

  int server_port = 8080;
  //try until we find a free port
  while (true) {
    try {
      server.listen(server_port);
      break;
    } catch (std::runtime_error &e) {
      spdlog::error("Error: {}", e.what());
      server_port++;
    }
  }

  return 0;
}