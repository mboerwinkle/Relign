#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "collisions.h"

void collisions(){
	int temp, temp2;
	mesh* mesh1;
	ent* ent1;
	for(temp = 0; temp < meshcount; temp++){
		mesh1 = meshes[temp];
		mesh1->collision = 0;
		for(temp2 = temp-1; temp2 >=0; temp2--){
			if(ObjObjCollisions(mesh1, meshes[temp2])){
				mesh1->collision = 1;
				meshes[temp2]->collision = 1;
			}
		}
	}
	for(temp = 0; temp < meshcount; temp++){
		mesh1 = meshes[temp];
		if(mesh1->collision) continue;;
		for(temp2 = 0; temp2 < meshcount; temp2++){
			if(temp2 == temp || !meshes[temp2]->collision) continue;
			if(backupObjObjCollisions(mesh1, meshes[temp2])){
				mesh1->collision = 1;
			}
		}
	}
	for(temp = 0; temp < entcount; temp++){
		ent1 = ents[temp];
		for(temp2 = 0; temp2 < entcount; temp2++){
			EntObjCollisions(meshes[temp2], ent1);
		}
	}
}
