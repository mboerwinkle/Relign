#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void tickAllObjects(){
	int temp;
	mesh *mesh1;
	for(temp = 0; temp < meshcount; temp++){
		mesh1 = meshes[temp];
		if(mesh1->moves){
			mesh1->vz2 = mesh1->vz * DRAG;
			mesh1->vx2 = mesh1->vx * DRAG;
			mesh1->vy2 = mesh1->vy * DRAG;
			mesh1->rot2[0] = mesh1->rot[0] * DRAG;
			mesh1->rot2[1] = mesh1->rot[1] * DRAG;
			mesh1->rot2[2] = mesh1->rot[2] * DRAG;
			applyForce(mesh1, 0, 0, 0, 0, 0, 1, GRAVITY/FRAMERATE);
			movemesh(mesh1);
		}
	}
}
