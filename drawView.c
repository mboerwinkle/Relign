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
	window = SDL_CreateWindow("Relign - Righteous - Radical.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, 0);
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
			if (z1 < 0 && z2 < 0)
				continue;
			if (z1 < 0) { // We only want to handle one case, so switch the points quite ungracefully.
				double tmp = z1;
				z1 = z2;
				z2 = tmp;
				tmp = x1;
				x1 = x2;
				x2 = tmp;
				tmp = y1;
				y1 = y2;
				y2 = tmp;
			}
			if (z2 < 0) { // In this case, we want to trace *away* from the projection of point 2, to the edge of the screen.
				int sx = (int)(100*x1/z1)+250;
				int sy = (int)(100*y1/z1)+250;
				int sx2 = (int)(100*x2/z2)+250;
				int sy2 = (int)(100*y2/z2)+250;
				int dx = sx - sx2; //dx and dy are the vector along which we trace.
				int dy = sy - sy2;
				int factor;
				if (abs(dx) > abs(dy)) { // We'll cover 500 pixels in the x direction first.
					factor = (500/abs(dx)) + 1; // Multiply both by the same factor, which ensures at least 500 pixels of ground covered.
				} else {
					factor = (500/abs(dy)) + 1;
				}
				dx *= factor;
				dy *= factor;
				SDL_RenderDrawLine(render, sx, sy, sx+dx, sy+dy);
			} else {
				SDL_RenderDrawLine(render, (int)(100*x1/z1)+250, (int)(100*y1/z1)+250, (int)(100*x2/z2)+250, (int)(100*y2/z2)+250);
			}
		}
		SDL_SetRenderDrawColor(render, 100, 100, 255, 255);
		SDL_RenderDrawPoint(render, (int)(x*100/z+250), (int)(y*100/z+250));
	}
	paint();
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);
}
