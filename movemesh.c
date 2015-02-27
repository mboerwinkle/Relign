#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "globals.h"

void movemesh(mesh *target){
	target->rot[3] = sqrt(target->rot[0]*target->rot[0]+target->rot[1]*target->rot[1]+target->rot[2]*target->rot[2]);
	if(target->rot[3] >= M_PI) target->rot[3] -= (2*M_PI);
	printf("%lf\n", target->rot[3]);
	int temp;
	double m00, m01, m02, m10, m11, m12, m20, m21, m22;//rotation matrix www.euclideanspace.com/maths/geomety/rotations/conversions/quaternionToMatrix/index.htm
	double qx = target->rot[0]*sin(target->rot[3]/2);
	double qy = target->rot[1]*sin(target->rot[3]/2);
	double qz = target->rot[2]*sin(target->rot[3]/2);
	double qw = cos(target->rot[3]/2);
	double sqw = qw*qw;
	double sqx = qx*qx;
	double sqy = qy*qy;
	double sqz = qz*qz;

	double invs = 1 / (sqx + sqy + sqz + sqw);
	m00 = ( sqx - sqy - sqz + sqw)*invs; // since sqw + sqx + sqy + sqz =1/invs*invs
	m11 = (-sqx + sqy - sqz + sqw)*invs;
	m22 = (-sqx - sqy + sqz + sqw)*invs;
    
	double tmp1 = qx*qy;
	double tmp2 = qz*qw;
	m10 = 2.0 * (tmp1 + tmp2)*invs;
	m01 = 2.0 * (tmp1 - tmp2)*invs;
    
	tmp1 = qx*qz;
	tmp2 = qy*qw;
	m20 = 2.0 * (tmp1 - tmp2)*invs;
	m02 = 2.0 * (tmp1 + tmp2)*invs;
	tmp1 = qy*qz;
	tmp2 = qx*qw;
	m21 = 2.0 * (tmp1 + tmp2)*invs;
	m12 = 2.0 * (tmp1 - tmp2)*invs; 
	double x, y, z;
	for(temp = target->points-1; temp >= 0; temp--){
		x = target->pointmatrix[temp*3+0];
		y = target->pointmatrix[temp*3+1];
		z = target->pointmatrix[temp*3+2];
		target->pointmatrix[temp*3+0] = (m00*x+m01*y+m02*z);
		target->pointmatrix[temp*3+1] = (m10*x+m11*y+m12*z);
		target->pointmatrix[temp*3+2] = (m20*x+m21*y+m22*z);
	}
	target->centermass[0] += target->vx;
	target->centermass[1] += target->vy;
	target->centermass[2] += target->vz;
}
