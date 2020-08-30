animacion: gfx.o animacion.o SpaceObject.o
	g++ gfx.o animacion.o SpaceObject.o -o animacion -lX11
animacion.o: animacion.cpp
	g++ animacion.cpp -c
SpaceObject.o: SpaceObject.cpp SpaceObject.h
	g++ SpaceObject.cpp -c
gfx.o: gfx.c
	gcc gfx.c -c
