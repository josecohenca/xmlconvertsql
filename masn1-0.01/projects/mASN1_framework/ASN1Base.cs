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
	using mASN1.framework.experimental;


	//using System;
	using System.Collections;

	#region Osnovne konstante, enum-ovi
	
	/// <summary>
	/// Vodeca dva bita svakog tag-a koji odredjuju
	/// klasu ASN1 tipa.
	/// </summary>




	
	
	/// <summary>
	/// A few bit-constants, important in BER
	/// (they identify different flavors of BER encoding).
	/// </summary>
	public enum BitConstant 
	{
		ConstructedEncoding = 0x20,
		IndefiniteLength    = 0x80,
		LongForm            = 0x1f
	}

	
	
	/// <summary>
	/// Intent is to expand ASN1Object's .asBER() method... when
	/// passed this enum ASN1Object should encode itself 
	/// apropriately.
	/// </summary>
	public enum BER
	{
		CONSTRUCTED,
		CONSTRUCTED_INDEFINITE
	}
	
	
	#endregion


	#region Basic interface of ASN.1 classes

	/// <summary>
	/// Ovo je osnovni interface koji se ocekuje 
	/// od ASN1 klasa.
	/// </summary>
	public interface IBERCodec 
	{
		BEREncoding asBER();
		BEREncoding asDER();

		void fromBER(BEREncoding inputBER);
	}

	/// <summary>
	/// Planned.
	/// </summary>
	public interface IPERCodec
	{
		PEREncoding asPER();
		void fromPER(PEREncoding inputPER);
	}



	#endregion


	#region Definicija "ASN1Object" klase
	/// <summary>
	/// Ovo je osnovna apstraktna klasa iz koje se
	/// izvode sve ostale ASN1 klase.
	/// </summary>
	abstract public class ASN1Object: IBERCodec //, IPERCodec
	{
		/*
		 *  .asDER() se mora implementirati, .asBER() ne mora. Ako se nista ne dira, virtualna
		 *  .asBER() poziva .asDER().
		 * 
		 */

		public int Tag;

		public ASN1TAG TAG; //experimental

		public abstract BEREncoding asDER();

		public  virtual BEREncoding asBER() 
		{
			return this.asDER();
		}
		
		
		public ASN1Object() 
		{
			/// parameterless constructor. 
			/// there is no need for it.
			/// compiler generates it anyway.
		}

				
		public ASN1Object(BEREncoding ulazBER):this()
		{
			this.fromBER(ulazBER);
		}

		
		public virtual void fromBER(BEREncoding ulazBER) 
		{
			ulazBER.Decode();
		}



		
		/*
		 *   07.10.
		 *   MORAM implementirati specijalni .Equals() za ASN1 objekte.
		 *    - najprije provjera Tag-a, zatim provjera membera...
		 * 
		 */

		public override bool Equals(object obj)
		{
			return base.Equals(obj);
		}

		public override int GetHashCode()
		{
			return (base.GetHashCode() ^ this.Tag); /* moram vezati za instancu */

		}

		public string ToBERString()
		{
			return this.asBER().ToString();
		}

		public string ToDERString()
		{
			return this.asDER().ToString();
		}


		
		
	}

	#endregion

	
	#region Definicija "ASN1Type" klase

	
	abstract public class ASN1Type 
	{
		public int Tag;
		public ArrayList Tags;
		
		protected bool SkipMatch=false;




		/* -------------------------------------------------------- */

		/// <summary>
		/// <para>
		/// Factory method - creates ASN1Object instances from their BER
		/// encoding. 
		/// </para>
		/// <para>
		/// Most types don't override this method.
		/// </para>
		/// </summary>
		/// <param name="ulazBER">BER code</param>
		/// <returns>Instance of ASN1Object derived class.</returns>
		public virtual ASN1Object CreateInstance(BEREncoding ulazBER) 
		{
			ASN1Object asn1Object = this.Instance();
			asn1Object.fromBER(ulazBER);
			return asn1Object;
		}

		
		/// <summary>
		/// This is a factory method of class XXX_type for class XXX. 
		/// </summary>
		/// <returns>An instance of ASN1Object derived class</returns>
		public abstract ASN1Object Instance();


		/// <summary>
		/// MUST CHANGE, MOVE OR WHATEVER....
		///    usage - u ASN1Sequence..
		///    
		///    <para>Returns true if ASN1Object ob and ASN1Type tip match.</para>
		/// </summary>
		/// <param name="ob"></param>
		/// <param name="tip"></param>
		/// <returns></returns>
		public static bool MatchType(ASN1Object ob, ASN1Type tip) 
		{
			if ((tip == null)||(ob==null)) 
			{
				/// Ovo je zbog SET OF.
				/// Nadam se da necu platiti penale za ovo. :-(
				return true;
			}
			if (tip.SkipMatch) 
			{
				return true;
			}
			else if (tip.Tags != null) 
			{
				return (tip.Tags.Contains(ob.Tag));
			}
			else return (ob.Tag == tip.Tag);
		}


		/// <summary>
		/// OVO ZELIM BACITI VAN!!! U DRUGU KLASU!!
		/// 
		/// koriste samo Choice i Sequence... to se mora moci lako srediti.
		/// </summary>
		/// <param name="tag"></param>
		/// <param name="tip"></param>
		/// <returns></returns>
		public static bool MatchTag (int tag, ASN1Type tip) 
		{
			if (tip.SkipMatch) 
			{
				return true;
			}
			else if (tip.Tags != null) 
			{
				return (tip.Tags.Contains(tag));
			}
			else return (tag == tip.Tag);
		}
	
	
	
	} 

	

	#endregion

	
	

	
	



	

	
	
}