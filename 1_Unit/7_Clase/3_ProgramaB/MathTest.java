import java.util.*;

public class MathTest
{
	public static void main(String[] args) 
	{
		final long MAX = 50000000;
		double i = 0;
		double seno = 0, coseno = 0, tangente = 0, loga = 0, raiz = 0;

		while(i < MAX)
		{
			seno += Math.sin(i);
			coseno += Math.cos(i);
			tangente += Math.tan(i);
			loga += Math.log(i);
			raiz += Math.sqrt(i);
			i++;
		}
	}	
}