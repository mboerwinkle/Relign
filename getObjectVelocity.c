#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "globals.h"
void getObjectVelocity(mesh *target, double colloc[3], double colforce[3]){
	double rotationspeed;
	colforce[0] = 0+target->vx;
	colforce[1] = 0+target->vy;
	colforce[2] = 0+target->vz;
	double uv[3];
	double *rot;
	rot = target->rot;
	if(!(rot[0] == 0 && rot[1] == 0 && rot[2] == 0)){
		rotationspeed = sqrt(rot[0]*rot[0]+rot[1]*rot[1]+rot[2]*rot[2]);
//		rotationspeed *= (distance(rot, colloc1)/one->radius);//speed of rotation of collision point
		CROSS(uv, rot, colloc);
		//CROSS(uv, colloc1, rot);
		norm(uv);
		colforce[0] += uv[0]*rotationspeed;
		colforce[1] += uv[1]*rotationspeed;
		colforce[2] += uv[2]*rotationspeed;
		if(isnan(rotationspeed) || !isfinite(rotationspeed)) puts("rotationspeed1 error");
	}
}
