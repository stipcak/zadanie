#ifndef _SOCKET_H
#define _SOCKET_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "robotArm.h"
int pripoj(char *ip,int *port);
void odosliRobotArm(struct RobotArm *buff);
void nacitajRobotArm(struct RobotArm *buff);
void odosliFloat(float *buff);
void nacitajFloat(float *buff);
void odosliInt(int *buff);
void nacitajInt(int *buff);
void uzavri(void);
#endif
