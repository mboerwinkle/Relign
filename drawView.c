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
	window = SDL_CreateWindow("Relign - Righteous - Radical - Refined.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
	if(window == NULL){
		SDL_Quit();
		return 1;
	}
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	return 0;
}

void drawLine(double one[3], double two[3]){
	if (one[2] < 0 && two[2] < 0)
		return;
	if (one[2] < 0) { // We only want to handle one case, so switch the points quite ungracefully.
		double *tmp;
		tmp = one;
		one = two;
		two = tmp;
	}
	if (one[3] < 0) { // In this case, we want to trace *away* from the projection of point 2, to the edge of the screen.
		int sx = (int)(VIEWZOOM*one[0]/one[2])+0.5*WIDTH;
		int sy = (int)(VIEWZOOM*one[1]/one[2])+0.5*HEIGHT;
		int sx2 = (int)(VIEWZOOM*two[0]/two[2])+0.5*WIDTH;
		int sy2 = (int)(VIEWZOOM*two[1]/two[2])+0.5*HEIGHT;
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
		SDL_RenderDrawLine(render, (int)(VIEWZOOM*one[0]/one[2])+0.5*WIDTH, (int)(VIEWZOOM*one[1]/one[2])+0.5*HEIGHT, (int)(VIEWZOOM*two[0]/two[2])+0.5*WIDTH, (int)(VIEWZOOM*two[1]/two[2])+0.5*HEIGHT);
	}
}

void drawView(){
	int temp, raycounter;
	double x, y, z, one[3], two[3];
	mesh * mesh1;

	for(temp = 0; temp < meshcount; temp++){
		mesh1 = meshes[temp];
		x = mesh1->centermass[0];
		y = mesh1->centermass[1];
		z = mesh1->centermass[2];
		SDL_SetRenderDrawColor(render, 0, 100, 10, 100);
		for(raycounter = mesh1->rays-1; raycounter >= 0; raycounter--){
			one[0] = x + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]*3+0];
			one[1] = y + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]*3+1];
			one[2] = z + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]*3+2];
			two[0] = x + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]*3+0];
			two[1] = y + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]*3+1];
			two[2] = z + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]*3+2];
			drawLine(one, two);
		}
		if(z > 0){
			SDL_SetRenderDrawColor(render, 100, 100, 255, 255);
			SDL_RenderDrawPoint(render, (int)(VIEWZOOM*x/z+0.5*WIDTH), (int)(VIEWZOOM*y/z+0.5*HEIGHT));
		}
	}
	paint();
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);
}
