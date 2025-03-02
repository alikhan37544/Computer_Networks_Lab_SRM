#include <stdio.h>      // Standard I/O functions
#include <stdlib.h>     // Memory allocation, process control
#include <string.h>     // String manipulation functions
#include <unistd.h>     // POSIX functions (close, read, write)
#include <sys/types.h>  // Data types used in socket programming
#include <sys/socket.h> // Socket definitions
#include <netinet/in.h> // Internet address structures

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *response = "Message received by server!";

    // Step 1: Create a socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Step 2: Define the server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Step 3: Bind the socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Step 4: Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);

    // Step 5: Accept a connection
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Step 6: Read data from client
    read(new_socket, buffer, 1024);
    printf("Message from client: %s\n", buffer);

    // Step 7: Send response to client
    send(new_socket, response, strlen(response), 0);
    printf("Response sent to client.\n");

    // Step 8: Close the connection
    close(new_socket);
    close(server_fd);

    return 0;
}
