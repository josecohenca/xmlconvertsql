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
//	using System;
//	using System.Collections;


	/// <summary>
	/// mASN1 class with INTEGER functionality. Linking with some Big-Int lib is
	/// in plan. So far these integers are never really used as integers, only
	/// as arrays of bytes (as they appear in BER/DER).
	/// </summary>
	public class ASN1Integer : ASN1Object 
	{
		public static readonly ASN1Type asnType = new ASN1Integer_type();

		public byte[] intOctets;

		public ASN1Integer() 
		{
			Tag = (int) ASN1TagNumber.IntegerTag;
		}

		public ASN1Integer(byte[] ulazOctets):this()
		{
			this.Assign(ulazOctets);
		}

		public override BEREncoding asDER()
		{
			return new BERPrimitiveEncoding(this.Tag, this.intOctets);
		}


		public override void fromBER(BEREncoding ulazBER)
		{
			this.intOctets = ulazBER.GetContents();
		}

		public virtual void Assign(byte[] ulazOctets)
		{
			this.intOctets = ulazOctets;
		}

		public override string ToString()
		{
			string retString="0x";

			foreach (byte b in intOctets) 
			{
				retString+=b.ToString("X");
			}

			return retString;

			/// ovo trba od-delegirati van...
		}


		
	}

	
	public class ASN1Integer_type : ASN1Type 
	{
		public ASN1Integer_type() 
		{
			this.Tag = (int) ASN1TagNumber.IntegerTag;
		} 

		public override ASN1Object Instance()
		{
			return new ASN1Integer();
		}
	}
}
