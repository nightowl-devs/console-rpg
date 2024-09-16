#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

void testConnection() {
    // Create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        cerr << "Error creating socket" << endl;
        return;
    }

    // Define server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        cerr << "Error connecting to server" << endl;
        close(client_socket);
        return;
    }

    // Receive message from server
    char buffer[4096];
    memset(buffer, 0, 4096);
    int bytes_received = recv(client_socket, buffer, 4096, 0);
    if (bytes_received == -1) {
        cerr << "Error receiving message" << endl;
    } else {
        cout << "Server message: " << string(buffer, bytes_received) << endl;
    }

    // Close socket
    close(client_socket);
}

int main() {
    testConnection();
    return 0;
}