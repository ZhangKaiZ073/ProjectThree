#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "make_log.h"

int main(int argc,char **argv)
{
    int ret = 0;

    LOG("main()", "main()", "main err() %d", ret);

    return 0;
}
