#!/bin/bash

CC = gcc
files = bouncer.c utils.c process_pkt.c ping_list.c ping.c tcp_list.c tcp.c
objects = bouncer.o utils.o process_pkt.o ping_list.o ping.o tcp_list.o tcp.o
LIBS = -lpcap
BOUNCER = bouncer

CFLAGS = -W -Wall -Wextra

.PHONY: all
all: $(BOUNCER)
	@echo "Compiled successfully"
debug:
	gcc -g $(files) -o bouncer -lpcap

$(BOUNCER): $(objects)
	$(CC) $(CFLAGS) $(objects) $(LIBS) -o $@

bouncer.o: bouncer.c bouncer.h globals.h
	$(CC) -c bouncer.c

utils.o: utils.c utils.h globals.h
	$(CC) -c utils.c

process_pkt.o: process_pkt.c globals.h
	$(CC) -c process_pkt.c

ping_list.o: ping_list.c ping_list.h globals.h
	$(CC) -c ping_list.c

ping.o: ping.c ping.h globals.h
	$(CC) -c ping.c

tcp_list.o: tcp_list.c tcp_list.h globals.h
	$(CC) -c tcp_list.c

tcp.o: tcp.c tcp.h globals.h
	$(CC) -c tcp.c

.PHONY: clean
clean:
	@echo "all *.o *.out *~ files will be deleted !!!"
	rm -f *.o *.out *~
