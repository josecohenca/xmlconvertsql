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


/* vidi System.TimeSpan, DateNow (formati ispisa itd...)  */

namespace mASN1
{
	using System;


	/// <summary>
	/// 
	/// This class captures common features of UTCTime and GeneralizedTime,
	/// both used in X.509.
	///
	/// </summary>
	abstract public class ASN1Time:ASN1StringBase 
	{
		public TimeInfo timeInfo;

		public ASN1Time() 
		{
			this.timeInfo = new TimeInfo();
		}
	}


	public class TimeInfo 
	{
		public int  year;
		public byte month;
		public byte day;
		public byte hours;
		public byte minutes;
		public byte seconds;
		
		public short zone;
		public byte  zone_minutes;


		public void Assign(int yyyy, byte mo, byte dd, byte hh, byte mi, byte sec, short zz, byte zz_mm) 
		{
			this.year = yyyy;
			this.month = mo;
			this.day = dd;
			this.hours = hh;
			this.minutes = mi;
			this.seconds = sec;
			this.zone = zz;
			this.zone_minutes = zz_mm;
		}
    }

	/// <summary>
	/// Jos nisam. Tu nema nejasnoca.
	/// </summary>
	public class ASN1UTCTime:ASN1Time
	{
		
		
		public ASN1UTCTime() 
		{
			this.Tag = (int)ASN1TagNumber.UTCTimeTag;
		}

		public override void CheckString() 
		{
			/// ovdje - puuuno posla.
		}

				
		/// <summary>
		/// Scans time values from StringValue.
		/// </summary>
		public void DecodeString() 
		{
			String tmp = this.StringValue;

			this.timeInfo.year     = Convert.ToInt32(tmp.Substring(0,2));
			this.timeInfo.month    = Convert.ToByte(tmp.Substring(2,2));
			this.timeInfo.day      = Convert.ToByte(tmp.Substring(4,2));
			this.timeInfo.hours    = Convert.ToByte(tmp.Substring(6,2));
			this.timeInfo.minutes  = Convert.ToByte(tmp.Substring(8,2));

			int i=10;
			char zone     = tmp[i];

			if (Char.IsDigit(zone))  
			{
				this.timeInfo.seconds = Convert.ToByte(tmp.Substring(i,2));
				zone = tmp[i+=2];
			}

			if (zone=='Z') 
			{
				this.timeInfo.zone   = 0;
				this.timeInfo.zone_minutes = 0;

				return;
			}

			//i++;

			this.timeInfo.zone         = Convert.ToInt16(tmp.Substring(i,3));
			this.timeInfo.zone_minutes = Convert.ToByte (tmp.Substring(i+2,2));



		}


		
		
		
		
		/// <summary>
		/// This method is delegated to TimeInfo's Assign(...).
		/// </summary>
		/// <param name="yyyy"></param>
		/// <param name="mo"></param>
		/// <param name="dd"></param>
		/// <param name="hh"></param>
		/// <param name="mi"></param>
		/// <param name="sec"></param>
		/// <param name="zz"></param>
		/// <param name="zz_mm"></param>
		public void Assign(int yyyy, byte mo, byte dd, byte hh, byte mi, byte sec, short zz, byte zz_mm) 
		{
			this.timeInfo.Assign(yyyy, mo, dd, hh, mi, sec, zz, zz_mm);
		}

//		public override string ToString()
//		{
//			return DateTime.Now.ToShortDateString();
//		}

	}

	
	public class ASN1UTCTime_Type:ASN1Type 
	{
		public ASN1UTCTime_Type() 
		{
			this.Tag = (int)ASN1TagNumber.UTCTimeTag;
		}

		public override ASN1Object Instance()
		{
			return new ASN1UTCTime();
		}
	}


	public class ASN1GeneralizedTime:ASN1Time 
	{
		public ASN1GeneralizedTime() 
		{
			this.Tag = (int) ASN1TagNumber.GeneralizedTimeTag;
		}

		public static void ProcessTimeString(String timeString) 
		{
			byte year     = Convert.ToByte(timeString.Substring(0,4));
			byte month    = Convert.ToByte(timeString.Substring(4,2));
			byte day      = Convert.ToByte(timeString.Substring(6,2));
			byte hours    = Convert.ToByte(timeString.Substring(8,2));
			byte minutes  = Convert.ToByte(timeString.Substring(10,2));
			byte seconds;

			byte zone_hours;
			byte zone_minutes;

			//bool greenwich=false;

			int i=12;
			char zone     = timeString[i];

			if (Char.IsDigit(zone))  
			{
				seconds = Convert.ToByte(timeString.Substring(i,2));
				zone = timeString[i+=2];
			}


			if (zone=='Z') 
			{
				zone_hours   = 0;
				zone_minutes = 0;
			}

			i++;

			zone_hours   = Convert.ToByte(timeString.Substring(i,2));
			zone_minutes = Convert.ToByte(timeString.Substring(i+2,2));

		}
	}

	public class ASN1GeneralizedTime_Type:ASN1Type 
	{
		public ASN1GeneralizedTime_Type() 
		{
			this.Tag = (int) ASN1TagNumber.GeneralizedTimeTag;
		}
		
		public override ASN1Object Instance()
		{
			return new ASN1GeneralizedTime();
		}


	}


}