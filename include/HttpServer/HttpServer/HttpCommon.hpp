#ifndef HTTPSERVER_HTTPCOMMON_HPP
#define HTTPSERVER_HTTPCOMMON_HPP

#include <cstdio>
#include "Config/config.hpp"

namespace http {

typedef enum STATUS {
  OK,  // OK
  ISE, // Internal Server Error
  NF   // Not Found
} status_t;

typedef struct RequestInfo {
  int connfd; // connection file descriptor

  char uri[config::MAX_URL];
  char method[config::MAX_METHOD];
  char version[config::MAX_VERSION];
} request_t;

struct ThreadArgs {
  int epollFd;
  int listenFd;
};

typedef enum REQUEST_STATUS { Reading, Writing, Ended } req_status_t;

typedef struct HttpStatus {
  int connfd;
  char *header;
  size_t readn;
  FILE *file;
  size_t left;
  req_status_t req_status;
} http_status_t;

} // namespace http

#endif // HTTPSERVER_HTTPCOMMON_HPP
