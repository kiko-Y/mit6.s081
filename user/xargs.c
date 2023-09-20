#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void run(char** args) {
    if (fork() == 0) {
        exec(args[0], args);
        exit(0);
    }
    return;
}

int main(int argc, char *argv[]) {
    char buf[512];              // 读入缓存
    char* p = buf;              // 读入缓存指针
    char* argsbuf[128];         // 参数列表
    char** argsptr = argsbuf;   // 参数列表指针
    for (int i = 1; i < argc; i++) {
        *argsptr++ = argv[i];
    }
    char** pa = argsptr;        // 下一个额外参数的指针
    char* last_p = buf;
    while (read(0, p, 1) != 0) {
        if (*p == ' ' || *p == '\n') {
            *pa++ = last_p;
            last_p = p + 1;
            if (*p == '\n') {
                *p = 0;
                *pa = 0;
                run(argsbuf);
                pa = argsptr;
            }
            *p = 0;
        }
        p++;
    }
    if (p != buf) {
        *pa++ = last_p;
        *p = 0;
        *pa = 0;
        run(argsbuf);
    }
    while(wait(0) != -1) {};
    exit(0);
}