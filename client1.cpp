#include "wing_tcp_client.h"
#include<iostream>
int main(){
    wings::wing_tcp_client client("127.0.0.1",14200);
    client.init();
    while(true){
        std::string message;
        std::cin>>message;
        if(message=="q"||message=="quit"){
            client.close();
            return 0;
        }
        client.send(message);
    }
}