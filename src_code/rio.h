//
// Latest edit by TeeKee on 2017/7/23.
//

/* RIO - Robust IO */
#ifndef RIO_H
#define RIO_H

#include <sys/types.h>
#define RIO_BUFSIZE 8192

typedef struct{
    int rio_fd;             /* descriptor for this internal buf */
    ssize_t rio_cnt;        /* unread bytes in internal buf */
    char *rio_bufptr;       /* next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* internal buffer */
}rio_t;

/* 管道向缓冲区写入的n个字符，返回实际读取的字节数或-1 */
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
/* 缓冲区向管道写入n个字符，返回写入的字节数n或-1 */
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
/* 初始化IO包结构 */
void rio_readinitb(rio_t *rp, int fd); 
/* 速度更快的rio_readn()版本 */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
/* 缓冲区从管道读入一行字符，返回实际读取的字节数（不包括最后添加的空字符）、0、-1或-EAGAIN */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

#endif
