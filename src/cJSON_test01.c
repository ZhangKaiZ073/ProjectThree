#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

int main(int argc,char **argv)
{
    cJSON *root, *fmt;
    char *out = NULL;

    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "zhangkai");
    cJSON_AddStringToObject(root, "sex", "man");
    cJSON_AddItemToObject(root, "联系方式", fmt = cJSON_CreateObject());
    cJSON_AddNumberToObject(fmt, "手机号", 13722323301);
    cJSON_AddStringToObject(fmt, "邮箱", "C785410a5@outlook.com");
    cJSON_AddFalseToObject(fmt, "interlace");

    out = cJSON_Print(root);
    cJSON_Delete(root);
    printf("%s\n", out);
    free(out);
    out = NULL;

    return 0;
}
