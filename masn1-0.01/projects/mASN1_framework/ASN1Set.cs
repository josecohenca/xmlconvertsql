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
	using System;
	using System.Collections;


	abstract public class ASN1SetOf : ASN1Object
	{
		public int Min_Elements=0, Max_Elements=0;
		public ArrayList Elements; ///!!!!
		/// ovo gore treba promijeniti u sorted list, or something...
		 
		public ASN1Type type;



		public ASN1SetOf() 
		{
			this.Tag = (int) ASN1TagNumber.SetTag;
			this.Elements = new ArrayList();
		}
		
		public ASN1SetOf(int min, int max):this()
		{
            this.Min_Elements = min;
			this.Max_Elements = max;
		}

//		public virtual void Init() 
//		{
////			this.Tag = (int) ASN1TagNumber.SetTag;
////			this.Elements = new ArrayList();
//		}

		public void SetBounds(int min, int max)
		{
			this.Min_Elements = min;
			this.Max_Elements = max;
		}
		
		public virtual bool CheckType(ASN1Object obo) 
		{
			return true; /* OOOPS */
		}

		protected bool CheckSizes() 
		{
		
			return (((Min_Elements==0) && (Max_Elements==0))||
				((Min_Elements <= Elements.Count) && (Elements.Count <= Max_Elements)));
		}

		
		public void  Add(params ASN1Object[] elements) 
		{
			foreach (ASN1Object elem in elements) 
			{
				/// MAX nula => pretpostavljam da 
				/// je velicina skupa neomedjena. Inace provjeri
				/// broj elemenata u skupu.
		
				if ((this.Max_Elements !=0) && (this.Elements.Count == this.Max_Elements)) 
				{
                    throw new InitializationException("SET OF structure is full");
				}
			

				/// provjeri jel dobar tip...
				if (this.CheckType(elem)) 
				{
					// Dodaj u sortiranu listu.
					// 
					// 
					// 
					

					// ovdje moram srediti da ne smiju postojati dva jednaka elementa... 
					// znaci moram napraviti dobar Equals za ASN1Objekt. 
					// 

					foreach(ASN1Object existing_element in this.Elements)
					{
						// ovu jednakost treba razraditi...
						if (existing_element==elem)
						{
							throw new InitializationException("This SET OF structure already contains this element!");
						}
					}

					// dodavanje bi trebalo biti sortirano...
					this.Elements.Add(elem);
				} 
				else 
				{
					throw new InitializationException("You tried to insert wrong type in SET OF structure!");
				}
			}
		}


		/// <summary>
		/// BEREncoding is constructed. Content octets are concatenation
		/// of elements' BER encodings.
		/// </summary>
		/// <returns>BER BEREncoding</returns>
		public override BEREncoding asBER()
		{
			if (!this.CheckSizes()) return null;
			
			ArrayList tmpContents = new ArrayList();

			foreach (ASN1Object obo in this.Elements) 
			{
				tmpContents.AddRange(obo.asBER().GetBERCode());
			}
				
			return new BERConstructedEncoding(this.Tag, tmpContents);
		}


		/// <summary>
		/// BERcode je constructed, uz dodatak da su
		/// DER kodovi elemenata (u contents) sortirani
		/// leksikografski u uzlaznom redoslijedu.
		/// </summary>
		/// <returns>DER BEREncoding</returns>
		public override BEREncoding asDER()
		{
			if (!this.CheckSizes()) return null;

			ArrayList tmpContents = new ArrayList();

			foreach (ASN1Object obo in this.Elements) 
			{
				tmpContents.Add(obo.asBER().GetBERCode());
			}
			
			/// sortiranje
			tmpContents.Sort(new BERComparer());
			
			/// prepisivanje (flattening u ovom slucaju)
			ArrayList sortedBERs = new ArrayList();
			for (int i=0; i<tmpContents.Count; i++) 
			{
				sortedBERs.AddRange(tmpContents[i] as Byte[]);
			}
			
			return new BERConstructedEncoding(this.Tag, sortedBERs);
		}

		public override void fromBER(BEREncoding ulazBER)
		{

			/// OVO TREBA TESTIRATI!!!

			ulazBER.Decode();

			/* FIXME: Dizi exception ako je 'type==null' */

			foreach(BEREncoding ber in ulazBER.Decompose()) 
			{
				this.Add(this.type.CreateInstance(ber));
			}
				
		}

		public override string ToString()
		{
			string retValue="";

			foreach (ASN1Object o in this.Elements) 
			{
				retValue+=o.ToString()+", ";
			}
			
			return retValue;
		}



		
			 


	}


	abstract public class ASN1SetOf_type:ASN1Type 
	{
		public ASN1Type type;

		public ASN1SetOf_type() 
		{
			this.Tag = (int) ASN1TagNumber.SetTag;
		}
	
		//
		// MUST REMOVE
		public void Init() 
		{
			//this.Tag = (int) ASN1TagNumber.SetTag;
		}

		public virtual ASN1SetOf SubCreateInstance(ASN1SetOf setOfObject, BEREncoding ulazBER) 
		{
			/// ovo treba baciti u .fromBER od setOfObject-a !!!!!
			ulazBER.Decode();

			foreach(BEREncoding ber in ulazBER.Decompose()) 
			{
				setOfObject.Add(this.type.CreateInstance(ber));
			}
			
			/// Dolazak ovdje predstavlja gresku!
			return setOfObject;
		}



		/// <summary>
		///  ovo sam zasrao. Ovo mora ici u ASN1SetOf.fromBER()
		/// </summary>
		/// <param name="ulazBER"></param>
		/// <returns></returns>
		public override ASN1Object CreateInstance(BEREncoding ulazBER)
		{
			ASN1SetOf setOfObject = this.Instance() as ASN1SetOf;
			ulazBER.Decode();

			foreach(BEREncoding ber in ulazBER.Decompose()) 
			{
				setOfObject.Add(this.type.CreateInstance(ber));
			}
			
			/// Dolazak ovdje predstavlja gresku!
			return setOfObject;
		}

	}

	
	/// <summary>
	/// So far there hasn't been a need to implement this.
	/// </summary>
	abstract public class ASN1Set : ASN1Object 
	{
	}
}