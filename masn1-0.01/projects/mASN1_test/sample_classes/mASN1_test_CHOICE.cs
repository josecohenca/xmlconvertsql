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

	public class test_CHOICE:ASN1Choice
	{
		public override void RegisterTypes()
		{
			this.Alternatives = new ASN1Type[] 
			{
				new ASN1BitString_type(),
				new ASN1Boolean_type()
			};
		}

	}

	public class test_CHOICE_type:ASN1Choice_type
	{
		public override ASN1Object Instance()
		{
			return new test_CHOICE();
		}

	}
}