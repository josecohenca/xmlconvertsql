namespace OID.Project
{
	using System;
	using System.Collections;

	using System.IO;

	using mASN1;


	/* SINGLETON pattern */

	/* ovdje hocu OIDHashtable */

	/// <summary>
	/// Summary description for OIDDictionary.
	/// </summary>
	public class OIDHashtable
	{
		private Hashtable hashTable = new Hashtable();
		private static OIDHashtable hashTableInstance;

		private OIDHashtable() 
		{
           		
			string input;
			string[] s; 

			StreamReader sr = new StreamReader(@"..\..\resources\marc.oids.txt");

			while((input = sr.ReadLine())!=null) 
			{
				s = input.Split(new char[]{','}, 2);
				try 
				{
					hashTable.Add(s[0], s[1]);
				} 
				catch {}
			}

		}

		
		public static OIDHashtable ht 
		{
			get 
			{
				if (hashTableInstance == null) 
				{
					hashTableInstance = new OIDHashtable();
				}
				return hashTableInstance;
			}
		}
		
		public string GetDescription(ASN1ObjectIdentifier oid) 
		{
			string retString = (hashTable[oid.ToString()] as string);
			if (retString == null) retString=oid.ToString();

			return retString;
		}
	}



}
