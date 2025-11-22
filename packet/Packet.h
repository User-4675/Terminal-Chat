
#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <string>
#include <cstdint>

enum class MessageType : uint8_t{
    
    /* Server side message to assign id to new client */
    IDENTIFICATION = 0,

    /* Client-to-client Text Message*/
    TEXT = 1, 

    /* Server side greetings message for new clients */
    GREETINGS = 2
};


class Packet {
private:

    /* Message type */
    MessageType type;

    /* Identifier of destination */
    uint32_t destination_id;

    /* Number of payload bytes */
    uint32_t data_length;
    
    /* Payload */
    std::string payload;

public:

    /* Defines the size of Packet Header in bytes */
    static constexpr std::size_t HEADER_SIZE = 2*sizeof(uint32_t) + sizeof(MessageType);

    /* No Args Constructor */
    Packet();

    /* All Args Constructor */
    Packet(MessageType type, uint32_t destination, std::string &payload);

    /* Peeks at packet info */
    void seeHeader() const;
    void seePayload() const;

    uint32_t getPayloadSize() const;
    MessageType getType() const;
    void copyPayload(std::string &ref) const; 


    /* Serializes packet into buffer */
    std::vector<char> serialize() const; // Const -> Does not modify the class

    /* Deserializes a header */
    void deserializeHeader(const std::vector<char> &buffer);

    /* Deserialize payload (Base on header)*/
    void deserializePayload(const std::vector<char> &buffer);
};

#endif // PACKET_H  
