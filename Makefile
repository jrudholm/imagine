
CFLAGS := -Werror -Wall -I/home/johan/yaml/include
LDFLAGS := -L/home/johan/yaml/lib -lyaml 
CC := gcc

all: imagine

imagine: imagine.c imagine_api.h common.h
	$(CC) $(CFLAGS) -o imagine imagine.c -ldl -lpthread $(LDFLAGS)

clean:
	rm -f *.o *.so imagine
