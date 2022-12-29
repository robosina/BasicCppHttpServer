#pragma once

#include <functional>
#include <netinet/in.h>
#include "Config/config.hpp"

namespace HTTP {

    typedef enum STATUS {
        OK,   // OK
        ISE,  // Internal Server Error
        NF    // Not Found
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

    typedef enum REQUEST_STATUS {
        Reading,
        Writing,
        Ended
    } req_status_t;

    typedef struct HttpStatus {
        int connfd;
        char *header;
        size_t readn;
        FILE *file;
        size_t left;
        req_status_t req_status;
    } http_status_t;

    class Server {

    public:
        Server();

        explicit Server(bool while_loop);

    public:
        void listen(int port);

    private:

        /// active server address when you want to use it
        std::string _activeServerAddress{};

    public:
        const std::string &getActiveServerAddress() const;

    private:

        inline static std::string _activeServerFile{"active_server.txt"};

        inline static bool while_loop = true;

        struct sockaddr_in _address;

        int _socket;

        /// get a socket listener, this is the main socket that will listen for new connections
        [[nodiscard]] int configureListener(int port) const;

        /// set a file descriptor to non blocking
        static void setNonBlocking(int fd);

        /// worker thread that will handle the connection
        static void *worker(void *arg);

        /// server
        static void server(http_status_t *status);

        /// send response
        static void sendResponse(int connfd, status_t status, const char *content, size_t content_length);

        static size_t rio_writen(int fd, const char *usrbuf, size_t n);

        static int parse_request(const char *req_str, request_t *req_info);

        static FILE *handle_request(const request_t *req);
    };
} // namespace HTTP