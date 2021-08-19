//
// Latest edit by TeeKee on 2017/7/23.
//

#ifndef TK_TIMER_H
#define TK_TIMER_H

#include "priority_queue.h"
#include "http_request.h"

/* ms */
#define TIMEOUT_DEFAULT 500

// 函数指针，负责超时处理，tk_add_timer时指定处理函数
typedef int (*timer_handler_pt)(tk_http_request_t* request);

typedef struct tk_timer{
    size_t key;    // 标记超时时间
    int deleted;    // 标记是否被删除
    timer_handler_pt handler;    // 超时处理，add时指定
    tk_http_request_t* request;    // 指向对应的request请求
} tk_timer_t;

// tk_pq_t定义在"priority_queue.h"中，优先队列中节点
extern tk_pq_t tk_timer;
/* size_t为什么要用extern，01？？？需要跨文件使用 */
extern size_t tk_current_msec;

/* 初始化timer优先级队列，比较函数设置为timer_comp，队列大小为 500？501？，02？？？
   优先级队列里面的成员为time_t结构体 */
int tk_timer_init(); 
/* 该函数返回队列中最早时间和当前时间之差 */
int tk_find_timer();
/* 超时处理函数 */
/* 处理优先级队列节点等待时间超时的request请求 */
/* 所谓超时，是指timer_t结构体设定时间超过当前时间 */
void tk_handle_expire_timers();
/* 将新创建的按照参数要求初始化的timer_node节点插入timer优先级队列中 */
void tk_add_timer(tk_http_request_t* request, size_t timeout, timer_handler_pt handler);
/* 将request结构体中timer节点的deleted成员置为1 */
/* 也就是将该timer节点标记为删除，后续在find_timer和handle_expire_timers 
   中删除并释放该节点 */
void tk_del_timer(tk_http_request_t* request);
/* 比较两个计时器的超时时间 */
int timer_comp(void *ti, void *tj);

#endif