#include <stdio.h>      // Standard I/O functions
#include <stdlib.h>     // Memory allocation, string conversion, exit()
#include <string.h>     // String manipulation functions
#include <unistd.h>     // POSIX API (close, read, write)
#include <sys/types.h>  // Data types used in socket programming
#include <sys/socket.h> // Core socket definitions
#include <netinet/in.h> // Internet address structures
#include <arpa/inet.h>  // Functions for IP address conversion

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1];
    int port = atoi(argv[2]); // Convert port argument to integer

    int sock;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};

    // Step 1: Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Step 2: Define server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    // Convert address from text to binary form
    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    // Step 3: Connect to the server
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Step 4: Generate a random number
    srand(time(NULL)); // Seed the random number generator
    int random_number = rand() % 100; // Generate a random number (0-99)

    char message[1024];
    sprintf(message, "%d", random_number);

    // Step 5: Send message to server
    write(sock, message, strlen(message));
    printf("Sent number to server: %d\n", random_number);

    // Step 6: Read response from server
    read(sock, buffer, 1024);
    printf("Server Response: %s\n", buffer);

    // Step 7: Close the socket
    close(sock);

    return 0;
}
