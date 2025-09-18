#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define SOCK_PATH "/tmp/mysocket"

int main() {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);   // 소켓 생성
    if (fd == -1) { perror("socket"); return 1; }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_PATH);

    unlink(SOCK_PATH); // 이전 실행 흔적 제거
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind"); return 1;
    }

    if (listen(fd, 5) == -1) { perror("listen"); return 1; }

    printf("서버 대기 중... (%s)\n", SOCK_PATH);

    for (;;) {
        int cfd = accept(fd, NULL, NULL);
        if (cfd == -1) { perror("accept"); continue; }

        char buf[100];
        int n = read(cfd, buf, sizeof(buf)-1);
        if (n > 0) {
            buf[n] = '\0';
            printf("클라이언트: %s\n", buf);

            const char *reply = "pong";
            write(cfd, reply, strlen(reply));
        }
        close(cfd);
    }

    close(fd);
    unlink(SOCK_PATH);
    return 0;
}

