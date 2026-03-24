// stream_server.c
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8888
#define PATTERN "0123456789"

int main() {
    int server_fd, client_sock;
    struct sockaddr_in server, client;
    socklen_t c;

    char buffer[1024];
    char remain[20] = "";
    int count = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server, sizeof(server));
    listen(server_fd, 5);

    printf("Waiting for client...\n");
    c = sizeof(struct sockaddr_in);
    client_sock = accept(server_fd, (struct sockaddr *)&client, &c);

    while (1) {
        int len = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (len <= 0) break;

        buffer[len] = '\0';

        // Ghép remain + buffer
        char data[2048];
        strcpy(data, remain);
        strcat(data, buffer);

        // Tìm chuỗi
        char *p = data;
        while ((p = strstr(p, PATTERN)) != NULL) {
            count++;
            p += strlen(PATTERN);
        }

        printf("Count = %d\n", count);

        // Lưu 9 ký tự cuối
        int data_len = strlen(data);
        if (data_len >= 9)
            strncpy(remain, data + data_len - 9, 9);
        else
            strcpy(remain, data);

        remain[9] = '\0';
    }

    close(client_sock);
    close(server_fd);
    return 0;
}