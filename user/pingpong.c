#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[]) {
    // 0 读 1 写
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    if (fork() == 0) {
        // 子进程
        char buf[8];
        close(p1[0]);
        close(p2[1]);
        read(p2[0], buf, sizeof buf);
        fprintf(1, "%d: received %s\n", getpid(), buf);
        write(p1[1], "pong", 4);
    } else {
        // 父进程
        char buf[8];
        close(p1[1]);
        close(p2[0]);
        write(p2[1], "ping", 4);
        read(p1[0], buf, sizeof buf);
        fprintf(1, "%d: received %s\n", getpid(), buf);
    }
    exit(0);
}