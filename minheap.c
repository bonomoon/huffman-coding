#include <stdio.h>
#include <stdlib.h>
#include "minheap.h"

#define PARENT(n)  (n - 1) / 2
#define LEFT(n)    2 * n + 1
#define RIGHT(n)   2 * n + 2


//새로운 data 생성하고 리턴
Node *minheap_new_node(void *data, int freq) {
    Node* node = (Node *)malloc(sizeof(Node));
    node->left = node->right = NULL;
    node->data = data;
    node->frequency = freq;

    return node;
}

//data들이 들어가는 최소 힙을 생성하고 리턴
MinHeap *minheap_new(int capacity) {
    MinHeap *min_heap = (MinHeap *)malloc(sizeof(MinHeap));
    min_heap->capacity = capacity;
    min_heap->size = 0;
    min_heap->buf = (Node **)malloc(min_heap->capacity * sizeof(Node *));

    return min_heap;
}

//data들을 최소힙에 집어 넣고 최소힙에 맞게 조정
void minheap_build(void *data, int *freq, int data_kinds, MinHeap *min_heap) {
    //data들을 최소힙에 집어넣음
    int heap_size = 0;
    for(int i = 0; i < data_kinds; ++i) {
        if(freq[i] != 0 && heap_size <= min_heap->capacity) {
            min_heap->buf[heap_size++] = minheap_new_node(&data[i], freq[i]);
            min_heap->size++;
            /** printf("asd %c\n", *(char *)min_heap->buf[heap_size - 1]->data); */
        }
    }

    int n = min_heap->size - 1;

    //최소힙의 정의에 맞게 조정
    for(int i = PARENT(n); i >= 0; --i) {
        minheap_heapify(min_heap, i);
    }
}

//data들을 최소힙에 맞게 조정
void minheap_heapify(MinHeap *min_heap, int index) {
    int smallest = index;
    int left = LEFT(index);
    int right = RIGHT(index);
    
    if(left < min_heap->size
            && min_heap->buf[left]->frequency < min_heap->buf[smallest]->frequency) {
        smallest = left;
    }

    if(right < min_heap->size
            && min_heap->buf[right]->frequency < min_heap->buf[smallest]->frequency) {
        smallest = right;
    }

    if(index != smallest) {
        Node *temp = min_heap->buf[smallest];
        min_heap->buf[smallest] = min_heap->buf[index];
        min_heap->buf[index] = temp;

        minheap_heapify(min_heap, smallest);
    }
}

//최소힙에 data를 삽입
void minheap_insert(MinHeap *min_heap, Node *data) {
    int i = (++min_heap->size) - 1;

    while(i && data->frequency < min_heap->buf[PARENT(i)]->frequency) {
        min_heap->buf[i] = min_heap->buf[PARENT(i)];
        i = PARENT(i);
    }

    min_heap->buf[i] = data;
}

//최소힙의 root data를 제거하고 리턴
Node *minheap_extract(MinHeap *min_heap) {
    Node *tmp = min_heap->buf[0];
    min_heap->buf[0] = min_heap->buf[--min_heap->size];
    minheap_heapify(min_heap, 0);

    return tmp;
}

//최소힙의 사이즈를 리턴
int minheap_get_size(MinHeap *min_heap) {
    return min_heap->size;
}
