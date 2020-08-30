test: test.cpp Rectangulo.o
	g++ test.cpp Rectangulo.o Coordenada.o -o test
Rectangulo.o: Rectangulo.cpp Coordenada.o Rectangulo.h
	g++ Rectangulo.cpp -c
Coordenada.o: Coordenada.cpp Coordenada.h
	g++ Coordenada.cpp -c