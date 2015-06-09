#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void tickAllEnts(){
	ent* target;
		SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
	for(int temp = entcount-1; temp >= 0; temp--){
		tickEnt(ents[temp]);
	}
}

void tickEnt(ent *target){
	double head[3];
	head[0] = target->center[0];
	head[1] = target->center[1]+target->height;
	head[2] = target->center[2];
	drawLine(target->center, head);
	target->aiFunc(target);
	target->typeFunc(target);
	
}

