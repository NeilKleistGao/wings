//
// Created by zhao chenyang on 2020/11/21.
//

#ifndef WINGS_WING_TCP_H
#define WINGS_WING_TCP_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <exception>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
namespace WINGS {
    using Status = const int;
    Status Server = 0;
    Status Client = 1;
    class wing_tcp {
    private:
        int status = -1;
        in_port_t port;
        in_addr_t addr; //server addr
        in_port_t client_port;
        in_addr_t client_addr; //client addr
        int server_fd;
        int listen_fd;
        int client_fd;
    public:
        /**
         * 构造时可省略客户端地址和目的端口号（默认为0）
         * @param port
         * @param addr
         * @param t_port
         * @param t_addr
         */
        wing_tcp() = delete;
        wing_tcp(const char* addr, in_port_t port, Status status, in_port_t t_port = 0, in_addr_t t_addr = 0) : port(
                port), status(status), client_port(t_port), client_addr(t_addr) {
            in_addr temp{};
            inet_aton(addr, &temp);
            this->addr = temp.s_addr;
        }

        /**
         * @return
         * 0: 初始化成功
         * -1: 初始化失败
         */
        int init_server() {
            if (status != Server) {
                std::cerr<< "The host is not a Server.";
                return -1;
            }
            try {
                while(true) {
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
                    auto temp = bind(listen_fd, (sockaddr * ) & server_addr, sizeof(server_addr));
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
                    const int MAXBUF = 1024;
                    char buffer[MAXBUF];
                    sockaddr_in client_addr;
                    socklen_t client_addr_len = sizeof(client_addr);
                    server_fd = accept(listen_fd, (sockaddr * ) & client_addr, &client_addr_len);
                    if (server_fd > 0) {
                        std::cout << "服务器已成功连接到";
                        std::cout<< " "<<client_addr.sin_addr.s_addr <<":"<<client_addr.sin_port<<"\n";
                        pid_t fpid = fork();
                        if(fpid > 0){
                            ::close(server_fd);
                            continue;
                        }else if(fpid == 0){
                            ::close(listen_fd);
                            break;
                        }else{
                            std::cerr<<"error in fork!";
                        }
                    }
                }
                return 0;
            } catch (std::exception e) {
                std::cerr << e.what();
                return -1;
            }
        }

        /**
         * @return
         * 0: 初始化成功
         * -1: 初始化失败
         */
        int init_client() {
            if (status != Client) {
                std::cerr<< "The host is not a Client.";
                return -1;
            }
            try {
                // socket()
                client_fd = socket(AF_INET,SOCK_STREAM,0);
                if(client_fd<0){
                    std::cerr<< "Client Can't create socket.";
                    return -1;
                }
                // 客户端地址由内核分配,配置服务器地址
                sockaddr_in server_addr;
                server_addr.sin_family = AF_INET;
                server_addr.sin_addr.s_addr = this->addr;
                server_addr.sin_port = this->port;

                // connect()
                int result = connect(client_fd,(sockaddr*)&server_addr,sizeof(server_addr));
                if(result<0){
                    std::cerr<< "Client Can't create socket.";
                    return -1;
                }
                return 0;
            } catch (std::exception e) {
                std::cout << e.what();
                return -1;
            }
        }
        /**
         * 发送数据
         * @return
         */
        void send(const std::string& message) const{

            if(status==-1){
                std::cerr<<"Please init first.";
                return;
            }
            try {
                if (status == Server) {
                    write(server_fd, message.c_str(), message.size());
                } else if (status == Client) {
                    write(client_fd, message.c_str(), message.size());
                }
            } catch (std::exception e) {
                std::cout<< e.what();
            }
            std::cout<< "Send: "<<message<<"\n";
        }
        /**
         * 接收数据
         * @return
         */
        std::string receive() const{
            if(status==-1){
                std::cerr<<"Please init first.";
                return "";
            }
            char* buf = new char[1024];

            std::string message;
            try{
                if(status==Server){
                    if(read(server_fd,buf,1024)==-1){
                        std::cerr<<"Receive fail.";
                        return "";
                    }
                }
                if(status==Client){
                    if(read(client_fd,buf,1024)==-1){
                        std::cerr<<"Receive fail.";
                        return "";
                    }
                }
            } catch (std::exception e) {
                std::cout<< e.what();
            }
            message  = std::string(buf);
            delete [] buf;
            std::cout<< "Received: "<<message<<"\n";
            return message;
        }
        /**
         * 关闭socket
         * @param how
         * @return
         */
        int shutdown(int how) const{

            if(status == Server){
                return ::shutdown(server_fd,how);
            }
            if(status == Client) {
                return ::shutdown(client_fd, how);
            }
            std::cerr<<"Please init first.";
            return -1;
        }
        /**
         * 回收socket
         * @return
         */
        int close() const{
            if(status == Server){
                return ::close(server_fd);
            }
            if(status == Client) {
                return ::close(client_fd);
            }
            std::cerr<<"Please init first.";
            return -1;
        }
    };
}


#endif //WINGS_WING_TCP_H
