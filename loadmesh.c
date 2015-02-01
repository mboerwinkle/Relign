#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
void loadmesh(char name[20], mesh *final){
	FILE *fp;
	fp = fopen(name, "r");
	fscanf(fp, "%d", &final->points);
	fscanf(fp, "%d", &final->triangles);
	fscanf(fp, "%d", &final->rays);
	final->pointmatrix = calloc(sizeof(point), final->points);
	final->raymatrix = calloc(sizeof(ray), final->rays);
	final->trianglematrix = calloc(sizeof(triangle), final->triangles);
	int raycount = 0, trianglecount = 0, pointcount = 0, temp, corner, one, two;
	int x, y, z;
	for(trianglecount = 0; trianglecount < final->triangles; trianglecount++){
		for(corner = 0; corner < 3; corner++){
			fscanf(fp, "%d %d %d", &x, &y, &z);
			temp = 0;
			while(temp < pointcount && !(x == final->pointmatrix[temp].x && y == final->pointmatrix[temp].y && z == final->pointmatrix[temp].z)){
				temp++;
			}
			if(temp >= final->points){
				puts("error!\n");
				return;
			}
			if(temp == pointcount){
//				if(news == 4) news = corner;//news variable: 4 is no new points, 0-2 is one point new, 3 is create all rays (2 or 3 new points) 
//				else news = 3;
				final->pointmatrix[temp].x = x;
				final->pointmatrix[temp].y = y;
				final->pointmatrix[temp].z = z;
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
/*		if(news < 3){
			printf("3 > %d?\n", news);
			final->raymatrix[raycount].ends[0] = final->trianglematrix[trianglecount].points[news];
			final->raymatrix[raycount].ends[1] = final->trianglematrix[trianglecount].points[(news+1)%3];
			printf("%d %d\n", final->raymatrix[raycount].ends[0], final->raymatrix[raycount].ends[1]);
			raycount++;
			final->raymatrix[raycount].ends[0] = final->trianglematrix[trianglecount].points[news];
			final->raymatrix[raycount].ends[1] = final->trianglematrix[trianglecount].points[(news+2)%3];
			printf("%d %d\n", final->raymatrix[raycount].ends[0], final->raymatrix[raycount].ends[1]);
			raycount++;
		}
		else if(news == 3){
			printf("3 = %d?\n", news);
			final->raymatrix[raycount].ends[0] = final->trianglematrix[trianglecount].points[0];
			final->raymatrix[raycount].ends[1] = final->trianglematrix[trianglecount].points[1];
			printf("%d %d\n", final->raymatrix[raycount].ends[0], final->raymatrix[raycount].ends[1]);
			raycount++;
			final->raymatrix[raycount].ends[0] = final->trianglematrix[trianglecount].points[1];
			final->raymatrix[raycount].ends[1] = final->trianglematrix[trianglecount].points[2];
			printf("%d %d\n", final->raymatrix[raycount].ends[0], final->raymatrix[raycount].ends[1]);
			raycount++;
			final->raymatrix[raycount].ends[0] = final->trianglematrix[trianglecount].points[2];
			final->raymatrix[raycount].ends[1] = final->trianglematrix[trianglecount].points[0];
			printf("%d %d\n", final->raymatrix[raycount].ends[0], final->raymatrix[raycount].ends[1]);
			raycount++;
		}
		printf("%d\n", raycount);
		news = 4;*/
	}
	fclose(fp);
}
