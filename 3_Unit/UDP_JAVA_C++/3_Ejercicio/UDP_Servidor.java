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

			// RECIBIMOS LOS PRIMEROS NÚMEROS (INT)
			sock.receive(reply);
			// ENVOLVEMOS EL BUFFER EN UN BYTEBUFFER PARA HACER MÁS CÓMODA LA LECTURA DE LOS DATOS
			ByteBuffer buf = ByteBuffer.wrap(buffer);
			// ESPECIFICAMOS EL ORDEN LITTLE_ENDIAN PARA LA CORRECTA INTERPRETACIÓN DE LOS DATOS
			buf.order(ByteOrder.LITTLE_ENDIAN);


			// RECIBIENDO INTS
			int entero1 = buf.getInt();
			int entero2 = buf.getInt();
			System.out.println("INT MIN = " + entero1 + " MAX = " + entero2);

			// RECIBIENDO SHORTS
			buf.clear();							// Reseteamos el "apuntador" del ByteBuffer
			sock.receive(reply);				
			short short1 = buf.getShort();
			short short2 = buf.getShort();
			System.out.println("SHORT MIN = " + short1 + " MAX = " + short2);
			
			// RECIBIENDO LONGS
			buf.clear();							// Reseteamos el "apuntador" del ByteBuffer
			sock.receive(reply);				
			long long1 = buf.getLong();
			long long2 = buf.getLong();
			System.out.println("LONG MIN = " + long1 + " MAX = " + long2);

			// RECIBIENDO FLOATS
			buf.clear();							// Reseteamos el "apuntador" del ByteBuffer
			sock.receive(reply);				
			float float1 = buf.getFloat();
			float float2 = buf.getFloat();
			System.out.println("FLOAT MIN = " + float1 + " MAX = " + float2);

			// RECIBIENDO LONGS
			buf.clear();							// Reseteamos el "apuntador" del ByteBuffer
			sock.receive(reply);				
			double double1 = buf.getDouble();
			double double2 = buf.getDouble();
			System.out.println("DOUBLE MIN = " + double1 + " MAX = " + double2);
		}
		catch(IOException e)
		{
			System.err.println("IOException " + e);
		}
	}
}
