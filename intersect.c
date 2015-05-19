#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "globals.h"
static double orig[3], dir[3], t, u, v, col[6];

void norm(double target[3]){
/*	if(isnan(target[0]) || !isfinite(target[0]));
	if(isnan(target[1]) || !isfinite(target[1]));
	if(isnan(target[2]) || !isfinite(target[2]));*/
	if(target[0] == 0 && target[1] == 0 && target[2] == 0) return;
	double howmuch = sqrt(target[0]*target[0]+target[1]*target[1]+target[2]*target[2]);
	target[0] /= howmuch;
	target[1] /= howmuch;
	target[2] /= howmuch;
}

double distance(double vect[3], double point[3]){
	double a[3] = {vect[0], vect[1], vect[2]};
	double b[3] = {point[0], point[1], point[2]};
	double distb = sqrt(b[0]*b[0] + b[1]*b[1] + b[2]*b[2]);
	double dista = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
	double angle = acos(DOT(a, b)/(dista*distb));
	double ans = sin(angle)*distb;
	if(isnan(ans) || !isfinite(ans)) return 0;
	return ans;
}

int collisions(mesh *one, mesh *two){
	if((one->radius+two->radius)<(sqrt(one->centermass2[0]-two->centermass2[0]*one->centermass2[0]-two->centermass2[0]+one->centermass2[1]-two->centermass2[1]*one->centermass2[1]-two->centermass2[1]+one->centermass2[2]-two->centermass2[2]*one->centermass2[2]-two->centermass2[2]))) return 0;
	int collides = 0;
	double z;//the third barycentric coordinate
	double *pone, *ptwo, *pthree;
	int temp, tempray, temptri;
	int howmany = 0;//not to be confused with the wack-ass corn...
	mesh *tempmesh;
	double offsetx, offsety, offsetz;
	for(temp = 0; temp < 2; temp++){
		offsetx = one->centermass2[0]-two->centermass2[0];
		offsety = one->centermass2[1]-two->centermass2[1];
		offsetz = one->centermass2[2]-two->centermass2[2];
		for(tempray = one->rays-1; tempray >=0; tempray--){
			for(temptri = two->triangles-1; temptri >=0; temptri--){
				if(intersect_triangle(&one->pointmatrix2[one->raymatrix[tempray].ends[0]*3], &one->pointmatrix2[one->raymatrix[tempray].ends[1]*3], &two->pointmatrix2[two->trianglematrix[temptri].points[0]*3], &two->pointmatrix2[two->trianglematrix[temptri].points[1]*3], &two->pointmatrix2[two->trianglematrix[temptri].points[2]*3], &t, &u, &v, offsetx, offsety, offsetz)){
					collides = 1;
					z = 1-(u+v);//find the third barycentric coordinate
					pone = &two->pointmatrix2[two->trianglematrix[temptri].points[0]*3];
					ptwo = &two->pointmatrix2[two->trianglematrix[temptri].points[1]*3];
					pthree = &two->pointmatrix2[two->trianglematrix[temptri].points[2]*3];
					col[0] = (z*pone[0]+u*ptwo[0]+v*pthree[0]+two->centermass2[0]+col[0]*howmany)/(howmany+1);
					col[1] = (z*pone[1]+u*ptwo[1]+v*pthree[1]+two->centermass2[1]+col[1]*howmany)/(howmany+1);
					col[2] = (z*pone[2]+u*ptwo[2]+v*pthree[2]+two->centermass2[2]+col[2]*howmany)/(howmany+1);
					howmany++;
					
				}
			}
		}
		tempmesh = one;
		one = two;
		two = tempmesh;
	}
	if(collides){
		double rotationspeed;
		double colloc1[3] = {col[0] - one->centermass[0], col[1] - one->centermass[1], col[2] - one->centermass[2]};
		double colloc2[3] = {col[0] - two->centermass[0], col[1] - two->centermass[1], col[2] - two->centermass[2]};
		col[3] = 0+one->vx-two->vx;
		col[4] = 0+one->vy-two->vy;
		col[5] = 0+one->vz-two->vz;

		double uv[3];
		double *rot;
		rot = one->rot;
		if(!(rot[0] == 0 && rot[1] == 0 && rot[2] == 0)){
			rotationspeed = sqrt(rot[0]*rot[0]+rot[1]*rot[1]+rot[2]*rot[2]);
			rotationspeed *= (distance(rot, colloc1)/one->radius);//speed of rotation of collision point
			CROSS(uv, rot, colloc1);
			//CROSS(uv, colloc1, rot);
			norm(uv);
			col[3] += uv[0]*rotationspeed;
			col[4] += uv[1]*rotationspeed;
			col[5] += uv[2]*rotationspeed;
			if(isnan(rotationspeed) || !isfinite(rotationspeed)) puts("rotationspeed1 error");
		}

		rot = two->rot;
		if(!(rot[0] == 0 && rot[1] == 0 && rot[2] == 0)){
			rotationspeed = sqrt(rot[0]*rot[0]+rot[1]*rot[1]+rot[2]*rot[2]);
			rotationspeed *= (distance(rot, colloc2)/two->radius);
			CROSS(uv, rot, colloc2);
			//CROSS(uv, colloc2, rot);
			norm(uv);
			col[3] -= uv[0]*rotationspeed;
			col[4] -= uv[1]*rotationspeed;
			col[5] -= uv[2]*rotationspeed;
			if(isnan(rotationspeed) || !isfinite(rotationspeed)) puts("rotationspeed2 error");
		}
		double relMass;
		if(one->moves){
			if(two->moves) relMass = 2*two->mass/(one->mass + two->mass);
			else relMass = ELASTICITY_NOMOVE;//it is like it is hitting something it's own weight, so it will get stopped.
			applyForce(one, colloc1[0], colloc1[1], colloc1[2], -(col[3]), -(col[4]), -(col[5]), sqrt(col[3]*col[3]+col[4]*col[4]+col[5]*col[5])*relMass);
		}
		if(two->moves){
			if(one->moves) relMass = 2*one->mass/(one->mass + two->mass);
			else relMass = ELASTICITY_NOMOVE;
			applyForce(two, colloc2[0], colloc2[1], colloc2[2], col[3], col[4], col[5], sqrt(col[3]*col[3]+col[4]*col[4]+col[5]*col[5])*relMass);
		}
	}
//	printf("%lf %lf %lf\n%lf\n\n", col[3], col[4], col[5], sqrt(col[3]*col[3]+col[4]*col[4]+col[5]*col[5]));
	return collides;
}

int intersect_triangle(double end1[3], double end2[3], double vert0[3], double vert1[3], double vert2[3], double *t, double *u, double *v, double offsetx, double offsety, double offsetz){
/*	double *orig, *dir;
	orig = calloc(sizeof(double), 3);
	dir = calloc(sizeof(double), 3);*/
	dir[0] = end2[0]-end1[0];
	dir[1] = end2[1]-end1[1];
	dir[2] = end2[2]-end1[2];
	orig[0] = end1[0] + offsetx;
	orig[1] = end1[1] + offsety;
	orig[2] = end1[2] + offsetz;
	double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
	double det, inv_det;
	/* find vectors for two edges sharing vert0 */
	SUB(edge1, vert1, vert0);
	SUB(edge2, vert2, vert0);

	/* begin calculating determinant - also used to calculate U parameter */
	CROSS(pvec, dir, edge2);

	/* if determinant is near zero, ray lies in plane of triangle */
	det = DOT(edge1, pvec);

	#ifdef TEST_CULL
	if (det < EPSILON){/* free(dir); free(orig);*/ return 0;}
	/* define TEST_CULL if culling is desired */
	/* calculate distance from vert0 to ray origin */
	SUB(tvec, orig, vert0);
	/* calculate U parameter and test bounds */
	*u = DOT(tvec, pvec);
	if (*u < 0.0 || *u > det){/* free(dir); free(orig);*/ return 0;}
	/* prepare to test V parameter */
	CROSS(qvec, tvec, edge1)
	/* calculate V parameter and test bounds */
	*v = DOT(dir, qvec)
	if (*v < 0.0 || *u + *v > det){/* free(dir); free(orig);*/ return 0;}
	/* calculate t, scale parameters, ray intersects triangle */
	*t = DOT(edge2, qvec);
	inv_det = 1.0 / det;
	*t *= inv_det;
	*u *= inv_det;
	*v *= inv_det;
	#else
	/* the non-culling branch */
	if (det > -EPSILON && det < EPSILON){/* free(dir); free(orig);*/ return 0;}
	inv_det = 1.0 / det;
	/* calculate distance from vert0 to ray origin */
	SUB(tvec, orig, vert0);
	/* calculate U parameter and test bounds */
	*u = DOT(tvec, pvec) * inv_det;
	if (*u < 0.0 || *u > 1.0){/* free(dir); free(orig);*/ return 0;}
	/* prepare to test V parameter */
	CROSS(qvec, tvec, edge1);
	/* calculate V parameter and test bounds */
	*v = DOT(dir, qvec) * inv_det;
	if (*v < 0.0 || *u + *v > 1.0){/* free(dir); free(orig);*/ return 0;}
	/* calculate t, ray intersects triangle */
	*t = DOT(edge2, qvec) * inv_det;
	#endif
//	if(*t < 0 || *t > 0.25*sqrt((end1[0]-end2[0])*(end1[0]-end2[0])+(end1[1]-end2[1])*(end1[1]-end2[1])+(end1[2]-end2[2])*(end1[2]-end2[2]))){ free(dir); free(orig); return 0;}
	if(*t > 1 || *t <0){/*free(dir); free(orig);*/ return 0;}
//	free(dir); free(orig);
//	printf("%lf %lf\n", *t, 0.25*sqrt((end1[0]-end2[0])*(end1[0]-end2[0])+(end1[1]-end2[1])*(end1[1]-end2[1])+(end1[2]-end2[2])*(end1[2]-end2[2])));
	return 1;
}
