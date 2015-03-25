#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "globals.h"

extern SDL_Window* window;
extern SDL_Renderer* render;
extern int running;
extern int meshcount;

static void paint(){
	SDL_RenderPresent(render);
}

int initView(){
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	window = SDL_CreateWindow("Relign - Righteous.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, 0);
	if(window == NULL){
		SDL_Quit();
		return 1;
	}
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	return 0;
}

void drawView(){
	int temp, raycounter;
	double x, y, z, x1, y1, z1, x2, y2, z2;
	mesh * mesh1;

	for(temp = 0; temp < meshcount; temp++){
		mesh1 = &meshes[temp];
		x = mesh1->centermass[0];
		y = mesh1->centermass[1];
		z = mesh1->centermass[2];
		SDL_SetRenderDrawColor(render, 0, 100, 10, 100);
		for(raycounter = mesh1->rays-1; raycounter >= 0; raycounter--){
			x1 = x + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]*3+0];
			y1 = y + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]*3+1];
			z1 = z + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]*3+2];
			x2 = x + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]*3+0];
			y2 = y + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]*3+1];
			z2 = z + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]*3+2];
			SDL_RenderDrawLine(render, (int)(100*x1/z1)+250, (int)(100*y1/z1)+250, (int)(100*x2/z2)+250, (int)(100*y2/z2)+250);
		}
		SDL_SetRenderDrawColor(render, 100, 100, 255, 255);
		SDL_RenderDrawPoint(render, (int)(x*100/z+250), (int)(y*100/z+250));
	}
	paint();
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);
}
