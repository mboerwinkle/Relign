#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void applyForce(mesh *target, double x, double y, double z, double dirx, double diry, double dirz, double power){
	if(!target->moves) puts("applying a force to something that doesn't move...?");
	double loc[3] = {x, y, z};
	double vec[3] = {dirx, diry, dirz};
	norm(vec);
	vec[0] *= power;
	vec[1] *= power;
	vec[2] *= power;
//http://docs.unity3d.com/Manual/ComputingNormalPerpendicularVector.html
	double push[3] = {-x, -y, -z};
	norm(push);
	double rotend[3]; //vector from the center (0, 0) to root->loc+rotation. used for finding the normal.
	double pushpower = DOT(push, vec);
	push[0] *= pushpower;
	push[1] *= pushpower;
	push[2] *= pushpower;
	double rotation[3] = {vec[0]-push[0], vec[1]-push[1], vec[2]-push[2]}; 
	double rotationpower = sqrt(rotation[0]*rotation[0]+rotation[1]*rotation[1]+rotation[2]*rotation[2]);//the length of the rotation force vector
	rotationpower *= target->radius/sqrt(loc[0]*loc[0]+loc[1]*loc[1]+loc[2]*loc[2]);//the distance from the center to the start of the force vector
	rotend[0] = loc[0] + rotation[0];
	rotend[1] = loc[1] + rotation[1];
	rotend[2] = loc[2] + rotation[2];
//	CROSS(rotation, rotend, loc);//now rotation is style quaternion, not force
	CROSS(rotation, loc, rotend);//now rotation is style quaternion, not force
	norm(rotation);
	rotation[0] *= rotationpower;
	rotation[1] *= rotationpower;
	rotation[2] *= rotationpower;
	target->vx2 += push[0];
	target->vy2 += push[1];
	target->vz2 += push[2];
	target->rot2[0] += rotation[0];
	target->rot2[1] += rotation[1];
	target->rot2[2] += rotation[2];

	double absloc[3] = {loc[0] + target->centermass[0], loc[1] + target->centermass[1], loc[2] + target->centermass[2]};
	double absend[3] = {absloc[0]+vec[0]*10, absloc[1]+vec[1]*100, absloc[2]+vec[2]*100};
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	drawLine(absloc, absend);
}
