#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "redis_op.h"
#include <hiredis.h>

int main(int argc,char **argv)
{
    int ret = 0;
    unsigned char *str = NULL;
    unsigned char key[128] = { 0 };
    unsigned char value[1024] = { 0 };
    redisContext *conn = NULL;
    
    // 连接数据库
    conn = rop_connectdb("127.0.0.1", "6379", "123456");
    if (conn == NULL) {
        printf("rop_connectdb error!\n");
        ret = -1;
        goto END;
    }

    // 输入key 和 value
    printf("Please in key : ");
    scanf("%s", key);
    printf("Please in value : ");
    scanf("%s", value);

    // 设置key 和 value
    ret = Rop_SetValue(conn, key, value);
    if (ret != 0) {
        printf("Rop_SetValue error!\n");
        goto END;
    }

    printf("成功设置key\n");

    // 将key 清空
    memset(key, 0, strlen(key));
    // 输入key
    printf("Please in key : ");
    scanf("%s", key);
    
    // 获取key中的值
    ret = Rop_GetValue(conn, key, &str);
    if (ret == -2) {
        printf("没有这个key\n");
        goto END;
    }
    else if (ret != 0) {
        printf("Rop_GetValue error!\n");
        goto END;
    }

    // 打印key中的值
    printf("value = %s\n", str);

END:
    // 释放内存
    if (str != NULL) {
        free(str);
        str = NULL;
    }
    // 关闭连接
    if (conn != NULL) {
        rop_disconnect(conn);
    }
    
    return ret;
}
