C_FLAGS := -I/usr/include/SDL2 -D_REENTRANT -Ofast
LIBS := -lSDL2 -lSDL2_ttf
UTILS := src/utils/*.cpp 
SCENE_OBJECTS := src/objects/*.cpp


build: bin/projectBOIDS

bin/projectBOIDS: utils scene_objects main.o
	g++ *.o -o $@ ${LIBS} ${C_FLAGS}

main.o: global.o Window.o Texture.o src/main.cpp
	g++ src/main.cpp -c ${LIBS} ${C_FLAGS} -o $@
Texture.o: Position.o src/Texture.cpp src/Texture.hpp
	g++ src/Texture.cpp -c ${LIBS} ${C_FLAGS}
Window.o: Position.o src/Window.cpp src/Window.hpp
	g++ src/Window.cpp -c ${LIBS} ${C_FLAGS}
global.o: Window.o Texture.o src/global.cpp src/global.hpp src/World.hpp src/utils/random.hpp
	g++ src/global.cpp -c ${LIBS} ${C_FLAGS}


utils: Position.o ErrorHandling.o

ErrorHandling.o: src/utils/ErrorHandling.cpp src/utils/ErrorHandling.hpp
	g++ src/utils/ErrorHandling.cpp -c ${LIBS} ${C_FLAGS}
Position.o: ErrorHandling.o src/utils/Position.cpp src/utils/ErrorHandling.hpp
	g++ src/utils/Position.cpp -c ${LIBS} ${C_FLAGS}


scene_objects: Boid.o Objects.o

Boid.o: Objects.o src/objects/Boid.cpp src/objects/Boid.hpp
	g++ src/objects/Boid.cpp -c ${LIBS} ${C_FLAGS}
Objects.o: Position.o src/objects/Objects.cpp src/objects/Objects.hpp
	g++ src/objects/Objects.cpp -c ${LIBS} ${C_FLAGS}

clean:
	rm *.o