
#include "Client.h"
#include "ClientNetwork.h"
#include <iostream>

int main(int argc, char *argv[]){
    if (argc != 2) {
        std::cerr << "Usage: ./client.out <client_id>" << std::endl;
        return 1;
    }
    
    char server_ip[] = "127.0.0.1";
    std::string path = "../clientJSON/client" + std::string(argv[1]) + ".json";
    ClientNetwork net = {5555, server_ip};
    Client local_client = {net, path};
    
    local_client.setup();
    local_client.run();
    return 0;
}
