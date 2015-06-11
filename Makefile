CC=gcc
CFLAGS=-Wall -Wno-strict-aliasing -O2 -c -ffast-math $(DEBUG)
LFLAGS=-lSDL2 -lm -pthread -lrt
SOURCES=main.c loadmesh.c movemesh.c intersect.c collisions.c drawView.c forces.c tickEnt.c createEnt.c printout.c getObjectVelocity.c entAis.c

OBJECTS=$(SOURCES:.c=.o)

.PHONY: debug remake clean

run: $(OBJECTS)
	$(CC) $(OBJECTS) $(DEBUG) -o run $(LFLAGS)

main.o: main.c globals.h
	$(CC) $(CFLAGS) main.c

loadmesh.o: loadmesh.c globals.h
	$(CC) $(CFLAGS) loadmesh.c

movemesh.o: movemesh.c globals.h
	$(CC) $(CFLAGS) movemesh.c

intersect.o: intersect.c globals.h
	$(CC) $(CFLAGS) intersect.c

collisions.o: collisions.c globals.h
	$(CC) $(CFLAGS) collisions.c

drawView.o: drawView.c globals.h
	$(CC) $(CFLAGS) drawView.c

forces.o: forces.c globals.h
	$(CC) $(CFLAGS) forces.c

tickEnt.o: tickEnt.c globals.h
	$(CC) $(CFLAGS) tickEnt.c

createEnt.o: createEnt.c globals.h
	$(CC) $(CFLAGS) createEnt.c

printout.o: printout.c globals.h
	$(CC) $(CFLAGS) printout.c

getObjectVelocity.o: getObjectVelocity.c globals.h
	$(CC) $(CFLAGS) getObjectVelocity.c

entAis.o: entAis.c globals.h ais/*
	$(CC) $(CFLAGS) entAis.c

clean:
	rm -f *.o run

debug:
	$(MAKE) DEBUG="-g -O0"
	
remake:
	$(MAKE) clean
	$(MAKE)
