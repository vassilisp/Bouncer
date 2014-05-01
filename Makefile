all:
	gcc -g bouncer.c process_pkt.c -o bouncer -lpcap -W -Wall

clean:
	rm -f *.o bouncer

debug:
	gcc -g bouncer.c process_pkt.c -o bouncer -lpcap
