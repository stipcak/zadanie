#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
void sigctrl(int param);
void sigctrl1(int param);
void sigpipe(int param);
void chyba(const char *text,int cislo){
        perror(text);
        exit(cislo);
}
int *pocitadlo=0;
int client;
char zap = 1;
int sem_id;
int poc_klientov=0;
int sockFileDesc;
int main(int argc,char *argv[]){
    struct sockaddr_in adresa,pripojilSa;
    int port;
    signal(SIGINT, sigctrl);
    if(argc > 1){
	port = atoi(argv[1]);
    } 
    else{
	printf("Nezadali ste port serveru!!\n");
	exit(0);
    }
    socklen_t velkost;
  if((sockFileDesc=socket(AF_INET, SOCK_STREAM, 0))<0)	chyba("Chyba pri vytvarani socketu:\n",-1);

//vytvorenie parametrickej struktury
    adresa.sin_family=AF_INET;
    adresa.sin_port=htons(port);
    adresa.sin_addr.s_addr=INADDR_ANY;
//Vystavenie socketu
    if(bind(sockFileDesc, (struct sockaddr *)&adresa, sizeof(adresa))<0)	chyba("Chyba pri vystavovani socketu:\n",-2);
//Nastavenie socketu
    if(listen(sockFileDesc,2)<0)						chyba("Chyba pri nastavovani pocuvania socketu:\n",-3);
//    vytvorZP(12345);
//    pripojZP(&pdataArm);    //spoji zdielanu pamet so strukturou armData
//defaultne hodnoty
    
//-------------------------
//    sem_id = semCreate(12345,1);   //vytvor semafor
//    semInit(sem_id,0,1);
//-------------------------
    printf("Server aktivny spusteny na porte %d\n",port);
    while(zap)
    {
	if((client=accept(sockFileDesc, (struct sockaddr *)&pripojilSa, &velkost))<0) chyba("Chyba pri vytvarani spojenia:\n",-4);
	//prijme klientov`
	poc_klientov++;
	switch(fork()){ //vytvori novy proces pre klienta
		case-1: chyba("Chyba pri vytvarani noveho procesu:\n",-104);break;
		case 0: if (close(sockFileDesc)<0)	chyba("Chyba pri zatvarani socketu:\n",-5);
			int por;
			float teplota;
			struct timeval timeout;
      
			timeout.tv_sec = 10;
    			timeout.tv_usec = 0;
			if (setsockopt (client, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                	sizeof(timeout)) < 0)	error("setsockopt failed\n");

			signal(SIGINT, sigctrl1);
			signal(SIGPIPE, sigpipe);
			recv(client,&por, sizeof(por),0);
			//pripojZP(&pdataArm);	//spoji zdielanu pamet so strukturou armData	
			printf("Pripojeny klient c:%d\n",por);			
			switch(por){
				case 1:	teplota = 100;
					while(zap){//klient izba 1
						send(client,&teplota, sizeof(teplota),0);
					}break;
				case 2: teplota = 200;
					while(zap){//klient izba 2
						send(client,&teplota, sizeof(teplota),0);
					}break;
				case 3:	teplota = 300;
					while(zap){//klient izba 3
						send(client,&teplota, sizeof(teplota),0);
					}break;
				case 4:	while(zap){// klient nastavovanie vonkajsej teploty
						recv(client,&teplota, sizeof(teplota),0);
						printf("teplota : %f\n",teplota);
					}break;
			}
			if (close(client)<0)	chyba("Chyba pri zatvarani socketu:\n",-6);
	    		printf("Spojenie ukoncene klient c.%d\n",por);
	    		exit(0);					
		default:
	    		if (close(client)<0)	chyba("Chyba pri zatvarani socketu:\n",-7);
		}
    }

}
void sigctrl(int param)
{
  printf("Server sa vypina\n");
  printf("Vymazavam zdielanu pamet\n");
//  uzavriZP();
  printf("Vymazavam semafor\n");
//  semRem(sem_id);
  sleep(1);	
  if (close(sockFileDesc)<0)    chyba("Chyba pri zatvarani socketu:\n",-7);
  exit(0);
}
void sigctrl1(int param)
{
//  printf("Vypinanie klientov\n");
  zap = 0;
}
void sigpipe(int param){
zap = 0;
}

