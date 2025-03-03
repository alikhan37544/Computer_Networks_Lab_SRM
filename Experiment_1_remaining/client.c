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

void resolve_hostname(const char *hostname) {
    struct hostent *he = gethostbyname(hostname);
    if (he == NULL) {
        herror("gethostbyname");
        return;
    }
    printf("IP Address of %s: %s\n", hostname, inet_ntoa(*(struct in_addr*)he->h_addr));
}

int main() {
    int sock;
    struct sockaddr_in server_address;
    char message[] = "Hello, Server!";
    char buffer[1024] = {0};

    resolve_hostname("localhost"); // Convert hostname to IP

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
    write(sock, message, strlen(message));

    // Step 5: Read response from server
    read(sock, buffer, 1024);
    printf("Server Response: %s\n", buffer);

    // Step 6: Close the socket
    close(sock);

    return 0;
}
