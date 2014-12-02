#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h> 

int sockFileDesc;
struct sockaddr_in adresa;
struct hostent *host;

char zap=1;

typedef union{
    int val; /* Value for SETVAL */
    struct semid_ds *buf; /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO
                                           (Linux-specific) */
}semun;
int sem_id;
struct sembuf my;

float teplota_vonku = 0;

void* readSocket(void *arg){
while(zap){
//blokovanie semafor--------------
    	my.sem_num = 1;
    	my.sem_op = -1;
    	my.sem_flg = 0;
	semop(sem_id, &my, 1);
//--------------------------------
	recv(sockFileDesc,&teplota_vonku, sizeof(teplota_vonku),0);
//uvolni semafor------------------
    	my.sem_op = 1;
	semop(sem_id, &my, 1);
//--------------------------------
}
}
void sigend(int param);
void sigpipe(int param);
int main(int argc,char *argv[]){
	int port;
	char *ip;
 	if(argc > 2){
		ip = &argv[1][0];//ip s argumentov
		port = atoi(argv[2]);// port s argumentov
		if((sockFileDesc=socket(AF_INET, SOCK_STREAM, 0))<0){ //vytvorenie socketu
        		perror("Chyba pri vytvarani socketu:\n");
        		exit(1);
    		}
	}
	else{ 
		perror("Chyba, nebola zadana IP a port\n");
		exit(0);
	}
	adresa.sin_family=AF_INET;
   	adresa.sin_port=htons(port);
	if((host=gethostbyname(ip))==NULL){
        	perror("Pocitac neexistuje:\n");
        	exit(2);
    	}
    	adresa.sin_addr.s_addr=*((in_addr_t *)(host->h_addr_list[0]));
	if((connect(sockFileDesc, (struct sockaddr *)&adresa, sizeof(adresa)))!=0){//pripojenie k socketu
        	perror("Chyba pripojenia\n");
        	exit(3);
    	}
	int por = 1;
        send(sockFileDesc,&por, sizeof(por),0); // odosli poradove cislo klienta
	signal(SIGINT, sigend);			//prerusenie pomocou ctrl+c
	signal(SIGPIPE, sigpipe);		//neocakavane prerusenie spojenia so serverom
  
        printf("Klient c.%d pripojeny(%s,%d)\n",por,ip,port);
	
	//Vytvorenie  semaforu
	int sem_id=0;
    	if((sem_id = semget("K1", 1, 0666 | IPC_CREAT)) < 0) {
        	printf("Chyba\n");
        	exit(-2);
    	}	
	//Inicializacia semaforu
	semun un;
	un.val = 1;
	semctl(sem_id, 0, SETVAL, un);

	//Vytvorenie vlakna pre primanie vonkajsej teploty
	pthread_t vlakno;
        pthread_create(&vlakno,NULL,&readSocket,NULL);

	while(zap){
		//blokovanie semafor----------
		my.sem_num = 1;
        	my.sem_op = -1;
        	my.sem_flg = 0;
        	semop(sem_id, &my, 1);
		//----------------------------
		printf("teplota vonku : %F\n",teplota_vonku);
		//uvolni semafor--------------
		my.sem_op = 1;
        	semop(sem_id, &my, 1);
		//----------------------------
	}

	//Uzatvorenie socketu
	if(close(sockFileDesc)<0){
		perror("Nepodarilo sa uzavriet socket");
		exit(4);
	}

	return 0;
}
void sigpipe(int param){
	printf("Server bol neocakavane zruseny\n");
	zap=0;
        if(close(sockFileDesc)<0){
                perror("Nepodarilo sa uzavriet socket");
                exit(4);
        }

	exit(0);

}
void sigend(int param){
	printf("Klient uspesne vypnuty\n");
        if(close(sockFileDesc)<0){
                perror("Nepodarilo sa uzavriet socket");
                exit(4);
        }
	exit(0);
}
