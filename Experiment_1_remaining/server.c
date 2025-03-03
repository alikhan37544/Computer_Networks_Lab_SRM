#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT 8080

void log_time() {
    time_t now;
    time(&now);
    printf("Server started at: %s", ctime(&now));
}

void check_file_status(const char *filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == 0) {
        printf("File '%s' exists. Size: %ld bytes\n", filename, file_stat.st_size);
    } else {
        perror("File status check failed");
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *response = "Hello from server!";

    log_time(); // Log server start time
    check_file_status("server.c"); // Check if the source file exists

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
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Step 6: Receive message from client
        read(new_socket, buffer, 1024);
        printf("Client sent: %s\n", buffer);

        // Step 7: Send response to client
        write(new_socket, response, strlen(response));
        printf("Response sent to client.\n");

        // Step 8: Close the socket
        close(new_socket);
    }

    close(server_fd);
    return 0;
}
