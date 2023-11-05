.PHONY: all clean

CC=cc
CFLAGS=-g -O0 -Wall -pedantic -lglfw -lpthread -lGLEW -lGLU -lGL
RM=rm

all: test

test: test.o load_shader.o
	$(CC) $(CFLAGS) -o test test.o load_shader.o

test.o: test.c
	$(CC) $(CFLAGS) -o test.o -c test.c

load_shader.o: load_shader.c
	$(CC) $(CFLAGS) -o load_shader.o -c load_shader.c

clean:
	$(RM) -f test *.o
