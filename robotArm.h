#ifndef _ROBOTARM_H
#define _ROBOTARM_H
#define RobotArm_default {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#include <math.h>
#include <stdio.h>
struct RobotArm{	
float r0,r1;
float x0,y0;
float x1,y1;
float x2,y2;
float alfa,beta;
float actX1,actY1;
float actX2,actY2;
float actAlfa,actBeta;
float rych0,rych1;
};
struct act{
        float datX1;
        float datY1;
        float datX2;
        float datY2;
};
void initRobotArm(struct RobotArm *data);
char angleArmEnd(struct RobotArm *data);
void angleArmAct(struct RobotArm *data);
#endif
