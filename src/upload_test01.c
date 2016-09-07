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
#include "redis_op.h"
#include <hiredis.h>

#define _FILE_NAME_     "upload_test01.c"

// 将文件上传到服务器
// filename     文件名
// buf          服务器中文件id
int Seve_server(char *filename, char *buf)
{
    int ret = 0;
    int pid[2] = { 0 };
    int lfd = 0;

    // 创建管道
    ret = pipe(pid);
    if (ret != 0) {
        LOG(_FILE_NAME_, "main()", "[error]: pipe() error ret == %d", ret);
        goto END;
    }

    // 创建进程
    lfd = fork();
    if (lfd < 0) {
        ret = -1;
        LOG(_FILE_NAME_, "main()", "[error] fork() %d", ret);
        goto END;
    }
    else if (lfd > 0) {

        // 子进程执行存储到服务器程序
        close(pid[0]);
        dup2(pid[1], STDOUT_FILENO);
        execlp("fdfs_upload_file", "fdfs_upload_file", "./conf/client.conf" ,filename, NULL);
        ret = -1;
        LOG(_FILE_NAME_, "main()", "[error] execlp() %d", ret);
    }
    else {
        // 父进程回收子进程和读取子进程返回内容
        close(pid[1]);
        wait(NULL);
        read(pid[0], buf, 1024);
        char *n = NULL;
        n = strstr(buf, "\n");
        if (n != NULL) {
            *n = '\0';
        }
    }
    
END:
    // 关闭管道
    close(pid[0]);

    return ret;
}

// 将文件ID和文件名对应存在数据库中
// filename         文件名
// filenID          文件ID
int Seve_redis(char *filename, char *fileID)
{
    int ret = 0;
    redisContext *rop = NULL;
    redisReply *reply = NULL;

    // 连接数据库
    rop = rop_connectdb_nopwd("127.0.0.1", "6379");
    if (rop == NULL) {
        ret = -1;
        LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "rop_connectdb_nopwd() error\n");
        goto END;
    }

    reply = redisCommand(rop, "HSET filename %s %s", filename, fileID);
    if (reply == NULL) {
        LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "Hset database %s %s %s error! %s\n","filename", filename, fileID, rop->errstr);
        ret = -1;
        goto END;
    }
 
END:
    if (reply != NULL) {
        freeReplyObject(reply);
    }

    if (rop != NULL) {
        rop_disconnect(rop);
    }

    return ret;
}


// 解析字符串
void INFO(char *buf)
{
    char filename[128] = { 0 };
    char fileID[1024] = { 0 };
    char name[1024] = { 0 };
    int fd = 0;

    int ret = 0;

    // 找到前缀
    char *p = strstr(buf, "\r\n");
    strncpy(name, buf, p-buf);

    // 找到文件名
    char *l = strstr(p,"filename");
    char *r = strchr(l, '"');
    char *ll = strchr(r+1, '"');
    strncpy(filename, r+1, ll-r-1);

    //  创建文件
    fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0664);
    if (fd == 0) {
        ret = -1;
        LOG(_FILE_NAME_, "main()", "[error] open() %d", ret);
        goto END;
    }
    
    // 找到文件内容
    char *strl = strstr(ll,"\r\n\r\n");
    char *temp = strl;
    char *strr = NULL;
    while ((buf - (strr = strstr(temp, name)) >= 0)) {
        temp++;
    }

    // 写入文件
    write(fd, strl+4, strr-strl-4);

    // 将文件上传到服务器
    ret = Seve_server(filename, &fileID[0]);
    if (ret != 0) {
        LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "Seve_server() error");
        goto END;
    }

    // 存储文件名对应到数据库中
    ret = Seve_redis(filename, fileID);
    if (ret != 0) {
        LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC,"[error] Seve_redis() %d", ret);
        goto END;
    }

    printf("\r\n%s --------> %s \r\n", filename, fileID);

END:
    // 关闭文件
    if (fd != 0) {
        close(fd);
    }
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

        INFO(buf);

        if (buf != NULL) {
            free(buf);
            buf = NULL;
        }
    } /* while */

    return 0;
}
