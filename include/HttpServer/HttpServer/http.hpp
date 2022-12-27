#pragma once

#include <functional>
#include <netinet/in.h>
#include "response.hpp"
#include "request.hpp"

namespace HTTP {
class Server {

public:
  Server();

  explicit Server(bool while_loop);

public:
  void listen(int port);

  std::function<void(Request, Response)> handle = [](const HTTP::Request& req, const HTTP::Response& res) { res.send(); };

private:
  bool while_loop = true;

  struct sockaddr_in _address;

  int _socket;

  int _port{};

  void configureSocket();
};
} // namespace HTTP