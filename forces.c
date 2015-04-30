#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void addForce(force **root, mesh *target, double x, double y, double z, double dirx, double diry, double dirz, double power){
	force *temp;
	temp = malloc(sizeof(force));
	temp->loc[0] = x;
	temp->loc[1] = y;
	temp->loc[2] = z;
	temp->dir[0] = dirx;
	temp->dir[1] = diry;
	temp->dir[2] = dirz;
	temp->power = power;
	temp->target = target;
	temp->next = *root;
	root = temp;
}

void applyForces(force *root){//http://docs.unity3d.com/Manual/ComputingNormalPerpendicularVector.html
	double velocity[3];
	double rotation[3];
	double rotend[3]; //vector from the center (0, 0) to root->loc+rotation. used for finding the normal.
	double speed, rotationspeed;
	while(root != NULL){
		if(root->target->moves) puts("applying a force to something that doesn't move...?");
		velocity[0] = -root->loc[0];
		velocity[1] = -root->loc[1];
		velocity[2] = -root->loc[2];
		norm(velocity);
		speed = DOT(velocity, root->dir);
		velocity[0] *= speed;
		velocity[1] *= speed;
		velocity[2] *= speed;
		rotation[0] = root->dir[0]-velocity[0];
		rotation[1] = root->dir[1]-velocity[1];
		rotation[2] = root->dir[2]-velocity[2];
		root->target->vx2 += velocity[0];
		root->target->vy2 += velocity[1];
		root->target->vz2 += velocity[2];
		rotationspeed = sqrt(rotation[0]*rotation[0]+rotation[1]*rotation[1]+rotation[2]*rotation[2]);
		rotend[0] = root->loc[0] + rotation[0];
		rotend[1] = root->loc[1] + rotation[1];
		rotend[2] = root->loc[2] + rotation[2];
		CROSS(rotation, rotend, root->loc);//now rotation is style quaternion, not force
		root->target->rot2[0] += rotation[0];
		root->target->rot2[1] += rotation[1];
		root->target->rot2[2] += rotation[2];
		root = root->next;
	}
}
