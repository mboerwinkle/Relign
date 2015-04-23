#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void addForce(force **root, mesh *target, double x, double y, double z, double dirx, double diry, double dirz, double power){
	force *temp;
	temp = malloc(sizeof(force));
	temp->x = x;
	temp->y = y;
	temp->z = z;
	temp->dirx = dirx;
	temp->diry = diry;
	temp->dirz = dirz;
	temp->power = power;
	temp->target = target;
	temp->next = *root;
	root = temp;
}
	
