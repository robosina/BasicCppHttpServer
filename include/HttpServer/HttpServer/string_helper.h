#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cstring>

class StringHelper {

  public:
    [[nodiscard]] static std::vector<std::string> split(const std::string& str) ;
    static std::vector<std::string> buffer_to_vector(char* buffer) ;

};

std::vector<std::string> StringHelper::split(const std::string& str) {
  std::string buffer;
  std::stringstream ss(str);
  std::vector<std::string> result;

  while (ss >> buffer) result.push_back(buffer);

  return result;
}

std::vector<std::string> StringHelper::buffer_to_vector(char* buffer) {
  std::vector<std::string> lines;
  std::string str;
  for (int i = 0; i < std::strlen(buffer); i++) {
    if (buffer[i] == '\n') {
      lines.push_back(str);
      str = "";
    }
    else str += buffer[i];
  }
  
  if (str.length() > 0) lines.push_back(str);

  return lines;
}