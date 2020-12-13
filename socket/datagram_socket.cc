/** MIT License
 * Copyright (c) 2020 chui & neilkleistgao
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
 *
 * */

#include "datagram_socket.h"

#include <iostream>

namespace wings {
DatagramSocket::DatagramSocket(const std::string &source_addr, const unsigned int source_port, const std::string &target_addr,
                                   const unsigned int target_port) {
#ifdef PLATFORM_LINUX
    this->_target_port = htons(target_port);
    this->_source_port = htons(source_port);
    in_addr temp{};
    inet_aton(target_addr.c_str(), &temp);
    this->_target_addr = temp.s_addr;

    inet_aton(source_addr.c_str(), &temp);
    this->_source_addr = temp.s_addr;
    try {
        this->_socket_id = socket(AF_INET, SOCK_DGRAM, 0);
        if (this->_socket_id < 0) {
            throw "socket create error!";
        }

        sockaddr_in s_addr{};
        s_addr.sin_family = AF_INET;
        s_addr.sin_addr.s_addr = this->_source_addr;
        s_addr.sin_port = this->_source_port;
        int result = ::bind(this->_socket_id, reinterpret_cast<sockaddr *>(&s_addr), sizeof(s_addr));
        if (result != 0) {
            throw "socket bind error!";
        }
    } catch (const char *e) {
        std::cout << *e << std::endl;
    }
#endif //PLATFORM_LINUX
}

DatagramSocket::~DatagramSocket() {
    this->close();
}

size_t DatagramSocket::read(char buff[], const int &len) {
    if (len == 0) {
        return len;
    }
#ifdef PLATFORM_LINUX
    sockaddr_in temp{};
    temp.sin_family = AF_INET;
    temp.sin_addr.s_addr = _target_addr;
    temp.sin_port = _target_port;
    socklen_t temp_len = sizeof(temp);
    auto result = ::recvfrom(_socket_id, buff, len, 0, reinterpret_cast<sockaddr *>(&temp), &temp_len);
    return result;
#endif //PLATFORM_LINUX
}

void DatagramSocket::write(const char *buff, const int &len) {
#ifdef PLATFORM_LINUX
    sockaddr_in temp{};
    temp.sin_family = AF_INET;
    temp.sin_addr.s_addr = _target_addr;
    temp.sin_port = _target_port;
    ::sendto(_socket_id, buff, len, 0, reinterpret_cast<sockaddr *>(&temp), sizeof(temp));
#endif //PLATFORM_LINUX
}

void DatagramSocket::close() {
#ifdef PLATFORM_LINUX
    ::close(this->_socket_id);
#endif //PLATFORM_LINUX
}

} // namespace wings
