/*
 * echo.c --
 *
 *	Produce a page containing all FastCGI inputs
 *
 *
 * Copyright (c) 1996 Open Market, Inc.
 *
 * See the file "LICENSE.TERMS" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */
#ifndef lint
static const char rcsid[] = "$Id: echo.c,v 1.5 1999/07/28 00:29:37 roberts Exp $";
#endif /* not lint */

#include "fcgi_config.h"

#include <stdlib.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

#include "fcgi_stdio.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "make_log.h"

#define _FILE_NAME_     "upload_test01.c"

static void PrintEnv(char *label, char **envp)
{
    printf("%s:<br>\n<pre>\n", label);
    for ( ; *envp != NULL; envp++) {
        printf("%s\n", *envp);
    }
    printf("</pre><p>\n");
}

void INFO(char *buf)
{
    char filename[128] = { 0 };
    int fd = 0;
    
    int pid[2];
    int ret = 0;
    int lfd = 0;

    char *l = strstr(buf,"filename");
    char *r = strchr(l, '"');
    char *ll = strchr(r+1, '"');

    strncpy(filename, r+1, ll-r-1);

    fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0664);
    if (fd == 0) {
        printf("<h1>3</h1>");
    }

    char *strl = strstr(ll,"\r\n\r\n");
    
    char *temp = strl;
    char *strr = NULL;
    while ((buf - (strr = strstr(temp,"-----")) >= 0)) {
            temp++;
    }

    printf("%d = %d\r\n", strr, strl);
    write(fd, strl+4, strr-strl);

    ret = pipe(pid);
    if (ret != 0) {
        LOG(_FILE_NAME_, "main()", "[error]: pipe() error ret == %d", ret);
        goto END;
    }

    lfd = fork();
    if (lfd < 0) {
        ret = -1;
        LOG(_FILE_NAME_, "main()", "[error] fork() %d", ret);
        goto END;
    }
    else if (lfd == 0) {

        close(pid[0]);
        dup2(pid[1], STDOUT_FILENO);
        execlp("fdfs_upload_file", "fdfs_upload_file", "./conf/client.conf" ,filename, NULL);
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

    if (fd != 0) {
        close(fd);
    }
END:
    return;
}

int main ()
{
    char **initialEnv = environ;
    int count = 0;
    char *buf = NULL;

    while (FCGI_Accept() >= 0) {
        char *contentLength = getenv("CONTENT_LENGTH");
        int len;

        printf("Content-type: text/html\r\n"
                "\r\n"
                "<title>FastCGI echo</title>"
                "<h1>FastCGI echo</h1>\n"
                "Request number %d,  Process ID: %d<p>\n", ++count, getpid());

        if (contentLength != NULL) {
            len = strtol(contentLength, NULL, 10);
            printf("----------------------%d\r\n", len);
        }
        else {
            len = 0;
        }

        if (len <= 0) {
            printf("No data from standard input.<p>\n");
        }
        else {
            int i, ch;
            buf = (char *)malloc(len);

            printf("Standard input:<br>\n<pre>\n");
            for (i = 0; i < len; i++) {
                if ((ch = getchar()) < 0) {
                    printf("Error: Not enough bytes received on standard input<p>\n");
                    break;
                }
                putchar(ch);
                buf[i] = ch;
            }
            printf("\n</pre><p>\n");
        }


        printf("buf = %s\n",  buf);
        INFO(buf);

        PrintEnv("Request environment", environ);
        PrintEnv("Initial environment", initialEnv);
        if (buf != NULL) {
            free(buf);
            buf = NULL;
        }
    } /* while */

    return 0;
}
