// info_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8888

int main() {
    int server_fd, client_sock;
    struct sockaddr_in server, client;
    socklen_t c;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server, sizeof(server));
    listen(server_fd, 5);

    printf("Waiting for client...\n");
    c = sizeof(struct sockaddr_in);
    client_sock = accept(server_fd, (struct sockaddr *)&client, &c);

    // Nhận thư mục
    int dir_len;
    recv(client_sock, &dir_len, 4, 0);

    char dir_name[1024];
    recv(client_sock, dir_name, dir_len, 0);
    dir_name[dir_len] = '\0';

    printf("%s\n", dir_name);

    // Nhận số file
    int file_count;
    recv(client_sock, &file_count, 4, 0);

    for (int i = 0; i < file_count; i++) {
        int name_len;
        long long size;
        char file_name[256];

        recv(client_sock, &name_len, 4, 0);
        recv(client_sock, file_name, name_len, 0);
        file_name[name_len] = '\0';
        recv(client_sock, &size, 8, 0);

        printf("%s - %lld bytes\n", file_name, size);
    }

    close(client_sock);
    close(server_fd);
    return 0;
}