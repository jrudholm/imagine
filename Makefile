
CFLAGS := -Werror -Wall
CC := gcc

all: imagine

imagine: imagine.c imagine_api.h common.h
	$(CC) $(CFLAGS) -o imagine imagine.c -ldl

clean:
	rm -f *.o *.so imagine
