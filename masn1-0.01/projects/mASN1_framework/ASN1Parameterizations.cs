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

	public interface IParameterizedASN1Type 
	{
		ASN1InformationObjectSet GetIOSet();
		UNIQUE_TypeInfo GetUNIQUE_TypeInfo();
	}


	/// <summary>
	/// Most parameterized types are based on SEQUENCE. This class
	/// derives from ASN1Sequence and serves as base class
	/// for classess that capture parameterized type's functionality.
	/// </summary>
	abstract public class ASN1ParameterizedSequence:ASN1Sequence, IParameterizedASN1Type 
	{
		protected UNIQUE_TypeInfo unique_TypeInfo;

	
		public virtual ASN1InformationObjectSet GetIOSet() 
		{
			return this.unique_TypeInfo.ioset;
		}

		public virtual UNIQUE_TypeInfo GetUNIQUE_TypeInfo() 
		{
			return this.unique_TypeInfo;
		}

		
		public override void Init()
		{
			/// Had to override base.Init(). 
			/// Everything in base.Init() follows
			/// after initialization of parameterIOSet.
		}

		protected virtual void lateInit(ASN1InformationObjectSet ioset)
		{
			this.unique_TypeInfo = new UNIQUE_TypeInfo(ioset);
			
			base.Init();
																/*	
								this.RegisterTypes();
								this.numOfComponents = ComponentTypes.Length;
								this.Components = new ASN1Object[numOfComponents];
																*/
		}

		// FIXME: provjeri jel se ovaj koristi..
		protected virtual void lateInit(UNIQUE_TypeInfo u_ti)
		{
			this.unique_TypeInfo = u_ti;
		}

		protected override void DoComponentAssign(int j, ASN1Object ulaz)
		{
			this.Components[j] = ulaz;
			if (this.ComponentTypes[j] is UNIQUE_TypeInfo)
			{
				this.unique_TypeInfo.uoid = Components[j] as ASN1ObjectIdentifier;
			}
		}

		protected void RegUniqueType()
		{
            RegType(this.unique_TypeInfo);
		}

		protected void RegDependentType() 
		{
			RegType(CTX_DEP_TypeInfo.Make(unique_TypeInfo));
		}

		protected void RegDependentOptionalType() 
		{
			RegType(CTX_DEP_TypeInfo.Make(unique_TypeInfo), OPTIONAL);
		}
		
	}

		
}