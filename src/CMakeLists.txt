add_library(HttpServer
  STATIC
  HttpServer/header.cpp
  HttpServer/http.cpp
  HttpServer/request.cpp
  HttpServer/response.cpp
  ${PROJECT_SOURCE_DIR}/include/HttpServer/HttpServer/header.hpp
  ${PROJECT_SOURCE_DIR}/include/HttpServer/HttpServer/http.hpp
  ${PROJECT_SOURCE_DIR}/include/HttpServer/HttpServer/request.hpp
  ${PROJECT_SOURCE_DIR}/include/HttpServer/HttpServer/response.hpp
  ${PROJECT_SOURCE_DIR}/include/HttpServer/HttpServer/string_helper.h
  ${PROJECT_SOURCE_DIR}/include/Config/Config/config.hpp
  )

target_include_directories(HttpServer
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include/HttpServer
  ${PROJECT_SOURCE_DIR}/include/Config
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include/HttpServer/HttpServer
  )

target_link_libraries(HttpServer
  PUBLIC
  pthread
  spdlog::spdlog
  )

