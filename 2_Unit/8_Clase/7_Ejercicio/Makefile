test: test.cpp Semaforo.o
	g++ test.cpp Semaforo.o -o test -lpthread
Semaforo.o: Semaforo.cpp Semaforo.h
	g++ -std=c++11 Semaforo.cpp -c -lpthread
