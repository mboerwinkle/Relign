#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "ents.h"

void createEnt(ent *final, int type, int aitype, char name[20], double x, double y, double z){
	memset(final, 0, sizeof(ent));
	final->center[0] = x;
	final->center[1] = y;
	final->center[2] = z;
	final->center2[0] = x;
	final->center2[1] = y;
	final->center2[2] = z;
	final->height = 5;
	if(aitype == 0) final->data = calloc(sizeof(dataUser), 1);
	final->aiFunc = &ai0user;
}
