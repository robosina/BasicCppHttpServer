#include <HttpServer/Http1_1.hpp>
#include <HttpServer/HttpInterface.hpp>
#include <spdlog/spdlog.h>
#include <fstream>

int main() {

  std::unique_ptr<http::HttpInterface> httpServer = std::make_unique<http::Http1_1>();

  int serverPort = 8080;
  // try until we find a free port
  while (true) {
    try {
      httpServer->listen(serverPort);
      break;
    } catch (std::runtime_error &e) {
      spdlog::error("Error: {}", e.what());
      serverPort++;
    }
  }

  return 0;
}
