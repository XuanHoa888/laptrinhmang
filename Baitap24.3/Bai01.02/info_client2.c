// stream_client.c
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8888

int main() {
    int sock;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    char *data[] = {
        "SOICTSOICT012345678901234567890123456789012345",
        "6789SOICTSOICTSOICT012345678901234567890123456",
        "7890123456789012345678901234567890123456789012",
        "3456789SOICTSOICT01234567890123456789012345678"
    };

    for (int i = 0; i < 4; i++) {
        send(sock, data[i], strlen(data[i]), 0);
        sleep(1);
    }

    close(sock);
    return 0;
}