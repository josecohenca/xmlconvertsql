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

namespace mASN1_tests.SampleClasses 
{

	/*
	 * Sample classes for ASN1SequenceOf (SEQUENCE OF) type.
	 */

	using mASN1;

	public class test_SEQUENCE_w_tagging:ASN1Sequence 
	{
		public i0_ASN1Boolean bl1;
		public i1_ASN1Boolean bl2;

		public override void RegisterTypes()
		{
			this.ComponentTypes = new ASN1ComponentTypeInfo[2];
			RegType(new i0_ASN1Boolean_type());
			RegType(new i1_ASN1Boolean_type());
		}

		public override void RegisterComponents()
		{
			this.bl1 = Components[0] as i0_ASN1Boolean;
			this.bl2 = Components[1] as i1_ASN1Boolean;
		}
	}


	public class i0_ASN1Boolean:ASN1ImplicitTag 
	{
		public i0_ASN1Boolean() 
		{
			Reg(0, new ASN1Boolean_type());
		}
	}
	
	public class i1_ASN1Boolean:ASN1ImplicitTag 
	{
		public i1_ASN1Boolean()
		{
			Reg(1, new ASN1Boolean_type());
		}
	}


	public class i0_ASN1Boolean_type:ASN1ImplicitTag_type
	{
		public i0_ASN1Boolean_type()
		{
			Reg(0, new ASN1Boolean_type());
		}

		public override ASN1Object Instance()
		{
			return new i0_ASN1Boolean();
		}
	}

	public class i1_ASN1Boolean_type:ASN1ImplicitTag_type
	{
		public i1_ASN1Boolean_type()
		{
			Reg(1, new ASN1Boolean_type());
		}

		public override ASN1Object Instance()
		{
			return new i1_ASN1Boolean();
		}

	}

		
	

}