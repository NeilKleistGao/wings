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

/// @file server_socket.h

#ifndef WINGS_SERVER_SOCKET_H
#define WINGS_SERVER_SOCKET_H

#include <string>

#include "socket.h"
#include "platforms/platforms.h"

namespace wings {

class ServerSocket {
public:
    static constexpr size_t MAX_LISTEN_NUMBER = 4;

    ServerSocket(const std::string& addr, const unsigned int& port);
    ~ServerSocket();

    Socket* accept();

    void close();
private:
#ifdef PLATFORM_LINUX
    in_port_t _port;
    in_addr_t _addr;
    int _listen_socket;
#endif

    size_t _connection_count;
};

} // namespace wings

#endif //WINGS_SERVER_SOCKET_H
