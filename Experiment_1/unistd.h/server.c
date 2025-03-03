#include <stdio.h>      // Standard I/O functions
#include <stdlib.h>     // Memory management functions
#include <string.h>     // String manipulation functions
#include <unistd.h>     // POSIX functions (fork, read, write, close)
#include <sys/types.h>  // Data types used in socket programming
#include <sys/socket.h> // Socket functions
#include <netinet/in.h> // Internet address structures

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *message = "Hello from server!";

    // Step 1: Create a socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Step 2: Define server address
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

    while (1) {
        // Step 5: Accept a connection
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Step 6: Create a new process using fork()
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } 
        else if (pid == 0) {  // Child process
            close(server_fd); // Close the listening socket in the child process
            read(new_socket, buffer, 1024);
            printf("Message from client: %s\n", buffer);

            write(new_socket, message, strlen(message)); // Send response
            printf("Response sent to client.\n");

            close(new_socket); // Close the socket
            exit(0);
        } 
        else {  // Parent process
            close(new_socket); // Parent closes the connected socket
        }
    }

    return 0;
}
