CC=gcc
CFLAGS=-Wall -Wno-strict-aliasing -O2 -c -ffast-math $(DEBUG)
LFLAGS=-lSDL2 -lm -pthread -lrt
SOURCES=main.c loadmesh.c movemesh.c
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

clean:
	rm -f *.o run

debug:
	$(MAKE) DEBUG="-g -O0"
	
remake:
	$(MAKE) clean
	$(MAKE)
