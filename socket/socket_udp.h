/**
 * @copybrief
 *
 * MIT License
 * Copyright (c) 2020 chui
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

#ifndef GOBACKN_SOCKET_UDP_H
#define GOBACKN_SOCKET_UDP_H

#include "../platforms/platforms.h"
#include <string>
using std::string;
class socket_udp {
public:
    socket_udp(const string& source_addr, const unsigned int source_port,
            const string& target_addr, const unsigned int target_port);
    ~socket_udp();
    void write(const char* buff,const int& len);
    size_t read(char* buff,const int& len);
    void close();

private:
#ifdef PLATFORM_LINUX
    int _socket_id;
    in_port_t _source_port;
    in_addr_t _source_addr;
    in_port_t _target_port;
    in_addr_t _target_addr;
#endif //PLATFORM_LINUX
};


#endif //GOBACKN_SOCKET_UDP_H
