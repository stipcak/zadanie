#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "robotArm.h"
#include "socket.h"
#include "decodeArgv.h"
//#include <pthread.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <signal.h>
#include <thread>
struct RobotArm dataArm=RobotArm_default;
struct RobotArm *pdataArm = &dataArm;
int sock;
char zap=1;
void *readSocket(void*){
	while(zap)	nacitajRobotArm(&dataArm);
}
void sigend(int param);
void sigpipe(int param);
int main(int argc,char *argv[]){
	int port;
	char *ip;
 if(decodeArgv(argc,argv,&ip,&port)){
	sock = pripoj(ip,&port);
	int *por=(int*)malloc(sizeof(int));
        *por=1;
	signal(SIGINT, sigend);
	signal(SIGPIPE, sigpipe);
        odosliInt(&(*por));
        printf("Klient c.%d pripojeny(%s,%d)\n",*por,ip,port);
        free(por);
	pthread_t vlakno;
        pthread_create(&vlakno,NULL,&readSocket,NULL);
	while(zap){
		if (pdataArm->alfa != pdataArm->actAlfa){
			if (pdataArm->alfa-0.005 > pdataArm->actAlfa)	pdataArm->actAlfa+=0.01;
			else if (pdataArm->alfa+0.005 < pdataArm->actAlfa)	pdataArm->actAlfa-=0.01;
		}
	  	odosliFloat(&pdataArm->actAlfa);
		usleep(1000000/(pdataArm->rych0/0.01));
	}
	uzavri();
	return 0;
}
}
void sigpipe(int param){
printf("Server bol neocakavane zruseny\n");
zap=0;
uzavri();
exit(0);

}
void sigend(int param){
printf("Klient uspesne vypnuty\n");
uzavri();
exit(0);
}
