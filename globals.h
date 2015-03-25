#define EPSILON 0.000001
#define FRAMERATE 40//IN HERTZ LOL
#define GRAVITY 0//standard earth.. 1.6 for moon. 3.7 for mars
#define DRAG 0.97 //wtf physix 0-1 higher is weakerr
#define CROSS(dest, v1, v2)\
	dest[0]=v1[1]*v2[2]-v1[2]*v2[1];\
	dest[1]=v1[2]*v2[0]-v1[0]*v2[2];\
	dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1, v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest, v1, v2)\
	dest[0]=v1[0]-v2[0];\
	dest[1]=v1[1]-v2[1];\
	dest[2]=v1[2]-v2[2];
#include <SDL2/SDL.h>
extern SDL_Window* window;
extern SDL_Renderer* render;
extern int running;
extern int meshcount;

typedef struct triangle{
	int points[3];
}triangle;
typedef struct ray{
	int ends[2];
}ray;
typedef struct mesh{
	int collision;//did it collide this turn
	int moves;
	double *cpointmatrix;//constant point matrix CURRENTLY NOT USED
	double *rot, *rot2;//rotation vector x, y, z, magnitude, current position(as each tick will be newly rotated from the pattern)
	double vx, vy, vz, vx2, vy2, vz2;//velocity
	double mass, drag;
	int triangles, rays, points;
	triangle *trianglematrix;
	ray *raymatrix;
	double *centermass, *centermass2, radius;//coordinates of center of mass in the "real world"
	double *pointmatrix, *pointmatrix2;
}mesh;

extern mesh* meshes;

typedef struct player{
	double center[3];
	double vx, vy, vz;
	double view[3];
}player;
extern void movemesh(mesh *target);
extern void loadmesh(char name[20], mesh *final, double x, double y, double z);
extern int collisions(mesh *one, mesh *two);
extern int initView();
extern void drawView();
extern int intersect_triangle(double orig[3], double dir[3], double vert0[3], double vert1[3], double vert2[3],  double *t, double *u, double *v, double offsetx, double offsety, double offsetz);
