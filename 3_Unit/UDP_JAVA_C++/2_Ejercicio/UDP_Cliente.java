import java.io.*;
import java.net.*;
import java.nio.ByteBuffer; //Para manejo de ByteBuffer
import java.nio.ByteOrder;

public class UDP_Cliente
{
	public static void main(String args[])
	{
		DatagramSocket sock = null;
		int port = 7200;

		try
		{
			// CREANDO SOCKET
			sock = new DatagramSocket();
			InetAddress host = InetAddress.getByName("localhost");
			
			final ByteBuffer buf = ByteBuffer.allocate(16); // Reserva 16 bytes para soportar todos los tipos de dato
			buf.order(ByteOrder.LITTLE_ENDIAN);

			System.out.println("Tipo\tMínimo\tMáximo\tBytes");
			System.out.println("byte\t" + Byte.MIN_VALUE + "\t" + Byte.MAX_VALUE + "\t1");
			System.out.println("short\t" + Short.MIN_VALUE + "\t" + Short.MAX_VALUE + "\t" + Short.BYTES);
			System.out.println("int\t" + Integer.MIN_VALUE + "\t" + Integer.MAX_VALUE + "\t" + Integer.BYTES);
			System.out.println("long\t" + Long.MIN_VALUE + "\t" + Long.MAX_VALUE + "\t" + Long.BYTES);
			System.out.println("float\t" + Float.MIN_VALUE + "\t" + Float.MAX_VALUE + "\t" + Float.BYTES);
			System.out.println("double\t" + Double.MIN_VALUE + "\t" + Double.MAX_VALUE + "\t" + Double.BYTES);

			// ENVIANDO LOS INT
			buf.putInt(Integer.MIN_VALUE); //Mínimo valor entero
			buf.putInt(Integer.MAX_VALUE); //Máximo valor entero

			DatagramPacket dp = new DatagramPacket(buf.array() , buf.limit(), host , port);
			sock.send(dp);

			// ENVIANDO SHORTS
			buf.clear();
			buf.putShort(Short.MIN_VALUE);
			buf.putShort(Short.MAX_VALUE);
			
			dp = new DatagramPacket(buf.array() , buf.limit(), host , port);
			sock.send(dp);

			// ENVIANDO LONGS
			buf.clear();
			buf.putLong(Long.MIN_VALUE);
			buf.putLong(Long.MAX_VALUE);
			
			dp = new DatagramPacket(buf.array() , buf.limit(), host , port);
			sock.send(dp);

			// ENVIANDO FLOATS
			buf.clear();
			buf.putFloat(Float.MIN_VALUE);
			buf.putFloat(Float.MAX_VALUE);
			
			dp = new DatagramPacket(buf.array() , buf.limit(), host , port);
			sock.send(dp);

			// ENVIANDO DOUBLES
			buf.clear();
			buf.putDouble(Double.MIN_VALUE);
			buf.putDouble(Double.MAX_VALUE);
			
			dp = new DatagramPacket(buf.array() , buf.limit(), host , port);
			sock.send(dp);
		}
		catch(IOException e)
		{
			System.err.println("IOException " + e);
		}
	}
}
