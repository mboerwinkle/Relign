#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void printcentermesh(mesh *target){
	printf("mesh %p center: %lf %lf %lf\n", target, target->centermass[0], target->centermass[1], target->centermass[2]);
}

void printvelocitymesh(mesh *target){
	printf("mesh %p velocity: %lf %lf %lf\n", target, target->vx, target->vy, target->vz);
}
