#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "globals.h"

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
 
	mesh *meshes;
	int meshcount = 0;
	meshes = calloc(sizeof(mesh), 5);
	loadmesh("square", &meshes[1], 0, 0, 30);
	meshcount++;
	loadmesh("ground", &meshes[0], 0, 0, 50);
	meshcount++;
	meshes[0].moves = 0;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	window = SDL_CreateWindow("Relign - Wunderbar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, 0);
	if(window == NULL){
		SDL_Quit();
		return 1;
	}
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	int temp, temp2, raycounter;
	double x, y, z, x1, y1, z1, x2, y2, z2;
	mesh *mesh1;
	double *col;
	double *temppoint;
	while(running){
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		SDL_RenderClear(render);
		for(temp = 0; temp < meshcount; temp++){
			mesh1 = &meshes[temp];
			movemesh(mesh1);
			mesh1->collision = 0;
			for(temp2 = temp-1; temp2 >= 0; temp2--){
				col = collisions(mesh1, &meshes[temp2]);
				if(col != NULL){
					SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
					SDL_RenderDrawLine(render, (int)(col[0]*100/col[2])+250, (int)(col[1]*100/col[2])+250, (int)((col[0]+col[3]*100)*100/(col[2]+col[5]*100)+250), (int)((col[1]+col[4]*100)*100/(col[2]+col[5]*100)+250));
//					printf("%lf %lf\n", ((col[0]+col[3])*100/(col[2]+col[5])+250), ((col[1]+col[4])*100/(col[2]+col[5])+250));
					free(col);
					col = NULL;
					mesh1->collision = 1;
					meshes[temp2].collision = 1;
				}
			}
		}
		for(temp = 0; temp < meshcount; temp++){
			mesh1 = &meshes[temp];
			if(mesh1->moves){
				mesh1->vz2 = (mesh1->vz - GRAVITY/FRAMERATE) * DRAG;
				mesh1->vx2 = mesh1->vx * DRAG;
				mesh1->vy2 = mesh1->vy * DRAG;
				mesh1->rot2[0] = mesh1->rot[0] * DRAG;
				mesh1->rot2[1] = mesh1->rot[1] * DRAG;
				mesh1->rot2[2] = mesh1->rot[2] * DRAG;
			}
		}
		for(temp = 0; temp < meshcount; temp++){
			mesh1 = &meshes[temp];
			if(mesh1->collision == 0){
				temppoint = mesh1->pointmatrix;
				mesh1->pointmatrix = mesh1->pointmatrix2;
				mesh1->pointmatrix2 = temppoint;

				temppoint = mesh1->centermass;
				mesh1->centermass = mesh1->centermass2;
				mesh1->centermass2 = temppoint;
			}
			temppoint = mesh1->rot;
			mesh1->rot = mesh1->rot2;
			mesh1->rot2 = temppoint;

			mesh1->vx = mesh1->vx2;
			mesh1->vy = mesh1->vy2;
			mesh1->vz = mesh1->vz2;
			

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
		SDL_Event evnt;
		do {
			SDL_PollEvent(&evnt);
		} while (evnt.type != SDL_KEYDOWN && evnt.type != SDL_QUIT);
		if (evnt.type == SDL_QUIT){
			running = 0;
		}
		clock_gettime(CLOCK_MONOTONIC, &otherTime);
		int32_t sleep = (int32_t)(1000000000/FRAMERATE) - (otherTime.tv_nsec-lastTime.tv_nsec) - 1000000000l*(otherTime.tv_sec-lastTime.tv_sec);
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
