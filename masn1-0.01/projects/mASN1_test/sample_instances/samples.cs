using X50x;

namespace mASN1_tests 
{

	using System;
	using System.IO;
	
	using mASN1;

	using X501;
	using X509;
	//	using X50x;

	public class Samples 
	{
		/// <summary>
		/// 29/01/2004, 19:07, CET
		/// </summary>
		public static ASN1UTCTime utcTime;
		public static Name        name;
		public static X50x.AlgorithmIdentifier algorithmIdentifier;
		public static SubjectPublicKeyInfo subjectPKInfo;

		public static ASN1BitString bitString = 
			new ASN1BitString(new byte[]{0xff, 0xf1, 0xf3, 0xf4, 0xf5}, 37);

		public static ASN1ObjectIdentifier Adnecto = new ASN1ObjectIdentifier(1, 2, 3, 4, 5, 6, 7);




		static Samples() 
		{
			/// utcTime
			utcTime = new ASN1UTCTime();
			utcTime.Assign("0401291907+0100"); /// 29/01/2004, 19:07, CET
			utcTime.DecodeString();

			
			
			/// Name (X.501)
			name = new Name();

			try 
			{
				using(FileStream fs = new FileStream(@"test.X501.Name.01.ber", FileMode.Open)) 
				{
					BinaryReader br = new BinaryReader(fs);
					byte[] ber_bytes = br.ReadBytes((int)fs.Length);

					BEREncoding ber = new BEREncoding(ber_bytes);
					name.fromBER(ber);

					fs.Close();
				}
			}
			catch (System.Exception e)
			{
				Console.WriteLine(e.ToString());
				throw new Exception("Samples pallette failed at initializing .name!");
			}


			/// SubjectPublicKeyInfo subjectPKInfo
			subjectPKInfo = new SubjectPublicKeyInfo();
			//			ContentEncryptionAlgorithmIdentifier alg = new ContentEncryptionAlgorithmIdentifier();
			//			alg.Assign(PKCS7.CMS_AlgorithmIdentifiers.rsaEncryption, null);
			//			subjectPKInfo.Assign(alg, new ASN1BitString(new byte[]{0x1, 0x1, 0x1}, 19));
			

		}
	}
}