#include <stdio.h>      // Standard I/O functions
#include <stdlib.h>     // Memory allocation, exit function
#include <string.h>     // String handling functions
#include <unistd.h>     // POSIX API (close, read, write)
#include <sys/types.h>  // Defines types like socklen_t, pid_t, clock_t
#include <sys/socket.h> // Core socket definitions
#include <netinet/in.h> // Internet address structures
#include <time.h>       // Used for time measurement

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);  // Using socklen_t for compatibility
    char buffer[1024] = {0};
    char *response = "Hello from server!";

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
        // Step 5: Accept a client connection
        new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Step 6: Measure time before processing request
        clock_t start_time = clock();

        // Step 7: Read message from client
        read(new_socket, buffer, 1024);
        printf("Client sent: %s\n", buffer);

        // Step 8: Send response to client
        write(new_socket, response, strlen(response));
        printf("Response sent to client.\n");

        // Step 9: Measure time after processing
        clock_t end_time = clock();
        double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Processing time: %f seconds\n", elapsed_time);

        // Step 10: Close the socket
        close(new_socket);
    }

    close(server_fd);
    return 0;
}
