#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <string.h>



int main(){

	int sock, sock1;
	struct sockaddr_un addr, addr1;
	char buf[25] = "Connection_succes!";
	char *buf_recv;
	char name[20]="socket";
	char name1[20]="socket1";
	socklen_t size_addr, *size;
	buf_recv = malloc(25);
	sock = socket(AF_LOCAL, SOCK_DGRAM, 0);
	sock1 = socket(AF_LOCAL, SOCK_DGRAM, 0);
	if(sock < 0){
		perror("socket");
		exit(1);
	}
	addr.sun_family = AF_LOCAL;
	snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", name);
	unlink(name);
	size_addr = sizeof(addr.sun_family)+strlen(addr.sun_path);
	if (bind(sock, (struct sockaddr *)&addr, size_addr) < 0){
		perror("bind");
		exit(1);
	}

	printf("Waiting for a message:\n");

	if(recvfrom(sock, (void *)buf_recv, 25, 0, (struct sockaddr *)&addr, size) < 0){
		perror("recvfrom");
		exit(1);
	}

	printf("%s\n", (char *)buf_recv);

	addr1.sun_family = AF_LOCAL;
	snprintf(addr1.sun_path, sizeof(addr1.sun_path), "%s", name1);

	if(sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&addr1, size_addr) < 0){
		perror("sendto");
		exit(1);
	}

	printf("Sended complite\n\n");

	exit(0);
}
