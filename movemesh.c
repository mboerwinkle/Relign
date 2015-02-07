#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void movemesh(mesh *target){
	int temp;
	for(temp = target->points-1; temp >= 0; temp--){
		target->pointmatrix[temp*3+0] = target->cpointmatrix[temp*3+0];
		target->pointmatrix[temp*3+1] = target->cpointmatrix[temp*3+1];
		target->pointmatrix[temp*3+2] = target->cpointmatrix[temp*3+2];
	}
	target->centermass[0] += target->vx;
	target->centermass[1] += target->vy;
	target->centermass[2] += target->vz;
//	printf("%d %d %d\n", target->centermass.x, target->centermass.y, target->centermass.z);
}
