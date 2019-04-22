#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minheap.h"    //최소힙 자료구조

#define MAX_CHAR 128 //아스키 코드 문자 128개
#define MAX_BUFSIZE 2047  //파일 버퍼 크기

char buf[MAX_BUFSIZE + 1] = {0};
void huffman_init(char *data, int *freq, int *data_cnt, const char *f_name);

Node *huffman_new_data(char data, int freq);
Node *huffman_build_tree(char *data, int *freq, int data_cnt);
void huffman_build_table(Node *huffman_tree, char **huffman_table, char *code, int top);

void huffman_encode(const char *i_file, const char *o_file, char **huffman_table);
void huffman_decode(const char *i_file, const char *o_file, Node *huffman_tree);

int main() {
    char data[MAX_CHAR];
    int freq[MAX_CHAR] = {0, };
    int data_cnt = 0;

    huffman_init(data, freq, &data_cnt, "./input.txt");
    printf("\n[ Start to build Huffman Tree ]\n");
    Node *huffman_tree = huffman_build_tree(data, freq, data_cnt);
    printf("\n[ Completed to build Huffman Tree ]\n\n");

    printf("[ Start to build Huffman Table ]\n");
    char *huffman_table[MAX_CHAR] = {0, };
    char code[17]; code[0] = '\0';
    huffman_build_table(huffman_tree, huffman_table, code, 0);
    printf("\n");
    //huffman code table 테스트 코드
    for(int i = 0; i < MAX_CHAR; ++i) {
        if(huffman_table[i]) {
            if(i < 32) {
                printf("\%c\\ : %s\n", i, huffman_table[i]);
                continue;
            }
            printf("'%c': %s\n", i, huffman_table[i]);
        }
    }
    printf("[ Completed to build Huffman Code Table ]\n\n");

    printf("[ Start to be encoded by Huffman ]\n");
    huffman_encode("./input.txt", "./output.txt", huffman_table);

    printf("[ Start to be decoded by Huffman ]\n");
    huffman_decode("./output.txt", "./decode.txt", huffman_tree);

    return 0;
}

//처음 변환할 파일을 읽고 아스키 코드값에 대응하는 문자들을 파싱한 후 빈도 수를 체크한다.
void huffman_init(char *data, int *freq, int *data_cnt, const char *f_name) {
    FILE *stream;
  
    for(int i = 0; i < MAX_CHAR; ++i) {
        data[i] = i;
    }
   
    //input 파일 읽어서 문자 빈도수 체크
    stream = fopen(f_name, "r");
    if(!stream) {
        perror("fopen input");
        exit(1);
    }

    while(!feof(stream)) {
        memset(buf, '\0', sizeof(buf));
        int read_cnt = fread(buf, sizeof(char), sizeof(buf), stream);
        
        for(int i = 0; i < read_cnt; ++i) {
            ++freq[(int)buf[i]];
        }
    }
    fclose(stream);

    for(int i = 0; i < MAX_CHAR; ++i) {
        if(freq[i] != 0) {
            ++(*data_cnt);
        }
    }

    //문자와 빈도 수 체크
    printf("[ Checking Character and frequency ]\n");
    for(int i = 0; i < 128; ++i) {
        if(freq[i] != 0) {
            printf("'%c': %d개\n", data[i], freq[i]);
        }
    }
}

//huffman tree 만들고 반환
Node *huffman_build_tree(char *data, int *freq, int data_cnt) {
    char bin_ch = '\0';
    Node *left_child, *right_child, *parent;
    MinHeap* min_heap = minheap_new(data_cnt);

    minheap_build(data, freq, MAX_CHAR, min_heap);
    
    while(minheap_get_size(min_heap) != 1) {
        left_child = minheap_extract(min_heap);
        right_child = minheap_extract(min_heap);

        parent = minheap_new_node(&bin_ch, left_child->frequency + right_child->frequency);
        parent->left = left_child;
        parent->right = right_child;

        minheap_insert(min_heap, parent);
        printf(".");
    }

    return minheap_extract(min_heap);
}

//huffman code table 만들기
void huffman_build_table(Node *huffman_tree, char *huffman_table[], char *code, int len) {
    char data = *(char *)huffman_tree->data;

    if(data) {
        printf(".");
        code[len] = '\0';
        huffman_table[(int)data] = (char *)malloc(sizeof(char)*strlen(code));
        strcpy(huffman_table[(int)data], code);
        return;
    }

    if(huffman_tree->left) {
        code[len] = '0';
        huffman_build_table(huffman_tree->left, huffman_table, code, len+1);
    }

    if(huffman_tree->right) {
        code[len] = '1';
        huffman_build_table(huffman_tree->right, huffman_table, code, len+1);
    }
}

//huffman code table을 사용하여 input file을 encoding한다.   
void huffman_encode(const char *i_file, const char *o_file, char **huffman_table) {
    FILE *i_stream = fopen(i_file, "r");
    FILE *o_stream = fopen(o_file, "w");

    if(!i_stream || !o_stream) {
        perror("fopen for encoding");
        exit(1);
    }

    while(!feof(i_stream)) {
        memset(buf, '\0', sizeof(buf));
        int read_cnt = fread(buf, sizeof(char), sizeof(buf), i_stream);

        /** printf("%s\n", buf); */
        /** fgets(buf, sizeof(buf), i_stream); */

        /** if(feof(i_stream)) { */
        /**     buf[0] = '\0'; */
        /**     break; */
        /** } */

        for(int i = 0; i < read_cnt; ++i) {
            if(huffman_table[(int)buf[i]]) {
                fputs(huffman_table[(int)buf[i]], o_stream);
            }
        }
    }
    fclose(i_stream);
    fclose(o_stream);

    printf("[ Completed to encode file by Huffman ]\n");
    printf("[ Please Open %s to check ]\n\n", o_file);
}

//huffman tree을 사용하여 encoding된 파일을 해석한다.
void huffman_decode(const char *i_file, const char *o_file, Node *huffman_tree) {
    FILE *i_stream = fopen(i_file, "r");
    FILE *o_stream = fopen(o_file, "w");

    if(!i_stream || !o_stream) {
        perror("fopen for decoding");
        exit(1);
    }

    while(!feof(i_stream)) {
        memset(buf, '\0', sizeof(buf));
        fread(buf, sizeof(char), sizeof(buf), i_stream);

        Node *temp = huffman_tree;
        for(int i = 0; buf[i] != 0; ++i) {
            if(buf[i] == '0') {
                if(*(char *)huffman_tree->left->data) {
                    fputc(*(char *)huffman_tree->left->data, o_stream);
                    huffman_tree = temp;
                    continue;
                }
                huffman_tree = huffman_tree->left;
            }
            if(buf[i] == '1') {
                if(*(char *)huffman_tree->right->data) {
                    fputc(*(char *)huffman_tree->right->data, o_stream);
                    huffman_tree = temp;
                    continue;
                }
                huffman_tree = huffman_tree->right;
            }
        }
    }
    fclose(i_stream);
    fclose(o_stream);

    printf("[ Completed to decode file by Huffman ]\n");
    printf("[ Please Open %s to check ]\n", o_file);
}
