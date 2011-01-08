namespace X509_test
{
	using NUnit.Framework;
	
	using mASN1;
	using mASN1.test.resources;

	using X501;
	using X509;
	using X50x;

	using System;
	using System.IO;

	[TestFixture]
	public class X509_test
	{
		[SetUp] 
		public void SetUp()
		{
			
		}

		public X509_test()
		{
			SetUp(); // Ovo je zbog debuggiranja, kad projekt pretvorim u executable.
		}


		[Test]
		public void Test_SubjectPublicKeyInfo()
		{
            SubjectPublicKeyInfo spki = new SubjectPublicKeyInfo();
			AlgorithmIdentifier ai = new AlgorithmIdentifier();
			ai.Assign(OID.RSA.sha1WithRSAEncryption);

			BERWriter.DumpHEX(ai);


			spki.Assign(ai, new ASN1BitString(new byte[]{0x1, 0x2}, 9));

			BERWriter.DumpHEX(spki);
			

		}
		
		[Test]
		public void Test_Extensions()
		{
			
		}

		[Test]
		public void Test_UniqueIdentifier()
		{
			
		}

		[Test]
		public void Test_Validity()
		{
			
		}

		[Test]
		public void Test_AlgorithmIdentifier()
		{
			
		}

		[Test]
		public void Test_tbsCertificate()
		{
			
		}

		[Test]
		public void Test_Certificate()
		{
			try
			{
				BEREncoding ber = BERReader.CreateBEREncoding("test.cert.der");
				BERWriter.DumpHEX(ber);

				Certificate cert = new Certificate();
				cert.fromBER(ber);

				ResourceLoader rl = new ResourceLoader();

				foreach (string fileName in ResourceLoader.fileNames) 
				{

					Console.WriteLine("\n"+fileName);
					Stream strm = rl.GetStream(fileName);

					ber = BERReader.CreateBEREncoding(strm);
					cert.fromBER(ber);


					X509.Extensions exts = 
						cert.certificate.extensions.underObject
						as X509.Extensions;

					foreach(X509.Extension ext in exts.Elements) 
					{
						//BERWriter.DumpHEX(ext);
						//BERWriter.DumpHEX(ext.etxnId);
						//Console.WriteLine(ext.etxnId.ToString());

						if (ext.critical.val) 
						{
							Console.Write("CRITICAL, ");
						} 
						else 
						{
							Console.Write("NON-CRITICAL, ");
						}
	
						Console.Write(OID.Helper.OIDHelper.ht.GetDescription(ext.etxnId)+",  ");

						
						ASN1OctetString os = ext.extnValue;
						BEREncoding innerBER = new BEREncoding(os.asDER().GetContents());
						Console.Write(ext.innerExtnValue.GetType().ToString()+", ");


						BERWriter.DumpHEX(ext.innerExtnValue);

					}

					BERWriter.DumpHEX(cert);
				}

				Console.WriteLine("Success.");
			}
			catch (UnrecognizedObjectIdentifierException oidException)
			{
				Console.WriteLine(oidException.Message);
			}
		}


		[Test]
		public void Test_SupportedAlgorithms()
		{
			
		}


		[Test]
		public void Test_ExtensionSet()
		{
			
		}

		[Test]
		public void TryCertificate() 
		{
//			///Name name = Samples.name;	
//			
//			BERWriter.DumpHEX(name.asDER());
//
//			tbsCertificate cert = new tbsCertificate();
//			
//			cert.version = new _Version();
//			cert.version.Assign(X509.Version.v1);
//
//			cert.serialNumber =	X509.Version.v0;
//
//			cert.signature = new ContentEncryptionAlgorithmIdentifier();
//			cert.signature.Assign(PKCS7.CMS_AlgorithmIdentifiers.sha_1, null);
//
//			/// issuer:
//			cert.issuer = name;
//
//		
//			/// validity:
//			cert.validity = new Validity();
//					
//			Time t1 = new Time();
//			t1.Assign(Samples.utcTime);
//			
//			cert.validity.Assign(t1, t1);
//
//
//			/// subject:
//			cert.subject = name; ///takodjer.
//
//			/// subjectPublicKeyInfo:
//			cert.subjectPublicKeyInfo = Samples.subjectPKInfo;
//				
//			cert.Assign(
//				cert.version,
//				cert.serialNumber,
//				cert.signature,
//				cert.issuer,
//				cert.validity,
//				cert.subject,
//				cert.subjectPublicKeyInfo
//				);
//
//
//			BERWriter.DumpHEX(cert.asDER());
//
//			Certificate certifikat = new Certificate();
//			certifikat.Assign(cert, cert.signature, new ASN1BitString(new byte[]{0x9, 0x9, 0x9, 0x9}, 25));
//
//			BERWriter.DumpHEX(certifikat.asDER());
//			BERWriter.DumpDERToFile(certifikat, "test.cert.der");



		}

	}
}