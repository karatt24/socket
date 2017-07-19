all: srv_udp  clt_udp

srv_udp: server.c
		gcc server.c -o srv_udp -pthread -Wall

clt_udp: client.c
		gcc client.c -o clt_udp -Wall
	
