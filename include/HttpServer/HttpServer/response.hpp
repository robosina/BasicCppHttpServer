#pragma once

#include <vector>
#include "header.hpp"

namespace HTTP {
class Response {

public:
  explicit Response(int socket);

public:
  [[nodiscard]] std::string get_status() const;

  [[nodiscard]] std::string get_content() const;

  [[nodiscard]] std::vector<HTTP::Header> get_headers() const;

  [[nodiscard]] int _get_socket() const;

public:
  void set_status(std::string status);

  void set_content(std::string content);

  void set_headers(std::vector<HTTP::Header> headers);

public:
  [[nodiscard]] std::string to_string() const;

public:
  void send() const;

  void close() const;

private:
  int socket;

  std::string status;

  std::string content;

  std::vector<HTTP::Header> headers = {};
};
} // namespace HTTP