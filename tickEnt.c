#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void tickAllEnts(){
	ent* target;
		SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
	double head[3];
	for(int temp = entcount-1; temp >= 0; temp--){
		target = ents[temp];
		head[0] = target->center[0];
		head[1] = target->center[1]+target->height;
		head[2] = target->center[2];
		drawLine(target->center, head);
		
	}
}

void tickEnt(ent *target){
}
void moveEnt(ent *target, int dir){
}
void jumpEnt(ent *target){
}
void lookhorizEnt(ent *target, int howmuch){
}

