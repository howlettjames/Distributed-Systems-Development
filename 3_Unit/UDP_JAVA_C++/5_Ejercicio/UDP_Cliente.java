import java.io.*;
import java.net.*;
import java.nio.ByteBuffer; //Para manejo de ByteBuffer
import java.nio.ByteOrder;

public class UDP_Cliente
{
	// CREAMOS UNA CLASE MENSAJE COMO SI SE TRATASE DE LA ESTRUCTURA MANEJADA EN C/C++
	public class Mensaje
	{
		public int entero;
		public float flotante;
		public  double doble;
		public long largo;

		public Mensaje(int entero, float flotante, double doble, long largo)
		{
			this.entero = entero;
			this.flotante = flotante;
			this.doble = doble;
			this.largo = largo;
		}

		// AGREGAMOS EL MÉTODO PARA ENVIAR LA CLASE EN FORMA DE CADENA
		public String toString()
		{
			return entero + " " + flotante + " " + doble + " " + largo;
		}
	}

	public void startUDP()
	{
		DatagramSocket sock = null;
		int port = 7200;

		try
		{
			// CREANDO SOCKET
			sock = new DatagramSocket();
			InetAddress host = InetAddress.getByName("localhost");
			
			// CREANDO UN OBJETO DE LA CLASE MENSAJE	
			Mensaje message = new Mensaje(Integer.MAX_VALUE, Float.MAX_VALUE, Double.MAX_VALUE, Long.MAX_VALUE);

			// RESERVANDO MEMORIA PARA GUARDAR EL MENSAJE EN FORMA DE CADENA
			final ByteBuffer buf = ByteBuffer.allocate(500);
			buf.order(ByteOrder.LITTLE_ENDIAN);

			// COLOCAMOS LOS MIEMBROS DE LA CLASE MENSAJE DENTRO DEL BUFFER A MANERA DE CADENA TRANSFORMADA EN BYTES
			buf.put(message.toString().getBytes());

			// ENVIAMOS EL PAQUETE
			DatagramPacket dp = new DatagramPacket(buf.array() , buf.limit(), host , port);
			sock.send(dp);
			
		}
		catch(IOException e)
		{
			System.err.println("IOException " + e);
		}
	}

	public static void main(String args[])
	{
		UDP_Cliente udp = new UDP_Cliente();
		udp.startUDP();	
	}
}
