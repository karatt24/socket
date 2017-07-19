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
#include <pthread.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

#undef max
#define max(x,y) ((x) > (y) ? (x) : (y))

int main(){

	int s_tcp, s_udp, newsock, *fd;
	pthread_t tid;
	time_t timp;
        char *tim;
	char mess[1024] = "test";
	char buf[1024] = "test";
	struct sockaddr_in addr, client;
	fd_set rfds;
	struct timeval tv;

	s_tcp = socket(AF_INET, SOCK_STREAM, 0);
	if(s_tcp < 0){
		perror("socket");
		exit(1);
        }
	s_udp = socket(AF_INET, SOCK_STREAM, 0);
	if(s_udp < 0){
		perror("socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(32654);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(s_tcp, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("bind");
		exit(1);
	}
	listen(s_tcp, 5);
	while(1){
		newsock = accept(s_tcp, NULL, NULL);
		if(newsock < 0){
			perror("accept");
			exit(1);
		}
		recv(newsock, buf, 1024, 0);

		timp=time(NULL);
                tim = ctime(&timp);

		send(newsock, tim, strlen(tim), 0);
	}

}
