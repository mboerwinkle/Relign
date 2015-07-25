#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

int EntObjCollisions(mesh* M, ent* E){
	int temptri;
	double zeros[3] = {0, 0, 0};
	double t, u, v, z;
	double moveend[3] = {E->vx, E->vy, E->vz};
	double *p = M->pointmatrix2;
	double *c = M->centermass2;
	double offsetx, offsety, offsetz;
	int collides = 0;
	triangle *tri = M->trianglematrix;
	for(temptri = M->triangles-1; temptri >=0; temptri--){
		offsetx = E->center2[0]-c[0];
		offsety = E->center2[1]-c[1];
		offsetz = E->center2[2]-c[2];
		if(intersect_triangle(zeros, moveend, &p[tri[temptri].points[0]*3], &p[tri[temptri].points[1]*3], &p[tri[temptri].points[2]*3], &t, &u, &v, offsetx, offsety, offsetz)){
			collides = 1;
			SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
			drawLine(c, E->center2);
		}
	}
	return collides;
}
