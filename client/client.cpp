#include "../packet/Packet.h"

#include <nlohmann/json.hpp>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using json = nlohmann::json;



bool RUNNING = true;
const int port = 5555;
const int ID_LEN = 6;
static string id;
static string PATH_TO_ID = "../client/client.json";

void fetchClientInfo(int server_fd, string &id);
void receive_message(int sock_fd);
void setup(int *client_fd, string &id);
void sendGreetings(int *sock_fd, string &id);


ssize_t recv_all(int sock, void* buffer, size_t length);

int main(){

    int client_fd;
    string client_input, id;
    vector<char> buffer;

    setup(&client_fd, id);
    cout << "Wellcome Back " << id << endl;

    // Spawn thread that listens to server
    thread receiver(receive_message, client_fd);

    while (RUNNING){
        
        cout << "[You]> ";
        getline(cin, client_input);

        if (client_input == "\\kill"){
            RUNNING = false;
            break;
        }
        
        Packet p(MessageType::TEXT, 67, client_input);
        buffer = p.serialize();
        send(client_fd, buffer.data(), buffer.size(), 0);
    }

    shutdown(client_fd,SHUT_RDWR);
    receiver.join();
    close(client_fd);
    return 0;
}

/* Listens for incoming message */
void receive_message(int sock_fd){
    while (RUNNING) {
        Packet p;
        vector<char> buffer(Packet::HEADER_SIZE);

        // Receive header
        ssize_t bytes = recv_all(sock_fd, buffer.data(), Packet::HEADER_SIZE);

        if (bytes <= 0) {
            if (RUNNING) cout << "\r\33[2k" << "Disconnected from server..." << endl;
            else cout << "Terminanting the session... Cya !" << endl;
            RUNNING = false;
            break;
        }

        p.deserializeHeader(buffer);
        p.seeHeader();

        // Recieve Payload
        buffer.resize(p.getPayloadSize());
        recv_all(sock_fd, buffer.data(), p.getPayloadSize());
        p.deserializePayload(buffer);

        // Display message
        cout << "\r\33[2k" << "[Server]> ";
        p.seePayload(); 
        cout << "[You]> " << flush;
    } 
}

/* Function makes sure that full stream is collected */
ssize_t recv_all(int sock, void* buffer, size_t length){
    size_t total = 0;
    ssize_t bytes;
    char* ptr = (char*)buffer;

    while (total < length) {
        bytes = recv(sock, ptr + total, length - total, 0);
        if (bytes <= 0) return bytes;
        total += bytes;
    }
    return total;
}

/* Connects client to server address */
void setup(int *client_fd, string &id){
    
    // Conect Client
    struct sockaddr_in server_address;

    (*client_fd) = socket(AF_INET, SOCK_STREAM, 0);
    if ((*client_fd) < 0){
        perror("Failed socket Creation");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
    
    // This is where we connect client to server 
    connect((*client_fd), (const sockaddr *)&server_address, sizeof(server_address));
    fetchClientInfo(*client_fd, id);
    sendGreetings(client_fd, id);
}

/* Reads client.json and fetches saved credentials */
void fetchClientInfo(int server_fd, string &id){

    ifstream file(PATH_TO_ID);
    if (!file.is_open()){
        perror("Failed to open .json file.");
        exit(EXIT_FAILURE);
    }

    json data;
    file >> data;

    // If ID was never generated before...
    if (data["id"] == "None") {
        cout << "Requesting ID..." << endl;
        // Request ID From Server
        return;
    }

    id = data["id"];
}

void sendGreetings(int *sock_fd, string &id){
    Packet p(MessageType::GREETINGS, 11111, id);
    vector<char> buffer = p.serialize();
    send(*sock_fd, buffer.data(), buffer.size(), 0);
}