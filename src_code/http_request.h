//
// Latest edit by TeeKee on 2017/7/23.
//

#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "util.h"
#include "list.h"

#define TK_AGAIN EAGAIN

/* 请求方法中的字符ch满足(ch < 'A' || ch > 'Z') && ch != '_'时，返回该宏定义 */
#define TK_HTTP_PARSE_INVALID_METHOD        10 
/* 两种情况返回该宏定义，其一，无效URL地址，URL首字符不是'/'或' '（' '会被过滤掉）；其二，版本号中没出现该有的字符或出现不该有的字符 */
#define TK_HTTP_PARSE_INVALID_REQUEST       11
/* 两种情况返回该宏定义，其一，key后面不是冒号或空格；其二，换行标志/表示首部结束的空行中，回车符后面不是换行符 */
#define TK_HTTP_PARSE_INVALID_HEADER        12

/* 请求方法中的字符ch满足'A' <= ch <= 'Z' || ch == '_'时，返回该宏定义 */
#define TK_HTTP_UNKNOWN                     0x0001
#define TK_HTTP_GET                         0x0002
#define TK_HTTP_HEAD                        0x0004
#define TK_HTTP_POST                        0x0008

/* 找到了该资源，并且一切正常 */
#define TK_HTTP_OK                          200
/* 该资源在上次请求之后没有任何修改,通常用于浏览器的缓存机制。 */
#define TK_HTTP_NOT_MODIFIED                304
/* 在指定的位置不存在所申请的资源。 */
#define TK_HTTP_NOT_FOUND                   404
#define MAX_BUF 8124

/* 请求结构 */
typedef struct tk_http_request{
    char* root; // 配置目录
    int fd; // 描述符（监听、连接）
    int epoll_fd; // epoll描述符
    char buff[MAX_BUF]; // 用户缓冲
    /* 累计已经解析缓冲区的字节数，从数组下标来看，也是该次http请求解析的起始位置 */
    size_t pos;
    /* 累计已经写入缓冲区的字节数，从数组下标来看，也是该次http请求解析的终止位置 */
    size_t last;
    /* 记录http的解析状态 */
    int state; // 请求头解析状态
    /* 在初始化函数tk_init_request_t中，除了buff数组，以上变量都已初始化 */

    /* 指向请求方法中的首个字符，比如GET中的'G'，HTTP版本考虑的是1.0，项目代码实际只考虑的是GET的情况 */
    void* request_start;
    /* 指向请求方法后的空格字符 */
    void* method_end;
    /* 仅支持http 1.0，因此请求方法只有GET、POST和HEAD */
    int method; // 请求方法
    /* 指向URL地址的第二个字符，即'/'后的一个字符 */
    void* uri_start;
    /* 指向URL地址后面的空格字符 */
    void* uri_end;
    /* 暂未使用 */
    void* path_start;
    /* 暂未使用 */
    void* path_end;
    /* 暂未使用 */
    void* query_start;
    /* 暂未使用 */
    void* query_end;
    /* http的主版本号 */
    int http_major;
    /* http的副版本号 */
    int http_minor;
    /* 指向请求行末尾的换行标志的第一个字符，对于Windows操作系统为回车符，对于Unix系统为换行符 */
    /* 该服务器不考虑Mac系统 */
    void* request_end;

    struct list_head list; // 存储请求头，list.h中定义了此结构

    /* cur指的是当前的一条key:value请求头 */
    /* 指向key的第一个字符 */
    void* cur_header_key_start;
    /* 指向key后的空格或冒号 */
    void* cur_header_key_end;
    /* 指向value的第一个字符 */
    void* cur_header_value_start;
    /* 指向value后换行标志的第一个字符 */
    void* cur_header_value_end;
    /* 设置http_request结构体的timer成员，该成员是timer优先级队列中的节点  */
    void* timer; // 指向时间戳结构
}tk_http_request_t;

/* 响应结构 */
typedef struct tk_http_out{
    /* 连接描述符 */
    int fd;
    /* 是否开启长连接 */
    int keep_alive;
    /* 文件上次修改的时间 */
    time_t mtime;
    /* 文件是否修改 */
    int modified;
    /* 返回码 */
    int status;
}tk_http_out_t;

/* 请求头结构 */
typedef struct tk_http_header{
    /* 指向key的第一个字符 */
    void* key_start;
    /* 指向key后的空格或冒号 */
    void* key_end;
    /* 指向value的第一个字符 */
    void* value_start;
    /* 指向value后换行标志的第一个字符 */
    void* value_end;
    struct list_head list;
}tk_http_header_t;

typedef int (*tk_http_header_handler_pt)(tk_http_request_t* request, tk_http_out_t* out, char* data, int len);

/* 请求头处理结构体，包含指向key的char指针和请求头处理函数 */
typedef struct tk_http_header_handle{
    char* name;
    tk_http_header_handler_pt handler;    // 函数指针
}tk_http_header_handle_t;

/* tk_http_header_handle_t数组的声明，定义可能不在当前文件中，需要在其他文件中找 */
extern tk_http_header_handle_t tk_http_headers_in[]; 

/* 请求头处理函数 */
void tk_http_handle_header(tk_http_request_t* request, tk_http_out_t* out);
/* 关闭描述符，释放请求数据结构 */
int tk_http_close_conn(tk_http_request_t* request);
/* 初始化请求数据结构 */
int tk_init_request_t(tk_http_request_t* request, int fd, int epoll_fd, char* path);
/* 初始化响应数据结构 */
int tk_init_out_t(tk_http_out_t* out, int fd);
/* 根据状态码返回shortmsg */
const char* get_shortmsg_from_status_code(int status_code);

#endif