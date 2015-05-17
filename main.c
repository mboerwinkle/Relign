#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "globals.h"

int running = 1;
int meshcount = 0;
SDL_Window* window = NULL;
SDL_Renderer* render = NULL;
mesh* meshes = NULL;

int main(){
	srandom(time(NULL));
	struct timespec t = {.tv_sec=0};
	struct timespec lastTime = {.tv_sec = 0, .tv_nsec = 0};
	struct timespec otherTime = {.tv_sec = 0, .tv_nsec = 0};
	SDL_Event evnt;
 
	meshes = calloc(sizeof(mesh), 5);
	loadmesh("square", &meshes[0], 2, 2, 20);
	meshcount++;
	loadmesh("square", &meshes[1], 0, 0, 10);
	meshcount++;
	loadmesh("square", &meshes[2], 10.5, -5, 10);
	meshcount++;
//	meshes[0].vx = -0.1;
//	meshes[0].rot[2] = 0.05;
//	meshes[2].rot[2] = 0.1;
	if(initView()){
		puts("SDL window became an SDL WIDOW");
	}
	int temp, temp2;
	mesh *mesh1;
	double *temppoint;	
	while(running){
		for(temp = 0; temp < meshcount; temp++){
			mesh1 = &meshes[temp];
			if(mesh1->moves){
				mesh1->vz2 = (mesh1->vz - GRAVITY/FRAMERATE) * DRAG;
				mesh1->vx2 = mesh1->vx * DRAG;
				mesh1->vy2 = mesh1->vy * DRAG;
				mesh1->rot2[0] = mesh1->rot[0] * DRAG;
				mesh1->rot2[1] = mesh1->rot[1] * DRAG;
				mesh1->rot2[2] = mesh1->rot[2] * DRAG;
				movemesh(mesh1);
//				printf("V %lf %lf %lf\n", mesh1->vx, mesh1->vy, mesh1->vz);
			}
		}
		for(temp = 0; temp < meshcount; temp++){
			mesh1 = &meshes[temp];
			mesh1->collision = 0;
			for(temp2 = temp-1; temp2 >= 0; temp2--){
				if(collisions(mesh1, &meshes[temp2])){
					mesh1->collision = 1;
					meshes[temp2].collision = 1;
				}
			}
		}
		for(temp = 0; temp < meshcount; temp++){
			mesh1 = &meshes[temp];
//			if(!mesh1->collision){
				temppoint = mesh1->pointmatrix;
				mesh1->pointmatrix = mesh1->pointmatrix2;
				mesh1->pointmatrix2 = temppoint;
				temppoint = mesh1->centermass;
				mesh1->centermass = mesh1->centermass2;
				mesh1->centermass2 = temppoint;
//			}
			temppoint = mesh1->rot;
			mesh1->rot = mesh1->rot2;
			mesh1->rot2 = temppoint;
			mesh1->vx = mesh1->vx2;
			mesh1->vy = mesh1->vy2;
			mesh1->vz = mesh1->vz2;
		}
		drawView();

		do {
			SDL_PollEvent(&evnt);
		} while (evnt.type != SDL_KEYDOWN && evnt.type != SDL_QUIT);
		if(evnt.type == SDL_KEYDOWN){
			if(evnt.key.keysym.sym == SDLK_LEFT){
				puts("hi");
			}
		}else if (evnt.type == SDL_QUIT){
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
