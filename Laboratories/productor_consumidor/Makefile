# Makefile simple para cliente-servidor FIFO

CC = gcc
CFLAGS = -Wall

all: cliente servidor

cliente: cliente.c
	$(CC) $(CFLAGS) -o cliente cliente.c

servidor: servidor.c
	$(CC) $(CFLAGS) -o servidor servidor.c

clean:
	rm -f cliente servidor

