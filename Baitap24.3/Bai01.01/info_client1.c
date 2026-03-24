// info_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8888

int main() {
    int sock;
    struct sockaddr_in server;
    char cwd[1024];

    // Lấy thư mục hiện tại
    getcwd(cwd, sizeof(cwd));
    int dir_len = strlen(cwd);

    // Tạo socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    // Gửi độ dài thư mục + tên thư mục
    send(sock, &dir_len, 4, 0);
    send(sock, cwd, dir_len, 0);

    DIR *d = opendir(".");
    struct dirent *dir;
    struct stat st;

    // Đếm file
    int file_count = 0;
    while ((dir = readdir(d)) != NULL) {
        stat(dir->d_name, &st);
        if (S_ISREG(st.st_mode))
            file_count++;
    }

    send(sock, &file_count, 4, 0);

    rewinddir(d);

    // Gửi từng file
    while ((dir = readdir(d)) != NULL) {
        stat(dir->d_name, &st);
        if (S_ISREG(st.st_mode)) {
            int name_len = strlen(dir->d_name);
            long long size = st.st_size;

            send(sock, &name_len, 4, 0);
            send(sock, dir->d_name, name_len, 0);
            send(sock, &size, 8, 0);
        }
    }

    closedir(d);
    close(sock);
    return 0;
}