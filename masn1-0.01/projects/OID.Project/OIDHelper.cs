namespace OID.Helper
{
	using System;
	using System.Collections;

	using System.IO;
	
	using mASN1;


	/* SINGLETON pattern */


	/// <summary>
	/// Summary description for OIDDictionary.
	/// </summary>
	public class OIDHelper
	{
		private Hashtable hashTable = new Hashtable();
		private static OIDHelper hashTableInstance;

		private readonly string oidsFile="OID.Project.marc.oids.txt";

		private OIDHelper() 
		{
           		
			string input;
			string[] s; 
			StreamReader sr=null;

			try 
			{
				
				Stream strm = this.GetType().Assembly.GetManifestResourceStream(oidsFile);
				sr = new StreamReader(strm);

				while((input = sr.ReadLine())!=null) 
				{
					s = input.Split(new char[]{','}, 2);
					try 
					{
						hashTable.Add(s[0], s[1]);
					} 
					catch {} //FIXME: Uhvati hashTable specific-exception
				}
			}
			finally 
			{
				if (sr!=null) 
				{
					sr.Close();
				}
			}
		}

		
		public static OIDHelper ht 
		{
			get 
			{
				if (hashTableInstance == null) 
				{
					hashTableInstance = new OIDHelper();
				}
				return hashTableInstance;
			}
		}
		
		public string GetDescription(ASN1ObjectIdentifier oid) 
		{
			string retString = (hashTable[oid.ToString()] as string);
			
			if (retString == null) retString=""; //oid.ToString();

			return retString + " ("+oid.ToString()+")";
		}
	}



}
