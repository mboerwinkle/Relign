#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "globals.h"
//#define ANAGLYPH

static SDL_Window* window;
static SDL_Renderer* render;
static int running = 1;

static void paint(){
	SDL_RenderPresent(render);
}

int main(){
	srandom(time(NULL));
	struct timespec t = {.tv_sec=0};
	struct timespec lastTime = {.tv_sec = 0, .tv_nsec = 0};
	struct timespec otherTime = {.tv_sec = 0, .tv_nsec = 0};
 
	mesh *square, *tetrahedron, *meshes[2];
	square = malloc(sizeof(mesh));
	tetrahedron = malloc(sizeof(mesh));
	loadmesh("square", square, 10, 0, 100);
	loadmesh("square", tetrahedron, -10, 0, 100);
	meshes[0] = square;
	meshes[1] = tetrahedron;
	tetrahedron->rot[0] = 0;
	tetrahedron->rot[1] = 0;
	tetrahedron->rot[2] = 1;
	tetrahedron->vx = 0.02;
	square->rot[0] = 0;
	square->rot[1] = 0;
	square->rot[2] = 0;
	square->vy = 0.0;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	window = SDL_CreateWindow("Relign - Wunderbar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, 0);
	if(window == NULL){
		SDL_Quit();
		return 1;
	}
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	int temp, raycounter;
	double x, y, z, x1, y1, z1, x2, y2, z2;
	mesh *mesh1;
	double *col;
	while(running){
		col = collisions(square, tetrahedron);
		for(temp = 0; temp < 2; temp++){
			mesh1 = meshes[temp];
			movemesh(mesh1);
			x = mesh1->centermass[0];
			y = mesh1->centermass[1];
			z = mesh1->centermass[2];
			#ifdef ANAGLYPH
			for(raycounter = mesh1->rays-1; raycounter >= 0; raycounter--){
				x1 = x + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]*3+0];
				y1 = y + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]*3+1];
				z1 = z + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]*3+2];
				x2 = x + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]*3+0];
				y2 = y + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]*3+1];
				z2 = z + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]*3+2];
				SDL_SetRenderDrawColor(render, 255, 0, 0, 125);
				SDL_RenderDrawLine(render, (int)((100*x1-50)/z1)+250, (int)(100*y1/z1)+250, (int)((100*x2-50)/z2)+250, (int)(100*y2/z2)+250);
				SDL_SetRenderDrawColor(render, 0, 0, 255, 125);
				SDL_RenderDrawLine(render, (int)((100*x1+50)/z1)+250, (int)(100*y1/z1)+250, (int)((100*x2+50)/z2)+250, (int)(100*y2/z2)+250);
			}
			SDL_SetRenderDrawColor(render, 255, 0, 0, 125);
			SDL_RenderDrawPoint(render, (int)((100*x-50)/z+250), (int)(y*100/z+250));
			SDL_SetRenderDrawColor(render, 0, 0, 255, 125);
			SDL_RenderDrawPoint(render, (int)((100*x+50)/z+250), (int)(y*100/z+250));
			#else
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
			#endif
		}
		if(col != NULL){
			SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
			SDL_RenderDrawLine(render, (int)(col[0]*100/col[2])+250, (int)(col[1]*100/col[2])+250, (int)((col[0]+col[3]*100)*100/(col[2]+col[5]*100)+250), (int)((col[1]+col[4]*100)*100/(col[2]+col[5]*100)+250));
//			printf("%lf %lf\n", ((col[0]+col[3])*100/(col[2]+col[5])+250), ((col[1]+col[4])*100/(col[2]+col[5])+250));
			free(col);
		}
		paint();
		#ifdef ANAGLYPH
		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);//you need a white background for anaglyph...
		#else
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		#endif
		SDL_RenderClear(render);
		SDL_Event evnt;
//		do {
			SDL_PollEvent(&evnt);
//		} while (evnt.type != SDL_KEYDOWN && evnt.type != SDL_QUIT);
		if (evnt.type == SDL_QUIT){
			running = 0;
		}
		clock_gettime(CLOCK_MONOTONIC, &otherTime);
		int32_t sleep = (int32_t)25000000 - (otherTime.tv_nsec-lastTime.tv_nsec) - 1000000000l*(otherTime.tv_sec-lastTime.tv_sec);
		if(sleep > 0){
			t.tv_nsec = sleep;
			nanosleep(&t, NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &lastTime);
	}
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
