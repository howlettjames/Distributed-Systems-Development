emisor: emisor.cpp SocketMulticast.o
	g++ emisor.cpp SocketMulticast.o PaqueteDatagrama.o -o emisor
receptor: receptor.cpp SocketMulticast.o
	g++ receptor.cpp SocketMulticast.o PaqueteDatagrama.o -o receptor
SocketMulticast.o: SocketMulticast.cpp PaqueteDatagrama.o SocketMulticast.h
	g++ SocketMulticast.cpp -c
PaqueteDatagrama.o: PaqueteDatagrama.cpp PaqueteDatagrama.h
	g++ PaqueteDatagrama.cpp -c
