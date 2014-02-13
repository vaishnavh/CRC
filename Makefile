CC = gcc
CFLAGS  = -g -Wall

ec: crc.o message.o apply_crc.o corrupt.o
	$(CC) $(CFLAGS) -o ec crc.o message.o apply_crc.o corrupt.o

apply_crc: crc.h apply_crc.c 
	$(CC) $(CFLAGS) -o apply_crc apply_crc.c 

crc: crc.c crc.h message.h
	$(CC) $(CFLAGS) -c crc.c -o crc

message: message.c message.h
	$(CC) $(CFLAGS) -c message.c -o message

corrupt: corrupt.c corrupt.h message.h
	$(CC) $(CFLAGS) -c corrupt.c -o corrupt
