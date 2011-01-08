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
	using mASN1;

	public class test_TAG_EXPLICIT:ASN1ExplicitTag 
	{
		public byte[] BitString;
		public test_TAG_EXPLICIT() 
		{
			Reg(0, ASN1BitString.asnType);
		}

		public override void RegisterComponents()
		{
			this.BitString = (this.underObject as ASN1BitString).bitString;
		}

	}

	public class test_TAG_EXPLICIT_Type:ASN1ExplicitTag_type 
	{
		public override ASN1Object Instance()
		{
			return new test_TAG_EXPLICIT();
		}
	}

}