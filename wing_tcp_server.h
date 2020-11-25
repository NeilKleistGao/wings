//
// Created by zcy on 2020/11/25.
//

#ifndef WINGS_WING_TCP_SERVER_H
#define WINGS_WING_TCP_SERVER_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <exception>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>

namespace wings {
    static const int MAX_BUF = 1024;
    class wing_tcp_server {
    private:
        in_port_t port;
        in_addr_t addr;
        int connect_count{};
        int listen_fd{};
        int server_fd{};
    public:
        wing_tcp_server(const std::string &addr, const int &port) {
            this->port = htons(port);
            in_addr temp{};
            inet_aton((const char *) addr.c_str(), &temp);
            this->addr = temp.s_addr;
        }

        bool has_connect() const{
            return connect_count != 0;
        }
        void start(void (*op) (std::string) = nullptr) {
            if(init()<0){
                std::cerr<<"init failed!";
            }
            while (true){
                char* buf = new char[MAX_BUF];
                if(read(server_fd,buf,1024)==-1){
                    std::cerr<<"Receive fail.";
                }else{
                    std::string message(buf);
                    delete [] buf;
                    if(op== nullptr){
                        std::cout<<"received "<<message<<"\n";
                    }else {
                        op(message);
                    }
                }
            }
            return;
        }
        int send(std::string message) const{
            return ::write(server_fd,message.c_str(),message.size());
        }

        int close() const{
            return ::close(server_fd);
        }

    private:
        int init() {
            try {
                // socket()
                listen_fd = socket(AF_INET, SOCK_STREAM, 0);
                if (listen_fd < 0) {
                    std::cerr << "Server Can't create socket.";
                    return -1;
                }
                // bind()
                sockaddr_in server_addr;
                server_addr.sin_family = AF_INET;
                server_addr.sin_addr.s_addr = this->addr;
                server_addr.sin_port = this->port;
                auto temp = bind(listen_fd, (sockaddr *) &server_addr, sizeof(server_addr));
                if (temp < 0) {
                    std::cerr << "Server Bind failed.";
                    return -1;
                }
                // listen()
                if (listen(listen_fd, 5) < 0) {
                    std::cerr << "Server Listen failed.";
                    return -1;
                }
                // accept()
                sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                server_fd = accept(listen_fd, (sockaddr *) &client_addr, &client_addr_len);
                if (server_fd > 0) {
                    ++connect_count;
                    std::cout << "服务器已成功连接到";
                    std::cout << " " << client_addr.sin_addr.s_addr << ":" << client_addr.sin_port << "\n";
                }
                return 0;
            } catch (std::exception e) {
                std::cerr << e.what();
                return -1;
            }
        }
    };
}


#endif //WINGS_WING_TCP_SERVER_H
