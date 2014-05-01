all:
	gcc -g bouncer.c process_pkt.c ping.c utils.c -o bouncer -lpcap

clean:
	rm -f *.o bouncer

debug:
	gcc -g bouncer.c process_pkt.c -o bouncer -lpcap
