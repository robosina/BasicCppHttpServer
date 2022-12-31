#pragma once

#include <functional>
#include <netinet/in.h>
#include <sys/epoll.h>
#include "HttpInterface.hpp"
#include "Config/config.hpp"
#include "HttpCommon.hpp"

namespace http {
/// @brief Http1.1 web server
class Http1_1 : public HttpInterface {

public:
  /// @brief Constructor
  Http1_1();

  /** @brief constructor
  /// @param while_loop - loop variable for while loop
  */
  [[maybe_unused]] explicit Http1_1(bool while_loop);

  /// @copydoc HttpInterface::listen
  void listen(int port) override;

  /// @brief destructor
  ~Http1_1() override = default;

  /// @brief Copy constructor
  Http1_1(const Http1_1 &) = delete;

  /// @brief Move constructor
  Http1_1(Http1_1 &&) = delete;

  /// @brief get Active server address
  [[nodiscard]] const std::string &getActiveServerAddress() const;

private:
  /// active server address when you want to use it
  std::string _activeServerAddress{};

private:
  /// @brief This is used when we want to know which address is active for benchmarking
  inline static std::string _activeServerFile{"active_server.txt"};

  /// @brief Webserver should keep running
  inline static bool while_loop = true;

  struct sockaddr_in _address; ///< address of server

  int _socket; ///< socket of server

  /// get a socket listener, this is the main socket that will listen for new connections
  [[nodiscard]] int configureListener(int port) const;

  /// set a file descriptor to non blocking
  static void setNonBlocking(int fd);

  /// worker thread that will handle the connection
  static void *worker(void *commonArgs);

  /// server
  static void server(http_status_t *status);

  /// send response
  static void sendResponse(int connfd, status_t status, const char *content, size_t content_length);

  /// write n bytes to a descriptor
  static size_t rio_writen(int fd, const char *usrbuf, size_t n);

  /// parse request
  static int parse_request(const char *req_str, request_t *req_info);

  /// handle request
  static FILE *handle_request(const request_t *req);

  /// process connections
  static void processConnections(const std::vector<struct epoll_event> &events, epoll_event &epollEvent, int epollFd,
                                 int mainListenerFd, sockaddr_in &clnt_addr, socklen_t &clnt_addr_len,
                                 int numberOfFileDescriptors);
};
} // namespace http