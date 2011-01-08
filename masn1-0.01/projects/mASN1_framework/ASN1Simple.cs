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


namespace mASN1 
{
	using System;

	/*
	      Simple classes: NULL, BIT STRING, OCTET STRING
	 */

	/// <summary>
	/// A class representing ASN.1's NULL type.
	/// </summary>
	public class ASN1Null : ASN1Object 
	{
		public static readonly ASN1Type asn1type = new ASN1Null_type();

		public ASN1Null() 
		{
			this.Tag = (int) ASN1TagNumber.NullTag;
		}
		
		/// <summary>
		/// Method returns ASN.1's NULL encoded as DER/BER.
		/// </summary>
		/// <returns>BER encoded null.</returns>
		public override BEREncoding asDER() 
		{
			return new BERPrimitiveEncoding(this.Tag, 0, new Byte[]{});
		}
	}

	public class ASN1Null_type: ASN1Type 
	{
		internal ASN1Null_type() 
		{
			this.Tag = (int) ASN1TagNumber.NullTag;
		}

		public override ASN1Object CreateInstance(BEREncoding ulazBER) 
		{
			/// A vjerojatno bi trebalo provjeriti da li ovaj 
			/// BER odgovara NULL kodu.
			return Instance();
		}

		public override ASN1Object Instance()
		{
			return new ASN1Null();
		}

	}


	/// <summary>
	/// ASN.1's BOOLEAN type.
	/// </summary>
	public class ASN1Boolean:ASN1Object 
	{

		public bool val;

		
		public readonly static ASN1Type asn1type = new ASN1Boolean_type();
		
		public ASN1Boolean() 
		{
			this.Tag = (int) ASN1TagNumber.BooleanTag;
		}

		public ASN1Boolean(bool b):this() 
		{
			this.Assign(b);
		}

		public void Assign(bool b)
		{
			this.val = b;
		}

		public override BEREncoding asDER()
		{
			byte boolcode = 0x00;
			if (this.val) boolcode = 0xff;
			
			return new BERPrimitiveEncoding(Tag, new byte[1]{boolcode});
		}

		
		public override void fromBER(BEREncoding ulazBER)
		{
			ulazBER.Decode();

			this.val = (ulazBER.GetContents()[0] != 0x00);
		}

		public readonly static ASN1Boolean TRUE  = new ASN1Boolean(true);
		public readonly static ASN1Boolean FALSE = new ASN1Boolean(false);

	}

	public class ASN1Boolean_type:ASN1Type 
	{
		//public int tag = (int) ASN1TagNumber.BooleanTag;

		public ASN1Boolean_type() 
		{
			this.Tag = (int) ASN1TagNumber.BooleanTag;
		}

		public override ASN1Object CreateInstance(BEREncoding ulazBER)
		{
			ASN1Boolean tmp = this.Instance() as ASN1Boolean;
			tmp.fromBER(ulazBER);
            return tmp;
		}

		public override ASN1Object Instance()
		{
			return new ASN1Boolean();
		}


	}






	/// <summary>
	/// Class representing 
	/// ASN.1's BIT STRING type.
	/// </summary>
	public class ASN1BitString : ASN1Object
	{   
		#region Data Members

		public Byte[] bitString;
		public int NumOfBits;

		#endregion
		
		#region Constructors
		public ASN1BitString() 
		{
			this.Tag = (int) ASN1TagNumber.BitStringTag;
		}

	
		/// <summary>
		/// Class constructor, used for encoding ASN.1's Bit String
		/// type to BER. Initializes ASN1BitString class with
		/// decoded parts of a BitString packed in a byte array.
		/// </summary>
		/// <param name="ulaz">byte array input</param>
		/// <param name="n">integer denoting the number of bits used (MSF)</param>
		public ASN1BitString(Byte[] ulaz, int n)  :this() 
		{ 
			bitString = ulaz;
			NumOfBits = n;
		}


		/// <summary>
		/// Class constructor, initialized with BER encoded input.
		/// Used for decoding BER objects to ASN.1's BitString type
		/// class.
		/// </summary>
		/// <param name="ulazBER">BER encoded input</param>
		public ASN1BitString(BEREncoding ulazBER) :this() 
		{
			this.fromBER(ulazBER);
		}
	
		
		#endregion
		
		#region Interface requiremenents
		
		public override BEREncoding asDER() 
		{
//			int len;
			Byte bitsToMultipleOfEight;
			Byte[] contOct;


			bitsToMultipleOfEight = (Byte)((8-this.NumOfBits % 8) % 8);

//			len  = this.NumOfBits / 8;
//			if (bitsToMultipleOfEight > 0) len++;
			

//			if (len != BitString.Length ) 
//			{
//				throw new IndexOutOfRangeException("Badly formated input!");
//			}
//			len += ((this.NumOfBits % 8 ==0)?0:1) ;
		
		
		
			contOct = new Byte[bitString.Length+1];
			contOct.SetValue(bitsToMultipleOfEight, 0);
			bitString.CopyTo(contOct, 1);

			if(bitsToMultipleOfEight > 0) 
				/// maskiranje bitova za DER (suvisni bitovi
				/// moraju biti nula.
			{
				int pos = contOct.Length - 1;
				Byte mask = (Byte)(0xff >> bitsToMultipleOfEight);

				contOct.SetValue(   (Byte)( mask & contOct[pos]) , pos);
			}

			return new BERPrimitiveEncoding(
				this.Tag, 
				bitString.Length+1, 
				contOct   );
		}

	
		public override void fromBER(BEREncoding ulazBER) 
		{
			Byte[] tmp;
			int i;
		
			base.fromBER (ulazBER);

			tmp = ulazBER.GetContents();

			this.NumOfBits = (tmp.Length-1)*8 - tmp[0];
			this.bitString = new Byte[tmp.Length - 1];
			for (i=0;i<this.bitString.Length; i++) 
			{
				this.bitString[i]=tmp[i+1];
			}
        
		
		}

		#endregion

		public static readonly ASN1Type asnType = new ASN1BitString_type();

		public override string ToString()
		{
			string retString="";

			foreach (byte b in bitString) 
			{
				if (b<16) retString+="0";
				retString+=b.ToString("x")+" ";
			}

			return retString;
		}

	}



	public class ASN1BitString_type: ASN1Type 
	{
		
		public ASN1BitString_type() 
		{
			this.Tag = (int) ASN1TagNumber.BitStringTag;
		}

		public override ASN1Object CreateInstance(BEREncoding ulazBER)
		{
			return new ASN1BitString(ulazBER);
		}

		public override ASN1Object Instance() 
		{
			return new ASN1BitString();
		}
	}
	
	
	/// <summary>
	/// A class representing ASN.1's OCTET STRING 
	/// type instances.
	/// </summary>
	public class ASN1OctetString: ASN1Object 
	{
		
		/// <summary>
		/// Data Member, as Byte[].. OCTET STRING's value is stored 
		/// in OctetString data member.
		/// </summary>
 		public Byte[] OctetString;


		public ASN1OctetString() 
		{
			Tag = (int) ASN1TagNumber.OctetStringTag;
		}

		/// <summary>
		/// Class constructor, used for encoding. Initialized 
		/// with (decoded) OctetString as a Byte[].
		/// </summary>
		/// <param name="ulazOctetString">input OctetString as Byte[]</param>
		public ASN1OctetString(Byte[] ulazOctetString):this()
		{
			this.Init(ulazOctetString);
		}
	
		public void Init(Byte[] ulazOctetString) 
		{
			OctetString = ulazOctetString;
		}
		
		/// <summary>
		/// Class constructor, used for decoding to ASN.1's type class.
		/// Initializes with (encoded) BER code.
		/// </summary>
		/// <param name="ulazBER">input BER code (as BEREncoding)</param>
		public ASN1OctetString(BEREncoding ulazBER):this()
		{
			this.fromBER(ulazBER);
		}


		/// <summary>
		/// Overriden asDER() method, specific for OctetString type.
		/// Returns primitive BER code.
		/// </summary>
		/// <returns>Primitive BER code (as BEREncoding)</returns>
		public override BEREncoding asDER() 
		{
			return new BERPrimitiveEncoding(
				this.Tag,
				this.OctetString.Length,
				OctetString );
		}

		
		/// <summary>
		/// Constructor used for initializing ASN.1's type class with
		/// BER code normally calls this method.
		/// This method explicitly initializes OctetString's data 
		/// member from BER code.
		/// </summary>
		/// <param name="ulazBER">input BER code</param>
		public override void fromBER(BEREncoding ulazBER)
		{
			/// invoke ulazBER's Decode() method.
			//base.fromBER (ulazBER);

			this.OctetString =  ulazBER.GetContents();
		}

	}



	/// <summary>
	/// A class representing ASN.1's OCTET STRING type.
	/// </summary>
	public class ASN1OctetString_type: ASN1Type
	{
		public ASN1OctetString_type() 
		{
			Tag = (int) ASN1TagNumber.OctetStringTag;
		}
		
		public override ASN1Object Instance()
		{
			return new ASN1OctetString();
		}


	}

}
