
SOURCES = $(wildcard *.c)
OBJS = $(SOURCES:.c=.o)
CC=gcc
CFLAGS=-Wall -O -g -std=c99
TARGET= a

%.o:%.c
	$(CC) $(CFLAGS) -c  $< -o $@

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
	chmod a+x $(TARGET)
clean:
	rm -rf *.o $(TARGET)
