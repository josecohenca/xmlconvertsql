// mASN1 - mini ASN.1 framework for .NET 
// Copyright (C) 2004 Marcel Maretic
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Marcel Maretic (marcelix@users.sourceforge.net)
// Davor Runje (eszlikowski@users.sourceforge.net)

namespace mASN1_tests 
{

	using NUnit.Framework;
	using System;
	using mASN1;
	using mASN1_tests.SampleClasses;


	[TestFixture]
	public class IntermediateTests
	{
//		[SetUp] 
//		public void SetUp()
//		{
//		}
//
//		[TearDown] 
//		public void TearDown()
//		{
//		}
		

		[Test]
		public void Test_SEQUENCE_w_OPTIONAL()
		{
            test_SEQUENCE_OPTIONAL t1 = new test_SEQUENCE_OPTIONAL();

			t1.Assign(SimpleSamples.bitString);

			BERWriter.DumpHEX(t1);
			BEREncoding der = t1.asDER();

			test_SEQUENCE_OPTIONAL t2 = new test_SEQUENCE_OPTIONAL();
			t2.fromBER(der);

			BERWriter.DumpHEX(t2);


			test_SEQUENCE_OPTIONAL1 t3 = new test_SEQUENCE_OPTIONAL1();
			t3.Assign(ASN1Boolean.FALSE, ASN1Boolean.TRUE);

			BERWriter.DumpHEX(t3);

			BEREncoding ber = t3.asBER();

			test_SEQUENCE_OPTIONAL1 t4 = new test_SEQUENCE_OPTIONAL1();

			t4.fromBER(ber);

			BERWriter.DumpHEX(t4);

			test_SEQUENCE_OPTIONAL1 t5 = new test_SEQUENCE_OPTIONAL1();
			t5.Assign
				(ASN1Boolean.TRUE, new ASN1OctetString(new byte[]{0x01, 0x01, 0xff}), ASN1Boolean.FALSE);

			BERWriter.DumpHEX(t5);

			test_SEQUENCE_OPTIONAL1 t6 = new test_SEQUENCE_OPTIONAL1();
			t6.fromBER(t5.asBER());

			BERWriter.DumpHEX(t6);
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
			ti.Assign(SimpleSamples.funnyOID, ASN1Null.asn1type);
		   

			TypeIdentifier ti1 = new TypeIdentifier();
			ti1.Assign(SimpleSamples.funnyOID, new ASN1ObjectIdentifier_type());

			
		}

		[Test]
		public void Test_INFORMATION_OBJECT_SET ()
		{
			ASN1InformationObjectSet iob_set1 = new ASN1InformationObjectSet();
				
			TypeIdentifier ti1 = new TypeIdentifier();
			ti1.Assign(SimpleSamples.funnyOID, ASN1Null.asn1type);

			TypeIdentifier ti2 = new TypeIdentifier();
			ti2.Assign(new ASN1ObjectIdentifier(1, 2, 3), new ASN1Boolean_type());

			iob_set1.Add(ti1);
			
			ASN1ObjectIdentifier unique = new ASN1ObjectIdentifier();
			unique.Assign(1, 2, 3, 4);


			Console.WriteLine("To be cont...");
		}

		[Test]
		public void Test_TYPE_IDENTIFIER_SET()
		{
			TypeIdentifierSet typeIdSet = new TypeIdentifierSet();
			typeIdSet.Add(new ASN1ObjectIdentifier(1, 2), ASN1Null.asn1type);
		}


		[Test]
		public void Test_SEQUENCE_w_TAGGING()
		{
			test_SEQUENCE_w_tagging t1 = new test_SEQUENCE_w_tagging();

			i0_ASN1Boolean i0 = new i0_ASN1Boolean();
			i0.Assign(ASN1Boolean.FALSE);

			i1_ASN1Boolean i1 = new i1_ASN1Boolean();
			i1.Assign(ASN1Boolean.TRUE);

			t1.Assign(i0, i1);

			BERWriter.DumpHEX(t1);
			
		}
	}


}