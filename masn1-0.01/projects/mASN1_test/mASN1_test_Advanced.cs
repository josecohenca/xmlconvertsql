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
	public class AdvancedTests
	{

		public void Test_PARAMETERIZED ()
		{
			try
			{
				Console.WriteLine("This test uses new _TypeInfo hierarchy for parameterization..");
				Console.WriteLine("Test is based on 'AlgorithmIdentifier' class.");


				AlgorithmIdentifier aid = new AlgorithmIdentifier();
				aid.Assign(MyIOSet.aux_oid1, ASN1Boolean.FALSE);

				BERWriter.DumpHEX(aid);

				
				BEREncoding der = aid.asDER();
				AlgorithmIdentifier aid_from_ber = new AlgorithmIdentifier();
				aid_from_ber.fromBER(der);

				BERWriter.DumpHEX(aid_from_ber);

			}
			catch(mASN1Exception e)
			{
				Console.WriteLine("Shit - exception happened"+e.Message);
				Assert.Fail();
			}


			try
			{
				Console.WriteLine("This test is using new _TypeInfo hierarchy for parameterization..");


				AlgorithmIdentifier aid = new AlgorithmIdentifier();
				aid.Assign(MyIOSet.aux_oid2, new ASN1Null());

				BERWriter.DumpHEX(aid);

				
				BEREncoding der = aid.asDER();
				
				AlgorithmIdentifier aid_from_ber = new AlgorithmIdentifier();
				aid_from_ber.fromBER(der);

				BERWriter.DumpHEX(aid_from_ber);

			}
			catch(Exception e)
			{
				Console.WriteLine("Shit - exception happened" + e.Message);
				Assert.Fail();
			}

		
		}
		
		[Ignore("ignored test")]
		[Test]
		public void Test_PARAMETERIZED_OPEN ()
		{
			/* For example:
			 *   AlgorithmIdentifier could be the base for
			 *   EncryptionAlgorithmIdentifier, or KeyEncryptionAlgorithmIdentifier. The
			 *  difference would be that the latter ones would have different parameterization
			 *  sets, namely EncryptionAlgorithms, or KeyEncryptionAlgorithms...
			 */

			Console.WriteLine("I actually need to figure what I was thinking about here...");

			/*  Mozda "SIGNED" iz X.509? */
		}


		[Test]
		public void Test_PARAMETERIZED_DEEP()
		{
			Console.WriteLine("DEEP parametrizacija: ovaj je mrcina");
			Console.WriteLine("... prvo bez OPTIONAL komponente...");
		
			ATADV t1 = new ATADV();
			t1.Assign(MyIOSet.aux_oid2, new ASN1Null());

			UNIQUE_TypeInfo uti = t1.GetUNIQUE_TypeInfo();
			
			BERWriter.DumpHEX(t1);
			BEREncoding ber = t1.asDER();

			ATADV t2 = new ATADV();
			t2.fromBER(ber);

			BERWriter.DumpHEX(t2);

			Console.WriteLine("... a sad moram smisliti ValuesWithContext");


			ValuesWithContext vwc = new ValuesWithContext(uti);
			vwc.Add(new ASN1Null());



			BERWriter.DumpHEX(vwc);
			ber = vwc.asBER();

			ValuesWithContext_type vwc_t = new ValuesWithContext_type(uti);
			ValuesWithContext vwc1 = vwc_t.CreateInstance(ber) as ValuesWithContext;

			BERWriter.DumpHEX(vwc1);

			Console.WriteLine("A sad ide ono pravo... DEEP tip...");

			ATADV t3 = new ATADV();
			t3.Assign(MyIOSet.aux_oid2, new ASN1Null(), vwc);
			
			BERWriter.DumpHEX(t3);
			

			ATADV t4 = new ATADV();
			ber = t3.asDER();
			t4.fromBER(ber);

			BERWriter.DumpHEX(t4);
		}

		[Test]
		public void Test_PHANTOM_ANY()
		{
		}

		[Test]
		public void Test_NICE_ANY()
		{
		}
	}
}