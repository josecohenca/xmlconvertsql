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


namespace mASN1_tests
{
	using mASN1;
	/// <summary>
	/// Summary description for mASN1_samples.
	/// </summary>


	/// <summary>
	/// A class containing static instances of following types:
	///   - ASN1ObjectIdentifier (OBJECT IDENTIFIER)
	///   - test_SEQUENCE        (SEQUENCE)
	///   - test_SEQUENCE_OF
	///   - ASN1UTCTime          (UTCTIME)
	/// </summary>
	public class SimpleSamples 
	{
		/// <summary>
		/// 29/01/2004, 19:07, CET
		/// </summary>
		public static ASN1UTCTime utcTime;
	

		public static ASN1BitString bitString = 
			new ASN1BitString(new byte[]{0xff, 0xf1, 0xf3, 0xf4, 0xf5}, 37);

		public static ASN1ObjectIdentifier funnyOID = new ASN1ObjectIdentifier(1, 2, 3, 4, 5, 6, 7);




		static SimpleSamples() 
		{
			/// utcTime
			utcTime = new ASN1UTCTime();
			utcTime.Assign("0401291907+0100"); /// 29/01/2004, 19:07, CET
			utcTime.DecodeString();

			
			

		}
	}
}
