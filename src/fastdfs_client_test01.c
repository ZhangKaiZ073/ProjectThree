#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#include "make_log.h"

#define _FILE_NAME_     "fastdfs_client_test01.c"

int main(int argc,char **argv)
{
    if (argc < 3) {
        printf("./a.out ...\n");
    }

    int pid[2];
    int ret = 0;
    int fd = 0;

    ret = pipe(pid);
    if (ret != 0) {
        LOG(_FILE_NAME_, "main()", "[error]: pipe() error ret == %d", ret);
        goto END;
    }

    fd = fork();
    if (fd < 0) {

        ret = -1;
        LOG(_FILE_NAME_, "main()", "[error] fork() %d", ret);
    }
    else if (fd == 0) {

        close(pid[0]);
        dup2(pid[1], STDOUT_FILENO);
        if (argc == 3) {
            execlp(argv[1], argv[1], argv[2], NULL);
        }
        else if (argc == 4) {
            execlp(argv[1], argv[1], argv[2], argv[3], NULL);
        }
        ret = -1;
        LOG(_FILE_NAME_, "main()", "[error] execlp() %d", ret);
    }
    else {
        close(pid[1]);
        wait(NULL);
        char buf[1024] = { 0 };
        read(pid[0], buf, sizeof(buf));
        printf("%s", buf);
    }

END:
    return ret;
}
