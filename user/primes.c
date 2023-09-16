#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



void sieve(int p[], int cur, const int end) {
    int x;
    if (cur == end) {
        while (1) {
            int sz = read(p[0], &x, sizeof x);
            // printf("cur: %d, read x = %d\n", cur, x);
            if (sz == 0) {
                close(p[0]);
                break;
            }
            printf("prime %d\n", x);
        }
    } else {
        int next_p[2];
        pipe(next_p);
        if (fork() != 0) {
            close(next_p[0]);
            while (1) {
                int sz = read(p[0], &x, sizeof x);
                // printf("cur: %d, read x = %d\n", cur, x);
                if (sz == 0) {
                    close(p[0]);
                    break;
                }
                if (x == cur || x % cur != 0) {
                    write(next_p[1], &x, sizeof x);
                }
            }
        } else {
            close(next_p[1]);
            close(p[0]);
            sieve(next_p, cur + 1, end);
        }
    }

}


int main(int argc, char *argv[]) {
    int cur = 1;
    int end = 35;
    if (argc > 1) {
        end = atoi(argv[1]);
    }
    int p[2];
    pipe(p);
    if (fork() != 0) {
        close(p[0]);
        int i = 1;
        while (++i <= end) {
            write(p[1], &i, sizeof i);
        }
    } else {
        close(p[1]);
        sieve(p, cur + 1, end);
    }
    exit(0);
}