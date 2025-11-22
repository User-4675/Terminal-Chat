
#include "Packet.h"

#include <iostream> 
#include <arpa/inet.h>
#include <vector>
#include <string.h>


/* Bytes size of header */
const int HEADER_SIZE = 2*sizeof(uint32_t) + sizeof(uint8_t);

// No Args Constructor
Packet::Packet() = default;

// All Args Constructor
Packet::Packet(MessageType type, uint32_t destination, std::string &payload)
    : type(type), destination_id(destination), 
    payload(payload), data_length(payload.size()){}

/* Display header information */
void Packet::seeHeader() const {
    std::cout << "Message for " << destination_id;
    std::cout << " of length " << data_length;
    std::cout << " (Type " << (int)type << ")\n";
}

/* Display Payload */
void Packet::seePayload() const{std::cout << payload << std::endl;}

/* Return message type */
MessageType Packet::getType() const{return this->type;}


/* Return payload size */
uint32_t Packet::getPayloadSize() const {return this->data_length;}

/* Copy payload into buffer */
void Packet::copyPayload(std::string &ref) const {ref = this->payload;} 


// Serializes packet to buffer
std::vector<char> Packet::serialize() const {
    std::vector<char> buffer(HEADER_SIZE + data_length);
    int offset = 0;

    auto serializeInt32 = [&](uint32_t x){
        uint32_t x_net = htonl(x);
        memcpy(buffer.data() + offset, &x_net, sizeof(x_net));
        offset += sizeof(x_net);
    };

    // Serialize Header
    memcpy(buffer.data() + offset, &type, sizeof(type));
    offset += sizeof(type);
    serializeInt32(destination_id);
    serializeInt32(data_length);
    
    // Serialize payload
    memcpy(buffer.data() + offset, payload.data(), data_length);
    offset += data_length;
    return buffer;
}

// Deserialize Header
void Packet::deserializeHeader(const std::vector<char> &buffer){
    int offset = 0;
    
    memcpy(&type, buffer.data(), sizeof(type));
    offset += sizeof(type);
    memcpy(&destination_id, buffer.data() + offset, sizeof(destination_id));
    offset += sizeof(destination_id);
    memcpy(&data_length, buffer.data() + offset, sizeof(data_length));

    // Convert from network byt order to host
    destination_id = ntohl(destination_id);
    data_length = ntohl(data_length);
}   

void Packet::deserializePayload(const std::vector<char> &buffer){
    payload.resize(data_length); // Allocate enought memory for payload
    memcpy(payload.data(), buffer.data(), data_length);
}

