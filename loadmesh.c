#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
void loadmesh(char name[20], mesh *final, int centerx, int centery, int centerz){
	final->centermass.x = centerx;
	final->centermass.y = centery;
	final->centermass.z = centerz;
	FILE *fp;
	fp = fopen(name, "r");
	fscanf(fp, "%d", &final->points);
	fscanf(fp, "%d", &final->triangles);
	fscanf(fp, "%d", &final->rays);
	final->pointmatrix = calloc(sizeof(point), final->points);
	final->cpointmatrix = calloc(sizeof(point), final->points);
	final->raymatrix = calloc(sizeof(ray), final->rays);
	final->trianglematrix = calloc(sizeof(triangle), final->triangles);
	int raycount = 0, trianglecount = 0, pointcount = 0, temp, corner, one, two;
	int x, y, z;
	for(trianglecount = 0; trianglecount < final->triangles; trianglecount++){
		for(corner = 0; corner < 3; corner++){
			fscanf(fp, "%d %d %d", &x, &y, &z);
			temp = 0;
			while(temp < pointcount && !(x == final->cpointmatrix[temp].x && y == final->cpointmatrix[temp].y && z == final->cpointmatrix[temp].z)){
				temp++;
			}
			if(temp >= final->points){
				puts("error!\n");
				return;
			}
			if(temp == pointcount){
				final->cpointmatrix[temp].x = x;
				final->cpointmatrix[temp].y = y;
				final->cpointmatrix[temp].z = z;
				pointcount++;
//				printf("%d %d %d\n", x, y, z);
			}
			final->trianglematrix[trianglecount].points[corner] = temp;
		}
		for(corner =  0; corner < 3; corner++){
			one = final->trianglematrix[trianglecount].points[corner];
			two = final->trianglematrix[trianglecount].points[(corner+1)%3];
			temp = 0;
			while(temp < raycount && !( (one == final->raymatrix[temp].ends[0] && two == final->raymatrix[temp].ends[1]) || (two == final->raymatrix[temp].ends[0] && one == final->raymatrix[temp].ends[1]) )){
				temp++;
			}
			if(!(temp < raycount)){
				final->raymatrix[temp].ends[0] = one;
				final->raymatrix[temp].ends[1] = two;
				raycount++;
			}
		}
	}
	fclose(fp);
	movemesh(final);
}
