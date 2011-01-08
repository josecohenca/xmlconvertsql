namespace mASN1.framework.experimental
{
	using System;

	public class DERPrettyPrinter 
	{
		public static int wrapNumber = 20;
		public static int maxBytes   = 84;
	
		public static void Print(byte[] byteArray) 
		{
			int len = byteArray.Length;
			int loop;

			if (len > maxBytes) { 
				loop = maxBytes;
			}
			else 
			{
				loop = len;
			}
			 


			for(int i=0; i < loop; i++) 
			{
				if (i%wrapNumber==0) { Console.WriteLine(); }
				Console.Write(String.Format("{0}:", byteArray[i].ToString("X")).PadLeft(3, '0'));
			}

			if (len > maxBytes) 
			{
				Console.Write(" ... continued ...");
			}

			Console.WriteLine();
		}
	
	
		public static string ByteArrayToString(byte[] byteArray) 
		{
			int len = byteArray.Length;
			int loop;

			string retstring = "";

			if (len > maxBytes) 
			{ 
				loop = maxBytes;
			}
			else 
			{
				loop = len;
			}
			 
			for(int i=0; i < loop; i++) 
			{
				if (i%wrapNumber==0) { retstring+="\n"; }
				retstring +=
					(String.Format("{0}:", byteArray[i].ToString("X")).PadLeft(3, '0'));
			}

			if (len > maxBytes) 
			{
				retstring += " ... continued ... ";
			}

			return retstring;

		}
	}
}