#include "sharedMem.h"
void *shared_memory;
int shmid;
void vytvorZP(key_t kluc){
        shmid = shmget(kluc, sizeof(struct sharedStruct), 0666 | IPC_CREAT);
        if (shmid == -1){
                perror("Problem s vytvorenim zdielanej pamate\n");
                exit(1);
        }
}
void pripojZP(struct sharedStruct **name){
        shared_memory = shmat(shmid,NULL,0);
        if (shared_memory == (void *)-1){
                perror("Problem s pripojenim zdielanej pamate\n");
                exit(2);
        }
        *name = (struct sharedStruct *)shared_memory;
}
void uvolniZP(void){
        if (shmdt(shared_memory) == -1) {
                perror("Zastavenie pristupu k zdielanej pamate zlyhalo\n");
                exit(EXIT_FAILURE);
        }
}
void uzavriZP(void){
        if (shmctl(shmid, IPC_RMID, 0) == -1) {
                perror("Zmazanie zdielanej pamate zlyhalo\n");
                exit(EXIT_FAILURE);
        }
}

