#include <stdio.h>      // Standard I/O functions
#include <stdlib.h>     // Memory management, exit function
#include <string.h>     // String handling functions
#include <unistd.h>     // POSIX API (close, read, write)
#include <sys/types.h>  // Data types used in socket programming
#include <sys/socket.h> // Core socket definitions
#include <netinet/in.h> // Internet address structures
#include <arpa/inet.h>  // Functions for IP address conversion

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};
    char message[1024];

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

    while (1) {
        // Step 4: Get user input
        printf("Enter command (HELLO/EXIT): ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0; // Remove newline character

        // Step 5: Send message to server
        write(sock, message, strlen(message));

        // Step 6: Read response from server
        read(sock, buffer, 1024);
        printf("Server Response: %s\n", buffer);

        // Exit loop if command is "EXIT"
        if (strcmp(message, "EXIT") == 0) {
            break;
        }
    }

    // Step 7: Close the socket
    close(sock);

    return 0;
}
