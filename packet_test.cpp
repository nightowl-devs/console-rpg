#include <iostream>
#include <cassert>
#include <string> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "packet.cpp"
#include <unistd.h>

using namespace std;

void testPacketEncoding() {
    Packet packet;
    packet.packet_name = "TestPacket";
    packet.packet_sender = "Sender";
    packet.packet_type = PacketType::CHAT;
    packet.packet_content = "This is a test content";

    string encoded_text = packet.encode(); 
    string expected_text = "TestPacket Sender 4 This is a test content";

    cout << encoded_text << endl;
    if (encoded_text != expected_text)
    {
        cout << "test failed!" << endl;
         cout << "Expected: " << expected_text << endl;
         cout << "Got: " << encoded_text << endl;
    } else {
        cout << "test passed!" << endl;
        }
    
}

void testPacketDecoding() {
    string encoded_text = "TestPacket Sender 0 This is a test content";
    Packet packet;
    packet = packet.decode(encoded_text);

    cout << packet.toString() << endl;

    cout << "testPacketType passed!" << endl;
}


void createSocket() {
    // Create socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        cerr << "Error creating socket" << endl;
        return;
    }
    cout << "Socket created" << endl;

    // Define server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(54000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    int bind_result = bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if (bind_result == -1) {
        cerr << "Error binding socket" << endl;
        close(server_socket);
        return;
    }
    cout << "Socket bound" << endl;

    // Listen for connections
    if (listen(server_socket, 5) == -1) {
        cerr << "Error listening on socket" << endl;
        close(server_socket);
        return;
    }
    cout << "Listening for connections..." << endl;

    // Accept client connection
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        cerr << "Error accepting client" << endl;
        close(server_socket);
        return;
    }
    cout << "Client accepted" << endl;

    // Send message to client
    // string message = "Hello client!";
    Packet packet;
    packet.packet_name = "playerChat";
    packet.packet_sender = "Server";
    packet.packet_type = PacketType::CHAT;
    packet.packet_content = "Hello client!";
    string message = packet.encode();
    
    if (send(client_socket, message.c_str(), message.size() + 1, 0) == -1) {
        cerr << "Error sending message" << endl;
    } else {
        cout << "Message sent to client" << endl;
    }

    // Close sockets
    close(client_socket);
    close(server_socket);
    cout << "Sockets closed" << endl;
}

int main() {
    testPacketEncoding();
    testPacketDecoding();
    createSocket();

    cout << "All tests passed!" << endl;
    cout << "All tests passed!" << endl;
}


//
//
//
//  
//
//