#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void createEnt(ent *final, char name[20], double x, double y, double z){
	final->center[0] = x;
	final->center[1] = y;
	final->center[2] = z;
	final->center2[0] = x;
	final->center2[1] = y;
	final->center2[2] = z;
	final->vx = 0;
	final->vy = 0;
	final->vz = 0;
	final->vx2 = 0;
	final->vy2 = 0;
	final->vz2 = 0;
	final->vdir = 0;
	final->height = 5;
	final->data = malloc(sizeof(dataUser));
}
