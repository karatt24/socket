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


struct msgreq{
	long mtype;
	char message[256];
	struct sockaddr_in client;
};

struct msgresp{
	long mtype;
	char message[256];
	struct sockaddr_in client;
};

pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;

int queue(int m, int n){
        key_t key;
        key = ftok("/", m);
        if(n == 0)
                return msgget(key, IPC_CREAT | 0666);
        return msgget(key, 0);
}

void vremya(int *fd){

	time_t timp;
	char *tim;
	struct msgreq req;
	struct msgresp resp;
	printf("Tut\n");
	while(1){
		if(msgrcv(fd[0], &req, sizeof(struct msgreq), 1L, 0) < 0){
			perror("msgrcv");
			exit(1);
		}
		printf("%s\n", req.message);
		timp=time(NULL);
		tim = ctime(&timp);
		resp.mtype = 1L;
		resp.client = req.client;
	//	memset(&resp.message, 0, sizeof(resp.message));
		snprintf(resp.message, 256, "%s", tim);
		printf("Polozshili otvet\n");
		pthread_mutex_lock(&m);
		if(msgsnd(fd[1], &resp, sizeof(struct msgresp), 0) < 0){
			perror("msgsnd");
			exit(0);
		}
		pthread_mutex_unlock(&m);
	}
}


int main(){

	int sock, *fd, err, err1;
	pthread_t tid;
	char buf[1024];
	struct msgreq request;
	struct msgresp response;
	struct sockaddr_in addr, client;
	fd = malloc(sizeof(int)*2);
	memset(&client, 0, sizeof(struct sockaddr_in));
	fd[0] = queue('m', 0);
	fd[1] = queue('n', 0);

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		perror("socket");
		exit(1);
	}

	fcntl(sock, F_SETFL, O_NONBLOCK);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(33425);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("bind");
		exit(2);
	}

	socklen_t len=sizeof(socklen_t);
	pthread_create(&tid, NULL, (void *)vremya, (void*)fd);
	while(1){
//		sleep(1);
		if(recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&client, &len) < 0){
			err = errno;
			if(err == EWOULDBLOCK){
				if(msgrcv(fd[1], &response, sizeof(struct msgresp), 1L, IPC_NOWAIT) < 0){
					err1 = errno;
                                        if(err1 == ENOMSG)
                                                continue;
                                        perror("msgrcv");
                                        exit(1);
                                }
				if(sendto(sock, response.message, 256, 0, (struct sockaddr *)&client, sizeof(client)) < 0){
					perror("sendto");
					exit(1);
				}
				continue;
			}
			perror("recvfrom");
			exit(1);
		}
		printf("Запрос принят: %s\n", buf);
/*		snprintf(buf, 256, "Answer...time po pozshe)");
		if(sendto(sock, buf, 256, 0, (struct sockaddr *)&client, sizeof(client)) < 0){
			perror("sendto");
			exit(1);
		}*/
		request.mtype = 1L;
		snprintf(request.message, 256, "%s", buf);
		request.client = client;
		printf("Insert request in ochered'\n");
		pthread_mutex_lock(&m);
		msgsnd(fd[0], &request, sizeof(struct msgreq), 0);
		pthread_mutex_unlock(&m);
	}
}
