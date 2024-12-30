C_FLAGS := -I/usr/include/SDL2 -D_REENTRANT -Ofast
LIBS := -lSDL2 -lSDL2_ttf
SRC_FILES := src/*.cpp src/utils/*.cpp src/objects/*.cpp


build:
	g++ ${SRC_FILES} -o bin/projectBOIDS ${LIBS} ${C_FLAGS}

clean:
	rm *.o