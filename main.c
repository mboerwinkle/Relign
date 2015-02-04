#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "globals.h"

static SDL_Window* window;
static SDL_Renderer* render;
static int running = 1;

static void paint(){
	SDL_RenderPresent(render);
}

int main(){
//
	mesh *square, *tetrahedron, *meshes[2];
	square = malloc(sizeof(mesh));
	tetrahedron = malloc(sizeof(mesh));
	loadmesh("square", square, 0, 0, 1000);
	loadmesh("mesh1", tetrahedron, 0, 0, 1000);
	square->vx = 5;
	tetrahedron->vy = 10;
	meshes[0] = square;
	meshes[1] = tetrahedron;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	window = SDL_CreateWindow("Relign, BITCHES! Finally used that name! BLAM!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, 0);
	if(window == NULL){
		SDL_Quit();
		return 1;
	}
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	int temp, raycounter, x, y, z, x1, y1, z1, x2, y2, z2;
	mesh *mesh1;
	while(running){
		for(temp = 0; temp < 2; temp++){
			mesh1 = meshes[temp];
			movemesh(mesh1);
			x = mesh1->centermass.x;
			y = mesh1->centermass.y;
			z = mesh1->centermass.z;
			SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
			for(raycounter = mesh1->rays-1; raycounter >= 0; raycounter--){
				x1 = x + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]].x;
				y1 = (y + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]].y);
				z1 = z + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]].z;
				x2 = x + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]].x;
				y2 = (y + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]].y);
				z2 = z + mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]].z;
				SDL_RenderDrawLine(render, (100*x1/z1)+250, (100*y1/z1)+250, (100*x2/z2)+250, (100*y2/z2)+250);
			}
			SDL_SetRenderDrawColor(render, 0, 0, 255, 255);
			SDL_RenderDrawPoint(render, x*100/z+250, y*100/z+250);
		}
		paint();
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		SDL_RenderClear(render);
		SDL_Event evnt;
		do {
			SDL_WaitEvent(&evnt);
		} while (evnt.type != SDL_KEYDOWN && evnt.type != SDL_QUIT);
		if (evnt.type == SDL_QUIT){
			running = 0;
		}
	}
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
