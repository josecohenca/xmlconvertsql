namespace mASN1_tests 
{

	using NUnit.Framework;
	using System;
	using System.IO;

	using mASN1;
	using mASN1_tests.SampleClasses;

	//!!! EXPERIMENTAL

	using mASN1.framework.experimental;


	[TestFixture]
	public class ExperimentalTests
	{
		[Test]
		public void Test_TAG_model() 
		{
			e0_BOOLEAN e = new e0_BOOLEAN();
			Console.WriteLine(e.TAG);
			Console.WriteLine("   Multi: {0}",    e.TAG.IsMultiTag());
			Console.WriteLine("Implicit: {0}",    e.TAG.IsImplicit());
		}

		[Test]
		public void Test_DERStaticFunctions() 
		{
			byte[] test1 = new byte[]{0x1, 0x2, 0x3, 0x4};
			byte[] test2 = new byte[]{0x1, 0x82, 0x0f, 0x0f, 0x1};
			byte[] test3 = new byte[]{0x7f, 0x83, 0x01, 0x2, 0x5, 0x5};
			byte[] test4 = new byte[]{0x7f, 0x83, 0x81, 0x2, 0x82, 0x1, 0x2, 0x7 /* ... */};

			

			Console.WriteLine("Testing DER static functions");
			Console.WriteLine("FindStartOfValueOctets:");
			int s = DERMethods.FindStartOfValueOctets(0, test1);
			Console.WriteLine(s);
			Assert.IsTrue(s==2);

			s = DERMethods.FindStartOfValueOctets(0, test2);
			Console.WriteLine(s);
			Assert.IsTrue(s==4);

			s = DERMethods.FindStartOfValueOctets(0, test3);
			Console.WriteLine(s);
			Assert.IsTrue(s==4);

			s = DERMethods.FindStartOfValueOctets(0, test4);
			Console.WriteLine(s);
			Assert.IsTrue(s==7);

			Console.WriteLine("ScanDERLength:");

			s = DERMethods.ScanBERLength(0, test1);
			Assert.IsTrue(s==4);
			DERPrettyPrinter.Print(test1);
			Console.WriteLine(s); //4
			

			s = DERMethods.ScanBERLength(0, test2);
			Assert.IsTrue(s==3859);
			Console.WriteLine(s); //3859

			s = DERMethods.ScanBERLength(0, test3);
			Assert.IsTrue(s==6);
			Console.WriteLine(s); //6

			s = DERMethods.ScanBERLength(0, test4);
			Assert.IsTrue(s==265);
			Console.WriteLine(s); //265

		
		}

		
		
		[Test]
		public void Test_SimpleDER() 
		{
			byte[] test1 = new byte[]{0x1, 0x2, 0x3, 0x4};
			byte[] test2 = new byte[]{0x1, 0x82, 0x0f, 0x0f, 0x1};
			byte[] test3 = new byte[]{0x7f, 0x83, 0x01, 0x2, 0x5, 0x5};
			byte[] test4 = new byte[]{0x7f, 0x83, 0x81, 0x2, 0x82, 0x1, 0x2, 0x7 /* ... */};
			
			SimpleDER der = new SimpleDER(test1);

			der = new SimpleDER(test2);
			der = new SimpleDER(test3);
			der = new SimpleDER(test4);



		}
		
		[Test]
		public void Test_DEREncoder()
		{
			test1Sequence t1 = new test1Sequence();	
			BOOLEAN bl1 = new BOOLEAN();
			bl1.boolValue = true;

			NULL nl1 = new NULL();

			t1.Components[0] = bl1;
			t1.Components[1] = nl1;

			DEREncoder encoder = new DEREncoder();
			encoder.Visit(t1);

			encoder.ShowResult();

		}

		[Test]
		public void Test_DERDecoder() 
		{
			SimpleDER der = new SimpleDER(new byte[]{0x01, 0x01, 0xff});
			DERPrettyPrinter.Print(der.GetAllOctets());

			BOOLEAN_type bool_t = new BOOLEAN_type();

			DERDecoder decoder = new DERDecoder();
			decoder.Init(der);
			decoder.Visit(bool_t);

			BOOLEAN decodedObject = decoder.ShowResult() as BOOLEAN;
			Console.WriteLine(decodedObject.boolValue.ToString());
		}

		[Test]
		public void Test_DEREncoder_w_expansion() 
		{
			e0_BOOLEAN e = new e0_BOOLEAN();
			e.boolValue = true;

			DEREncoder encoder = new DEREncoder();
			encoder.Encode(e);
			TagValueNode n = encoder.ShowResult();

			System.IO.MemoryStream memoryStream = new System.IO.MemoryStream();

			DERWriter derWriter = new DERWriter(memoryStream);
			derWriter.Write(n);

			byte[] derValue = memoryStream.ToArray();
			DERPrettyPrinter.Print(derValue);
			derWriter.Close();
		}

	
		[Test]
		public void Test_MemoryStream() 
		{
			Console.WriteLine("Testing MemoryStream");

			MemoryStream memoryStream = new MemoryStream();
			BinaryWriter binaryWriter = new BinaryWriter(memoryStream);

			byte[] byteArray = new byte[]{0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9};

			for(int i=0; i<2500001; i++) 
			{
				if (i%50000==0) {
					if (i%250000==0) 
					{
						Console.Write("|");
					}
					else 
					{
						Console.Write(".");
					}
					Console.Out.Flush();
				}
				binaryWriter.Write(byteArray);
			}
			Console.WriteLine();
			

			byte[] newByteArray = memoryStream.ToArray();
			DERPrettyPrinter.Print(newByteArray);

		

			binaryWriter.Close();
		}
	}
}
			