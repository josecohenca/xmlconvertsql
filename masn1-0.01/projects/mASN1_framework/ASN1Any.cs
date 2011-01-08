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
	/// UniqueObject is a class that extends ASN1ComponentTypeInfo and that has the functionality of
	/// ASN1ObjectIdentifier_type.
	/// 
	/// It is needed for (final) parameterization in ASN.1 data structures.
	/// </summary>
	
	#region ANY

	/// <summary>
	/// 'Any' reminds of old ASN.1 syntax with 'ANY DEFINED BY',
	///  but actually models parametrized types 
	///  (introduced to ASN.1 in 1997) in a unified way.
	/// </summary>
	public class ASN1Any_type:ASN1DependentType 
	{
		///private ASN1InformationObjectSet parIOSet;
		
	
		/// <summary>
		/// This should suffice for the "column" coordinate.
		/// Since return is non-void - system derives the delegate from
		/// System.Delegate (not System.MulitcastDelegate)
		/// </summary>
	

		public delegate ASN1Type ProjectionDelegate(InformationObject iob);

		public ProjectionDelegate Projection;


	
		public ASN1Any_type() 
		{
			/// I will make this default. In a situation where InformationObject
			/// has "more than two columns" this will have to be overriden..
			this.Projection += new ASN1Any_type.ProjectionDelegate(TypeIdentifier.Project_type);
		}



		public ASN1Any_type(UNIQUE_TypeInfo u_ti):this()
		{
			setUID(u_ti);
		}


		public void setUID(UNIQUE_TypeInfo u_ti)
		{
			this.dependsOnThis = u_ti;
		}


	


		private void CheckUnique()
		{
			if (this.dependsOnThis == null)
			{
				throw new InitializationException
					("This ANY-like instance is not initialized. Missing .dependOnThis");
            }

			if (this.dependsOnThis.uoid == null)
			{
				throw new InitializationException
					("This ANY-like instance is not fully intialized. Unique OID missing");
			}
		}

		private InformationObject GetInformationObject() 
		{
			/// Uffff! Odvratno.
			return this.dependsOnThis.ioset.GetInformationObject(this.dependsOnThis.uoid);
		}

		
		
		public override ASN1Object CreateInstance(BEREncoding ulazBER) 
		{
			/* Vrati, odnosno zovi CreateInstance od odgovarajuceg tipa, koji ce
			 * biti setiran sa parIOSet i Column (at compile-time) i odgovarajuceg unique objekta
			 * (at run-time).  
			 */                                           

			CheckUnique();

			InformationObject iob = GetInformationObject();

			try 
			{
				ASN1Type tip = this.Projection(iob);
				return tip.CreateInstance(ulazBER);
			}
			catch 
			{
				/// Unrecognized/unregistered OBJECT IDENTIFIER Exception
				///   ... could not resolve type ...
				throw new UnrecognizedObjectIdentifierException
					("<unrecognized> see http://www.alvestrand.no/objectid/"
					+ this.dependsOnThis.uoid.ToString()+".html");
			}
		}

		/// <summary>
		/// This is a remnant from ASN1Type. Here, and all "ANY"-derived types,
		/// calls to .Instance must be avoided.
		/// </summary>
		/// <returns>Never returns.</returns>
		public sealed override ASN1Object Instance()
		{
			throw new InitializationException
				("Calls to .Instance method's of ANY-derived types are forbidden.");
		}
	}



	/// <summary>
	/// This is a "gentler" version of ANY. If the InformationObject instance encountered
	/// is not recognised, i.e. OID doesn't belong to this IOSet's know OIDs, it
	/// creates an ASN1Phantom object that will gobble the contents and proceed.
	/// Thus, this class doesn't return null, it ignores unknown BER data and 
	/// proceeds.
	/// </summary>
	public class ASN1NiceAny_type:ASN1Any_type 
	{
		
		public ASN1NiceAny_type(UNIQUE_TypeInfo uti):base(uti) {}

		public override ASN1Object CreateInstance(BEREncoding ulazBER)
		{
			ASN1Object retObject=null;
			try 
			{
				retObject = base.CreateInstance(ulazBER);
			}
			catch (UnrecognizedObjectIdentifierException)
			{

				if (retObject == null) 
				{
					retObject = new ASN1PhantomObject();
					retObject.fromBER(ulazBER);
				}
			}

			return retObject;
		}

	}


	/// <summary>
	/// This ANY class is tailored to fit the most common situation
	/// where context dependent type is parameterized using
	/// InformationObject sets of TYPE-IDENTIFIERS.
	/// </summary>
	public class ASN1TypeIdAny_type:ASN1Any_type
	{
		
	}
	

	/// <summary>
	/// This class serves a very special purpose - to gobble/ignore
	/// unrecognised data. For example, in X.509 it can ignore unrecognised
	/// non-critical X.509 extensions (very common).
	/// </summary>
	public class ASN1PhantomObject:ASN1Object 
	{
		/// <summary>
		/// Remembering member field:-)
		/// </summary>
		private BEREncoding unrecognisedBERCode;
		
		public override BEREncoding asDER()
		{
			/// Ako nije ranije pozvan .fromBER
			/// ovaj se nema sto pozivati.
			/// Vratiti ce null, sto ce rezultirati greskom.
			return unrecognisedBERCode;
		}

		/// <summary>
		/// This method does not decode BER code. It just "remembers" it.
		/// This way unrecognised data doesn't break parsing. If this object
		/// has to be encoded again, it encodes itself as the BER code it
		/// remembered earlier.
		/// </summary>
		/// <param name="ulazBER">BER/DER code</param>
		public override void fromBER(BEREncoding ulazBER)
		{
			/// remember this BER and do nothing.
			this.unrecognisedBERCode = ulazBER;
		}

		public override string ToString()
		{
			return " unrecognised data (phantom) ";
		}


		public readonly static ASN1Type asn1type = new ASN1PhantomObject_type();
	}

	
	
	public class ASN1PhantomObject_type:ASN1Type 
	{
		public ASN1PhantomObject_type() 
		{
			this.SkipMatch = true;
		}

		public override ASN1Object Instance()
		{
			return new ASN1PhantomObject();
		}

	}
	#endregion


	/// <summary>
	/// This is a base type for all *_type classes that are context
	/// dependent (ANY and derived classes).
	/// </summary>
	abstract public class ASN1DependentType:ASN1Type
	{ 

		/// <summary>
		/// This is the TypeInfo this _type depends on.
		/// </summary>
		public UNIQUE_TypeInfo        dependsOnThis;

		
		/// <summary>
		/// Since I wrote non-trivial constructor C# compiler
		/// complained and now I have to write the trivial .ctr too.
		/// </summary>
		public ASN1DependentType()
		{
			this.SkipMatch = true;

			/// Mozda ne bi bilo lose da dize exception (?).
			/// Pravi konstruktor je onaj koji se poziva sa 
			/// UNIQUE_TypeInfo parametrom.
			/// 

			/// Time bi onda bio rijesen i onaj prazni
			/// konstruktor u Any.
			/// 
			
			///throw new InitializationException("bad constructor call.");
		}

		public ASN1DependentType(UNIQUE_TypeInfo uti)
		{
			this.dependsOnThis = uti;
		}



	}

	

}