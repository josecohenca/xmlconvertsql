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
	#region mASN1 Exceptions

	
	public abstract class mASN1Exception: System.ApplicationException
	{
		public mASN1Exception(string mesg):base(mesg) {}
	}

	
	
	public class InitializationException:mASN1Exception 
	{
		public InitializationException(string mesg):base(mesg) {}	
	}

	
	public class UnrecognizedObjectIdentifierException:InitializationException
	{
		public UnrecognizedObjectIdentifierException(string mesg):base(mesg) {}
	}

	public class SEQUENCEComponentMismatchException:InitializationException 
	{
		public SEQUENCEComponentMismatchException(string mesg):base(mesg) {}
	}
	#endregion
}