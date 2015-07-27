#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "globals.h"

int running = 1;
int meshcount = 0;
int entcount = 0;
SDL_Window* window = NULL;
SDL_Renderer* render = NULL;
mesh** meshes = NULL;
ent** ents = NULL;

int main(){
	srandom(time(NULL));
	struct timespec t = {.tv_sec=0};
	struct timespec lastTime = {.tv_sec = 0, .tv_nsec = 0};
	struct timespec otherTime = {.tv_sec = 0, .tv_nsec = 0};
	SDL_Event evnt;
 
	meshes = calloc(sizeof(mesh*), 5);
	ents = calloc(sizeof(ent*), 5);
	ents[0] = malloc(sizeof(ent));
	createEnt(ents[0], 0, 0, "Francois Hollande", 6, 0, 6);
	entcount++;
	meshes[0] = malloc(sizeof(mesh));
	loadmesh("meshes/square", meshes[0], 0, 0, 10);
	meshcount++;
	meshes[1] = malloc(sizeof(mesh));
	loadmesh("meshes/square", meshes[1], -11, 0, 10);
	meshcount++;
//	meshes[2] = malloc(sizeof(mesh));
//	loadmesh("meshes/square", meshes[2], -2, -20, 30);
//	meshcount++;
//	loadmesh("planeY", &meshes[2], 0, 0, 30);
//	meshcount++;
//	meshes[0]->moves = 0;
//	meshes[1]->vy = -0.2;
//	meshes[0].mass = ;
//	meshes[1]->rot[1] = 0.25;
	meshes[0]->vx = -0.1;
	ents[0]->vx = -0.1;
	if(initView()){
		puts("SDL window became an SDL WIDOW");
	}
	int temp;
	mesh *mesh1;
	ent *ent1;
	double *temppoint;	
	while(running){
		tickAllEnts();
		tickAllObjects();
		collisions();
		for(temp = 0; temp < entcount; temp++){
			ent1 = ents[temp];
			if(!ent1->collision){
				ent1->center[0] = ent1->center2[0];
				ent1->center[1] = ent1->center2[1];
				ent1->center[2] = ent1->center2[2];
			}
			ent1->vx = ent1->vx2;
			ent1->vy = ent1->vy2;
			ent1->vz = ent1->vz2;
		}
		for(temp = 0; temp < meshcount; temp++){
			mesh1 = meshes[temp];
			if(!mesh1->collision && mesh1->moves){
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
			printcentermesh(mesh1);
		}
		drawView();

		do {
			SDL_WaitEvent(&evnt);
		} while (evnt.type != SDL_KEYDOWN && evnt.type != SDL_QUIT);
		if(evnt.type == SDL_KEYDOWN){
			if(evnt.key.keysym.sym == SDLK_LEFT){
				puts("hi");
			}
		}else if (evnt.type == SDL_QUIT){
			running = 0;
		}

/*
		while(SDL_PollEvent(&evnt)){
			if(evnt.type == SDL_QUIT){
				running = 0;
			}
		}
*/
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
