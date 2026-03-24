#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8888

int main() {
    int sockfd;
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("UDP Echo Server is running...\n");

    while (1) {
        client_len = sizeof(client_addr);

        int len = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                           (struct sockaddr *)&client_addr, &client_len);

        if (len > 0) {
            buffer[len] = '\0';
            printf("Received: %s\n", buffer);

            // Echo lại cho client
            sendto(sockfd, buffer, len, 0,
                   (struct sockaddr *)&client_addr, client_len);
        }
    }

    close(sockfd);
    return 0;
}