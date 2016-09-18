
CFLAGS := -Werror -Wall
CC := gcc

all: libalgo1.so source.so

source.o: source.c ../imagine_api.h
	$(CC) $(CFLAGS) -fPIC -Wall -g -c $<

source.so: source.o
	$(CC) $(CFLAGS) -shared -Wl,-soname,source.so.0 -o source.so source.o -lc

libalgo1.o: libalgo1.c ../imagine_api.h
	$(CC) $(CFLAGS) -fPIC -Wall -g -c libalgo1.c

libalgo1.so: libalgo1.o
	$(CC) $(CFLAGS) -shared -Wl,-soname,libalgo1.so.0 -o libalgo1.so libalgo1.o -lc

clean:
	rm -f *.o *.so
