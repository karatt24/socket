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
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>


int main(){

	pid_t pid;
        int i, sock, *fd, err, err1;
	char buf[1024] = "Gettime";
	struct sockaddr_in addr, client;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		perror("socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
        addr.sin_port = htons(33425);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
/*	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("bind");
                exit(2);
	}*/
	socklen_t len = sizeof(socklen_t);
	if(connect(sock, (struct sockaddr *)&addr, len) < 0){
		perror("connect");
		exit(1);
	}

	if(sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("sendto");
		exit(1);
	}

	if(recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&client, &len) < 0){
		perror("recvfrom");
		exit(1);
	}

	exit(0);
}
