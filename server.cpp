#include "wing_tcp_server.h"

int main(){
    wings::wing_tcp_server server("127.0.0.1",14200);
    server.start();
}