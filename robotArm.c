#include "robotArm.h"
#define na2(a) (a*a)

void initRobotArm(struct RobotArm *data){
	int i;
	float *x = &data->r0;
	for(i=0;i<18;i++){
		*(x+i)=0;
	}
}
char angleArmEnd(struct RobotArm *data){
data->x2=data->x2-data->x0;
data->y2=data->y2-data->y0;
float c2 = (na2(data->x2)+na2(data->y2)-na2(data->r0)-na2(data->r1))/(2*(data->r0)*(data->r1));
if ((1-na2(c2)) >=0){
	float s2 = sqrt(1-na2(c2));
	float K1 = data->r0+data->r1*c2;
	float K2 = data->r1*s2;
    	if (data->x2 > 0){
		data->x2=-data->x2;
		data->alfa = atan2(data->y2,data->x2)-atan2(K2,K1);
    		data->beta = atan2(s2,c2);
    	       	data->alfa = 3.14-data->alfa;
        	data->beta = -data->beta;
		data->x2=-data->x2;
    	}
	else{
		data->alfa = atan2(data->y2,data->x2)-atan2(K2,K1);
                data->beta = atan2(s2,c2);
	}
    	data->x1 = cos(data->alfa)*data->r0+data->x0;
    	data->y1 = sin(data->alfa)*data->r0+data->y0;
    	data->x2 = data->x2+data->x0;
    	data->y2 = data->y2+data->y0;
    	return 0;
}
else{	
	data->x2 = data->x2+data->x0;
	data->y2 = data->y2+data->y0;
	return -1;
}
}
void angleArmAct(struct RobotArm *data){
	data->actX1 = cos(data->actAlfa)*(data->r0)+(data->x0);
	data->actY1 = sin(data->actAlfa)*(data->r0)+(data->y0);
	data->actX2 = cos((data->actAlfa)+(data->actBeta))*(data->r1)+(data->actX1);
	data->actY2 = sin((data->actAlfa)+(data->actBeta))*(data->r1)+(data->actY1);
}
