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

namespace mASN1.framework.experimental
{
	using System;
	using System.Collections;

	using mASN1;

	/// <summary>
	/// Summary description for Tag.
	/// </summary>
	abstract public class ASN1TAG 
	{
		// Default za tagove je Universal...
		public EncodingClass encodingClass = EncodingClass.Universal;
		internal bool ConstructedBitOn = false;

		abstract public bool IsAutomatic();

		public virtual bool IsSimple() 
		{
			return false;
		}

					
		public virtual bool IsMultiTag() 
		{
			return false;
		}

		abstract public SimpleASN1TAG ExtractSimpleOuterTag();

		
		/// <summary>
		///  ????
		/// </summary>
		/// <param name="inputTag"></param>
		/// <returns></returns>
		public bool Match(int inputTag) 
		{
			return false;
		}

		public virtual bool IsImplicit() 
		{
			return false;
		}

		public virtual bool IsExplicit() 
		{
			return false;
		}
	}

	
	
	/// <summary>
	/// <para>
	/// This would be a very simple Tag - unmodified standard tag. Instances of this class
	/// would be tags for ASN.1's INTEGER, BOOLEAN etc.. Vidi TagTable.
	/// </para>
	/// 
	/// <para>
	/// SimpleASN1TAG instances could and probably should be static.
	/// </para>
	/// </summary>
	public class SimpleASN1TAG:ASN1TAG 
	{
		private int tagNumber;
		public  int tagValue 
		{
			get 
			{
				return tagNumber;
			}
		}

		
		#region Konstruktori

		public SimpleASN1TAG(int num) 	
		{
			tagNumber = num;
		}

		public SimpleASN1TAG(ASN1TagNumber sTag):this((int)sTag){}

		public SimpleASN1TAG(int num, EncodingClass encClass):this(num)
		{
			encodingClass = encClass;
		}

		#endregion konstruktori

		public override bool IsAutomatic()
		{
			return false;
		}

		public override bool IsMultiTag()
		{
			return false;
		}

		public override bool IsSimple() 
		{
			return true;
		}

		public override SimpleASN1TAG ExtractSimpleOuterTag()
		{
			return this;
		}



	}



	public class StructuredSimpleASN1TAG:SimpleASN1TAG
	{
		private void SetConstructedBit() 
		{
			this.ConstructedBitOn = true;
		}

		public StructuredSimpleASN1TAG(int num):base(num) 
		{
			SetConstructedBit();
		}

		public StructuredSimpleASN1TAG(ASN1TagNumber sTag):base(sTag) 
		{
			SetConstructedBit();
		}

		public StructuredSimpleASN1TAG(int num, EncodingClass encClass):base(num, encClass) 
		{
			SetConstructedBit();
		}

	}	

	
	/// <summary>
	/// This would represent more complex tags that arise from explicit or implicit tagging, 
	/// along with context changing (UNIVERSAL, APPLICATION, etc.)
	/// </summary>
	abstract public class TaggedTAG:ASN1TAG 
	{

		
		

		public ASN1TAG underTag;
		protected int t;

		protected TaggedTAG() 
		{
			encodingClass = EncodingClass.ContextSpecific;
		}

		public TaggedTAG(int tagNumber, ASN1TAG inputUnderTag):this() 
		{
			t = tagNumber;
			underTag = inputUnderTag;
		}
		
		public override bool IsAutomatic() 
		{
			return false;
		}

		public override bool IsMultiTag() 
		{
			return false;
		}

		public override SimpleASN1TAG ExtractSimpleOuterTag() 
		{
			SimpleASN1TAG returnTag;

			//TODO:
			returnTag = new SimpleASN1TAG(t, this.encodingClass); //+ ContextDependent

			return returnTag;
		}


	}

		
	public class EXPLICITTag:TaggedTAG 
	{
		public EXPLICITTag(int tagNumber, ASN1TAG inputUnderTag):base(tagNumber, inputUnderTag){}

		public override bool IsExplicit()
		{
			return true;
		}
	}

	
	public class IMPLICITTag:TaggedTAG 
	{
		public IMPLICITTag(int tagNumber, ASN1TAG inputUnderTag):base(tagNumber, inputUnderTag){}

		public override bool IsImplicit()
		{
			return true;
		}
	}


	/// <summary>
	/// This would represent a collection of tags in CHOICE structure.
	/// </summary>
	public class MultiASN1TAG:ASN1TAG 
	{
		private ASN1TAG[] possibleTags;

		public override bool IsAutomatic() 
		{
			return false;
		}

		public override bool IsMultiTag() 
		{
			return true;
		}


		public MultiASN1TAG(params ASN1TAG[] tagz) 
		{
			possibleTags = tagz;
		}

		
		public ASN1TAG[] GetAllTags() 
		{
			return possibleTags;
		}

		public override SimpleASN1TAG ExtractSimpleOuterTag()
		{
			throw new NotImplementedException("Ne to zvati...!!");
		}

	
	}


}
