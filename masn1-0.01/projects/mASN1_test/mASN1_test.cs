namespace mASN1_tests {
	using System.Diagnostics;
	using NUnit.Framework;
	using System;
	using mASN1;
	using mASN1_tests.SampleClasses;
 
	 


	[TestFixture] 
	public class BasicTests 
	{
		[SetUp] 
		public void SetUp()
		{
		}

		[TearDown] 
		public void TearDown()
		{
		}

  
//		[Ignore("ignored test")]
//		[Test] public void TestMe()
//		{
//			Assert.IsFalse(false);
//		}

		[Test] 
		public void TestTrace()
		{
			// Trace/Debug messages won't be output when running all tests in assembly.
			Trace.WriteLine("Trace...");
			Trace.Assert(true ,  "True", "- more detail");
			Trace.WriteLine("Hello World!", "Trace");
		}

		[Test]
		public void Test_SimpleASN1Types()
		{
			//Console.WriteLine(SimpleSamples.funnyOID.ToString());

			Console.WriteLine("ASN1Null test:");


			ASN1Null nl = new ASN1Null();
			BERWriter.DumpHEX(nl);

			BEREncoding ber = nl.asBER();
			BERWriter.DumpHEX(ber);

			ASN1Null_type nl_t = new ASN1Null_type();
			ASN1Null nl1 = nl_t.CreateInstance(ber) as ASN1Null;

			BERWriter.DumpHEX(nl1);


		}

		[Test]
		public void Test_BOOLEAN()
		{
			ASN1Boolean bl = new ASN1Boolean();

			Console.WriteLine("True and false:");
			BERWriter.DumpHEX(ASN1Boolean.TRUE);
			BERWriter.DumpHEX(ASN1Boolean.FALSE);

			Console.WriteLine("--------------------");

			bl.Assign(true);

			ASN1Boolean_type bl_t = new ASN1Boolean_type();
			ASN1Boolean bl2 = bl_t.CreateInstance(bl.asBER()) as ASN1Boolean;

			BERWriter.DumpHEX(bl2);
			
		}

		[Test]
		public void Test_OBJECT_IDENTIFIER()
		{
			ASN1ObjectIdentifier oid = SimpleSamples.funnyOID;

			BERWriter.DumpHEX(oid);
			Console.WriteLine(oid.ToString());

			ASN1ObjectIdentifier oid1 = new ASN1ObjectIdentifier();

			oid1.Assign(1, 2, 3, 4, 5, 6, 7);
			BERWriter.DumpHEX(oid1);
			Console.WriteLine(oid1.ToString());

			Assert.AreEqual(oid, oid1);

			BEREncoding ber = oid.asDER();
			ASN1ObjectIdentifier_type oid_t = new ASN1ObjectIdentifier_type();

			ASN1ObjectIdentifier oid2 = oid_t.CreateInstance(ber) as ASN1ObjectIdentifier;

			BERWriter.DumpHEX(oid2);





			Console.WriteLine("Unfinished business");
		}

		[Test]
		public void Test_SEQUENCE()
		{
			test_SEQUENCE tSEQ = new test_SEQUENCE();
			tSEQ.Assign(
				// first 11 bits of {0x1, 0x2}
				new ASN1BitString(new byte[]{0x1, 0x2}, 11), 
				// byte string  {0x1, 0x2}
				new ASN1OctetString(new byte[2]{0x1, 0x2}));

			Console.WriteLine(tSEQ.asDER().ToString());

			BEREncoding _ber = tSEQ.asBER();
            

			test_SEQUENCE tSEQ1 = new test_SEQUENCE();
			tSEQ1.fromBER(_ber);

//			Console.WriteLine(tSEQ1.asBER().ToString());
			Console.WriteLine(tSEQ1.ToBERString());
		}

		[Test]
		public void Test_SEQUENCE_OF()
		{
			test_SEQUENCE_OF tSEQ_OF = new test_SEQUENCE_OF();

			tSEQ_OF.Add(SimpleSamples.funnyOID);
			tSEQ_OF.Add(SimpleSamples.funnyOID);
			tSEQ_OF.Add(SimpleSamples.funnyOID);
			tSEQ_OF.Add(SimpleSamples.funnyOID);

			BERWriter.DumpHEX(tSEQ_OF);

			test_SEQUENCE_OF t2 = new test_SEQUENCE_OF();

			BEREncoding ber = tSEQ_OF.asBER();

			t2.fromBER(ber);

			BERWriter.DumpHEX(t2);

			Assert.IsTrue(BERComparer.Equals(t2.asDER(), tSEQ_OF.asDER()));


			Console.WriteLine("Unfinished business");
			
		}

		[Test]
		public void Test_SET_OF()
		{
			test_SET_OF tSET_OF = new test_SET_OF();
			tSET_OF.Add(SimpleSamples.funnyOID);
		 


			test_SET_OF_type t_type = new test_SET_OF_type();
			test_SET_OF t1 = t_type.CreateInstance(tSET_OF.asBER()) as test_SET_OF;


			BERWriter.DumpHEX(t1);
			BERWriter.DumpHEX(tSET_OF);
			
			
			
			Console.WriteLine("\nTest invalid element insertion:");
			/// ovo bi trebalo baciti Exception
			try
			{
				tSET_OF.Add(SimpleSamples.funnyOID);
			}
			catch(mASN1Exception e)
			{
				Console.WriteLine(".. was expecting SET struct to complain about already existing element");
				Console.WriteLine("Exception: "+e.Message);
			}
			finally
			{
				Console.WriteLine("This was \"SET OF\" Exception testing");
			}
			

			
			
			Console.WriteLine("\nTest DER-sorting:");
			ASN1ObjectIdentifier oid1=new ASN1ObjectIdentifier(), 
				oid2=new ASN1ObjectIdentifier(), 
				oid3=new ASN1ObjectIdentifier();
			oid1.Assign(1, 2, 0);
			oid2.Assign(1, 2, 1);
			oid3.Assign(1, 2, 2);

			tSET_OF.Add(oid1, oid3, oid2);
			Console.WriteLine(tSET_OF.ToBERString());
			Console.WriteLine(tSET_OF.ToDERString());

			// These shouldn't be equal, because DER is BER-sorted.
			Assert.IsFalse(BERComparer.Equals(tSET_OF.asBER(), tSET_OF.asDER()));






			
		}

	
		[Test]
		public void Test_INTEGER()
		{
			ASN1Integer i1 = new ASN1Integer();

			i1.Assign(new byte[]{0x01, 0x02, 0x03});
			BERWriter.DumpHEX(i1);


			Console.WriteLine("Unfinished business");	
		}

		[Test]
		public void Test_CHOICE()
		{
			test_CHOICE t1 = new test_CHOICE();
			ASN1Boolean bl = new ASN1Boolean(true);

			t1.Assign(bl);
			BERWriter.DumpHEX(t1);
			BERWriter.DumpHEX(bl);

			/* CHOICE is an union type: therefore
			 * DERs of choice and underlying type are
			 * the same.. */
			
			Assert.IsTrue(BERComparer.Equals(t1.asDER(), bl.asDER()));

			BEREncoding ber = bl.asDER();

			test_CHOICE t2 = new test_CHOICE();
			t2.fromBER(ber);

			BERWriter.DumpHEX(t2);
			

			Console.WriteLine("Unfinished business");
		}



		[Test]
		public void Test_TAG_IMPLICIT()
		{
			// ImplicitTagSample ::= [1] IMPLICIT BIT STRING

			test_TAG_IMPLICIT t1 = new test_TAG_IMPLICIT();
			t1.Assign(new ASN1Boolean(true));

			BERWriter.DumpHEX(t1);

			test_TAG_IMPLICIT t2 = new test_TAG_IMPLICIT();
			t2.fromBER(t1.asBER());

			BERWriter.DumpHEX(t2);
            

			

			Console.WriteLine("Unfinished business");
		}



		[Test]
		public void Test_TAG_EXPLICIT()
		{
			// ImplicitTagSample ::= [1] IMPLICIT BIT STRING

			test_TAG_EXPLICIT t1 = new test_TAG_EXPLICIT();
			t1.Assign(new ASN1BitString(new byte[]{0x7f},5));   // first 5 bits of "0x7f" byte

			BERWriter.DumpHEX(t1);

			test_TAG_EXPLICIT t2 = new test_TAG_EXPLICIT();
			t2.fromBER(t1.asBER());

			BERWriter.DumpHEX(t2);

			Assert.IsTrue(BERComparer.Equals(t1.asDER(),t2.asDER()));
			
			Console.WriteLine("Previous 2 DER's should match");

			Console.WriteLine("Unfinished business");
		}


		[Test]
		public void Test_STRING()
		{

			ASN1PrintableString ps = new ASN1PrintableString();
			ps.Assign("Marcelix at users.sourceforge.net");
  
			BERWriter.DumpHEX(ps);
			Console.WriteLine( " >" + ps.ToString());

			mASN1.ASN1T61String ts = new ASN1T61String();
			//ts.Assign("Maretiæ"); ovo ne radi jer treba imati bolju unicode konverziju
			ts.Assign("Mareti$");

			BERWriter.DumpHEX(ts);
			Console.WriteLine( " >" + ps.ToString());

			Console.WriteLine("TBCont...");
		}

	}


	[TestFixture]
	public class IntermediateTests
	{
		[SetUp] 
		public void SetUp()
		{
		}

		[TearDown] 
		public void TearDown()
		{
		}
		

		[Test]
		public void Test_TAG_EXPLICIT()
		{
			// ImplicitTagSample ::= [1] IMPLICIT BIT STRING

			test_TAG_EXPLICIT t1 = new test_TAG_EXPLICIT();
			t1.Assign(new ASN1BitString(new byte[]{0x7f},5));

			BERWriter.DumpHEX(t1);

			test_TAG_EXPLICIT t2 = new test_TAG_EXPLICIT();
			t2.fromBER(t1.asBER());

			BERWriter.DumpHEX(t2);

			Assert.IsTrue(BERComparer.Equals(t1.asDER(),t2.asDER()));

			Console.WriteLine("Unfinished business");
		}


		[Test]
		public void Test_TYPE_IDENTIFIER ()
		{
			Console.WriteLine("Ustvari i nemam neku inspiraciju oko toga sto bi mogao\n"+
				"testirati samo sa TYPE-IDENTIFIER konstruktima\n");


			// TYPE IDENTIFIER sa OBJECT IDENTIFIEROM i NULL memberom...
			TypeIdentifier ti = new TypeIdentifier();
			ti.Assign(SimpleSamples.funnyOID, new ASN1Null_type());
		   

			TypeIdentifier ti1 = new TypeIdentifier(SimpleSamples.funnyOID, new ASN1ObjectIdentifier_type());
			
		}

		[Test]
		public void Test_INFORMATION_OBJECT_SET ()
		{
			ASN1InformationObjectSet iob_set1 = new ASN1InformationObjectSet();
				
			TypeIdentifier ti = new TypeIdentifier();
			ti.Assign(SimpleSamples.funnyOID, new ASN1Null_type());

			iob_set1.Add(ti);
			
			UniqueObject uniqo = new UniqueObject();
			//uniqo.


			
		}
	}


	
	[TestFixture]
	public class AdvancedTests
	{
		
	}
}