CC=gcc
CFLAGS=-Wall -Wextra -I. 

BINARY=main
OBJECTS=main.o server.o observer.o client.o request.o

$(BINARY) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(BINARY)
