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




int main(){

	int s_tcp;
	struct sockaddr_in addr, client;
        char mess[1024] = "test";
        char buf[1024] = "test";

	s_tcp = socket(AF_INET, SOCK_STREAM, 0);
	if (s_tcp < 0){
		perror("socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
        addr.sin_port = htons(32654);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        if(connect(s_tcp, (struct sockaddr *)&addr, sizeof(addr))){
                perror("connect");
                exit(1);
        }

        send(s_tcp, mess, sizeof(mess), 0);
        recv(s_tcp, buf, sizeof(buf), 0);
        printf("Ответ: %s\n", buf);


}




