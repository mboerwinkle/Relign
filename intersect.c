#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "globals.h"
static double orig[3], dir[3];

void norm(double target[3], double dist){
/*	if(isnan(target[0]) || !isfinite(target[0]));
	if(isnan(target[1]) || !isfinite(target[1]));
	if(isnan(target[2]) || !isfinite(target[2]));*/
	if(target[0] == 0 && target[1] == 0 && target[2] == 0) return;
	double howmuch = sqrt(target[0]*target[0]+target[1]*target[1]+target[2]*target[2]);
	target[0] *= dist/howmuch;
	target[1] *= dist/howmuch;
	target[2] *= dist/howmuch;
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

int intersect_triangle(double end1[3], double end2[3], double vert0[3], double vert1[3], double vert2[3], double *t, double *u, double *v, double offsetx, double offsety, double offsetz){//credit Tomas Moller and Ben Trumbore "Fast, minimum storage ray/triangle intersection"
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
