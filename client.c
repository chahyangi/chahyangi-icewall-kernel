#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define SOCK_PATH "/tmp/mysocket"

int main() {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) { perror("socket"); return 1; }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_PATH);

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect"); return 1;
    }

    const char *msg = "ping";
    write(fd, msg, strlen(msg));

    char buf[100];
    int n = read(fd, buf, sizeof(buf)-1);
    if (n > 0) {
        buf[n] = '\0';
        printf("서버 응답: %s\n", buf);
    }

    close(fd);
    return 0;
}

