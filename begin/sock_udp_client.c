#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unisyd.h>
#include <errno.h>
#include <sys/un.h>
#include <string.h>



int main(){

	int sock, len;
	struct sockaddr_un addr, client;
	char buf[50];
	char name[20]="Connection";
	sock = soket(AF_LOLOCAL, SOCK_DGRAM, 0);
	if(sock < 0){
		perror("socket");
		exit(1);
	}
	addr.sun_family = AF_LOCAL;
	addr.sun_path = "./socket";

	

}
