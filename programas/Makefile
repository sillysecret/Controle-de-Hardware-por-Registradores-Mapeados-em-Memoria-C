CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g
LDFLAGS=

SRCS=main.c registers.c
OBJS=$(SRCS:.c=.o)

all: programa libregisters.so libregisters.a

programa: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

libregisters.so: registers.o
	$(CC) -shared -o $@ $^

libregisters.a: registers.o
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<
