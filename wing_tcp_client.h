//
// Created by zcy on 2020/11/25.
//

#ifndef WINGS_WING_TCP_CLIENT_H
#define WINGS_WING_TCP_CLIENT_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <exception>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include "wing_tcp_server.h"

namespace wings {
    class wing_tcp_client {
    private:
        in_port_t port;
        in_addr_t addr;
        int client_fd{};
    public:
        wing_tcp_client(const std::string &addr, const int &port) {
            this->port = htons(port);
            in_addr temp{};
            inet_aton((const char *) addr.c_str(), &temp);
            this->addr = temp.s_addr;
        }

        int init() {
            try {
                // socket()
                client_fd = socket(AF_INET, SOCK_STREAM, 0);
                if (client_fd < 0) {
                    std::cerr << "Client Can't create socket.";
                    return -1;
                }
                // 客户端地址由内核分配,配置服务器地址
                sockaddr_in server_addr{};
                server_addr.sin_family = AF_INET;
                server_addr.sin_addr.s_addr = this->addr;
                server_addr.sin_port = this->port;

                // connect()
                int result = connect(client_fd, (sockaddr *) &server_addr, sizeof(server_addr));
                if (result < 0) {
                    std::cerr << "Client Can't create socket.";
                    return -1;
                }
                return 0;
            } catch (std::exception e) {
                std::cout << e.what();
                return -1;
            }
        }

        int send(std::string message) const {
            return ::write(client_fd, message.c_str(), message.size());
        }

        std::string receive() const {

            char *buf = new char[MAX_BUF];
            if (read(client_fd, buf, 1024) == -1) {
                std::cerr << "received failed!";
                return "";
            }
            std::string message(buf);
            delete[] buf;
            return message;
        }


        int close() const {
            return ::close(client_fd);
        }
    };
}


#endif //WINGS_WING_TCP_CLIENT_H
