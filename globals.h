#define EPSILON 0.000001
#define CROSS(dest, v1, v2)\
	dest[0]=v1[1]*v2[2]-v1[2]*v2[1];\
	dest[1]=v1[2]*v2[0]-v1[0]*v2[2];\
	dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1, v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest, v1, v2)\
	dest[0]=v1[0]-v2[0];\
	dest[1]=v1[1]-v2[1];\
	dest[2]=v1[2]-v2[2];
typedef struct point{
	double x, y, z;
}point;
typedef struct triangle{
	int points[3];
}triangle;
typedef struct ray{
	int ends[2];
}ray;
typedef struct mesh{
	double *cpointmatrix;//constant point matrix
	double rotvx, rotvy, rotvz, rotvm, rotcp;//rotation vector x, y, z, magnitude, current position(as each tick will be newly rotated from the pattern)
	double vx, vy, vz;//velocity
	double mass;
	int triangles, rays, points;
	triangle *trianglematrix;
	ray *raymatrix;
	double centermass[3];//coordinates of center of mass in the "real world"
	double *pointmatrix;
	double **metapointmatrix;
}mesh;
extern void movemesh(mesh *target);
extern void loadmesh(char name[20], mesh *final, double x, double y, double z);
extern int collisions(mesh *one, mesh *two);
extern int intersect_triangle(double orig[3], double dir[3], double vert0[3], double vert1[3], double vert2[3],  double *t, double *u, double *v, double offsetx, double offsety, double offsetz);
