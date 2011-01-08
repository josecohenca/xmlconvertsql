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

	/// <summary>
	/// Abstract class on top of which I build
	/// ASN.1's string types.
	/// </summary>
	abstract public class ASN1StringBase: ASN1Object 
	{
		
		public ASN1StringBase() {}

		public ASN1StringBase(string inString) 
		{
			this.StringValue = inString;
		}
		
		/// <summary>
		/// This member points to a String value of 
		/// corresponding ASN.1's string type.
		/// </summary>
		public String StringValue;

		/// <summary>
		/// This method is used for checking string values. Each string type
		/// has its own set of restriction. Therefore - each should
		/// override this method to meet these restrictions.
		/// </summary>
		public virtual void CheckString() 
		{
			/// baci Exception ako je krivi input...
		}
		
		
		/// <summary>
		/// Read values from BER code. Inherited by 
		/// ASN.1's string types.
		/// </summary>
		/// <param name="ulazBER">BER encoded input</param>
		public override void fromBER(BEREncoding ulazBER) 
		{
			ulazBER.Decode();
			
			foreach (BEREncoding objBER in ulazBER.Decompose() ) 
			{
				Byte[] tmpByteArray = objBER.GetContents();
				int len             = tmpByteArray.Length;

				// --------------------------------------------------
				
				Char[] tmpCharArray = new Char[len];
				for(int i=0; i<len; i++) 
				{
					tmpCharArray[i]=Convert.ToChar(tmpByteArray[i]);     
				}														 
 
				this.StringValue += new String(tmpCharArray);
				
				// --------------------------------------------------
			}

			this.CheckString();
		}

		
		
		/// <summary>
		/// DER encoding of ASN.1-string types is primitive.
		/// </summary>
		/// <returns>DER encoding as BEREncoding object</returns>
		public override BEREncoding asDER() 
		{
			Byte[] tmpByteArray;
			int i,
				len = this.StringValue.Length;

			tmpByteArray = new Byte[len];

			for(i=0; i<len; i++) 
			{
				tmpByteArray[i]=Convert.ToByte(this.StringValue[i]);
			}

			return new BERPrimitiveEncoding(
				this.Tag ,
				len,
				tmpByteArray  );
		}

		/// <summary>
		/// For ASN.1's string types, ToString() simply
		/// returns ASN.1-type's StringValue.
		/// </summary>
		/// <returns>Type's string Value</returns>
		public override string ToString()   
		{
			return this.StringValue;
		}

		public virtual void Assign(String val) 
		{
			this.StringValue = val;
			this.CheckString();
		}

	}

	
	
	
	/// <summary>
	/// ASN.1's PRINTABLE STRING. 
	/// PrintableString has certain character limits:
	///     A..Z, a..z, 0..9,
	///     (space) ' ( ) + , - . / : = ?
	/// </summary>
	public class ASN1PrintableString: ASN1StringBase 
	{
		
		/// <summary>
		/// Parameterless constructor, sets up ASN.1-type's
		/// tag (as integer). Implicitly invoked by other 
		/// constructors.
		/// </summary>
		public ASN1PrintableString() 
		{
			this.Tag = (int) ASN1TagNumber.PrintableStringTag;
		}

		
		/// <summary>
		/// Class constructor for initializing ASN.1's PrintableString
		/// type from decoded input String. 
		/// </summary>
		/// <param name="ulazniString">String input</param>
		public ASN1PrintableString ( String ulazniString ):this() 
		{
			this.StringValue = ulazniString;
		}
		
		
		/// <summary>
		/// Constructor, initializes from BER code. Intended
		/// for decoding BER to corresponding ASN.1-type.
		/// </summary>
		/// <param name="ulazBER">BER encoded input</param>
		public ASN1PrintableString (BEREncoding ulazBER) 
		{
			this.fromBER(ulazBER);
		}

	
		/// <summary>
		/// Overriden CheckString() method. Enforces
		/// restrictions imposed on ASN.1's PrintableString
		/// type. Throws exception if appropriate.
		/// </summary>
		public override void CheckString()
		{
			int i;
			Char ch;
	
			for(i=0; i < this.StringValue.Length; i++) 
			{
				ch = this.StringValue[i];
				if (    (('A' <= ch) &&  (ch <= 'Z'))                  // slova 
					||  (('a' <= ch) &&  (ch <= 'z'))                  // ......
					||  (('\''<= ch) &&  (ch <= ':') && (ch != '*'))   // znamenke i svi osim 'space'
					||  (ch == ' ')                                    // 'space' 
					||  (ch == '?')
					||  (ch == '=')
					)
				{
					/// ne radi nista, ulaz je OK.
				} 
				else 
				{
					/// treba baciti bolji
					/// Exception.
					/// 
					Console.WriteLine(ch);
					throw new Exception("Krivi ulaz!");
				}
			}
		}

	}


	public class ASN1PrintableString_type:ASN1Type 
	{
		public ASN1PrintableString_type() 
		{
			this.Tag = (int) ASN1TagNumber.PrintableStringTag;
		}

		public override ASN1Object Instance()
		{
			return new ASN1PrintableString();
		}

	}

	/// <summary>
	/// ASN.1's T61 String. CheckString() is redundant, as
	/// this type accepts all 256 possible values of
	/// 8-bit character in its StringValue member.
	/// </summary>
	public class ASN1T61String: ASN1StringBase 
	{
		/// <summary>
		/// Parameterless constructor, sets up ASN.1-type's
		/// tag (as integer). Implicitly invoked by other 
		/// constructors.
		/// </summary>
		public ASN1T61String() 
		{
			Tag = (int) ASN1TagNumber.T61StringTag;
		}

		
		/// <summary>
		/// Class constructor - intent is to initialize a class 
		/// representing ASN.1's T61String type from (decoded) input string. 
		/// </summary>
		/// <param name="ulazniString">(decoded) string input</param>
		public ASN1T61String(String ulazniString): this() 
		{
			this.StringValue = ulazniString;
		}

		
		/// <summary>
		/// Class constructor - intent is to initialize a class
		/// representing ASN.1's T61String type from (encoded) 
		/// BER code.
		/// </summary>
		/// <param name="ulazniBER">(encoded) BER code</param>
		public ASN1T61String(BEREncoding ulazniBER):this() 
		{
			this.fromBER(ulazniBER);
		}

	}


	public class ASN1T61String_type: ASN1Type 
	{
		public ASN1T61String_type() 
		{
			this.Tag = (int) ASN1TagNumber.T61StringTag;
		}

		public override ASN1Object Instance()
		{
			return new ASN1T61String();
		}

	}


	/// <summary>
	/// Nedovrseno.
	/// </summary>
	public class ASN1IA5String: ASN1StringBase 
	{
		/// <summary>
		/// Parameterless constructor, sets up ASN.1-type's
		/// tag (as integer). Implicitly invoked by other 
		/// constructors.
		/// </summary>
		public ASN1IA5String() 
		{
			this.Tag = (int) ASN1TagNumber.IA5StringTag;
		}

		public ASN1IA5String(String ulazniString):   this() 
		{
			this.StringValue = ulazniString;
			this.CheckString();
		}

		public ASN1IA5String(BEREncoding ulazniBER): this() 
		{
			this.fromBER(ulazniBER);
		}

		/// <summary>
		/// This method validates string input. An exception is
		/// thrown incase type's requirements are not met (both for
		/// encoding/decoding)
		/// </summary>
		public override void CheckString()
		{
			/// zasad ovako:
			base.CheckString ();
		}


	}


	public class ASN1IA5String_type: ASN1Type 
	{
		public ASN1IA5String_type() 
		{
			this.Tag = (int) ASN1TagNumber.IA5StringTag;
		}

		public override ASN1Object Instance()
		{
			return new ASN1IA5String();
		}

	}

}
