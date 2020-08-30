
cliente: cliente.cpp Solicitud.o registro.h
	g++ cliente.cpp Solicitud.o SocketDatagrama.o PaqueteDatagrama.o -o cliente
servidor: servidor.cpp Respuesta.o registro.h
	g++ servidor.cpp Respuesta.o SocketDatagrama.o PaqueteDatagrama.o -o servidor
Respuesta.o: Respuesta.cpp SocketDatagrama.o Respuesta.h mensaje.h
	g++ Respuesta.cpp -c
Solicitud.o: Solicitud.cpp SocketDatagrama.o Solicitud.h mensaje.h
	g++ Solicitud.cpp -c
SocketDatagrama.o: SocketDatagrama.cpp PaqueteDatagrama.o SocketDatagrama.h
	g++ SocketDatagrama.cpp -c
PaqueteDatagrama.o: PaqueteDatagrama.cpp PaqueteDatagrama.h
	g++ PaqueteDatagrama.cpp -c
