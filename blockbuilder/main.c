#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct conj{
	double x1, y1, z1, x2, y2, z2;
}conj;
	

int iscube(int *searchmat, int matsize, int x, int y, int z){
	int temp;
	for(temp = matsize-1; temp >= 0; temp--){
		if(searchmat[temp*3+0] == x && searchmat[temp*3+1] == y && searchmat[temp*3+2] == z) return 1;
	}
	return 0;
}

int main(int argc, char *argv[]){
	char name[40];
	if(argc > 1){
		strcpy(name, argv[1]);
	}else{
		puts("input filename");
		if(EOF == scanf("%s", name)) puts("input error");
	}
	FILE *fp;
	int cubes = 0;
	int *cubematrix = NULL;
	int x, y, z;
	int xmax = 0, ymax = 0, zmax = 0;
	double scale;
	fp = fopen(name, "r");
	if(EOF == fscanf(fp, "%lf", &scale)) puts("read error");
	while(EOF != fscanf(fp, "%d %d %d", &x, &y, &z)){
		cubes++;
		cubematrix = realloc(cubematrix, sizeof(int)*3*cubes);
		cubematrix[(cubes-1)*3+0] = x;
		cubematrix[(cubes-1)*3+1] = y;
		cubematrix[(cubes-1)*3+2] = z;
		if(x > xmax) xmax = x;
		if(y > ymax) ymax = y;
		if(z > zmax) zmax = z;
	}
	fclose(fp);
	conj faces[(xmax+2)*(ymax+2)*(zmax+2)];//+1 to change to size, not max, +1 for both sides of the cubes
	int facescounter = 0, addnext = 0;
	for(x = xmax+1; x >= 0; x--){
		for(y = ymax; y >= 0; y--){
			for(z = zmax; z >= 0; z--){
				if(iscube(cubematrix, cubes, x, y, z) ^ iscube(cubematrix, cubes, x-1, y, z)){
					if(addnext){
						faces[facescounter-1].z2 = z;
					}else{
						facescounter++;
						faces[facescounter-1].x1 = x;
						faces[facescounter-1].y1 = y+1;
						faces[facescounter-1].z1 = z+1;
						faces[facescounter-1].x2 = x;
						faces[facescounter-1].y2 = y;
						faces[facescounter-1].z2 = z;
						addnext = 1;
					}
				}else addnext = 0;
			}
			addnext = 0;
		}
	}
	for(y = ymax+1; y >= 0; y--){
		for(x = xmax; x >= 0; x--){
			for(z = zmax; z >= 0; z--){
				if(iscube(cubematrix, cubes, x, y, z) ^ iscube(cubematrix, cubes, x, y-1, z)){
					if(addnext){
						faces[facescounter-1].z2 = z;
					}else{
						facescounter++;
						faces[facescounter-1].x1 = x+1;
						faces[facescounter-1].y1 = y;
						faces[facescounter-1].z1 = z+1;
						faces[facescounter-1].x2 = x;
						faces[facescounter-1].y2 = y;
						faces[facescounter-1].z2 = z;
						addnext = 1;
					}
				}else addnext = 0;
			}
			addnext = 0;
		}
	}
	for(z = zmax+1; z >= 0; z--){
		for(x = xmax; x >= 0; x--){
			for(y = ymax; y >= 0; y--){
				if(iscube(cubematrix, cubes, x, y, z) ^ iscube(cubematrix, cubes, x, y, z-1)){
					if(addnext){
						faces[facescounter-1].y2 = y;
					}else{
						facescounter++;
						faces[facescounter-1].x1 = x+1;
						faces[facescounter-1].y1 = y+1;
						faces[facescounter-1].z1 = z;
						faces[facescounter-1].x2 = x;
						faces[facescounter-1].y2 = y;
						faces[facescounter-1].z2 = z;
						addnext = 1;
					}
				}else addnext = 0;
			}
			addnext = 0;
		}
	}
	int temp;
	double x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;
	sprintf(name, "%s.rrm", name);
	fp = fopen(name, "w");
	for(temp = facescounter-1; temp >= 0; temp--){
		if(faces[temp].x1 == faces[temp].x2){
			x1 = faces[temp].x1 * scale;
			x2 = faces[temp].x1 * scale;
			x3 = faces[temp].x1 * scale;
			x4 = faces[temp].x1 * scale;
			y1 = faces[temp].y1 * scale;
			y2 = faces[temp].y2 * scale;
			y3 = faces[temp].y2 * scale;
			y4 = faces[temp].y1 * scale;
			z1 = faces[temp].z1 * scale;
			z2 = faces[temp].z1 * scale;
			z3 = faces[temp].z2 * scale;
			z4 = faces[temp].z2 * scale;
		}else if(faces[temp].y1 == faces[temp].y2){
			y1 = faces[temp].y1 * scale;
			y2 = faces[temp].y1 * scale;
			y3 = faces[temp].y1 * scale;
			y4 = faces[temp].y1 * scale;
			x1 = faces[temp].x1 * scale;
			x2 = faces[temp].x2 * scale;
			x3 = faces[temp].x2 * scale;
			x4 = faces[temp].x1 * scale;
			z1 = faces[temp].z1 * scale;
			z2 = faces[temp].z1 * scale;
			z3 = faces[temp].z2 * scale;
			z4 = faces[temp].z2 * scale;
		}else if(faces[temp].z1 == faces[temp].z2){
			z1 = faces[temp].z1 * scale;
			z2 = faces[temp].z1 * scale;
			z3 = faces[temp].z1 * scale;
			z4 = faces[temp].z1 * scale;
			x1 = faces[temp].x1 * scale;
			x2 = faces[temp].x2 * scale;
			x3 = faces[temp].x2 * scale;
			x4 = faces[temp].x1 * scale;
			y1 = faces[temp].y1 * scale;
			y2 = faces[temp].y1 * scale;
			y3 = faces[temp].y2 * scale;
			y4 = faces[temp].y2 * scale;
		}else{
			puts("error!");
			 return 1;//I think this is right...
		}
		fprintf(fp, "%lf %lf %lf\n%lf %lf %lf\n%lf %lf %lf\n\n", x1, y1, z1, x2, y2, z2, x3, y3, z3);
		fprintf(fp, "%lf %lf %lf\n%lf %lf %lf\n%lf %lf %lf\n\n", x4, y4, z4, x1, y1, z1, x3, y3, z3);
	}
	return 0;
}
