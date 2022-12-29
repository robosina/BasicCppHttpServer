#ifndef HTTPSERVER_CONFIG_HPP
#define HTTPSERVER_CONFIG_HPP

namespace config {
    static inline const int MAX_CONN = 10;
    static inline const int MAX_THREADS = 10;
    static inline const int MAX_BUFFER_SIZE = 1024 * 8; // 8kb
    static inline const int MAX_EVENTS = 64;
    static inline const int MAX_HEADER_SIZE = 8192;
    static inline const int MAX_URL = 4096;
    static inline const int MAX_METHOD = 8;
    static inline const int MAX_VERSION = 16;

    static inline const char *content_404 = "404 Not Found";
    static inline const char *content_500 = "500 Internal Server Error";
    static inline const char *content_200 = "Hello from Server";
} // namespace config

#endif //HTTPSERVER_CONFIG_HPP
