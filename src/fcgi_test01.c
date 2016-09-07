#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fcgi_stdio.h>

int main(int argc,char **argv)
{
    int count = 0;

    while (FCGI_Accept() >= 0) {
        printf("Content-type: text/html\r\n");
        printf("\r\n");
        printf("<title>Fast CGI Hello!</title>");
        printf("<h1>Fasr CGI Hello!</h1>");
        printf("Request num ber %d running on host <i>%s</i>\r\n", ++count, getenv("SERVER_NAME"));

        printf("%s\r\n", getenv("SCRIPT_FILENAME"));
    }

    return 0;
}
