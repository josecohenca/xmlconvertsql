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

namespace mASN1_tests.SampleClasses {

	using mASN1;

	public class test_SEQUENCE : ASN1Sequence
	{
		/*  
		 * MySeq ::= SEQUENCE 
		 *   {
		 *      bitString     BIT STRING,
		 *      octetString OCTET STRING
		 *   }
		 */ 

		// komponente 
		public ASN1BitString      bitString;
		public ASN1OctetString  octetString;

		

		public override void RegisterTypes() 
		{
			this.ComponentTypes = new ASN1ComponentTypeInfo[]
			{
				ASN1ComponentTypeInfo.Make(new ASN1BitString_type()),
				ASN1ComponentTypeInfo.Make(new ASN1OctetString_type())
			};
		}

		public override void RegisterComponents() 
		{
			this.bitString   = NextComponent() as ASN1BitString;
			this.octetString = NextComponent() as ASN1OctetString;
		}

	}

	
	
	
	
	public class test_SEQUENCE_type : ASN1Sequence_type 
	{
		public override ASN1Object Instance()
		{
			return new test_SEQUENCE();
		}
	}





	public class test_SEQUENCE_OPTIONAL : ASN1Sequence 
	{
		/* 
		 *  MySeqOptional ::= SEQUENCE {
		 *		bitString    BIT STRING,
		 *      octetString  OCTET STRING OPTIONAL
		 *		}
		 */

		public ASN1BitString bitString;
		public ASN1OctetString octetString;

//		public test_SEQUENCE_OPTIONAL() 
//		{
//			this.numOfComponents = 2;
//			this.Init();
//		}
		
		
		/// <summary>
		/// ovo treba prebaciti u Type dio.... ne u Object
		/// </summary>
		public override void RegisterTypes()
		{
			this.ComponentTypes = new ASN1ComponentTypeInfo[2];

			RegType(new ASN1BitString_type());
			RegType(new ASN1OctetString_type(), OPTIONAL);
		}


	
		public override void RegisterComponents()
		{
			this.bitString    = this.Components[0] as   ASN1BitString; 
			this.octetString  = this.Components[1] as ASN1OctetString;
		}
	}

	public class test_SEQUENCE_OPTIONAL_type:ASN1Sequence_type
	{
		public override ASN1Object Instance()
		{
			return new test_SEQUENCE_OPTIONAL();
		}

	}



	public class test_SEQUENCE_OPTIONAL1: ASN1Sequence
	{
		public ASN1Boolean bool1;
		public ASN1OctetString octetString;
		public ASN1Boolean bool2;

		public override void RegisterTypes()
		{
			this.ComponentTypes = new ASN1ComponentTypeInfo[3];  /// mogo bi se i ovog rijesiti.. :-)

			RegType(new ASN1Boolean_type());
			RegType(new ASN1OctetString_type(), OPTIONAL);
			RegType(new ASN1Boolean_type());
		

		}

		public override void RegisterComponents()
		{
			
		}


	}
 

}