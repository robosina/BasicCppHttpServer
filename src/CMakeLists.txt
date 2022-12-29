add_library(HttpServer
  STATIC
  HttpServer/http.cpp
  ${PROJECT_SOURCE_DIR}/include/HttpServer/HttpServer/http.hpp
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

