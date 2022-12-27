#pragma once

#include <functional>
#include "response.hpp"
#include "request.hpp"

namespace HTTP {
class Server {

public:
  Server();

  explicit Server(bool while_loop);

public:
  void listen(int port) const;

  std::function<void(Request, Response)> handle = [](const HTTP::Request& req, HTTP::Response res) { res.send(); };

private:
  bool while_loop = true;
};
} // namespace HTTP