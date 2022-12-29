#include <HttpServer/http.hpp>
#include <spdlog/spdlog.h>
#include <fstream>

int main() {

  HTTP::Server server;

  int serverPort = 8080;
  //try until we find a free port
  while (true) {
    try {
      server.listen(serverPort);
      break;
    } catch (std::runtime_error &e) {
      spdlog::error("Error: {}", e.what());
      serverPort++;
    }
  }


  return 0;
}
