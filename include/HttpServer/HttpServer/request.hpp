#pragma once

#include <vector>

#include "header.hpp"

namespace HTTP {
class Request {

public:
  explicit Request(char buffer[1024]);

public:
  [[nodiscard]] std::string get_method() const;

  [[nodiscard]] std::string get_path() const;

  [[nodiscard]] std::string get_version() const;

  [[nodiscard]] std::vector<HTTP::Header> get_headers() const;

  [[nodiscard]] std::vector<std::string> get_content() const;

  [[nodiscard]] HTTP::Header get_header(const std::string& key) const;

private:
  std::string method;

  std::string path;

  std::string version;

  std::vector<HTTP::Header> headers = {};

  std::vector<std::string> content = {};
};
} // namespace HTTP