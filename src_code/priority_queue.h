//
// Latest edit by TeeKee on 2017/7/23.
//

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdlib.h>

#define TK_PQ_DEFAULT_SIZE 10

/* 指向优先队列比较函数的指针 */
typedef int (*tk_pq_comparator_pt)(void *pi, void *pj);

/* 优先队列结构体 */
typedef struct priority_queue{
    /* 优先队列指针 */
    void **pq;
    /* 优先队列实际元素个数 */
    size_t nalloc;
    /* 优先队列大小 */
    size_t size;
    /* 堆模式，应该为指向优先队列比较函数的指针 */
    tk_pq_comparator_pt comp;
}tk_pq_t;

/* 初始化优先队列结构体 */
int tk_pq_init(tk_pq_t *tk_pq, tk_pq_comparator_pt comp, size_t size); 
/* 判断优先队列实际元素个数是否为空 */
int tk_pq_is_empty(tk_pq_t *tk_pq);
/* 获取优先队列实际元素的个数，非大小 */
size_t tk_pq_size(tk_pq_t *tk_pq);
/* 返回指向优先队列最小值节点的指针 */
void *tk_pq_min(tk_pq_t *tk_pq);
/* 删除优先队列最小值节点，并（由首节点向下sink）保持堆结构 */
int tk_pq_delmin(tk_pq_t *tk_pq);
/* 在优先队列最大节点后插入新节点，并（由末节点向上swim）保持堆结构 */
int tk_pq_insert(tk_pq_t *tk_pq, void *item);
/* 从序号为k的节点开始，向下交换以保持堆结构 */
int tk_pq_sink(tk_pq_t *tk_pq, size_t i);

#endif 
