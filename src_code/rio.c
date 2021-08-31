//
// Latest edit by TeeKee on 2017/7/23.
//

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "rio.h"

/* size_t - long unsigned int */
/* ssize_t - long */
/* 缓冲区从管道读入n个字符，返回实际读取的字节数或-1 */
ssize_t rio_readn(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = (char *)usrbuf;
    while(nleft > 0){
        if((nread = read(fd, bufp, nleft)) < 0){
            /* 出错，被信号中断 */
            if(errno == EINTR)
                nread = 0;
            /* EAGAIN等错误 */
            else
                return -1;
        }
        /* 到达文件尾端 */
        else if(nread == 0)
            break;
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);
}

/* 缓冲区向管道写入n个字符，返回写入的字节数n或-1 */
ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
    /* 缓冲区要向管道写入的字节数 */
    size_t nleft = n;
    /* 每次写入管道的字节数 */
    ssize_t nwritten;
    /* 指向下一个要写入字符的指针 */
    char *bufp = (char *)usrbuf;

    while(nleft > 0){
        if((nwritten = write(fd, bufp, nleft)) <= 0){
            /* 出错，被信号中断 */
            if (errno == EINTR)
                nwritten = 0;
            /* 出错，包括EAGAIN的情况 */
            else{
                return -1;
            }
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}

/* 缓冲区从读入n个字符，返回实际读取的字节数、0或-1 */
/* 与readn()不同的是，在管道和缓冲区之间又增加了一层中间缓冲区，减少了陷入内核态时的开销，速度会更快 */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    size_t cnt;
    while(rp->rio_cnt <= 0){
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0){
            /* 管道中无字符可读，但又没有到文件尾端 */
            if(errno == EAGAIN){
                return -EAGAIN;
            }
            /* 非信号中断 */
            if(errno != EINTR){
                return -1;
            }
            /* 信号中断则重新开始read */
        }
        /* 已读到文件的尾端 */
        else if(rp->rio_cnt == 0)
            return 0;
        else
            rp->rio_bufptr = rp->rio_buf;
    }
    cnt = n;
    if(rp->rio_cnt < (ssize_t)n)
        cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

/* 初始化IO包结构 */
void rio_readinitb(rio_t *rp, int fd)
{
    /* 猜测是连接描述符，03？ */
    rp->rio_fd = fd; 
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

/* 速度更快的rio_readn()版本 */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = (char *)usrbuf;
    while (nleft > 0){
        if((nread = rio_read(rp, bufp, nleft)) < 0){
            /* errno == EINTR，这个判断无意义，rio_read()内部处理了这种情况 */
            if(errno == EINTR)
                nread = 0;
            else
                /* EAGAIN等错误 */
                return -1;
        }
        /* 已读到文件的尾端 */
        else if(nread == 0)
            break;
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);
}

/* 缓冲区从管道读入一行字符，返回实际读取的字节数（不包括最后添加的空字符）、0、-1或-EAGAIN */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    size_t n;
    ssize_t rc;
    char c, *bufp = (char *)usrbuf;
    for(n = 1; n < maxlen; n++){
        if((rc = rio_read(rp, &c, 1)) == 1){
            *bufp++ = c;
            if(c == '\n')
            break;
        }
        else if(rc == 0){
            if (n == 1){
                return 0;
            }
            else
                break;
        }
        else if(rc == -EAGAIN){
            return rc;
        }
        else{
            return -1;
        }
    }
    /* 在最后读入的字符后添加空字符，即读入了一行字符串 */
    *bufp = 0;
    return n;
}
