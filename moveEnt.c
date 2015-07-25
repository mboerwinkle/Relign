#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void moveEnt(ent *target){
	target->center2[0] = target->center[0]+target->vx;
	target->center2[1] = target->center[1]+target->vy;
	target->center2[2] = target->center[2]+target->vz;
}
