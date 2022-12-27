#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <thread>
#include <strings.h>
#include <arpa/inet.h>
#include "http.hpp"
#include "response.hpp"
#include "request.hpp"

using namespace HTTP;

Server::Server(void) {}

Server::Server(bool while_loop) {
  this->while_loop = while_loop;
}

void Server::configureSocket() {
  int opt = 1;

  if ((_socket = ::socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(1);
  }

  if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(1);
  }

  _address.sin_family = AF_INET;
  _address.sin_addr.s_addr = inet_addr("127.0.0.1");
  _address.sin_port = htons(_port);

  if (bind(_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0) {
    perror("bind failed");
    exit(0);
  }

  if (::listen(_socket, 3) < 0) {
    perror("listen");
    exit(0);
  }
}

void Server::listen(int port) {
  _port = port;
  configureSocket();
  do {
    int new_socket = 0;
    struct sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);

    if ((new_socket = accept(_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) < 0) {
      perror("accept");
      exit(0);
    }

    auto connection_handler = [this](int socket) {
      // while (true) {
      char buffer[1024];
      bzero(buffer, 1024);

      if (read(socket, buffer, 1024) <= 0) {
        close(socket);
        // break;
      }

      handle(Request(buffer), Response(socket));
      // }
    };

    // std::thread th(connection_thread, new_socket); // this is not multithreading @todo multithreading
    // th.join();
    connection_handler(new_socket);
  } while (this->while_loop);
}
