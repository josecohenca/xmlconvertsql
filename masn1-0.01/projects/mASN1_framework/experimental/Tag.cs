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

	/// <summary>
	/// Sadrzi tagove osnovnih tipova.
	/// </summary>
	public enum ASN1TagNumber
	{
		
		BooleanTag          = 1,
		IntegerTag          = 2,
		BitStringTag        = 3,
		OctetStringTag      = 4,
		NullTag             = 5,
		ObjectIdentifierTag = 6,
		RealTag             = 9,
		UTF8StringTag       = 12,
		RelativeOID         = 13,
		PrintableStringTag  = 19,
		T61StringTag        = 20,
		IA5StringTag        = 23, 
		SequenceTag         = 16, 
		SetTag              = 17,
		UTCTimeTag          = 23,
		GeneralizedTimeTag  = 24,
		GeneralizedStringTag= 27,
		UniversalStringTag  = 28,
		BMPStringTag        = 30
	}

	public enum EncodingClass:byte 
	{
		Universal        = 0x00,
		Application      = 0x40,
		ContextSpecific  = 0x80,
		Private          = 0xc0,
	}

	public enum XXPLICITY 
	{
		EXPLICIT,
		IMPLICIT
	}
}

