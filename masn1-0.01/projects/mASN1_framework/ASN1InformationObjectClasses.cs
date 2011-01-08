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

//	using System;
	using System.Collections;

	public class InformationObjectClass 
	{
		public ASN1Object Unique;
	}

	public class ASN1InformationObjectSet 
	{
		/// <summary>
		/// Elements is an ArrayList. Elements contains 
		/// ASN.1's Information-Objects.
		/// </summary>
		public ArrayList Elements;

		
		public ASN1InformationObjectSet() 
		{
			this.Init();
		}

		public void Init() 
		{
			this.Elements = new ArrayList();
		}
		
		public void Add(InformationObject iob) 
		{
			this.Elements.Add(iob);
		}

		
//		public InformationObject GetInformationObjectFromUID(UniqueObject uid) 
//		{
//			foreach (InformationObject iob in this.Elements) 
//			{
//				/* As soon as InformationObject is found - return */
//                if (uid.id == iob.uniqueId) return iob;
//			}
//
//			/* none found */
//			return null;
//		}

		public InformationObject GetInformationObjectFromUID(ASN1ObjectIdentifier uoid)
		{
			foreach (InformationObject iob in this.Elements)
			{
				if((uoid as ASN1ObjectIdentifier)==iob.uniqueId)
				{
					return iob;
				}
			}

			return null;
		}


		
		public InformationObject GetInformationObject(ASN1ObjectIdentifier oid) 
		{
			foreach (InformationObject iob in Elements) 
			{
				if (iob.uniqueId==oid) return iob;
			}

			throw 
				new mASN1.UnrecognizedObjectIdentifierException("unrecognized OID:"+oid+"(not in IOSet)");
		}

		
	}

	
	
	/// <summary>
	/// This class represents InformationObjects set consisting of
	/// TYPE-IDENTIFIERS. This is the most common IOSet.
	/// </summary>
	public class TypeIdentifierSet:ASN1InformationObjectSet
	{
		public void Add(ASN1ObjectIdentifier oid, ASN1Type type)
		{
			TypeIdentifier ti = new TypeIdentifier(oid, type);
			this.Add(ti);
		}

		public void Remove(ASN1ObjectIdentifier oid)
		{
			
		}
	}

	/// <summary>
	/// ASN.1:
	/// Every information object must have a unique component. Unique component
	/// is a value component (here I assume it is an instance of ObjectIdentifier).
	/// Other components are type components.
	/// </summary>
	abstract public class InformationObject 
	{
		public ASN1ObjectIdentifier uniqueId;
	}




	/// <summary>
	/// <para>
	/// This is a very common ASN.1 construct - TYPE IDENTIFIER used for situations
	/// where component's type is context-dependent. For example, in a SEQUENCE type
	/// a _type of second member may be dependant on the value of first component which 
	/// should be some OBJECT IDENTIFIER. A list of possible pairs (OIDs and _types)
	/// is actualy a list of TypeIdentifiers.
	/// </para>
	/// 
	/// <para>
	/// TYPE IDENTIFIER is the only Information Object used in PKCS#7 and X.509, X.501
	/// standards.</para>
	/// </summary>
	public class TypeIdentifier:InformationObject
	{
		public ASN1ObjectIdentifier id;
		public ASN1Type type;

		public TypeIdentifier() 
		{
		}

		public TypeIdentifier(ASN1ObjectIdentifier _id, ASN1Type _type) 
		{
			this.Assign(_id, _type);
		}

		/// <summary>
		/// Koji je UNIQUE?
		/// </summary>
		public void Assign(ASN1ObjectIdentifier _id, ASN1Type _type) 
		{
			this.id   = _id;
			this.type = _type;
			this.uniqueId = this.id;
		}

		public static ASN1Type Project_type(InformationObject type_id) 
		{
			try 
			{
				return (type_id as TypeIdentifier).type;
			}
			catch
			{
				// AU. OVO MORAM SREDITI!!!
				return null;
			}
		}
	}
}