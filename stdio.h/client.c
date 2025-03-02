#include <stdio.h>      // Standard I/O functions
#include <stdlib.h>     // Memory allocation, process control
#include <string.h>     // String manipulation functions
#include <unistd.h>     // POSIX functions (close, read, write)
#include <sys/types.h>  // Data types used in socket programming
#include <sys/socket.h> // Socket definitions
#include <netinet/in.h> // Internet address structures
#include <arpa/inet.h>  // Functions for IP address conversion

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_address;
    char message[1024] = "Hello, Server!";
    char buffer[1024] = {0};

    // Step 1: Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Step 2: Define server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    // Step 3: Connect to the server
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Step 4: Send message to server
    send(sock, message, strlen(message), 0);
    printf("Message sent to server: %s\n", message);

    // Step 5: Read response from server
    read(sock, buffer, 1024);
    printf("Response from server: %s\n", buffer);

    // Step 6: Close the socket
    close(sock);

    return 0;
}
