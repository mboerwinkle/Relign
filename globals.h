#define WIDTH 640
#define HEIGHT 480
#define VIEWZOOM 100

#define EPSILON 0.000001
#define FRAMERATE 40//IN HERTZ LOL
#define ELASTICITY_NOMOVE 1.2//defines how elastic (for collisions) the ground and other non-moving objects are (mesh.moves = 0). less than 1 or more than 2 are not recommended.
#define ELASTICITY_MOVE 1//defines how elastic normal collisions between two objects are. from 0 to 1
#define GRAVITY 9.8// for standard earth.. 1.6 for moon. 3.7 for mars. m/s/s
#define DRAG 1
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
extern int entcount;

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

extern mesh** meshes;

typedef struct dataUser{//user, as in "I FIGHT FOR THE USER"
	//ip or something
	//chatting or something
	int heartbeat;//time since the last heartbeat
//	int swagga;
//	tweeta: hashtag include
}dataUser;

typedef struct ent{
	int impulse;//bits: jump, forward, backward, left, right ...
	void (*aiFunc)(struct ent*);//the controlling function that changes impulse. defined by aitype
	double center[3];//at the center of the feet?
	double center2[3];
	double vx, vy, vz, vx2, vy2, vz2;//velocities
	double height;
	int hp;//hp...
	int shield;//you know: the thing that protects you, then goes beep beep beep, and then goes booooouuuuup.
//	int vang;//view angle   -   this should be handled uniquely by the client
	int vdir;//view direction   -   needed for drawing sprites.
	void *data;//persistent(between ticks) information for use by the actFunc
}ent;

extern ent** ents;

extern void printcentermesh(mesh *target);

extern void applyForce(mesh *target, double x, double y, double z, double dirx, double diry, double dirz, double power);
extern void drawLine(double one[3], double two[3]);
extern double distance(double vect[3], double point[3]);
extern void norm(double target[3]);
extern void movemesh(mesh *target);
extern void loadmesh(char name[20], mesh *final, double x, double y, double z);
extern void createEnt(ent *final, int type, int aitype, char name[20], double x, double y, double z);
extern void tickAllObjects();
extern void tickAllEnts();
extern void tickEnt(ent *target);
extern void collisions();
extern void getObjectVelocity(mesh* target, double colloc[3], double colforce[3]);
extern int initView();
extern void drawView();
extern int intersect_triangle(double orig[3], double dir[3], double vert0[3], double vert1[3], double vert2[3],  double *t, double *u, double *v, double offsetx, double offsety, double offsetz);
