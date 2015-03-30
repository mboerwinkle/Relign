#include <stdio.h>
#include <stdlib.h>

typedef struct conj{
	double x1, y1, z1, x2, y2, z2;
}conj;

int main(){
	char name[40];
	scanf("%s", name);
	FILE *fp;
	int x, y, z;
	int xsize, ysize, zsize;
	int points = 0;
	int triangles = 0;
	int rays = 0;
	double scale;
	fp = fopen(name, "r");
	fscanf(fp, "%d %d %d\n %lf", &xsize, &ysize, &zsize, &scale);
	int mat[xsize][ysize][zsize];
	int facesx[xsize+1][ysize][zsize];
	memset(facesx, 0, sizeof(facesx));
	int facesy[xsize][ysize+1][zsize];
	memset(facesy, 0, sizeof(facesy));
	int facesz[xsize][ysize][zsize+1];
	memset(facesz, 0, sizeof(facesz));
	while(EOF != fscanf(fp, "%d %d %d", &x, &y, &z)){
		mat[x][y][z] = 1;
		if(facesx[x][y][z] == 0){
			facesx[x][y][z] = 1;
		}else if(facesx[x][y][z] == 1){
			facesx[x][y][z] = 2;
		}
		if(facesx[x+1][y][z] == 0){
			facesx[x+1][y][z] = 1;
		}else if(facesx[x+1][y][z] == 1){
			facesx[x+1][y][z] = 2;
		}

		if(facesy[x][y][z] == 0){
			facesy[x][y][z] = 1;
		}else if(facesy[x][y][z] == 1){
			facesy[x][y][z] = 2;
		}
		if(facesy[x][y+1][z] == 0){
			facesy[x][y+1][z] = 1;
		}else if(facesy[x][y+1][z] == 1){
			facesy[x][y+1][z] = 2;
		}

		if(facesz[x][y][z] == 0){
			facesz[x][y][z] = 1;
		}else if(facesz[x][y][z] == 1){
			facesz[x][y][z] = 2;
		}
		if(facesz[x][y][z+1] == 0){
			facesz[x][y][z+1] = 1;
		}else if(facesz[x][y][z+1] == 1){
			facesz[x][y][z+1] = 2;
		}
	}
	fclose(fp);
	conj faces[(xsize+1)*(ysize+1)*(zsize+1)];
	int facescounter = 0;
	int addNext = 0;
	for(x = xsize; x >= 0; x--){
		for(y = ysize-1; y >= 0; y--){
			for(z = zsize-1; z >= 0; z--){
				if(facesx[x][y][z] == 1){
					if(addNext){
						faces[facescounter-1].y2 = y;
						faces[facescounter-1].z2 = z;
					}else{
						facescounter++;
						faces[facescounter-1].x1 = x;
						faces[facescounter-1].y1 = y+1;
						faces[facescounter-1].z1 = z+1;
						faces[facescounter-1].x2 = x;
						faces[facescounter-1].y2 = y;
						faces[facescounter-1].z2 = z;
						addNext = 1;
					}
				}else addNext = 0;
			}
		addNext = 0;
		}
	}
	for(y = ysize; y >= 0; y--){
		for(x = xsize-1; x >= 0; x--){
			for(z = zsize-1; z >= 0; z--){
				if(facesy[x][y][z] == 1){
					if(addNext){
						faces[facescounter-1].x2 = x;
						faces[facescounter-1].z2 = z;
					}else{
						facescounter++;
						faces[facescounter-1].x1 = x+1;
						faces[facescounter-1].y1 = y;
						faces[facescounter-1].z1 = z+1;
						faces[facescounter-1].x2 = x;
						faces[facescounter-1].y2 = y;
						faces[facescounter-1].z2 = z;
						addNext = 1;
					}
				}else addNext = 0;
			}
		addNext = 0;
		}
	}
	for(z = zsize; z >= 0; z--){
		for(x = xsize-1; x >= 0; x--){
			for(y = ysize-1; y >= 0; y--){
				if(facesz[x][y][z] == 1){
					if(addNext){
						faces[facescounter-1].x2 = x;
						faces[facescounter-1].y2 = y;
					}else{
						facescounter++;
						faces[facescounter-1].x1 = x+1;
						faces[facescounter-1].y1 = y+1;
						faces[facescounter-1].z1 = z;
						faces[facescounter-1].x2 = x;
						faces[facescounter-1].y2 = y;
						faces[facescounter-1].z2 = z;
						addNext = 1;
					}
				}else addNext = 0;
			}
		addNext = 0;
		}
	}
	int temp, temp2;
	triangles = facescounter*2;
	double x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;
	double pointmatrix[triangles*2][3];
	int raymatrix[triangles*(5/2)][2];
	int trianglematrix[triangles][3];
	triangles = 0;
	sprintf(name, "%s.rrm", name);
	fp = fopen(name, "w");
	for(temp2 = facescounter-1; temp2 >= 0; temp2--){
		if(faces[temp2].x1 == faces[temp2].x2){
			x1 = faces[temp2].x1;
			x2 = faces[temp2].x1;
			x3 = faces[temp2].x1;
			x4 = faces[temp2].x1;
			y1 = faces[temp2].y1;
			y2 = faces[temp2].y2;
			y3 = faces[temp2].y2;
			y4 = faces[temp2].y1;
			z1 = faces[temp2].z1;
			z2 = faces[temp2].z1;
			z3 = faces[temp2].z2;
			z4 = faces[temp2].z2;
		}else if(faces[temp2].y1 == faces[temp2].y2){
			y1 = faces[temp2].y1;
			y2 = faces[temp2].y1;
			y3 = faces[temp2].y1;
			y4 = faces[temp2].y1;
			x1 = faces[temp2].x1;
			x2 = faces[temp2].x2;
			x3 = faces[temp2].x2;
			x4 = faces[temp2].x1;
			z1 = faces[temp2].z1;
			z2 = faces[temp2].z1;
			z3 = faces[temp2].z2;
			z4 = faces[temp2].z2;
		}else if(faces[temp2].z1 == faces[temp2].z2){
			z1 = faces[temp2].z1;
			z2 = faces[temp2].z1;
			z3 = faces[temp2].z1;
			z4 = faces[temp2].z1;
			x1 = faces[temp2].x1;
			x2 = faces[temp2].x2;
			x3 = faces[temp2].x2;
			x4 = faces[temp2].x1;
			y1 = faces[temp2].y1;
			y2 = faces[temp2].y1;
			y3 = faces[temp2].y2;
			y4 = faces[temp2].y2;
		}else{
			puts("error!");
			 return 1;//I think this is right...
		}
		temp = points-1;
		while(temp >= 0 &&  !(x1 == pointmatrix[temp][0] && y1 == pointmatrix[temp][1] && z1 == pointmatrix[temp][2])){
			temp--;
		}
		if(temp < 0){
			temp = points;
			pointmatrix[points][0] = x1;
			pointmatrix[points][1] = y1;
			pointmatrix[points][2] = z1;
			points++;
		}
		trianglematrix[triangles][0] = points;
		temp = points-1;
		while(temp >= 0 &&  !(x2 == pointmatrix[temp][0] && y2 == pointmatrix[temp][1] && z2 == pointmatrix[temp][2])){
			temp--;
		}
		if(temp < 0){
			temp = points;
			pointmatrix[points][0] = x2;
			pointmatrix[points][1] = y2;
			pointmatrix[points][2] = z2;
			points++;
		}
		trianglematrix[triangles][1] = points;
		temp = points-1;
		while(temp >= 0 &&  !(x3 == pointmatrix[temp][0] && y3 == pointmatrix[temp][1] && z3 == pointmatrix[temp][2])){
			temp--;
		}
		if(temp < 0){
			temp = points;
			pointmatrix[points][0] = x3;
			pointmatrix[points][1] = y3;
			pointmatrix[points][2] = z3;
			points++;
		}
		trianglematrix[triangles][2] = points;
		temp = rays-1;
		while(temp >= 0 && !((raymatrix[temp][0] == trianglematrix[triangles][0] && raymatrix[temp][1] == trianglematrix[triangles][1]) || (raymatrix[temp][1] == trianglematrix[triangles][0] && raymatrix[temp][0] == trianglematrix[triangles][1]))){
			temp--;
		}
		if(temp < 0){
			raymatrix[rays][0] = trianglematrix[triangles][0];
			raymatrix[rays][1] = trianglematrix[triangles][1];
			rays++;
		}
		temp = rays-1;
		while(temp >= 0 && !((raymatrix[temp][0] == trianglematrix[triangles][1] && raymatrix[temp][1] == trianglematrix[triangles][2]) || (raymatrix[temp][1] == trianglematrix[triangles][1] && raymatrix[temp][0] == trianglematrix[triangles][2]))){
			temp--;
		}
		if(temp < 0){
			raymatrix[rays][0] = trianglematrix[triangles][1];
			raymatrix[rays][1] = trianglematrix[triangles][2];
			rays++;
		}
		temp = rays-1;
		while(temp >= 0 && !((raymatrix[temp][0] == trianglematrix[triangles][2] && raymatrix[temp][1] == trianglematrix[triangles][0]) || (raymatrix[temp][1] == trianglematrix[triangles][2] && raymatrix[temp][0] == trianglematrix[triangles][0]))){
			temp--;
		}
		if(temp < 0){
			raymatrix[rays][0] = trianglematrix[triangles][2];
			raymatrix[rays][1] = trianglematrix[triangles][0];
			rays++;
		}
		triangles++;
		temp = points-1;
		while(temp >= 0 &&  !(x4 == pointmatrix[temp][0] && y4 == pointmatrix[temp][1] && z4 == pointmatrix[temp][2])){
			temp--;
		}
		if(temp < 0){
			temp = points;
			pointmatrix[points][0] = x4;
			pointmatrix[points][1] = y4;
			pointmatrix[points][2] = z4;
			points++;
		}
		trianglematrix[triangles][0] = points;
		temp = points-1;
		while(temp >= 0 &&  !(x1 == pointmatrix[temp][0] && y1 == pointmatrix[temp][1] && z1 == pointmatrix[temp][2])){
			temp--;
		}
		if(temp < 0){
			temp = points;
			pointmatrix[points][0] = x1;
			pointmatrix[points][1] = y1;
			pointmatrix[points][2] = z1;
			points++;
		}
		trianglematrix[triangles][1] = points;
		temp = points-1;
		while(temp >= 0 &&  !(x3 == pointmatrix[temp][0] && y3 == pointmatrix[temp][1] && z3 == pointmatrix[temp][2])){
			temp--;
		}
		if(temp < 0){
			temp = points;
			pointmatrix[points][0] = x3;
			pointmatrix[points][1] = y3;
			pointmatrix[points][2] = z3;
			points++;
		}
		trianglematrix[triangles][2] = points;
		temp = rays-1;
		while(temp >= 0 && !((raymatrix[temp][0] == trianglematrix[triangles][0] && raymatrix[temp][1] == trianglematrix[triangles][1]) || (raymatrix[temp][1] == trianglematrix[triangles][0] && raymatrix[temp][0] == trianglematrix[triangles][1]))){
			temp--;
		}
		if(temp < 0){
			raymatrix[rays][0] = trianglematrix[triangles][0];
			raymatrix[rays][1] = trianglematrix[triangles][1];
			rays++;
		}
		temp = rays-1;
		while(temp >= 0 && !((raymatrix[temp][0] == trianglematrix[triangles][1] && raymatrix[temp][1] == trianglematrix[triangles][2]) || (raymatrix[temp][1] == trianglematrix[triangles][1] && raymatrix[temp][0] == trianglematrix[triangles][2]))){
			temp--;
		}
		if(temp < 0){
			raymatrix[rays][0] = trianglematrix[triangles][1];
			raymatrix[rays][1] = trianglematrix[triangles][2];
			rays++;
		}
		temp = rays-1;
		while(temp >= 0 && !((raymatrix[temp][0] == trianglematrix[triangles][2] && raymatrix[temp][1] == trianglematrix[triangles][0]) || (raymatrix[temp][1] == trianglematrix[triangles][2] && raymatrix[temp][0] == trianglematrix[triangles][0]))){
			temp--;
		}
		if(temp < 0){
			raymatrix[rays][0] = trianglematrix[triangles][2];
			raymatrix[rays][1] = trianglematrix[triangles][0];
			rays++;
		}
		triangles++;
		fprintf(fp, "%lf %lf %lf\n%lf %lf %lf\n%lf %lf %lf\n\n", x1, y1, z1, x2, y2, z2, x3, y3, z3);
		fprintf(fp, "%lf %lf %lf\n%lf %lf %lf\n%lf %lf %lf\n\n", x4, y4, z4, x1, y1, z1, x3, y3, z3);
	}
	rewind(fp);
	fprintf(fp, "%d %d %d\n", points, triangles, rays);
	return 0;
}
