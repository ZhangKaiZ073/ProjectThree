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
    if (argc < 2) {
        printf("./a.out ...");
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

    }
    else if (fd == 0) {
    }

END:
    return ret;
}
