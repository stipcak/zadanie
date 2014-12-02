#include "socket.h"         
int sockFileDesc;
struct sockaddr_in adresa;
struct hostent *host;
int pripoj(char *ip,int *port){
	if((sockFileDesc=socket(AF_INET, SOCK_STREAM, 0))<0){
        	perror("Chyba pri vytvarani socketu:\n");
        	exit(1);
    	}
	adresa.sin_family=AF_INET;
   	adresa.sin_port=htons(*port);
	if((host=gethostbyname(ip))==NULL){
        	perror("Pocitac neexistuje:\n");
        	exit(2);
    	}
    	adresa.sin_addr.s_addr=*((in_addr_t *)(host->h_addr_list[0]));
	if((connect(sockFileDesc, (struct sockaddr *)&adresa, sizeof(adresa)))!=0){
        	perror("Chyba pripojenia\n");
        	exit(3);
    	}
/*	struct timeval timeout;      
    	timeout.tv_sec = 5;
    	timeout.tv_usec = 0;
	if (setsockopt (sockFileDesc, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0);
//        	error("setsockopt failed\n");
    	if (setsockopt (sockFileDesc, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(timeout)) < 0);
  //      	error("setsockopt failed\n");
*/	return sockFileDesc;
}
void odosliRobotArm(struct RobotArm *buff){
	send(sockFileDesc,&(*buff), sizeof(*buff),0);
}             
void nacitajRobotArm(struct RobotArm *buff){
        recv(sockFileDesc,&(*buff), sizeof(*buff),0);
}            
void odosliFloat(float *buff){
        send(sockFileDesc,&(*buff), sizeof(*buff),0);
}
void nacitajFloat(float *buff){
        recv(sockFileDesc,&(*buff), sizeof(*buff),0);
}
void odosliInt(int *buff){
        send(sockFileDesc,&(*buff), sizeof(*buff),0);
}
void nacitajInt(int *buff){
        recv(sockFileDesc,&(*buff), sizeof(*buff),0);
}

void uzavri(void){
	if(close(sockFileDesc)<0){
		perror("Nepodarilo sa uzavriet socket");
		exit(4);
	}
}
