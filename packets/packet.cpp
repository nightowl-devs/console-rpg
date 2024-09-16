#include <iostream>
#include "packet_type.h"
using namespace std;

class Packet
{

public:
    string uuid;
    string packet_sender;
    PacketType packet_type;
    string packet_content;


    string encode() {
        string encoded_text;

        encoded_text = uuid + " " + packet_sender + " " + to_string(packet_type) + " " + packet_content;
        return encoded_text;
    }

    Packet decode(string encoded_text) {
        string delimiter = " ";
        size_t pos = 0;
        string token;
        int i = 0;

        while ((pos = encoded_text.find(delimiter)) != string::npos) {
            token = encoded_text.substr(0, pos);
            switch (i) {
                case 0:
                    uuid = token;
                    break;
                case 1:
                    packet_sender = token;
                    break;
                case 2:
                    packet_type = (PacketType)stoi(token);
                    break;
                case 3:
                    packet_content = token;
                    break;
            }
            encoded_text.erase(0, pos + delimiter.length());
            i++;
        }

        return *this;
    }

    string toString() {
    return encode();

    }
};
