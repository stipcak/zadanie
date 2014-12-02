#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "robotArm.h"
#define sharedStruct RobotArm
void vytvorZP(key_t kluc);
void pripojZP(struct sharedStruct **name);
void uvolniZP(void);
void uzavriZP(void);
