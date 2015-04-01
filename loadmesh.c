#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "globals.h"
void loadmesh(char name[20], mesh *final, double centerx, double centery, double centerz){
	final->centermass = calloc(sizeof(double), 3);
	final->centermass2 = calloc(sizeof(double), 3);
	final->centermass[0] = centerx;
	final->centermass[1] = centery;
	final->centermass[2] = centerz;
	final->centermass2[0] = centerx;
	final->centermass2[1] = centery;
	final->centermass2[2] = centerz;
	final->rot = calloc(sizeof(double), 4);
	final->rot2 = calloc(sizeof(double), 4);
	final->rot[0] = 0;
	final->rot[1] = 0;
	final->rot[2] = 0;
	final->rot[3] = 0;
	final->rot2[0] = 0;
	final->rot2[1] = 0;
	final->rot2[2] = 0;
	final->rot2[3] = 0;
	final->vx = 0;
	final->vy = 0;
	final->vz = 0;
	final->vx2 = 0;
	final->vy2 = 0;
	final->vz2 = 0;
	final->moves = 1;
	final->mass = 1;
	final->radius = 0;
	final->collision = 0;
	final->points = 0;
	final->triangles = 0;
	final->rays = 0;
	final->pointmatrix = NULL;
	final->pointmatrix2 = NULL;
	final->cpointmatrix = NULL;
	final->trianglematrix = NULL;
	final->raymatrix = NULL;
	FILE *fp;
	void *temppoint;
	fp = fopen(name, "r");
	int temp, corner, one, two, run = 1;
	double x3[3], y3[3], z3[3], x, y, z;
	while(run){
		if(EOF == fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", &x3[0], &y3[0], &z3[0], &x3[1], &y3[1], &z3[1], &x3[2], &y3[2], &z3[2])) return;
		final->triangles++;
		final->trianglematrix = realloc(final->trianglematrix, sizeof(triangle)*final->triangles);
		for(corner = 0; corner < 3; corner++){
			x = x3[corner];
			y = y3[corner];
			z = z3[corner];
			temp = 0;
			while(temp < final->points && !(x == final->cpointmatrix[temp*3+0] && y == final->cpointmatrix[temp*3+1] && z == final->cpointmatrix[temp*3+2])){
				temp++;
			}
			if(temp == final->points){
				final->points++;
				printf("pointer being passed is %p\n", final->pointmatrix2);
				temppoint = realloc(final->pointmatrix2, (final->points*sizeof(double)*3));
				printf("pointer recieved is %p\n", temppoint);
				final->pointmatrix2 = temppoint;
				final->cpointmatrix = realloc(final->cpointmatrix, (final->points*sizeof(double)*3));
				final->pointmatrix = realloc(final->pointmatrix, (final->points*sizeof(double)*3));
				if(final->pointmatrix == NULL || final->pointmatrix2 == NULL || final->cpointmatrix == NULL) puts("NULLL???");
				final->cpointmatrix[temp*3+0] = x;
				final->cpointmatrix[temp*3+1] = y;
				final->cpointmatrix[temp*3+2] = z;
				final->pointmatrix[temp*3+0] = x;
				final->pointmatrix[temp*3+1] = y;
				final->pointmatrix[temp*3+2] = z;
				final->pointmatrix2[temp*3+0] = x;
				final->pointmatrix2[temp*3+1] = y;
				final->pointmatrix2[temp*3+2] = z;
				if(sqrt(x*x+y*y+z*z) > final->radius) final->radius = sqrt(x*x+y*y+z*z);
			}
			final->trianglematrix[final->triangles-1].points[corner] = temp;
		}
		for(corner =  0; corner < 3; corner++){
			one = final->trianglematrix[final->triangles-1].points[corner];
			two = final->trianglematrix[final->triangles-1].points[(corner+1)%3];
			temp = 0;
			while(temp < final->rays && !((one == final->raymatrix[temp].ends[0] && two == final->raymatrix[temp].ends[1]) || (two == final->raymatrix[temp].ends[0] && one == final->raymatrix[temp].ends[1]))){
				temp++;
			}
			if(temp == final->rays){
				final->rays++;
				final->raymatrix = realloc(final->raymatrix, final->rays*sizeof(ray));
				final->raymatrix[temp].ends[0] = one;
				final->raymatrix[temp].ends[1] = two;
			}
		}
	}
//	final->drag = ;
	fclose(fp);
}
