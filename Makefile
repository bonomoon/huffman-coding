CC = gcc
CFLAGS = -g -Wall
OBJS = huffman.o minheap.o
TARGET = app.out

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

huffman.o: minheap.h huffman.c
minheap.o: minheap.h minheap.c

clean:
	rm -f *.o
	rm -f $(TARGET)
