#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* basename(char *path) {
    static char buf[512 + 1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    // Return blank-padded name.
    memmove(buf, p, strlen(p));
    buf[strlen(p)] = 0;
    return buf;
}

void find(char* path, const char* filename) {
    // printf("finding %s ...\n", path);
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch(st.type){
        case T_FILE:
            // eq
            // printf("f1: %s, f2: %s, cmp: %d", basename(path), filename, strcmp(basename(path), filename));
            if (strcmp(basename(path), filename) == 0) {
                printf("%s\n", path);
            }
            break;

        case T_DIR:
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                // printf("de.inum: %d, de.name: %s, filename: %s\n", de.inum, de.name, filename);
                if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
                
                memcpy(p, de.name, strlen(de.name));
                p[strlen(de.name)] = 0;
                if(stat(buf, &st) < 0){
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                find(buf, filename);
            }
            break;
    }
    close(fd);
}


int main(int argc, char *argv[]) {
    if(argc != 3){
        fprintf(2, "find: args error");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}