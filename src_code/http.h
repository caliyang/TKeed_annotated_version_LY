//
// Latest edit by TeeKee on 2017/7/23.
//

#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "timer.h"
#include "util.h"
#include "rio.h"
#include "epoll.h"
#include "http_parse.h"
#include "http_request.h"

#define MAXLINE 8192
#define SHORTLINE 512

/* 宏定义理解：比较两侧的字节排列；
   对于左边，将m由u_char *转换为u_int32_t *，可以实现从地址起始处读1个字节到4个字节的扩展；
   对于右边，因为数组中的元素是从低地址到高地址排列的，所以相应位置的字节需要左移一定的位数； */
#define tk_str3_cmp(m, c0, c1, c2, c3)                                       \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)
#define tk_str3Ocmp(m, c0, c1, c2, c3)                                       \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)
#define tk_str4cmp(m, c0, c1, c2, c3)                                        \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

// 用key-value表示mime_type_t
/* MIME是描述文件媒体类型的互联网标准，媒体类型是由Web服务器通过HTTP协议的Content-Type申请头来告知浏览器的 */
typedef struct mime_type{
    const char *type;
    const char *value;
}mime_type_t;

void do_request(void *ptr);

#endif