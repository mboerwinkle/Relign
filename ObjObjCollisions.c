#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "globals.h"
static double t, u, v, col[3], colvector[3], *c1, *c2, *p1, *p2;
static triangle *t1, *t2;
static ray *r1, *r2;
int ObjObjCollisions(mesh *one, mesh *two){
	c1 = one->centermass2;
	c2 = two->centermass2;
	p1 = one->pointmatrix2;
	p2 = two->pointmatrix2;
	t1 = one->trianglematrix;
	t2 = two->trianglematrix;
	r1 = one->raymatrix;
	r2 = two->raymatrix;

	if((one->radius+two->radius)<(sqrt(c1[0]-c2[0]*c1[0]-c2[0]+c1[1]-c2[1]*c1[1]-c2[1]+c1[2]-c2[2]*c1[2]-c2[2]))) return 0;
	int collides = 0;
	double z;//the third barycentric coordinate
	double *pone, *ptwo, *pthree;
	int temp, tempray, temptri;
	int howmany = 0;//not to be confused with the wack-ass corn...
	void *temppt;
	double offsetx, offsety, offsetz;
	for(temp = 0; temp < 2; temp++){
		offsetx = c1[0]-c2[0];
		offsety = c1[1]-c2[1];
		offsetz = c1[2]-c2[2];
		for(tempray = one->rays-1; tempray >=0; tempray--){
			for(temptri = two->triangles-1; temptri >=0; temptri--){
				pone = &p2[t2[temptri].points[0]*3];
				ptwo = &p2[t2[temptri].points[1]*3];
				pthree = &p2[t2[temptri].points[2]*3];
				if(intersect_triangle(&p1[r1[tempray].ends[0]*3], &p1[r1[tempray].ends[1]*3], pone, ptwo, pthree, &t, &u, &v, offsetx, offsety, offsetz)){
					collides = 1;
					z = 1-(u+v);//find the third barycentric coordinate
					col[0] = (z*pone[0]+u*ptwo[0]+v*pthree[0]+c2[0]+col[0]*howmany)/(howmany+1);
					col[1] = (z*pone[1]+u*ptwo[1]+v*pthree[1]+c2[1]+col[1]*howmany)/(howmany+1);
					col[2] = (z*pone[2]+u*ptwo[2]+v*pthree[2]+c2[2]+col[2]*howmany)/(howmany+1);
					howmany++;
					
				}
			}
		}
		temppt = c1;
		c1 = c2;
		c2 = temppt;
		temppt = p1;
		p1 = p2;
		p2 = temppt;
		temppt = t1;
		t1 = t2;
		t2 = temppt;
		temppt = r1;
		r1 = r2;
		r2 = temppt;
		temppt = one;
		one = two;
		two = temppt;
	}
	if(collides){
		double colloc1[3] = {col[0] - c1[0], col[1] - c1[1], col[2] - c1[2]};
		double colloc2[3] = {col[0] - c2[0], col[1] - c2[1], col[2] - c2[2]};
		double colforce1[3], colforce2[3];
		getObjectVelocity(one, colloc1, colforce1);
		getObjectVelocity(two, colloc2, colforce2);
		colvector[0] = colforce1[0]-colforce2[0];
		colvector[1] = colforce1[1]-colforce2[1];
		colvector[2] = colforce1[2]-colforce2[2];
		double relMass;
		if(one->moves){
			if(two->moves) relMass = 2*two->mass/(one->mass + two->mass);//how much of the force is given to one, in funtion of one and two's masses. between 0 and 2.
			else relMass = ELASTICITY_NOMOVE;//it is like it is hitting something it's own weight, so it will get stopped.
			applyForce(one, colloc1[0], colloc1[1], colloc1[2], -(colvector[0]), -(colvector[1]), -(colvector[2]), sqrt(colvector[0]*colvector[0]+colvector[1]*colvector[1]+colvector[2]*colvector[2])*relMass*ELASTICITY_MOVE);
		}
		if(two->moves){
			if(one->moves) relMass = 2*one->mass/(one->mass + two->mass);
			else relMass = ELASTICITY_NOMOVE;
			applyForce(two, colloc2[0], colloc2[1], colloc2[2], colvector[0], colvector[1], colvector[2], sqrt(colvector[0]*colvector[0]+colvector[1]*colvector[1]+colvector[2]*colvector[2])*relMass*ELASTICITY_MOVE);
		}

	}
//	printf("%lf %lf %lf\n%lf\n\n", col[3], col[4], col[5], sqrt(col[3]*col[3]+col[4]*col[4]+col[5]*col[5]));
	return collides;
}
int backupObjObjCollisions(mesh *one, mesh *two){
	c1 = one->centermass2;
	c2 = two->centermass;
	p1 = one->pointmatrix2;
	p2 = two->pointmatrix;
	t1 = one->trianglematrix;
	t2 = two->trianglematrix;
	r1 = one->raymatrix;
	r2 = two->raymatrix;

	if((one->radius+two->radius)<(sqrt(c1[0]-c2[0]*c1[0]-c2[0]+c1[1]-c2[1]*c1[1]-c2[1]+c1[2]-c2[2]*c1[2]-c2[2]))) return 0;
	int collides = 0;
	double z;//the third barycentric coordinate
	double *pone, *ptwo, *pthree;
	int temp, tempray, temptri;
	int howmany = 0;//not to be confused with the wack-ass corn...
	void *temppt;
	double offsetx, offsety, offsetz;
	for(temp = 0; temp < 2; temp++){
		offsetx = c1[0]-c2[0];
		offsety = c1[1]-c2[1];
		offsetz = c1[2]-c2[2];
		for(tempray = one->rays-1; tempray >=0; tempray--){
			for(temptri = two->triangles-1; temptri >=0; temptri--){
				pone = &p2[t2[temptri].points[0]*3];
				ptwo = &p2[t2[temptri].points[1]*3];
				pthree = &p2[t2[temptri].points[2]*3];
				if(intersect_triangle(&p1[r1[tempray].ends[0]*3], &p1[r1[tempray].ends[1]*3], pone, ptwo, pthree, &t, &u, &v, offsetx, offsety, offsetz)){
					collides = 1;
					z = 1-(u+v);//find the third barycentric coordinate
					col[0] = (z*pone[0]+u*ptwo[0]+v*pthree[0]+c2[0]+col[0]*howmany)/(howmany+1);
					col[1] = (z*pone[1]+u*ptwo[1]+v*pthree[1]+c2[1]+col[1]*howmany)/(howmany+1);
					col[2] = (z*pone[2]+u*ptwo[2]+v*pthree[2]+c2[2]+col[2]*howmany)/(howmany+1);
					howmany++;
					
				}
			}
		}
		temppt = c1;
		c1 = c2;
		c2 = temppt;
		temppt = p1;
		p1 = p2;
		p2 = temppt;
		temppt = t1;
		t1 = t2;
		t2 = temppt;
		temppt = r1;
		r1 = r2;
		r2 = temppt;
		temppt = one;
		one = two;
		two = temppt;
	}
	if(collides){
		double colloc1[3] = {col[0] - c1[0], col[1] - c1[1], col[2] - c1[2]};
		double colloc2[3] = {col[0] - c2[0], col[1] - c2[1], col[2] - c2[2]};
		double colforce1[3], colforce2[3];
		getObjectVelocity(one, colloc1, colforce1);
		getObjectVelocity(two, colloc2, colforce2);
		colvector[0] = colforce1[0]-colforce2[0];
		colvector[1] = colforce1[1]-colforce2[1];
		colvector[2] = colforce1[2]-colforce2[2];
		double relMass;
		if(one->moves){
			if(two->moves) relMass = 2*two->mass/(one->mass + two->mass);//how much of the force is given to one, in funtion of one and two's masses. between 0 and 2.
			else relMass = ELASTICITY_NOMOVE;//it is like it is hitting something it's own weight, so it will get stopped.
			applyForce(one, colloc1[0], colloc1[1], colloc1[2], -(colvector[0]), -(colvector[1]), -(colvector[2]), sqrt(colvector[0]*colvector[0]+colvector[1]*colvector[1]+colvector[2]*colvector[2])*relMass*ELASTICITY_MOVE);
		}
		if(two->moves){
			if(one->moves) relMass = 2*one->mass/(one->mass + two->mass);
			else relMass = ELASTICITY_NOMOVE;
			applyForce(two, colloc2[0], colloc2[1], colloc2[2], colvector[0], colvector[1], colvector[2], sqrt(colvector[0]*colvector[0]+colvector[1]*colvector[1]+colvector[2]*colvector[2])*relMass*ELASTICITY_MOVE);
		}
	}
//	printf("%lf %lf %lf\n%lf\n\n", col[3], col[4], col[5], sqrt(col[3]*col[3]+col[4]*col[4]+col[5]*col[5]));
	return collides;
}
