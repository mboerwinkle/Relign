#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "globals.h"
static double orig[3], dir[3], t, u, v;

int collisions(mesh *one, mesh *two){
	int temp, tempray, temptri;
	mesh *tempmesh;
	double offsetx, offsety, offsetz;
	for(temp = 0; temp < 2; temp++){
		offsetx = one->centermass[0]-two->centermass[0];
		offsety = one->centermass[1]-two->centermass[1];
		offsetz = one->centermass[2]-two->centermass[2];
		for(tempray = one->rays-1; tempray >=0; tempray--){
			for(temptri = two->triangles-1; temptri >=0; temptri--){
				if(intersect_triangle(&one->pointmatrix[one->raymatrix[tempray].ends[0]*3], &one->pointmatrix[one->raymatrix[tempray].ends[1]*3], &two->pointmatrix[two->trianglematrix[temptri].points[0]*3], &two->pointmatrix[two->trianglematrix[temptri].points[1]*3], &two->pointmatrix[two->trianglematrix[temptri].points[2]*3], &t, &u, &v, offsetx, offsety, offsetz)) return 1;
			}
		}
		tempmesh = one;
		one = two;
		two = tempmesh;
	}
	return 0;
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
