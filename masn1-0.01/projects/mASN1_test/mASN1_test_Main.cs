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

namespace mASN1_tests {

	using System;


	public class DebugRunner
	{
		/// <summary>
		/// The main entry point for the application.
		//		/// </summary>
		[STAThread]
		public static void Main()
		{
			BasicTests basic = new BasicTests();
			basic.Test_DER_HighTagNumberForm();

			ExperimentalTests runner = new ExperimentalTests();

//			runner.Test_stuffForVisitor();
//
			runner.Test_MemoryStream();

			runner.Test_DEREncoder_w_expansion();

			runner.Test_DERStaticFunctions();

			runner.Test_SimpleDER();

			runner.Test_DERDecoder();
 

//			BasicTests runner = new BasicTests();
//
//			runner.Test_SEQUENCE_OF();
//
//			IntermediateTests med_runner = new IntermediateTests();
//			//
//			//			med_runner.Test_INFORMATION_OBJECT_SET();
//			med_runner.Test_SEQUENCE_w_TAGGING();
//			med_runner.Test_SEQUENCE_w_OPTIONAL();
//
//
//			AdvancedTests adv_runner = new AdvancedTests();
//
//
//			adv_runner.Test_PARAMETERIZED_DEEP();
//
//			X501_test.X501_test x501test = new X501_test.X501_test();
//			x501test.Test_Name();
//			x501test.Test_AttributeTypeAndDistinguishedValue();
//
//			X509_test.X509_test x509test = new X509_test.X509_test(); 
//			x509test.Test_SubjectPublicKeyInfo();
//			x509test.Test_Certificate();
		}
	}
}
