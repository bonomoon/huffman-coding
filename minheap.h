#ifndef MINHEAP_H
#define MINHEAP_H

typedef struct _Node {
    void         *data;
    int          frequency;
    struct _Node *left, *right;
} Node;

typedef struct _MinHeap {
    int  capacity;
    int  size; //current size
    Node **buf;
} MinHeap;

Node    *minheap_new_node(void *data, int frequency);
MinHeap *minheap_new(int capacity);
void    minheap_build(void *data, int *frequency, int data_kinds, MinHeap *min_heap);

void    minheap_heapify(MinHeap *min_heap, int index);
void    minheap_insert(MinHeap *min_heap, Node *data);
Node    *minheap_extract(MinHeap *min_heap);

int     minheap_get_size(MinHeap *min_heap);

#endif
