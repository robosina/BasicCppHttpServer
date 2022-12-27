#include <header.hpp>
#include <utility>

using namespace HTTP;

Header::Header(std::string key, std::string value) {
  this->key = std::move(key);
  this->value = std::move(value);
}

std::string Header::get_key() const {
  return this->key;
}

std::string Header::get_value() const {
  return this->value;
}

void Header::set_key(const std::string &key) {
  this->key = key;
}

void Header::set_value(const std::string &value) {
  this->value = value;
}

std::string Header::to_string() const {
  return this->get_key() + ": " + this->get_value();
}