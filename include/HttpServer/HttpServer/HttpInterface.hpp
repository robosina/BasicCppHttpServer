#ifndef HTTPSERVER_IHTTP_HPP
#define HTTPSERVER_IHTTP_HPP

namespace http {
/// @brief Interface for HttpServer(each other HttpServer should inherit from this interface)
class HttpInterface {
public:
  /// @brief Which port webserver will listen
  virtual void listen(int port) = 0;

  /// @brief Destructor
  virtual ~HttpInterface() = default;

  /// @brief Constructor
  HttpInterface() = default;

  /// @brief Copy constructor
  HttpInterface(const HttpInterface &) = default;

  /// @brief Move constructor
  HttpInterface(HttpInterface &&) = default;
};

} // namespace http

#endif // HTTPSERVER_IHTTP_HPP
