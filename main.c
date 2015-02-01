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
	mesh *mesh1;
	mesh1 = malloc(sizeof(mesh));
	loadmesh("mesh1", mesh1);
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	window = SDL_CreateWindow("Relign, BITCHES! Finally used that name! BLAM!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, 0);
	if(window == NULL){
		SDL_Quit();
		return 1;
	}
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	int raycounter, x1, y1, z1, x2, y2, z2;
	while(running){
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		SDL_RenderClear(render);
		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
		for(raycounter = mesh1->rays-1; raycounter >= 0; raycounter--){
			x1 = mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]].x;
			y1 = mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]].y;
			z1 =  0.1 *(20+ mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[0]].z);
			x2 = mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]].x;
			y2 = mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]].y;
			z2 = 0.1 *(20+ mesh1->pointmatrix[mesh1->raymatrix[raycounter].ends[1]].z);
			printf("%d %d\n", mesh1->raymatrix[raycounter].ends[0], mesh1->raymatrix[raycounter].ends[1]);
			printf("%d %d %d\n%d %d %d\n\n", x1, y1, z1, x2, y2, z2);
			SDL_RenderDrawLine(render, (x1/z1)+250, (y1/z1)+250, (x2/z2)+250, (y2/z2)+250);
		}
		paint();
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
