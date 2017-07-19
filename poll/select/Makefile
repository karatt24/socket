all: srv clt

srv: server.c
		gcc server.c -o srv_udp -pthread -Wall

clt: client.c
		gcc client.c -o clt_udp -Wall
	
clear:
		rm srv clt
