CC=gcc
CFLAGS=-W -g -std=c99 -I./deps/glad/include/ -I./deps/stb/

LDFLAGS=
# for remove: opengl winmm and gdi for linux
LDLIBS=-lSDL3 -lm
APP_NAME=game
EXEC=bin/$(APP_NAME)

SOURCES=$(wildcard src/*.c src/**/*.c ./deps/glad/src/glad.c)
HEADERS = $(wildcard src/*.h src/**/*.h)
OBJS=$(SOURCES:.c=.o)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS) $(LDLIBS)

copy_resources:
	rm -fr bin/resources
	cp -r resources bin/

all: copy_resources $(EXEC) makefile

clean:
	rm -fr $(EXEC) $(OBJS)

run:
	./$(EXEC)

.PHONY: all clean run
