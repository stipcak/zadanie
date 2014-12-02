#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "robotArm.h"
#include "socket.h"
#include "decodeArgv.h"
#include "timer.h"
#include <pthread.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

struct RobotArm dataArm=RobotArm_default;
struct RobotArm *pdataArm=&dataArm;
int mod=3;
char buff[15];
char prem[15];
char zap=1;
void socketAprint (int signal , siginfo_t * siginfo, void * ptr);
void readKey();
void sigpipe(int param);
void sigend(int param);

int main(int argc,char *argv[]){
	int port;
	char *ip;
	timer_t casovac;
 	if(decodeArgv(argc,argv,&ip,&port)){
		int sock = pripoj(ip,&port);
		int *por=(int*)malloc(sizeof(int));
		*por=3;
		signal(SIGINT, sigend);
		signal(SIGPIPE, sigpipe);
		odosliInt(&(*por));
		printf("Klient c.%d pripojeny(%s,%d)\n",*por,ip,port);
                free(por);
		buff[0]='\0';
		usleep(1000);
		//-------------
  		funkciaCasovac(SIGUSR2,socketAprint);
  		casovac=vytvorCasovac(SIGUSR2);
  		opakovanyCasovac(casovac,100);
		//------------
		while(zap){
			readKey();
			usleep(50);
		}
		uzavri();
		return 0;
	}
}
int kbhit(void){
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}

const char *x[] = {"r0","r1","x2","y2","rych0","rych1","x0","y0"};
void readKey(){
	if (kbhit()){
		char znak = getchar();
		if(mod != -1 && mod != 3 && mod != -2){
			int i;
			if(znak!='\n' && znak != 127)	sprintf(buff,"%s%c",buff,znak);
			else if(znak == '\n'){
				if(mod == 0){
					mod = -1;
					sprintf(prem,"%s",buff);	
					for(i=0;i<8;i++){ 
						if(!strcmp(buff,x[i])){
							mod = 1;
							break;
						}
						else if(!strcmp(buff,"odosli")){
							if(angleArmEnd(&dataArm)==0)	mod = 4;
							else				mod = 0;
							break;
						}
						else if(!strcmp(buff,"pokracuj")){
                                                        mod = 3;
                                                }

					}
				}
				else{
					mod = 0;
					if (strcmp(buff,"")){
						if(!strcmp(prem,x[0]))		dataArm.r0 = atof(buff);
						else if(!strcmp(prem,x[1]))  	dataArm.r1 = atof(buff);
						else if(!strcmp(prem,x[2]))  	dataArm.x2 = atof(buff);
						else if(!strcmp(prem,x[3]))  	dataArm.y2 = atof(buff);
						else if(!strcmp(prem,x[4])){  	
							
							if(atof(buff) > 0.5) 	mod = -2;
							else 			dataArm.rych0 = atof(buff);
						}
						else if(!strcmp(prem,x[5])){
							if(atof(buff) > 0.5) 	mod = -2;
							else			dataArm.rych1 = atof(buff);
						} 
						else if(!strcmp(prem,x[6]))     dataArm.x0 = atof(buff);
						else if(!strcmp(prem,x[7]))     dataArm.y0 = atof(buff);
					}
					else mod = -1;
				}
				buff[0]='\0';
			}
			else if(znak == 127) buff[strlen(buff)-1]='\0';
		}
		else	mod = 0;
	}
}
int i=0;
void vypis(){
        printf("Stale hodnoty\n");
	printf("r0:\t\t%f\tr1:\t%f\n",pdataArm->r0,pdataArm->r1);
	printf("rych0:\t\t%f\trych1:\t%f\n",pdataArm->rych0,pdataArm->rych1);
	printf("x0:\t\t%f\ty0:\t%f\n",pdataArm->x0,pdataArm->y0);
	printf("x1:\t\t%f\ty1:\t%f\n",pdataArm->x1,pdataArm->y1);
	printf("x2:\t\t%f\ty2:\t%f\n",pdataArm->x2,pdataArm->y2);
	printf("alfa:\t\t%f\tbeta:\t%f\n",pdataArm->alfa,pdataArm->beta);
	printf("\nAktualne hodnoty\n");
	printf("actX1:\t\t%f\tactY1:\t\t%f\n",pdataArm->actX1,pdataArm->actY1);
	printf("actX2:\t\t%f\tactY2:\t\t%f\n",pdataArm->actX2,pdataArm->actY2);
	printf("actAlfa:\t%f\tactBeta:\t%f\n",pdataArm->actAlfa,pdataArm->actBeta);
	printf("\nJe mozne prestavit tieto parametre ");
	for(i=0;i<8;i++) printf("%s, ",x[i]);
	printf("\nK dispozicii su tieto prikazy: odosli, pokracuj\n\n");
	if (mod ==0)            printf("Vyber premennu:%s",buff);
        else if(mod==1)         printf("Nastav premennu %s na:%s",prem,buff);
	else if(mod==-2)	printf("Rychlost moze byt maximalne 0.5 rad/s");
        else if(mod==-1)        printf("Nebola zadana platna premenna alebo hodnota");
        else                    printf("Stlac nejaku klavesu pre nastavenia");
}
int test;
void socketAprint(int signal , siginfo_t * siginfo, void * ptr)
{
  switch (signal)
  {
    case SIGUSR2:
      if(mod==4){
	odosliInt(&mod);
	odosliRobotArm(&dataArm);
      	mod=3;
      }
      else if(mod==3){
        odosliInt(&mod);
      	nacitajRobotArm(&dataArm);
      }
      printf("\033[2J");
      printf("\033[H");
      vypis();
      break;
  }
}
void sigpipe(int param){
printf("\nServer bol neocakavane zruseny\n");
zap=0;
}
void sigend(int param){
zap = 0;
}

