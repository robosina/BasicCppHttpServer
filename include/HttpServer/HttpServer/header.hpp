#pragma once

#include <string>

namespace HTTP {

  class Header {

    public:
      Header(std::string key, std::string value);

    public:
      [[nodiscard]] std::string get_key() const;
      [[nodiscard]] std::string get_value() const;

    public:
      void set_key(const std::string &key);
      void set_value(const std::string &value);

    public:
      [[nodiscard]] std::string to_string() const;

    private:
      std::string key;
      std::string value;

  };

};