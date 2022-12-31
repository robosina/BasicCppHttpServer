#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <thread>
#include <strings.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "Http1_1.hpp"
#include <pthread.h>
#include <spdlog/spdlog.h>
#include <sys/epoll.h>
#include <climits>
#include <sys/sendfile.h>
#include <fstream>

using namespace http;

Http1_1::Http1_1() {}

[[maybe_unused]] Http1_1::Http1_1(bool while_loop) {
  ::Http1_1::while_loop = while_loop;
}

int Http1_1::configureListener(const int port) const {
  int opt = 1;

  struct sockaddr_in serverAddress {};

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(port);

  int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (listener < 0) {
    throw std::runtime_error("Error while opening listenfd");
  }

  if (bind(listener, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0) {
    throw std::runtime_error("Error while binding listenfd to address");
  }

  if (::listen(listener, config::MAX_CONN) < 0) {
    throw std::runtime_error("Error while listening on listenfd");
  }

  return listener;
}

void Http1_1::setNonBlocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    flags = 0;
  }
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void Http1_1::listen(int port) {
  spdlog::info("Listening on port {}", port);
  spdlog::info("For access to the server, use http://localhost:{}/index.html", port);
  _activeServerAddress = "http://localhost:" + std::to_string(port) + "/index.html";

  auto listenFd = configureListener(port);
  setNonBlocking(listenFd);

  // create a thread pool
  pthread_t threadPool[config::MAX_CONN];

  // epoll
  int epollFd = epoll_create1(0);
  if (epollFd < 0) {
    throw std::runtime_error("Error while creating epoll");
  }

  ThreadArgs commonArg{epollFd, listenFd};

  struct epoll_event epevent;
  epevent.events = EPOLLIN | EPOLLET;
  epevent.data.fd = listenFd;

  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, listenFd, &epevent) < 0) {
    throw std::runtime_error("Error while adding listenFd to epoll");
  }

  std::ofstream file;
  file.open(_activeServerFile);
  file << _activeServerAddress;
  file.close();

  for (unsigned long &i : threadPool) {
    if (pthread_create(&i, nullptr, worker, &commonArg) != 0) {
      throw std::runtime_error("Error while creating thread");
    }
  }

  worker(&commonArg);

  close(epollFd);
  close(listenFd);
  spdlog::info("Server stopped");
}

void *Http1_1::worker(void *commonArgs) {
  auto events = std::vector<struct epoll_event>(config::MAX_EVENTS);

  struct epoll_event epollEvent {};

  int epollFd = ((struct ThreadArgs *)commonArgs)->epollFd;
  int mainListenerFd = ((struct ThreadArgs *)commonArgs)->listenFd;

  struct sockaddr_in clnt_addr {};

  socklen_t clnt_addr_len = sizeof(clnt_addr);

  int numberOfFileDescriptors;
  while (while_loop) {
    numberOfFileDescriptors = epoll_wait(epollFd, (struct epoll_event *)events.data(), config::MAX_EVENTS, -1);

    if (numberOfFileDescriptors <= 0) {
      perror("epoll_wait");
      continue;
    }
    processConnections(events, epollEvent, epollFd, mainListenerFd, clnt_addr, clnt_addr_len, numberOfFileDescriptors);
  }
}

void Http1_1::processConnections(const std::vector<struct epoll_event> &events, epoll_event &epollEvent, int epollFd,
                                 int mainListenerFd, sockaddr_in &clnt_addr, socklen_t &clnt_addr_len,
                                 int numberOfFileDescriptors) {
  for (int n = 0; n < numberOfFileDescriptors; ++n) {
    auto &event = events[n];
    if (event.data.fd == mainListenerFd) {
      while (true) {
        int connfd = accept(mainListenerFd, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
        if (connfd < 0) {
          if (errno == EAGAIN | errno == EWOULDBLOCK) {
            break;
          } else {
            perror("accept");
            break;
          }
        }

        setNonBlocking(connfd);

        epollEvent.events = EPOLLIN | EPOLLET | EPOLLONESHOT;

        auto *status = (http_status_t *)malloc(sizeof(http_status_t));
        char *header = (char *)malloc(config::MAX_HEADER_SIZE);
        if (header == nullptr || status == nullptr) {
          perror("malloc");
          exit(1);
        }
        status->header = header;
        status->connfd = connfd;
        status->readn = 0;
        status->req_status = Reading;
        epollEvent.data.ptr = status;

        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, connfd, &epollEvent) < 0) {
          perror("epoll_ctl");
          continue;
        }
      }
    } else {
      auto *status = (http_status_t *)event.data.ptr;
      server(status);
      if (status->req_status == Reading) {
        epollEvent.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
        epollEvent.data.ptr = status;
        if (epoll_ctl(epollFd, EPOLL_CTL_MOD, status->connfd, &epollEvent) < 0) {
          perror("epoll_ctl");
          continue;
        }
      } else if (status->req_status == Writing) {
        epollEvent.events = EPOLLOUT | EPOLLET | EPOLLONESHOT;
        epollEvent.data.ptr = status;
        if (epoll_ctl(epollFd, EPOLL_CTL_MOD, status->connfd, &epollEvent) < 0) {
          perror("epoll_ctl");
          continue;
        }
      } else if (status->req_status == Ended) {
        if (status->file != nullptr) {
          fclose(status->file);
          status->file = nullptr;
        }
        if (status->header != nullptr) {
          free(status->header);
          status->header = nullptr;
        }
        close(status->connfd);
        free(status);
      }
    }
  }
}

void Http1_1::server(http_status_t *status) {
  int connfd = status->connfd;
  char *header = status->header;
  size_t readn = status->readn;

  if (status->req_status == Reading) {
    int is_end = 0;

    while (true) {
      ssize_t ret = read(connfd, header + readn, 1);
      if (readn >= config::MAX_HEADER_SIZE) {
        // entity too large
        status->req_status = Ended;
        sendResponse(connfd, ISE, config::content_500, sizeof(config::content_500));
        return;
      }
      if (ret < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
          // read end normally
          break;
        } else {
          perror("read");
          break;
        }
      } else if (ret == 0) {
        // EOF encountered
        break;
      } else {
        readn++;
        if (readn >= 4 && header[readn - 1] == '\n' && header[readn - 2] == '\r' && header[readn - 3] == '\n' &&
            header[readn - 4] == '\r') {
          is_end = 1;
          break;
        }
      }
    }

    if (!is_end) {
      status->readn = readn;
      return;
    }

    request_t req_info;
    if (parse_request(header, &req_info) < 0) {
      sendResponse(connfd, ISE, config::content_500, strlen(config::content_500));
      status->req_status = Ended;
      return;
    }

    req_info.connfd = connfd;
    FILE *file = handle_request(&req_info);

    fseek(file, 0L, SEEK_END);
    __off_t file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    if (file != nullptr) {
      status->file = file;
      status->left = file_size;
      status->req_status = Writing;
    } else {
      status->req_status = Ended;
      return;
    }
  }

  std::string statusStr{"200 OK"};
  std::string content{"Hello"};
  if (status->req_status == Writing) {
    std::string response = "HTTP/1.1 " + statusStr + "\n";
    std::string finalContent{};
    //    response += "\n" + content;
    // append File * file to response
    const int size = 255;
    char str[size];
    while (fgets(str, size, status->file) != nullptr) {
      finalContent += str;
    }
    response += "Content-Length: " + std::to_string(finalContent.size()) + "\n\n";
    response += finalContent;
    ::send(connfd, response.c_str(), response.size(), 0);
    status->req_status = Ended;
  }
}

void Http1_1::sendResponse(int connfd, status_t status, const char *content, size_t content_length) {
  char buf[128];

  if (status == NF) {
    sprintf(buf, "HTTP/1.0 404 Not Found\r\n");
  } else if (status == OK) {
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
  } else {
    sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\n");
  }

  sprintf(buf, "%sContent-Length: %lu\r\n\r\n", buf, content_length);

  size_t buf_len = strlen(buf);

  if (rio_writen(connfd, buf, buf_len) < buf_len) {
    spdlog::error("error while sending response");
    return;
  }
  if (rio_writen(connfd, content, content_length) < content_length) {
    fprintf(stderr, "error while sending response\n");
  }
}

size_t Http1_1::rio_writen(int fd, const char *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nwritten;
  const char *bufp = usrbuf;

  while (nleft > 0) {
    if ((nwritten = write(fd, bufp, nleft)) <= 0) {
      return 0;
    }
    nleft -= nwritten;
    bufp += nwritten;
  }

  return n;
}

int Http1_1::parse_request(const char *req_str, request_t *req_info) {
  auto scanfResult = sscanf(req_str, "%s %s %[^\r\n]", req_info->method, req_info->uri, req_info->version);
  auto isMalformed = scanfResult != 3;
  if (isMalformed) {
    spdlog::info("{} \n {} \n {}", req_str, scanfResult, isMalformed);
    spdlog::error("malformed http request, can't parse request");
    spdlog::error("method:{}", req_info->method);
    spdlog::error("uri:{}", req_info->uri);
    spdlog::error("version:{}", req_info->version);
    return -1;
  }
  return 0;
}

FILE *Http1_1::handle_request(const request_t *req) {
  if (strncmp(req->method, "GET", 3) != 0) {
    fprintf(stderr, "only support `GET` method\n");
    return nullptr;
  }

  char *abs_path = (char *)malloc(PATH_MAX);
  if (abs_path == nullptr) {
    perror("malloc");
  }
  const char *rel_path = req->uri[0] == '/' ? req->uri + 1 : req->uri;
  char *cur_dir = (char *)malloc(PATH_MAX);
  if (cur_dir == nullptr) {
    perror("malloc");
  }

  realpath(rel_path, abs_path);
  getcwd(cur_dir, PATH_MAX);

  if (strncmp(cur_dir, abs_path, strlen(cur_dir)) != 0) {
    sendResponse(req->connfd, ISE, config::content_500, strlen(config::content_500));
    return nullptr;
  }

  FILE *file = fopen(abs_path, "rb");
  free(abs_path);
  free(cur_dir);
  if (file == nullptr) {
    if (errno == ENOENT) {
      sendResponse(req->connfd, NF, config::content_404, strlen(config::content_404));
    } else {
      sendResponse(req->connfd, ISE, config::content_500, strlen(config::content_500));
    }
    return nullptr;
  }

  return file;
}

const std::string &Http1_1::getActiveServerAddress() const {
  return _activeServerAddress;
}
