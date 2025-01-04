C_FLAGS := -I/usr/include/SDL2 -D_REENTRANT -Ofast
LIBS := -lSDL2 -lSDL2_ttf
UTILS := src/utils/*.cpp 
SCENE_OBJECTS := src/objects/*.cpp


build: utils scene_objects
	g++ src/*.cpp *.o -o bin/projectBOIDS ${LIBS} ${C_FLAGS}

utils:
	g++ ${UTILS} -c ${LIBS} ${C_FLAGS}

scene_objects:
	g++ ${SCENE_OBJECTS} -c ${LIBS} ${C_FLAGS}

clean:
	rm *.o