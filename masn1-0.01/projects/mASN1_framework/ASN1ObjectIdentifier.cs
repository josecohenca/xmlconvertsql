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
	using System.Collections;


	/// <summary>
	/// ASN1ObjectIdentifier is a class
	/// representing ASN1's
	/// OBJECT IDENTIFIER type.
	/// </summary>
	public class ASN1ObjectIdentifier : ASN1Object 
	{
		public int[] Values;


		public readonly static ASN1Type asn1type = new ASN1ObjectIdentifier_type();

		public void Init() 
		{
			this.Tag = (int) ASN1TagNumber.ObjectIdentifierTag;
		}

		public ASN1ObjectIdentifier() 
		{
			this.Init();
		}

		public ASN1ObjectIdentifier(params int[] values) 
		{
			if (values.Length!=0) 
			{
				this.Assign(values);
			}
		}

		public void Assign(params int[] values)
		{
			this.Init();
			ASN1ObjectIdentifier.CheckValues(values);
			this.Values = values;
		}

		public ASN1ObjectIdentifier(BEREncoding ulazBER) 
		{
			this.Init();
			this.fromBER(ulazBER);
		}

		/// <summary>
		/// Constructor for creating extensions of existing OIDs.
		/// </summary>
		/// <param name="oid">base OID</param>
		/// <param name="values">added ints</param>
		public ASN1ObjectIdentifier(ASN1ObjectIdentifier oid, params int[] values) 
		{
			this.Init();
			this.Values = new int[values.Length+oid.Values.Length];
			oid.Values.CopyTo(this.Values, 0);
			values.CopyTo(this.Values, oid.Values.Length);
		}

		/// <summary>
		/// Check values for conformity with OID specification.
		/// </summary>
		private static void CheckValues(int[] values) 
		{
			int len = values.Length;
			if (len < 2) 
				throw new System.ArgumentException("X.208 mandates at least 2 values");

			if ( (values[0]<0) || (values[0]>2) )
				throw new System.ArgumentException("First value must be 0, 1 or 2");

			if ( (values[1]<0) || (values[1]>39) )
				throw new System.ArgumentException("Second value must be within 0..39");
		}

		
		public static byte[] ToBase128(int v) 
		{
			if (v<128) 
			{
				return new byte[] {(byte)v};
			}
			else 
			{
				int i, tmp = v;
				byte[] tmpbytearray = new byte[5];

				for (i=0; i<5; i++) 
				{
					tmpbytearray[i]=(byte)(0x80 | (tmp & 0x7f));
					tmp >>= 7;
				}

				i=4;

				//strip leading zeros.
				while(tmpbytearray[i]==0x80) i--;
                
				byte[] ret = new byte[i+1];


				// return other bytes in reverse order, in
				// order to have MSB first.
				int j=0;
				while (i>=0) 
				{
					
					ret[i]=tmpbytearray[j];
					j++;					
					i--;
				}
				ret[ret.Length-1] &= 0x7f;

				return ret;
			}
		}
		
		
		public override BEREncoding asDER() 
		{
			int len = this.Values.Length;

			ArrayList list = new ArrayList();
			
			list.Add( Convert.ToByte( 40*Values[0] + Values[1] ) );
			for ( int i=2; i < len; i++ ) 
			{
				list.AddRange(ToBase128(this.Values[i]));
			}
			
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
			 * TREBA NAPRAVITI konstruktor za primitivni BER koji prima
			 * ArrayList.
			 * 
			 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			Byte[] tmpContents = new Byte[list.Count];
			(list.ToArray()).CopyTo(tmpContents, 0);
			list.Clear();
			/* --- */
	
			return new BERPrimitiveEncoding(
				this.Tag,
				tmpContents 			);
		}

	

		public override void fromBER(BEREncoding ulazBER) 
		{
			

			ulazBER.Decode();
			byte[] tmparray = ulazBER.GetContents();
			ArrayList list = new ArrayList();
			
			/// procitaj Contents i prebaci u values...
			/// 

			list.Add(tmparray[0] / 40);
			list.Add(tmparray[0] % 40);

			/// Dekodiraj onaj nespretni zapis u bazi 128 
			
			int i=1, loop = tmparray.Length;
			
			int val, 
				tmpval, 
				sizea=2; /// To su ona dva obavezna, vec ranije obradjena

			while (i < loop)
			{
			
				val = 0;
				
				while (tmparray[i] > 127)
				{
					tmpval = (int) (tmparray[i] & 0x7f);
					
					val <<= 7;
					val |= tmpval;
					
					i++;
				};

				val <<= 7;
				val |= tmparray[i];				
				
				list.Add(val);
				sizea++;
				i++;
			}

			this.Values = new int[sizea];
			list.CopyTo(0, this.Values, 0, sizea);
		}
		
		
		public override string ToString() 
		{
			//string ret_string="OID:  ";
			string ret_string="";

			

			foreach (int i in this.Values) 
			{
				ret_string += i.ToString()+ "."; 
			}
			
			ret_string=ret_string.Remove(ret_string.Length-1, 1);

			return ret_string;
			
		}

		/// <summary>
		/// Two OID objects are considered equal if their Value fields have all values
		/// equal.
		/// </summary>
		/// <param name="obj">object compared to</param>
		/// <returns>bool</returns>
		public override bool Equals(object obj) 
		{
			if (obj == null) return false;

			if (obj is ASN1ObjectIdentifier) 
			{
				int [] tmpvalues = (obj as ASN1ObjectIdentifier).Values;

				bool tmpbool = true;
				if (tmpvalues.Length != this.Values.Length) 
				{
					return false;
				} 
				else 
				{
					for(int i = 0; i < tmpvalues.Length; i++) 
					{
						tmpbool = (tmpbool) && (this.Values[i] == tmpvalues[i]);
					}
				}
				return tmpbool;
			} 

			return false;
		}


		public static bool operator==(ASN1ObjectIdentifier ob1, ASN1ObjectIdentifier ob2) 
		{
			///
			/// ovo nisam najsretnije rijesio. prije je bilo problema kad su obje vrijednosti
			/// bile 'null', sve se zavrtilo i opa - StackOverflowException...
			if (System.Object.Equals(ob1,null)) return (System.Object.Equals(ob2, null));
            
			return ob1.Equals(ob2);
		}

		public static bool operator!=(ASN1ObjectIdentifier ob1, ASN1ObjectIdentifier ob2) 
		{
			return !(ob1==ob2);
		}

		
		/// <summary>
		/// Equals trazi override, pa eto.
		/// </summary>
		/// <returns></returns>
		public override int GetHashCode()
		{
			int tmp = base.GetHashCode();
			foreach (int val in this.Values) 
			{
				/* ajde sfiksaj ovo...  */
				tmp ^= (val ^ 0x4293a712);
			}
			return tmp;
		}

	}


	/// <summary>
	/// A class representing ASN.1-OIDs type.
	/// </summary>
	public class ASN1ObjectIdentifier_type : ASN1Type 
	{

		public ASN1ObjectIdentifier_type() 
		{
			this.Tag = (int) ASN1TagNumber.ObjectIdentifierTag;
		}

		public override ASN1Object CreateInstance(BEREncoding ulazBER)
		{
			return new ASN1ObjectIdentifier(ulazBER);
		}

		public override ASN1Object Instance()
		{
			return new ASN1ObjectIdentifier();
		}


	}
}