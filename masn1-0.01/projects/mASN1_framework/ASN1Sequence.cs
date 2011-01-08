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

	/*
	 *   Svaki SEQUENCE ASN1Object mora napraviti:
	 *      0. Naslijediti ASN1SeqProbe klasu
	 *      1. Inicijalizirati numOfComponents
	 *      2. Implementirati konstruktor bez parametara 
	 *         koji poziva this.Init() virtualnu metodu.
	 *      3. Implementirati RegisterTypes(), 
	 *                        RegisterComponents() 
	 *         metode.
	 */

	/// <summary>
	/// Abstract prototype-class for ASN.1's SEQUENCE 
	/// type (tuples).
	/// </summary>
	abstract public class ASN1Sequence: ASN1Object
	{
		/*------  BITNE KOMPONENTE I METODE  ------*/
		protected int numOfComponents;

		public ASN1Object[]     Components;
		public ASN1ComponentTypeInfo[]   ComponentTypes;

		protected static readonly bool OPTIONAL = true;

		private int typeCounter = 0;
		private int componentCounter = 0;

		
		/// <summary>
		/// <para>
		/// This method serves as a method where
		/// component types are declared (as in 
		/// variable declaration).
		/// </para>
		/// 
		/// <para>
		/// Descending (concrete) SEQUENCE class must 
		/// specify types of its components, and specify
		/// whether some are optional or not. 
		/// </para>
		/// 
		/// <para>
		/// Also, if a component has a DEFAULT value, it can
		/// be specified here... 
		/// (see examples in test project)
		/// </para>
		/// </summary>
		abstract public void RegisterTypes();

		protected ASN1Object NextComponent()
		{
			return Components[componentCounter++];
		}

		#region RegType macro-methods

//		protected void OPT() 
//		{
//			this.ComponentTypes[typeCounter].isOptional = true;
//		}

		protected void RegType(ASN1Type tip)
		{
			this.ComponentTypes[typeCounter] = ASN1ComponentTypeInfo.Make(tip);
			typeCounter++;
		}

		protected void RegType(ASN1Type tip, bool optionality)
		{
			this.ComponentTypes[typeCounter] = ASN1ComponentTypeInfo.Make(tip, optionality);
			typeCounter++;
		}

		protected void RegType(ASN1Type tip, ASN1Object defaultValue)
		{
			this.ComponentTypes[typeCounter] = ASN1ComponentTypeInfo.Make(tip, defaultValue);
			typeCounter++;
		}

		protected void RegType(ASN1ComponentTypeInfo typeInfo)
		{
			this.ComponentTypes[typeCounter] = typeInfo;
			typeCounter++;
		}

		protected void RegType(ASN1ComponentTypeInfo typeInfo, bool optionality)
		{
			this.ComponentTypes[typeCounter] = typeInfo;
			this.ComponentTypes[typeCounter].isOptional = optionality;
			typeCounter++;
		}
		#endregion

		/// <summary>
		/// This method does not care about OPTIONAL-ity or
		/// DEFAULTs of components. It merely corresponds to variable
		/// declarations - and performs necessary casts after
		/// initializations.
		/// 
		/// In SEQUENCE types the order of components is important.
		/// This method "registers" named variables (components) to its
		/// numeric places. Therefore, it is here only for coding
		/// convenience.
		/// </summary>
		public abstract void RegisterComponents();

		/* ----------------------------------------*/

		protected ASN1Sequence() 
		{
			this.Tag = (int) (ASN1TagNumber.SequenceTag);
			this.Init();
		}

		

		/// <summary>
		/// A MUST-CALL method for a constructor of descending class
		/// </summary>
		public virtual void Init () 
		{
			this.RegisterTypes();
			this.numOfComponents = ComponentTypes.Length;
			this.Components = new ASN1Object[numOfComponents];
		}

		
	
		protected virtual void DoComponentAssign(int j, ASN1Object ulaz)
		{
			this.Components[j] = ulaz;
		}
		
		public virtual void Assign(params ASN1Object[] ulaz) 
		{
			if  ( ulaz.Length > this.numOfComponents ) 
			{
				throw new InitializationException("too many parameters for this SEQUENCE type");
			}

			int i=0, j=0;
			while ( i < ulaz.Length ) 
			{
				if ( ASN1Type.MatchType(ulaz[i], ComponentTypes[j].Type) ) 
					/* trebalo bi srediti da drugi parametar bude
					 * ASN1ComponentTypeInfo, dakle 'ComponentTypes[i]'  
					 * Onda bi se iz toga u .MatchType mogli srediti
					 * svi slucajevi koji proizlaze iz parametriziranih
					 * tipova.  */
					 
				{
					DoComponentAssign(j, ulaz[i]);
				
					
					
					i++; // Grab next input.
				}
				else
				{
					if (!(this.ComponentTypes[j].isOptional))
					{
						throw new Exception("BAD Sequence ASSIGN! non-OPTIONAL type is missing!");
					}
				}

				j++;				
			}

			this.RegisterComponents();
			
		}
	


		public void Assign(BEREncoding nekiBER) 
		{
			this.fromBER(nekiBER);
		}
	

		public override BEREncoding asDER() 
		{

			ArrayList BERLista = new ArrayList();

			/// ovdje se treba "spustiti" do 
			/// .asDER() metode svakog od
			/// tipova.. No prije toga se
			/// treba pozabaviti sa komponentama oznacenim sa 
			/// OPTIONAL, DEFAULT
					
			for ( int i=0; i < this.numOfComponents; i++)  
			{
				ASN1Object obo = this.Components[i];
				if (obo != null) 
					/// ovo se moze dogoditi moguce ako je
					/// komponenta oznacena sa OPTIONAL
				{ 
					/// Provjeri je li "obo == DEFAULT value".
					/// Ako jest - ne kodiraj.
					/// 

					ASN1ComponentTypeInfo ext_t = this.ComponentTypes[i];

					if (obo == ext_t.DefaultValue) 
					{
                        // no op
						//Console.WriteLine("Noop");
					} 
					else 
					{
						BERLista.AddRange(obo.asDER().GetBERCode());
					}
				}
				else 
				{
					if (this.ComponentTypes[i].isOptional) 
					{
						/// U redu...
						///   Ne radi nista.
					}
					else if (true) 
					{
						if (this.ComponentTypes[i].DefaultValue == null) 
						{
							/// Nedostaje komponenenta koja nije oznacena sa OPTIONAL ili DEFAULT										
							 throw new MemberAccessException("SEQUENCE not completely initialized!"+
                                                 " missing "+i.ToString()+this.ToString());
						}
					}
				}
			}


			/*
			int len = BERLista.Count;
			Byte[] tmpBytes = new Byte[len];
			BERLista.ToArray().CopyTo(tmpBytes, 0);

			return new BERConstructedEncoding(
				this.Tag,
				len,
				tmpBytes
				);
			*/	
			

			return new BERConstructedEncoding(this.Tag, BERLista);
		}


		private bool CheckThisShitUpBitch(int tag, int currentCoordinate, ASN1ComponentTypeInfo currentTypeInfo)
		{
			bool ret_val=true;
			
			ret_val = (currentCoordinate < this.numOfComponents) &&
				(!ASN1Type.MatchTag(tag, currentTypeInfo.Type));

			return ret_val;
		}
		
		
		public override void fromBER(BEREncoding ulazBER) 
		{
			/// BER vraca niz BER kodova koji se 
			/// lako dekodiraju. Bitna stvar ovdje je
			/// provjeriti adekvatnost BER-a i "trenutnog" tipa:
			///     Ako BER ne odgovara ocekivanom tipu moze biti
			///     da je tip bio oznacen sa OPTIONAL ili DEFAULT.
			///  Ako je OPTIONAL, preskocimo tip i pokusamo sa 
			///  sljedecim tipom. Ako je DEFAULT, stavimo na odgovarajuce
			///  mjesto referencu na DefaultValue i u istoj 
			///  iteraciji petlje pokusamo za sljedeci tip.
		
			BEREncoding[] berArray = ulazBER.Decompose();
			BEREncoding   ber;

			ASN1ComponentTypeInfo currentTypeInfo;
		 
		
			
			int i=0,  /// i je indeks trenutnog BER koda
                j=0;  /// j je indeks trenutne komponente
					  ///  Vrijedi:   i <= j.

																		/*
						 ___        ___
			            |  0|      |  0|   
			            |___|      |___|
			     i -->  |  1|      |  1|   
			            |___|      |___|
			            |  2|      |  2| <--  j
			            |___|      |___|
						|  3|      |  3|
						       ...
																		*/

			
			// iteriraj po svim dobivenim ber-ovima.

			#region 1. Petlja po dobivenim ber-ovima
			for ( ; i < berArray.Length; i++, j++) 
			{
				ber = berArray[i];
				currentTypeInfo  = this.ComponentTypes[j];
				 

				int tag = BEREncoding.DecodeTagNumber(ber.GetIdOctets());

				/// Probaj Match-ati tag i currentTypeInfo.Type.Tag
				
				try
				{
					while (CheckThisShitUpBitch(tag, j, currentTypeInfo))
					{
						/// ako TAG ne odgovara trenutnoj komponenti, pokusaj povezati
						/// trenutni BER sa nekom od sljedecih komponenata. 
						/// 
						/// Dok skipas,
						/// provjeri da li ovaj SEQUENCE tip dozvoljava skipanje
						/// (tj. da li je komponenta markirana sa
						/// DEFAULT ili OPTIONAL)
						if(currentTypeInfo.DefaultValue != null) 
						{
							this.Components[j] = currentTypeInfo.DefaultValue;
						}
						else if (currentTypeInfo.isOptional) 
						{
							 
						}
//						else if (currentTypeInfo.DefaultValue != null)
//						{
//							this.Components[j] = currentTypeInfo.DefaultValue;
//						}
						else
						{
							throw new Exception ("A type that was not marked as optional is missing");
						}

//						j++;
//						currentTypeInfo = this.ComponentTypes[j];


						currentTypeInfo  = this.ComponentTypes[++j];
					}
				}
				catch (System.IndexOutOfRangeException) 
				{
					/* -- to znaci da je puklo pri 'skipanju' optionalnih tipova.
					 *    Moglo bi se zakljuciti da nedostaje odgovarajuci tip.
					 */
					Console.WriteLine("indeks j je pobjegao van");
					throw new Exception("Indeks j u ASN1Sequence.fromBER je pobjegao");
				}
				finally {}
				
				this.Components[j] = currentTypeInfo.CreateInstance(ber);
			
			}
			#endregion

			
			#region 2. Provjeri da li (ako) fale tipovi markirani sa DEFAULT ili OPTIONAL

			for ( ; j < this.numOfComponents; j++ ) 
			{
				/// Vise nemam BER ova, ali ako uopce 
				/// dodje do izvrsavanja ove petlje znaci da
				/// je ostalo neinicijaliziranih polja ovog tipa.
				/// Ako su polja optional - to nije problem. No - ako su 
				/// to bila Default polja DER kodiranja, te komponente treba
				/// inicijalizirati.
				/// 

				currentTypeInfo = this.ComponentTypes[j];
			 
			
				if (currentTypeInfo.isOptional) 
				{
				}
				else 
				{
					if (currentTypeInfo.DefaultValue != null)  
					{
						this.Components[j] = currentTypeInfo.DefaultValue;
					}
					else 
					{
						//FIXME
						throw new Exception("A type is missing (not default, not optional)");
					}
				 }
			}
			#endregion

			this.RegisterComponents();
		}          
	}


	abstract public class ASN1Sequence_type: ASN1Type 
	{
		public ASN1Sequence_type() 
		{
			this.Init();
		}

		public virtual void Init() 
		{
			this.Tag = (int) ASN1TagNumber.SequenceTag;
		}

		protected ASN1Sequence SubCreateInstance(ASN1Sequence asnseq, BEREncoding ulazBER)
		{
			asnseq.fromBER(ulazBER);
			asnseq.RegisterComponents();
			return asnseq;
		}

		public override ASN1Object CreateInstance(BEREncoding ulazBER)
		{
			ASN1Sequence aseq = this.Instance() as ASN1Sequence;
			aseq.fromBER(ulazBER);
			return aseq;
		}


		/// <summary>
		/// Ovo je metoda koja ce zamijeniti .CreateInstance u 
		/// SEQUENCE type-u, a i .CreateInstance u klasama koje naslijedjuju
		/// od SEQUENCE-a...
		/// </summary>
		/// <param name="ulazBER"></param>
		/// <returns></returns>
		public virtual ASN1Object NewCreateInstance(BEREncoding ulazBER) 
		{
			ASN1Sequence aseq = this.Instance() as ASN1Sequence;
			aseq.fromBER(ulazBER);
			return aseq;
		}
		
	}

	

	/// <summary>
	/// Base class for SEQUENCE OF structures in ASN.1
	/// </summary>
	abstract public class ASN1SequenceOf: ASN1Object 
	{
		/// <summary>
		/// contains elements of the sequence
		/// </summary>
		public ArrayList Elements;

		/// <summary>
		/// constructors of descendant classess must intialize this value to the
		/// _type object that corresponds to the type of elements in "SEQUENCE OF"
		/// structure.
		/// </summary>
		public ASN1Type  type;
		
		public int Min_Elements;
		public int Max_Elements;


		public ASN1SequenceOf() 
		{
			this.Init();
		}


		/// <summary>
		/// Must rename this to CheckTag... an introspection any deeper
		/// than this is pointless..
		/// </summary>
		/// <param name="obo"></param>
		/// <returns></returns>
		public virtual bool CheckType(ASN1Object obo) 
		{
			//return ASN1Type.MatchType(obo, _type);
			return true;
		}

		public virtual void Init() 
		{
			this.Tag = (int) (ASN1TagNumber.SequenceTag);
			this.Elements = new ArrayList();
		}

		
		public virtual void Add(ASN1Object obo) 
		{
			if (this.CheckType(obo))
			{
				this.Elements.Add(obo);
			}
			else 
			{
				throw new Exception("Wrong type for this SEQUENCE OF");
			}
		}

		
		public override BEREncoding asDER() 
		{
			ArrayList BERLista = new ArrayList();

			foreach (ASN1Object asn1object in this.Elements)
			{
				BERLista.AddRange(asn1object.asDER().GetBERCode());
			}
			
			return new BERConstructedEncoding(this.Tag, BERLista);
		}


		public override void fromBER(BEREncoding ulazBER)
		{
			object[] tmpObj = ulazBER.Decompose();

			for (int i=0 ; i < tmpObj.Length; i++) 
			{
				BEREncoding ber = (tmpObj[i] as BEREncoding);
				ASN1Object ob = this.type.CreateInstance(ber);
		
				this.Add(ob);
			}
		}

		public override string ToString()
		{
			string retValue="";

			foreach (ASN1Object o in this.Elements) 
			{
				retValue+=o.ToString()+",";
			}
			
			return retValue;
		}
	}

	
	abstract public class ASN1SequenceOf_type: ASN1Type 
	{
		
		/// <summary>
		/// !!! must check whether this one is needed...
		/// </summary>
		public ASN1Type type;

		protected ASN1SequenceOf_type() 
		{
			this.Tag = (int) ASN1TagNumber.SequenceTag;
		}

	

		public override ASN1Object CreateInstance(BEREncoding ulazBER)
		{
			ASN1SequenceOf asnseqof = this.Instance() as ASN1SequenceOf;


			ArrayList BERLista = new ArrayList(ulazBER.Decompose());

			/// sad imam listu BERova. 
			/// za svaki BER treba kreirati odgovarajuci objekt.
			
			ASN1Object tmpasn;

			foreach (BEREncoding tmpBER in BERLista) 
			{
				/// kreiranje odgovarajuceg objekta...
				tmpasn = type.CreateInstance(tmpBER);

				/// dodavanje objekta u SequenceOf-ov niz objekata.
				asnseqof.Add(tmpasn);				
			}

			return asnseqof;
		}

	}
}



