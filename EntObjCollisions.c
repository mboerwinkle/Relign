#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

int EntObjCollisions(mesh* M, ent* E){
	double *pone, *ptwo, *pthree;
	int howmany = 0;
	double col[3] = {0, 0, 0};
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
			z = 1-(u+v);
			pone = &p[tri[temptri].points[0]*3];
			ptwo = &p[tri[temptri].points[1]*3];
			pthree = &p[tri[temptri].points[2]*3];
			col[0] = (z*pone[0]+u*ptwo[0]+v*pthree[0]+c[0]+col[0]*howmany)/(howmany+1);
			col[1] = (z*pone[1]+u*ptwo[1]+v*pthree[1]+c[1]+col[1]*howmany)/(howmany+1);
			col[2] = (z*pone[2]+u*ptwo[2]+v*pthree[2]+c[2]+col[2]*howmany)/(howmany+1);
			howmany++;
			SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
			drawLine(c, E->center2);
		}
	}
	if(collides){
		double colloc[3] = {col[0]-c[0], col[1]-c[1], col[2]-c[2]};
		double colforce[3];
		getObjectVelocity(M, colloc, colforce);
		double colvector[3];
		colvector[0] = colforce[0]-E->vx;
		colvector[1] = colforce[1]-E->vy;
		colvector[2] = colforce[2]-E->vz;
		double relMass;
		if(M->moves){
			relMass = 2*E->mass/(M->mass+E->mass);
			applyForce(M, colloc[0], colloc[1], colloc[2], -(colvector[0]), -(colvector[1]), -(colvector[2]), sqrt(colvector[0]*colvector[0]+colvector[1]*colvector[1]+colvector[2]*colvector[2])*relMass*ELASTICITY_MOVE);
//			relMass = 2*M->mass/(E->mass+M->mass);
		}//else relMass = 2;		
		relMass = 2;
		applyEntForce(E, colvector[0], colvector[1], colvector[2], sqrt(colvector[0]*colvector[0]+colvector[1]*colvector[1]+colvector[2]*colvector[2])*relMass*ELASTICITY_MOVE);
	}
	return collides;
}
