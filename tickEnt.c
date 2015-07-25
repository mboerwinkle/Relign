#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void tickAllEnts(){
	ent* target;
	SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
	for(int temp = entcount-1; temp >= 0; temp--){
		target = ents[temp];
		double x = target->center[0];
		double y = target->center[1];
		double z = target->center[2];
		if(z > 0){
			SDL_RenderDrawPoint(render, (int)(VIEWZOOM*x/z+0.5*WIDTH), (int)(VIEWZOOM*y/z+0.5*HEIGHT));
		}
		target->aiFunc(target);	
		moveEnt(target);
	}
}

