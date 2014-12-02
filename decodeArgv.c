#include "decodeArgv.h"
char decodeArgv(int argc,char *argv[],char **ip,int *port){
       if(ip!=NULL){
		*ip = (char *)malloc(sizeof(char)*10);
		strcpy(*ip,"localhost");
	}
        *port = 1212;
	if(argc >1){
		int i;
		char *port_s;
		for (i=1;i<argc;i++){
			if(argv[i][0] == '-'){
				switch(argv[i][1]){
					case 'i':*ip = (char *)malloc(sizeof(char)*(sizeof(argv[i])-2));
                                 		 strncpy(*ip,argv[i]+=2,strlen(argv[i])-2);
				 	break;
					case 'p':port_s = (char *)malloc(sizeof(char)*(sizeof(argv[i])-2));
                                 		 strncpy(port_s,argv[i]+=2,strlen(argv[i])-2);
						 *port = atoi(port_s);
						 port_s = NULL;
                				 free(port_s);
				 		 break;
					default:if(ip!= NULL){	printf("\n\tPouzitie: %s -i[IP adresa] -p[Port]\n",argv[0]);
						 		printf("\tPre nastavenie IP adresy pouzite tvar\t\t -i192.168.1.1\n");
						}
						else printf("\n\tPouzitie: %s -p[Port]\n",argv[0]);
						printf("\tPre nastavenie portu na 1212 pouzite tvar\t -p1212\n\n");
						return 0;
					}
			}
			else{
				printf("\tBoli zadane neplatne argumenty\n");
				return 0;
			}
		}
	}
	return 1;
}

