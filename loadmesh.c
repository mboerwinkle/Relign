#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
void loadmesh(char name[20], mesh *final, double centerx, double centery, double centerz){
	final->centermass[0] = centerx;
	final->centermass[1] = centery;
	final->centermass[2] = centerz;
	FILE *fp;
	fp = fopen(name, "r");
	if(EOF == fscanf(fp, "%d %d %d", &final->points, &final->triangles, &final->rays)) puts("file read failed");
//	final->metapointmatrix = calloc(sizeof(*), final->points);
	final->pointmatrix = calloc(sizeof(double)*3, final->points);
	final->cpointmatrix = calloc(sizeof(double)*3, final->points);
	final->raymatrix = calloc(sizeof(ray), final->rays);
	final->trianglematrix = calloc(sizeof(triangle), final->triangles);
	int raycount = 0, trianglecount = 0, pointcount = 0, temp, corner, one, two;
//	for(temp = 0; temp < final->points; temp++) final->metapointmatrix[temp] = final->pointmatrix[temp*3];
	double x, y, z;
	for(trianglecount = 0; trianglecount < final->triangles; trianglecount++){
		for(corner = 0; corner < 3; corner++){
			if(EOF == fscanf(fp, "%lf %lf %lf", &x, &y, &z)) puts("file read failed");
			temp = 0;
			while(temp < pointcount && !(x == final->cpointmatrix[temp*3+0] && y == final->cpointmatrix[temp*3+1] && z == final->cpointmatrix[temp*3+2])){
				temp++;
			}
			if(temp >= final->points){
				puts("error!\n");
				return;
			}
			if(temp == pointcount){
				final->cpointmatrix[temp*3+0] = x;
				final->cpointmatrix[temp*3+1] = y;
				final->cpointmatrix[temp*3+2] = z;
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
