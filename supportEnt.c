#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void supportEnt(ent *target){
	double zeros[3] = {0, 0, 0};
	double down[3] = {0, 0, target->radius};
	int temptri;
	int tempmesh;
	double *pone, *ptwo, *pthree;
	double t, u, v;
	double offsetx, offsety, offsetz;
	mesh *meshpoint;
	triangle *tri;
	double record = 2;
	for(tempmesh = meshcount-1; tempmesh >= 0; tempmesh--){
		meshpoint = meshes[tempmesh];
		tri = meshpoint->trianglematrix;
		offsetx = target->center[0]-meshpoint->centermass[0];
		offsety = target->center[1]-meshpoint->centermass[1];
		offsetz = target->center[2]-meshpoint->centermass[2];
		for(temptri = meshpoint->triangles-1; temptri >= 0; temptri--){
			pone = &meshpoint->pointmatrix[tri[temptri].points[0]*3];
			ptwo = &meshpoint->pointmatrix[tri[temptri].points[1]*3];
			pthree = &meshpoint->pointmatrix[tri[temptri].points[2]*3];
			if(intersect_triangle(zeros, down, pone, ptwo, pthree, &t, &u, &v, offsetx, offsety, offsetz) && t <= 1 && t >= 0){
				if(t < record) record = t;
			}
		}
	}
	if(record != 2){
		printf("%lf\n", record*target->radius);
		double requiredvz = -(1-record);
		if(requiredvz < target->vz){
			double push = requiredvz - target->vz;
			applyEntForce(target, 0, 0, 1, push);
		}
	}
}
