import java.util.*;
import java.util.Random;

public class TString
{
	public static void main(String[] args) 
	{
		final int N = 300000;
		// Scanner input = new Scanner(System.in);
		Random rand = new Random();
		int i, n, found;
		StringBuilder cadenota = new StringBuilder();
		
		for(n = 0; n < N; n++)
		{
			for(i = 0; i < 3; i++)
				cadenota.append((char) (rand.nextInt(26) + 65));
			
			cadenota.append(" ");
		}

		// System.out.println("CADENOTA: " + cadenota);

		found = 0;
		int index_of = cadenota.indexOf("IPN");
		while(index_of >= 0)
		{
			index_of = cadenota.indexOf("IPN", index_of + 1);
			found++;
		}

		System.out.println("Found: " + found);
	}
}