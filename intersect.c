#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "globals.h"
static double orig[3], dir[3], t, u, v;

void norm(double * target){
	double howmuch = sqrt(target[0]*target[0]+target[1]*target[1]+target[2]*target[2]);
	target[0] = target[0] / howmuch;
	target[1] = target[1] / howmuch;
	target[2] = target[2] / howmuch;
}

double distance(double vect[3], double point[3]){
	double a[3] = {vect[0], vect[1], vect[2]};
	double b[3] = {point[0], point[1], point[2]};
	double distb = sqrt(b[0]*b[0] + b[1]*b[1] + b[2]*b[2]);
	double dista = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
	double angle = acos(DOT(a, b)/(dista*distb));
	double ans = sin(angle)*distb;
//	if(isnan(ans) || !isfinite(ans)) return 0;
	return ans;
}

double * collisions(mesh *one, mesh *two){
	double z;//the third barycentric coordinate
	double *pone, *ptwo, *pthree;
	int temp, tempray, temptri;
	double *col = NULL; //estimated center of collision
	int howmany = 0;//not to be confused with the wack-ass corn...
	mesh *tempmesh;
	double offsetx, offsety, offsetz;
	for(temp = 0; temp < 2; temp++){
		offsetx = one->centermass[0]-two->centermass[0];
		offsety = one->centermass[1]-two->centermass[1];
		offsetz = one->centermass[2]-two->centermass[2];
		for(tempray = one->rays-1; tempray >=0; tempray--){
			for(temptri = two->triangles-1; temptri >=0; temptri--){
				if(intersect_triangle(&one->pointmatrix[one->raymatrix[tempray].ends[0]*3], &one->pointmatrix[one->raymatrix[tempray].ends[1]*3], &two->pointmatrix[two->trianglematrix[temptri].points[0]*3], &two->pointmatrix[two->trianglematrix[temptri].points[1]*3], &two->pointmatrix[two->trianglematrix[temptri].points[2]*3], &t, &u, &v, offsetx, offsety, offsetz)){
					if(col == NULL) col = calloc(sizeof(double), 6);//first 3 for point of collision, next 9 for force
					z = 1-(u+v);//find the third barycentric coordinate
					pone = &two->pointmatrix[two->trianglematrix[temptri].points[0]*3];
					ptwo = &two->pointmatrix[two->trianglematrix[temptri].points[1]*3];
					pthree = &two->pointmatrix[two->trianglematrix[temptri].points[2]*3];
					col[0] = (z*pone[0]+u*ptwo[0]+v*pthree[0]+two->centermass[0]+col[0]*howmany)/(howmany+1);
					col[1] = (z*pone[1]+u*ptwo[1]+v*pthree[1]+two->centermass[1]+col[1]*howmany)/(howmany+1);
					col[2] = (z*pone[2]+u*ptwo[2]+v*pthree[2]+two->centermass[2]+col[2]*howmany)/(howmany+1);
					howmany++;
					
				}
			}
		}
		tempmesh = one;
		one = two;
		two = tempmesh;
	}
	if(col != NULL){
		double rotationspeed;
		double colloc[3];
		col[3] = 0+one->vx-two->vx;
		col[4] = 0+one->vy-two->vy;
		col[5] = 0+one->vz-two->vz;

		double speed;
		double uv[3];
		if(!(one->rot[0] == 0 && one->rot[1] == 0 && one->rot[2] == 0)){
			rotationspeed = sqrt(one->rot[0]*one->rot[0]+one->rot[1]*one->rot[1]+one->rot[2]*one->rot[2]);
			colloc[0] = col[0] - one->centermass[0];
			colloc[1] = col[1] - one->centermass[1];
			colloc[2] = col[2] - one->centermass[2];
			speed = rotationspeed*(distance(one->rot, colloc)/one->radius);//speed of rotation of collision point
			uv[0] = (one->rot[1]*col[2]-one->rot[2]*col[1]);
			uv[1] = (one->rot[2]*col[0]-one->rot[0]*col[2]);
			uv[2] = (one->rot[0]*col[1]-one->rot[1]*col[0]);
			norm(uv);
			col[3] -= uv[0]*speed;
			col[4] -= uv[1]*speed;
			col[5] -= uv[2]*speed;
		}

		if(!(two->rot[0] == 0 && two->rot[1] == 0 && two->rot[2] == 0)){
			rotationspeed = sqrt(two->rot[0]*two->rot[0]+two->rot[1]*two->rot[1]+two->rot[2]*two->rot[2]);
			colloc[0] = col[0] - two->centermass[0];
			colloc[1] = col[1] - two->centermass[1];
			colloc[2] = col[2] - two->centermass[2];
			speed = rotationspeed*(distance(two->rot, colloc)/two->radius);
			uv[0] = (two->rot[1]*col[2]-two->rot[2]*col[1]);
			uv[1] = (two->rot[2]*col[0]-two->rot[0]*col[2]);
			uv[2] = (two->rot[0]*col[1]-two->rot[1]*col[0]);
			norm(uv);
			col[3] += uv[0]*speed;
			col[4] += uv[1]*speed;
			col[5] += uv[2]*speed;
		}
			rotationspeed = sqrt(col[3]*col[3]+col[4]*col[4]+col[5]*col[5]);
			
			colloc[0] = col[0] - one->centermass[0];
			colloc[1] = col[1] - one->centermass[1];
			colloc[2] = col[2] - one->centermass[2];
			z = distance(&col[3], colloc)/one->radius;//from here on, z is used for 1 divided by the quantity of force for rotation
/*			printf("z %lf\n", z);
			printf("0 %lf\n", col[3]);
			printf("1 %lf\n", col[4]);
			printf("2 %lf\n", col[5]);
			printf("rad %lf\n\n", one->radius);*/
			one->vx -= col[3]*(1-z)*(one->mass/two->mass);
			one->vy -= col[4]*(1-z)*(one->mass/two->mass);
			one->vz -= col[5]*(1-z)*(one->mass/two->mass);
			uv[0] = col[4]*colloc[2]-col[5]*colloc[1]; 
			uv[1] = col[5]*colloc[0]-col[3]*colloc[2];
			uv[2] = col[3]*colloc[1]-col[4]*colloc[0];
			norm(uv);
			two->rot[0] += uv[0]*z*rotationspeed;
			two->rot[1] += uv[1]*z*rotationspeed;
			two->rot[2] += uv[2]*z*rotationspeed;
			

			colloc[0] = col[0] - two->centermass[0];
			colloc[1] = col[1] - two->centermass[1];
			colloc[2] = col[2] - two->centermass[2];
			z = distance(&col[3], colloc)/two->radius;//from here on, z is used for 1 divided by the quantity of force for rotation
/*			printf("%lf\n", z);
			printf("0 %lf\n", col[3]);
			printf("1 %lf\n", col[4]);
			printf("2 %lf\n", col[5]);
			printf("rad %lf\n\n", two->radius);*/
			two->vx += col[3]*(1-z)*(two->mass/one->mass);
			two->vy += col[4]*(1-z)*(two->mass/one->mass);
			two->vz += col[5]*(1-z)*(two->mass/one->mass);
			uv[0] = col[4]*colloc[2]-col[5]*colloc[1]; 
			uv[1] = col[5]*colloc[0]-col[3]*colloc[2];
			uv[2] = col[3]*colloc[1]-col[4]*colloc[0];
			norm(uv);
			one->rot[0] -= uv[0]*z*rotationspeed;
			one->rot[1] -= uv[1]*z*rotationspeed;
			one->rot[2] -= uv[2]*z*rotationspeed;
			
/*		for(temp = 0; temp < 2; temp++){
			colloc[0] = col[0] - one->centermass[0];
			colloc[1] = col[1] - one->centermass[1];
			colloc[2] = col[2] - one->centermass[2];
			if(!(one->rot[0] == 0 && one->rot[1] == 0 && one->rot[2] == 0)){
				if(sqrt(one->rot[0]*one->rot[0]+one->rot[1]*one->rot[1]+one->rot[2]*one->rot[2]) != 1){
					double change = 1/sqrt(one->rot[0]*one->rot[0]+one->rot[1]*one->rot[1]+one->rot[2]*one->rot[2]);
					one->rot[0] *= change;
					one->rot[1] *= change;
					one->rot[2] *= change;
					if(change < 0){
						puts("wot?");
						one->rotationspeed *= -1;
						one->rot[3] += M_PI;
						if(one->rot[3] >= 2*M_PI) one->rot[3] -= 2*M_PI;
					}
				}
			}
			double speed = one->rotationspeed*(distance(one->rot, colloc)/one->radius);//speed of rotation of collision point
			col[3] += one->rot[1]*speed*col[2]-one->rot[2]*speed*col[1];
			col[4] += one->rot[2]*speed*col[0]-one->rot[0]*speed*col[2];
			col[5] += one->rot[0]*speed*col[1]-one->rot[1]*speed*col[0];
			tempmesh = one;
			one = two;
			two = tempmesh;
//		sqrt((col[0]-one->centermass[0])*(col[0]-one->centermass[0])+(col[1]-one->centermass[1])*(col[1]-one->centermass[1])+(col[2]-one->centermass[2])*(col[2]-one->centermass[2]));
		}
		for(temp = 0; temp < 2; temp++){
			colloc[0] = two->centermass[0] - col[0];
			colloc[1] = two->centermass[1] - col[1];
			colloc[2] = two->centermass[2] - col[2];
			z = distance(&col[3], colloc)/two->radius;//from here on, z is used for 1 divided by the quantity of force for rotation
			two->vx += col[3]*(1-z)*(two->mass/one->mass);
			two->vy += col[4]*(1-z)*(two->mass/one->mass);
			two->vz += col[5]*(1-z)*(two->mass/one->mass);
			col[3] *= -1;
			col[4] *= -1;
			col[5] *= -1;
			tempmesh = one;
			one = two;
			two = tempmesh;
		}*/
	}
	return col;
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
