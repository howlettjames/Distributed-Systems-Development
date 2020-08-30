import java.io.*;
import java.net.*;
import java.nio.ByteBuffer; //Para manejo de ByteBuffer
import java.nio.ByteOrder;

public class UDP_Servidor
{
	public static void main(String args[])
	{
		DatagramSocket sock = null;
		int port = 7200;

		try
		{
			sock = new DatagramSocket(port);
			
			// ASIGNANDO MEMORIA PARA RECIBIR LAS RESPUESTAS			
			byte[] buffer = new byte[65536];
			// CREANDO DATAGRAMA DEL TAMAÑO DEL BUFFER
			DatagramPacket reply = new DatagramPacket(buffer, buffer.length);

			// RECIBIMOS LA ESTRUCTURA
			sock.receive(reply);
			// ENVOLVEMOS EL BUFFER EN UN BYTEBUFFER PARA HACER MÁS CÓMODA LA LECTURA DE LOS DATOS
			ByteBuffer buf = ByteBuffer.wrap(buffer);
			// ESPECIFICAMOS EL ORDEN LITTLE_ENDIAN PARA LA CORRECTA INTERPRETACIÓN DE LOS DATOS
			buf.order(ByteOrder.LITTLE_ENDIAN);

			// LEYENDO DATOS DE LA ESTRUCTURA
			int entero = buf.getInt();
			float flotante = buf.getFloat();
			double doble = buf.getDouble();
			long largo = buf.getLong();

			System.out.println("ESTRUCTURA RECIBIDA: ");
			System.out.println("ENTERO: " + entero + "\nDOBLE: " + doble + "\nFLOTANTE: " + flotante + "\nLARGO: " + largo);
		}
		catch(IOException e)
		{
			System.err.println("IOException " + e);
		}
	}
}
