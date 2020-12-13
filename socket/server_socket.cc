/**
 * @copybrief
 * MIT License
 * Copyright (c) 2020 chui & NeilKleistGao
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/// @file server_socket.cc

#include "server_socket.h"

#include <iostream>

namespace wings {
ServerSocket::ServerSocket(const std::string& addr, const unsigned int& port) {
#ifdef PLATFORM_LINUX
    this->_port = htons(port);
    in_addr temp{};
    inet_aton(addr.c_str(), &temp);
    this->_addr = temp.s_addr;

    this->_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_listen_socket < 0) {
        /// @todo
    }

    sockaddr_in p_addr{};
    p_addr.sin_family = AF_INET;
    p_addr.sin_addr.s_addr = this->_addr;
    p_addr.sin_port = this->_port;

    auto result = bind(_listen_socket, reinterpret_cast<sockaddr*>(&p_addr), sizeof(p_addr));
    if (result < 0) {
        /// @todo throw
    }

    if (listen(_listen_socket, MAX_LISTEN_NUMBER) < 0) {
        /// @todo throw
    }
#endif
}

ServerSocket::~ServerSocket() {
    this->close();
}

Socket* ServerSocket::accept() {
#ifdef PLATFORM_LINUX
    sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    int client_socket = ::accept(this->_listen_socket, reinterpret_cast<sockaddr*>(&client_addr), &len);
    ++_connection_count;

    return new(std::nothrow) Socket(client_socket);
#endif
}

void ServerSocket::close() {
#ifdef PLATFORM_LINUX
    ::close(this->_listen_socket);
#endif
}
} // namespace wings