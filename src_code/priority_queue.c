//
// Latest edit by TeeKee on 2017/7/23.
//

#include <stdlib.h>
#include <string.h>
#include "priority_queue.h"

/* 交换i节点和j节点的位置 */
void exch(tk_pq_t *tk_pq, size_t i, size_t j){
    void *tmp = tk_pq->pq[i];
    tk_pq->pq[i] = tk_pq->pq[j];
    tk_pq->pq[j] = tmp;
}

/* 从序号为k的节点开始，向上交换以保持堆结构 */
void swim(tk_pq_t *tk_pq, size_t k){
    /* 这个语句的作用是比较k节点与其父节点的值，如果小于则循环（向上），直到大于为止 */
    while (k > 1 && tk_pq->comp(tk_pq->pq[k], tk_pq->pq[k/2])){
        exch(tk_pq, k, k/2);
        k /= 2;
    }
}

/* 从序号为k的节点开始，向下交换以保持堆结构 */
int sink(tk_pq_t *tk_pq, size_t k){
    size_t j;
    size_t nalloc = tk_pq->nalloc;
    while((k << 1) <= nalloc){
        /* j节点为k节点的左子节点 */
        j = k << 1;
        /* 不能是j <= nalloc，因为后面有tk_pq->pq[j+1]，否则tk_pq->pq[j+1]无意义 */
        /* 这个语句的作用是使j节点为k的两个子节点中值较小的那个节点 */
        if((j < nalloc) && (tk_pq->comp(tk_pq->pq[j+1], tk_pq->pq[j])))
            j++;
        
        /* 如果值较小子节点的值比父节点的值大，则已经符合堆结构的数组，break就行，否则后续需要循环（向下） */
        if(!tk_pq->comp(tk_pq->pq[j], tk_pq->pq[k]))
            break;

        exch(tk_pq, j, k);
        k = j;
    }
    return k;
}

/* 从序号为k的节点开始，向下交换以保持堆结构 */
int tk_pq_sink(tk_pq_t *tk_pq, size_t i){
    return sink(tk_pq, i);
}

/* 初始化优先队列结构体 */
/* 最后一个参数命名为size会有歧义，实际含义为可容纳的节点数，
   因此实际节点个数(size + 1)，“1”为头节点 */
int tk_pq_init(tk_pq_t *tk_pq, tk_pq_comparator_pt comp, size_t size){
    // 为tk_pq_t节点的pq分配(void *)指针
    /* 为什么是(size + 1)，02？？？加的“1”为头节点 */
    tk_pq->pq = (void **)malloc(sizeof(void *) * (size + 1));
    if (!tk_pq->pq)
        return -1;

    tk_pq->nalloc = 0;
    tk_pq->size = size + 1;
    tk_pq->comp = comp;
    return 0;
}

/* 判断优先队列实际元素个数是否为空 */
int tk_pq_is_empty(tk_pq_t *tk_pq){
    // 通过nalloc值款快速判断是否为空
    return (tk_pq->nalloc == 0) ? 1 : 0;
}

/* 获取优先队列实际元素的个数，非大小 */
size_t tk_pq_size(tk_pq_t *tk_pq){
    // 获取优先队列大小
    return tk_pq->nalloc;
}

/* 返回指向优先队列最小值节点的指针 */
void *tk_pq_min(tk_pq_t *tk_pq){
    // 优先队列最小值直接返回第一个元素（指针）
    if (tk_pq_is_empty(tk_pq))
        return (void *)(-1);
    /* 第一个元素不是tk_pq->pq[0]吗，02？？？这里有些不一样，第二个节点才是首节点 */
    return tk_pq->pq[1];
}

/* 将优先队列的大小重置为new_size，注意是大小，不是节点的最大序号 */
int resize(tk_pq_t *tk_pq, size_t new_size){
    if(new_size <= tk_pq->nalloc)
        return -1;

    void **new_ptr = (void **)malloc(sizeof(void *) * new_size);
    if(!new_ptr)
        return -1;
    // 将原本nalloc + 1个元素值拷贝到new_ptr指向的位置
    memcpy(new_ptr, tk_pq->pq, sizeof(void *) * (tk_pq->nalloc + 1));
    // 释放旧元素
    free(tk_pq->pq);
    // 重新改写优先队列元素pq指针为new_ptr
    tk_pq->pq = new_ptr;
    tk_pq->size = new_size;
    return 0;
}

/* 删除优先队列最小值节点，并（由首节点向下sink）保持堆结构 */
int tk_pq_delmin(tk_pq_t *tk_pq){
    if(tk_pq_is_empty(tk_pq))
        return 0;
    
    /* nalloc实际个数从1开始数起 */
    exch(tk_pq, 1, tk_pq->nalloc);
    /* 将最后一个元素释放只需将实际元素的个数减1 */
    --tk_pq->nalloc;
    sink(tk_pq, 1);
    /* 删除最小值节点后，如果实际使用空间不到可使用空间的1/4，则将可使用空间缩小1倍 */
    if((tk_pq->nalloc > 0) && (tk_pq->nalloc <= (tk_pq->size - 1)/4)){
        if(resize(tk_pq, tk_pq->size / 2) < 0)
            return -1;
    }
    return 0;
}

/* 在优先队列最大节点后插入新节点，并（由末节点向上swim）保持堆结构 */
int tk_pq_insert(tk_pq_t *tk_pq, void *item){
    /* 插入之前先考虑空间容量够不够的问题，否则就扩容1倍 */
    if(tk_pq->nalloc + 1 == tk_pq->size){
        if(resize(tk_pq, tk_pq->size * 2) < 0){
            return -1;
        }
    }

    tk_pq->pq[++tk_pq->nalloc] = item;
    swim(tk_pq, tk_pq->nalloc);
    return 0;
}