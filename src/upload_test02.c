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


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    char **initialEnv = environ;
    int fd = 0;
    int n = 0;
    char filename[128] = { 0 };
    int i = 0;
    char *buf = NULL;
    

    while (FCGI_Accept() >= 0) {
        char *contentLength = getenv("CONTENT_LENGTH");
        int len = 0;

        if (contentLength != NULL) {
            len = strtol(contentLength, NULL, 10);
        }
        else {
            len = 0;
            printf("<h1>1</h1>");
            break;
        }

       // buf = (char *)malloc(len);

       // if (len > 0) {
          //  i = read(STDIN_FILENO, buf, len);
          //  if (i != len) {
            //    printf("<h1>2</h1>");
            //    break;
           // }    
           // char *l = strstr(buf,"filename=");
           // char *r = strchr(l, '"');
           // char *ll = strchr(r, '"');

           // strncpy(filename, ll+1, ll-r);

           // fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0777);
           // if (fd == 0) {
            //    printf("<h1>3</h1>");
            //    break;
          //  }

        //    char *strl = strstr(buf,"\r\n\r\n");
        //    char *strr = strstr(buf,"------");
      //      write(fd, strl, strr-(strl+2));
    //    }

    } /* while */

    if (fd != 0) {
        close(fd);
    }

    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }

    return 0;
}
