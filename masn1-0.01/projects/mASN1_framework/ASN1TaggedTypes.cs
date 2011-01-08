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


	

	abstract public class ASN1Tag: ASN1Object 
	{
		public ASN1Type type;
		public ASN1Object underObject;
		public EncodingClass class_number = EncodingClass.ContextSpecific;

		protected ASN1Tag() 
		{
			this.Tag = 0; //default
		}

		public virtual void Assign(ASN1Object ulazASN1) 
		{
			this.underObject = ulazASN1;
			
			///This one is usually a no-op, unless overriden.
			this.RegisterComponents();
		}

		public virtual void RegisterComponents() 
		{
			/// if so requiered, or convenient - descendant class
			/// can "reconnect" the members of underlying asn object as its
			/// own by overriding this method.
		}

		public virtual void Reg(int tagNum, ASN1Type belongingType)
		{
			this.Tag = tagNum;
			this.type = belongingType;
		}

	
	}
	
	
	abstract public class ASN1Tag_type:ASN1Type 
	{
		public ASN1Type type;

		public virtual void Reg(int tagNum, ASN1Type underType)
		{
			this.Tag = tagNum;
			this.type = underType;
		}
	}

	abstract public class ASN1ImplicitTag : ASN1Tag 
	{
		private BEREncoding finishEncoding(BEREncoding under_code) 
		{
			Byte[] tmpContents = under_code.GetBERCode();

			return new BERConstructedEncoding(this.Tag, tmpContents, this.class_number);			
		}

		public sealed override BEREncoding asDER()
		{
			/* here I have to fool the underObject with a false tag to 
			 * encode itself properly.
			 * 
			 * When done, the old tag must be put back in place... This assures
			 * that implicit tagging doesn't mess with underlying objects */
			
			int tmpTag = underObject.Tag;
		
			underObject.Tag = this.Tag;
			BEREncoding der = underObject.asDER();
			der.ApplyClassNumber(this.class_number);

			underObject.Tag = tmpTag;

			return der;
		}

		public override void fromBER(BEREncoding ulazBER)
		{
			this.Assign(this.type.CreateInstance(ulazBER));
		}

	}

	
	abstract public class ASN1ImplicitTag_type : ASN1Tag_type 
	{

	}



	
	
	
	/// <summary>
	/// Base class for explicit tagged ASN.1 types (instance class)
	/// </summary>
	abstract public class ASN1ExplicitTag : ASN1Tag 
	{
		public ASN1ExplicitTag() 
		{
			/// skoro da bi mogao staviti 
			/// da defaultni tag bude 0
		}

		public ASN1ExplicitTag(ASN1Type type):this() 
		{
			this.type = type;
		}

		
		/// <summary>
		/// This private method reflects how similar asBER() and asDER() are.
		/// In EXPLICIT tagged types they differ only on sub-objects.
		/// </summary>
		/// <param name="under_code">BER/DER code of the underlying object</param>
		/// <returns>BER code</returns>
		private BEREncoding finishEncoding(BEREncoding under_code) 
		{
			Byte[] tmpContents = under_code.GetBERCode();
			return new BERConstructedEncoding(this.Tag, tmpContents, this.class_number);			
		}

		public sealed override BEREncoding asBER() 
		{
			return this.finishEncoding(this.underObject.asBER());			
		}
		
		public sealed override BEREncoding asDER() 
		{	
			return this.finishEncoding(this.underObject.asDER());
		}


		public sealed override void fromBER(BEREncoding ulazBER)
		{
			BEREncoding under_ber = new BEREncoding(ulazBER.GetContents());
			this.Assign(this.type.CreateInstance(under_ber));
		}

	}



	abstract public class ASN1ExplicitTag_type : ASN1Tag_type 
	{
		/// <summary>
		/// 'type' member could be OBSOLETE for EXPLICIT type, I think... :-)
		/// </summary>
		///public ASN1Type type;
	}

}