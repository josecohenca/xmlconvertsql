namespace X501_test
{
	using System;
	
	using NUnit.Framework;
	using mASN1;
	using X501;

	using mASN1_tests;

	using OID;

	using oid = mASN1.ASN1ObjectIdentifier;

	[TestFixture]
	public class X501_test
	{
		[SetUp] 
		public void SetUp() 
		{
			/// bezvezni ATRIBUTI
			Sets.SupportedAttributes.Add(new oid(2, 3, 4), new ASN1Boolean_type());
			Sets.SupportedAttributes.Add(new oid(2, 3, 5), new ASN1OctetString_type());

			Sets.SupportedAttributes.Add(RSA.rsasecurity,  new ASN1Boolean_type());

		}

		public X501_test()
		{
			this.SetUp();
		}

		
		[Test]
		public void Test_Attribute()
		{
			
			
		}
		
		[Test]
		public void Test_ValuesWithContext()
		{
			ValuesWithContext v1;

			UNIQUE_TypeInfo uti = new UNIQUE_TypeInfo(Sets.SupportedAttributes);
			uti.uoid = new oid(2, 3, 4);

			///* dakle sad kad iniciram ValuesWithContext sa ovim UNIQUE objektom
			///  podtip mora biti Boolean
			
			v1 = new ValuesWithContext(uti);

			v1.Add(ASN1Boolean.FALSE);

			BERWriter.DumpHEX(v1);


			/* ------------------- */
			/* ... ovdje imam ValuesWithContext instancu... koja nije parametrizirana, a dependent je type..
			 */

			System.Console.WriteLine("\nDifferent kind of test");

			ValuesWithContext v2 = new ValuesWithContext();
			v2.Add(ASN1Boolean.FALSE);

			BERWriter.DumpHEX(v2);

			BEREncoding ber = v2.asDER();

			ValuesWithContext v3 = new ValuesWithContext();
			v3.SetType(new ASN1Any_type(uti));

			v3.fromBER(ber);

			BERWriter.DumpHEX(v3);
			
		}

	

		[Test] 
		public void Test_AttributeTypeAndDistinguishedValue()
		{
			System.Console.WriteLine("ATADV test:");

			/// pocnimo sa AttributeTypeAndDistinguishedValue
			AttributeTypeAndDistinguishedValue atdv = new AttributeTypeAndDistinguishedValue();

			/// uzet cu objectClass
			///      .type je id_at_objectClass
			///      .value je tipa OBJECT IDENTIFIER
			atdv.type   = new oid(2,3,4);
			atdv.value  = ASN1Boolean.TRUE;

			atdv.primaryDistinguished = ASN1Boolean.TRUE;
			atdv.valuesWithContext = null;


			///atdv.valuesWithContext = new ValuesWithContext();

			

			atdv.Assign(atdv.type, atdv.value, atdv.primaryDistinguished);

			BERWriter.DumpHEX(atdv);

			BEREncoding ber = atdv.asDER();

			AttributeTypeAndDistinguishedValue atdv1 = new AttributeTypeAndDistinguishedValue();
			atdv1.fromBER(ber);

			BERWriter.DumpHEX(atdv1);
			
		}

		[Test]
		public void Test_RelativeDistinguishedName ()
		{
			
		}

		[Test]
		public void Test_RDNSequence ()
		{
			
		}

		[Test]
		public void Test_Name ()
		{

			BEREncoding ber = BERReader.CreateBEREncoding("test.X501.Name.01.ber");
			BERWriter.DumpHEX(ber);
			Console.WriteLine(ber);

			Name name = new Name();
			name.fromBER(ber);



			


			
		}

	}
}