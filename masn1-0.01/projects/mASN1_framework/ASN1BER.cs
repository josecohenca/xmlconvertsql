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

	using System.IO;

	using mASN1.framework.experimental;

	/// <summary>
	/// Class for handling (existing) BER encodings. BER is a Tag-Length-Value encoding.
	/// </summary>
	public class BEREncoding 
	{
		protected bool decQ, encQ;   // state of BEREncoding object
		public bool constructedQ;

		#region Members (Decoded data)
		/// <summary>
		/// "raskodirano"
		/// </summary>
		protected Byte[] IdOctets;
		protected Byte[] LenOctets;
		protected Byte[] ContentsOctets;
		#endregion

		#region Members (Encoded data)
		
		public Byte[] EncodedOctets;
		#endregion
		
		#region Constructors
		
		public BEREncoding() 
		{
			decQ = encQ = false;
		}

		/// <summary>
		/// Konstruktor za inicijaliziranje BEREncoding
		/// objekta sa kodom...
		/// </summary>
		/// <param name="kod"></param>
		public BEREncoding(Byte[] kod) 
		{
			this.EncodedOctets = kod;
				
			encQ = true;
			this.Decode();
		}

		
		/// <summary>
		/// Ovaj mora van. (FIXME)
		/// </summary>
		/// <param name="idOct">Identifier octets</param>
		/// <param name="contOct">Contents octets</param>
		public BEREncoding(Byte[] idOct, Byte[] contOct): this() 
		{
			this.IdOctets = idOct;

			/// ovo cu jednom elegantnije...
			this.LenOctets = BEREncoding.MakeIndefiniteLength();
		
			this.ContentsOctets = contOct;

			decQ = true;
		}

		
		
		/// <summary>
		/// Konstruktor sa raskodiranim parametrima.. 
		/// (za (primitive/constructed) definite length
		/// kodiranje)
		/// </summary>
		/// <param name="idOct">Identifier okteti</param>
		/// <param name="lenOct">Length okteti</param>
		/// <param name="contOct">Contents okteti</param>
		public BEREncoding(Byte[] idOct, Byte[] lenOct, Byte[] contOct) : this() 
		{

			/// Spajanje dijelova u primitivni BER kod..
			/// To bi trebalo elegantnije rijesiti..
			IdOctets       = idOct;
			LenOctets      = lenOct;
			ContentsOctets = contOct;

			decQ = true;

	
			this.Encode(idOct, lenOct, contOct);
		}



		
		#region EXPERIMENTAL
		public BEREncoding(ASN1TAG TAG, byte[] valueOctets):this() 
		{
			byte[] idOctets  = MakeIdOctets(TAG);
			byte[] lenOctets = MakeLenOctets(valueOctets.Length);

			IdOctets       = idOctets;
			LenOctets      = lenOctets;
			ContentsOctets = valueOctets;

			decQ = true;
			this.Encode(idOctets, lenOctets, valueOctets);
		}

#if EXPERIMENTAL
		public static BEREncoding Create(ASN1TAG TAG, ExperimentalObject valueObject) 
		{
			byte[] tag_Octets = MakeIdOctets(TAG);

		    byte[] value_Octets;

	
			if (TAG.IsExplicit())
			{
				value_Octets = (valueObject.asDER()).GetBERCode();
			}
			else 
			{
				value_Octets = valueObject.Get_BER_Value_Octets()
			}

			return new BEREncoding(tag_Octets, value_Octets);
		}
#endif

		#endregion

		#endregion

		#region Methods
		
		
		
		/// <summary>
		/// Metoda koja iz komponenti napravi
		/// BERCode (polje byte-ova), i sprema u
		/// "this.EncodedOctets"
		/// </summary>
		public virtual void Encode(byte[] tag_Octets, byte[] len_Octets, byte[] val_Octets) 
		{
			if(!this.encQ) 
			{
				int totalLen = 
					 tag_Octets.Length
					+len_Octets.Length
					+val_Octets.Length;


				EncodedOctets = new byte[totalLen];
				tag_Octets.CopyTo(EncodedOctets, 0);
				len_Octets.CopyTo(EncodedOctets, tag_Octets.Length);
				val_Octets.CopyTo(EncodedOctets, tag_Octets.Length+len_Octets.Length);
				
				this.encQ = true;

				if (this.constructedQ) 
				{
					this.MakeConstructed();
				}
			}
		}

		
		/// <summary>
		/// Metoda kojom BEREncoding objekt postavlja 
		/// IdOctets, LenOctets i ContentsOctets iz
		/// EncodedOctets.
		/// </summary>
		public virtual void Decode() 
		{
			if (!this.decQ) 
			{
				this.SetOctetsFromBER();
			}
		}
		
		
		
		
		/// <summary>
		/// Funkcija treba rasclaniti BER kod na cjeline:
		/// Identifier, Length i Contents oktete.
		/// </summary>
		private void           SetOctetsFromBER() 
		{	
			int bytesProcessed=0,
				tmpLen=0;

			if (this.EncodedOctets != null) 
			{
				this.constructedQ = ((0x20 & this.EncodedOctets[0])==0x20);
			}

		
			bool lowTagForm;
				
		
			lowTagForm = ( this.EncodedOctets[0] & 0x1f )!= 0x1f;

			if ( lowTagForm ) 
			{
				this.IdOctets = new byte[] {this.EncodedOctets[0]};
				
			} 
			else
			{
				/// !!!!!!!!!!!!!!!!!
				/// Ima High tag number form za identifier octets.
				/// Znaci da je tag number veci od 0x00011111, i da 
				/// je zapisan u jos nekoliko okteta u bazi 128.
				throw new Exception("Nisam jos implementirao highTagForm BEREncoding");
			}

			if (this.constructedQ) 
			{
				/// ???
				/// 
				//this.IdOctets[0] ^= (0x20); 
			}

			bytesProcessed += this.IdOctets.Length;
		
			/// ne znam jel ima nekog pretjeranog smisla, ali 
			/// ovu sam funkciju podjelio u tri bloka...
			/// Ovo je LENGTH blok.
		
			bool shortForm;
			shortForm = ((0x80 & this.EncodedOctets[bytesProcessed])==0);

			if(shortForm) 
			{
				this.LenOctets = new byte[] {this.EncodedOctets[this.IdOctets.Length]};
				tmpLen = (int) this.EncodedOctets[bytesProcessed];

				bytesProcessed++;
				
			}
			else 
			{
				/// !!!!!!!!!!!!!!
				/// izvuci Length bytove iz duge forme.. prvi byte kaze koliko byteova
				/// ima u zapisu (max 127), u sljedecim byteovima je zapisana duljina
				/// u bazi 256..
				/// 
				/// tu duljinu treba setirati u tmpLen. Ako ne stane u tmpLen, onda 
				/// treba baciti cijeli Exception, jer mislim da nema smisla raditi 
				/// sa prevelikim BER kodovima.
				/// 

				int len_len = (int) (this.EncodedOctets[bytesProcessed] & 0x7f);
				if (len_len == 0) 
				{
					/// Oopsy-daisy - imam indefinite-encoding
					/// 

					throw new Exception("Indefinite BER is not implemented - yet");
				}

				bytesProcessed += 1;

				if (len_len>4) 
				{
					/// to mi je previse.
					
					throw new Exception("BER too long!");
				}
			 
				for (int j=0; j<len_len; j++) 
				{
                    tmpLen <<=8;
					tmpLen |= (int) (this.EncodedOctets[bytesProcessed+j]);
				}

				bytesProcessed += len_len;

				/// Ovdje treba iskopirati len oktete u LenOctets..
				/// 

				/// ....
				/// 





			}

			///bytesProcessed += tmpLen;

		
			/// Contents dio.
		
			Byte[] tmpByteArray;
			int i;

			tmpByteArray = new Byte[this.EncodedOctets.Length - bytesProcessed];

			for(i=0; i<tmpByteArray.Length; i++) 
			{
				tmpByteArray[i] = this.EncodedOctets[ i + bytesProcessed];
			}
			
			this.ContentsOctets = tmpByteArray;

		

			this.decQ = true;
		}

		
		/// <summary>
		/// BER's method for applying encoding class bits.
		/// Possible options are UNIVERSAL, CONTEXT, APPLICATION, PRIVATE
		/// </summary>
		/// <param name="klasa">EncodingClass enum member</param>
		public void ApplyEncodingClass(EncodingClass klasa) 
		{
			if (this.decQ) 
			{
				this.IdOctets[0] |= (byte) klasa;
			}

			if (this.encQ)
			{
				this.EncodedOctets[0] |= (byte) klasa;
			}
		}

		public virtual byte[] GetContents() 
		{
			/// return null if undef (OK with me)
			return this.ContentsOctets;
		}

		public virtual byte[] GetIdOctets()
		{
			/// return null if undef (OK with me)
			return this.IdOctets;
		}

		/// <summary>
		/// Metoda vraca duljinu ContentsOctets dijela.
		/// </summary>
		/// <returns>duljina Contents dijela (int)</returns>
		public virtual int    GetLength() 
		{
			///
			/// ovo je naivan nacin, ustvari duljinu treba
			/// znati prije nego se uopce kreira polje
			/// ContentsOctets....
			/// 
			/// Mogu se ograniciti na Int64. To je dovoljno..
			///  +/-9,223,372,036,854,775,808 
			/// 
			return this.ContentsOctets.Length;
		}


		public virtual byte[] GetBERCode() 
		{
			if (!encQ) this.Encode(IdOctets, LenOctets, ContentsOctets);
			
			return this.EncodedOctets;
			
		}


		
		// vraca duljinu trenutnog BER komada.</returns>
		private static int scanBERlen (int start, byte[] bytes) 
		{
			int i=start, hl=0;
			byte[] tmpByteArray = bytes; //?

			byte b;

			///
			/// proskeniraj tag
			/// 
			b = tmpByteArray[i];
			if ((b & 0x1f) == 0x1f) 
			{	/// High Tag number form;
				do 
				{
					i++; hl++;
					b = tmpByteArray[i];
				} while ((b & 0x80) == 0x80); 
			}
			else 
			{	/// LowTag number form;
				i++; hl++;
				b = tmpByteArray[i];
			
			}

			/// sad hl sadrzi duljinu TAG dijela.

			
			/// procitaj Len oktete
			if ((b & 0x80) == 0x80 ) 
			{  /// LongForm
				int lenlen;
				
				lenlen = (int)(b & 0x7f);
				if (lenlen > 4) 
				{
					throw new Exception("Preveliki LENGTH BER koda!");
				}

				hl +=lenlen;

				

				int tmpLen=0;
				
				for (int j=1; j<=lenlen; j++) 
				{
					tmpLen <<=8;
					tmpLen |= (int) (tmpByteArray[i+j]);
				}

				i+=lenlen;

				return (hl+tmpLen);

			}
			else
			{  ///ShortForm
				
				return (hl + (int) b);
			}
		}
		 

		/// <summary>
		/// Ova metoda vraca polje BER kodova koji su "concatenated" u 
		/// contents dijelu.
		/// 
		/// 
		/// Mozda bi bilo bolje da se zove DecomposeContents...
		/// food for thought.
		/// </summary>
		/// <returns>polje BER kodova (BEREncoding objekata)</returns>
		public BEREncoding[] Decompose()
		{
			if (this.decQ == false) 
			{
				this.Decode();
			}
			
			ArrayList BERList = new ArrayList();
			if (this.constructedQ) 
			{
				int bytesProcessed=0;
				int totalLen = this.ContentsOctets.Length;
				int lastIndex;

				while ( bytesProcessed < totalLen ) 
				{
					Byte[] tmpByteArray;
					int i;

					lastIndex = bytesProcessed;
					bytesProcessed += BEREncoding.scanBERlen(bytesProcessed, this.ContentsOctets);
					bytesProcessed++;

					tmpByteArray = new Byte[bytesProcessed - lastIndex];
					for (i=0; i < (bytesProcessed - lastIndex); i++ ) 
					{
						tmpByteArray[i] = this.ContentsOctets[lastIndex+i];
					}
				
					BERList.Add(new BEREncoding(tmpByteArray));
				}
			} 
			else 
			{
				BERList.Add(this);
			}

			int l = BERList.Count;
			BEREncoding[] ret_array = new BEREncoding[l];
			BERList.CopyTo(0, ret_array, 0, l);
			return (ret_array);
		}

		
		
		/// <summary>
		/// BER-ova ToString() metoda, zgodno za 
		/// ispis...
		/// </summary>
		/// <returns></returns>
		public override string ToString() 
		{
			//this.decQ = false;
			//this.Decode();
			if (this.encQ) 
			{
				this.decQ=false;
				this.Decode();

			}


			string zaVratiti = "BER: \n     [id]   ";
			
			foreach (Byte b in IdOctets) 
			{
				zaVratiti += String.Concat(b.ToString(), ",");
			}

			zaVratiti += "\n    [len]   ";

			foreach (Byte b in LenOctets) 
			{
				zaVratiti += String.Concat(b.ToString(), ",");
			}

			zaVratiti += "\n   [cont]   ";

			foreach (Byte b in ContentsOctets)
			{
				zaVratiti += String.Concat(b.ToString(), ",");
			}

			zaVratiti += "\n";


			return zaVratiti;
		}

		
		/// <summary>
		/// Usporedjuje samo Contents oktete. To je glupo, moram 
		/// promijeniti... uskoro..
		/// </summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		public override bool Equals(object obj)
		{
			if ( this.GetBERCode().Length  ==	(obj as BEREncoding).GetBERCode().Length ) 
			{
				bool tmpBool = true;
				for (int i=0; i<this.GetBERCode().Length; i++) 
				{
					tmpBool &= (GetBERCode()[i] == (obj as BEREncoding).GetBERCode()[i]);
				}

				return tmpBool;
			}
			else 
			{
				return false;
			}
		}

		 
		public static bool operator==(BEREncoding ber1, BEREncoding ber2) 
		{
			return ber1.Equals(ber2);
		}

		public static bool operator!=(BEREncoding ber1, BEREncoding ber2) 
		{
			return !(ber1.Equals(ber2));
		}
		 

		public override int  GetHashCode()
		{
			return base.GetHashCode();
		}





        /* -----------------------------------------------------*/



		/// <summary>
		/// Trebaju mi static fje koje barataju i rade sa 
		/// prebacivanjem tag i length integera u pripadajuce
		/// oktete.
		/// </summary>
		/// <returns>Vraca odgovarajuce polje byte-va koje
		/// sadrzi tag oktete</returns>
		public static byte[] MakeTagOctets(int ulazTagNumber) 
		{
			/// treba odrediti koliki je int.
			/// Za ("tagnumber<32") dovoljan je prvi oktet,
			/// (to je 5 raspolozivih bitova)
			/// 
			/// Za veci tagNumber prelazi se u highTagNumber 
			/// format.
			if (ulazTagNumber < 0 ) 
			{
				throw new Exception("Nema negativnih tag numbera..");
			}
			
			if (ulazTagNumber < 0x1f) 
			{
				return new byte[]{Convert.ToByte(ulazTagNumber)};
			}
			else
			{	/// high tag number form
				/// zapisan u bazi 128, pocevsi od drugog byte-a.
				/// Prvi byte je oblika ???11111
				/// Svi osim zadnjeg od drugog nadalje su oblika
				/// 0???????
				/// Zadnji je oblika
				/// 1???????
				/// 

				/// Zasad pretpostavljam da je tag number MaxInt
				Byte[] b = new Byte[5];

				b[0] = Convert.ToByte((ulazTagNumber & 0x0000007f) | 0x80); // ovo je zadnji...
				b[1] = Convert.ToByte((ulazTagNumber & 0x00003f80) >>  7);
				b[2] = Convert.ToByte((ulazTagNumber & 0x001fb000) >> 14);
				b[3] = Convert.ToByte((ulazTagNumber & 0x0fe00000) >> 21);
				b[4] = Convert.ToByte((ulazTagNumber & 0xf0000000) >> 28);
				
				Queue q = new Queue();
				q.Enqueue((Byte)0x1f);

				int i=4;
				while (b[i]==0) i--;
				
				while ( i >= 0 ) 
				{
					q.Enqueue( b[i] );
					i--;
				}

				Byte[] tmpArray = new Byte[q.ToArray().Length];
				for ( i=0; i< q.ToArray().Length; i++) 
				{
					tmpArray[i] = (Byte)((q.ToArray())[i]);
				}

				return (  tmpArray  );
			}
		}

		public static byte[] MakeTagOctets(int ulazTagNumber, EncodingClass class_number) 
		{
			byte[] tmpArray = BEREncoding.MakeTagOctets(ulazTagNumber);
			tmpArray[0]|= (byte) class_number;

			return ( tmpArray );
		}

	
		/// <summary>
		/// Rutina za razbijanje duljine u pripadne byteove
		/// za LENGTH OCTETS.
		/// 
		/// FIXME: Ovo je vec sad obsolete...
		/// </summary>
		/// <param name="ulazLenNumber">uzimam da duljina moze 
		/// biti navise MaxInteger</param>
		/// <returns>polje byteova, Length Octets za zadanu duljinu</returns>
		public static byte[] MakeLenOctets(int ulazLenNumber)
		{
			/// za konkretnu duljinu postoje dva oblika:
			/// short form    i      long form.
			/// Short form pokriva duljine od 0..127, 
			/// Long form pokriva dovoljno velike brojeve..
		
			if (ulazLenNumber < 128) 
			{
				return new byte[]{Convert.ToByte(ulazLenNumber)};
			}
			else
			{
				// long form
				// prvi oktet je broj dodatnih okteta za zapis, 
				// preostali daju duljinu u bazi 256, sa MostSig..
				// bitom prvim.

				int tmp = ulazLenNumber;
				
				Queue q = new Queue();
				
				int i=0;
				while ((tmp & 0xff000000)==0) 
				{
					tmp = tmp << 8;
					i++;
				}
				
				while ( i<4 ) 
				{
					q.Enqueue( Convert.ToByte( (tmp & 0xff000000) >> 24 ) );
					tmp = tmp << 8;
					i++;
				}

				int len = q.ToArray().Length;
				Byte[] tmpArray = new Byte[len+1];
				
				tmpArray[0]=Convert.ToByte(len + 128);

				for ( i=0; i < len; i++) 
				{
					tmpArray[i+1] = (Byte)(q.Dequeue());
				}

				return (  tmpArray  );
			}
		}


		/// <summary>
		/// Funkcija iz Tag dijela BERCode-a treba
		/// prokuziti tag-number...
		/// 
		/// </summary>
		/// <returns>Tag number kao integer</returns>
		public static int DecodeTagNumber(Byte[] ulazTagOctets) 
		{
			/// IdOctets sadrze TagNumber.. 
			/// Ako je 
			byte prvi = ulazTagOctets[0];
 
			if ((prvi & (byte) BitConstant.LongForm) == (byte) BitConstant.LongForm) 
			{
				throw new Exception("Jos nije implementirano");
			}
			else
			{
				return (int) (prvi & (byte)BitConstant.LongForm);
			}


			
			
		}

		
		public static int DecodeLenOctets(Byte[] ulazLenOctets) 
		{
			Byte prviByte = ulazLenOctets[0];

			if (prviByte < 128) 
			{
				///.......... shortform;
				return Convert.ToInt32(prviByte);
			}
			else if (prviByte == 128) 
			{
				///... indefinite form
				throw new Exception("Indefinite - length BER code");

			}
			else 
			{
				///.......... maskiraj 8. bit.
				prviByte = Convert.ToByte(prviByte & 0x7f); 

				if (prviByte > 4) 
				{
					throw new Exception("Nisam predvidio toliku duljinu, jos.");
				}

				int i=0, shiftaj=0, ret_value=0;

				for ( i=prviByte; i>0 ; i--) 
				{
					ret_value |= ((ulazLenOctets[i]) << shiftaj);
					shiftaj += 8;
				}
				// sad u prviByte imam duljinu zapisa BER-duljine u
				// u bazi 256.
				return ret_value;
			}
		}

		//FIXME
		public static byte[] MakeIdOctets(ASN1TAG TAG) 
		{
			if (TAG.IsImplicit()) 
			{
			}
			else 
			{
			}

			return null; //FIXME
		}

		/// <summary>
		/// This static method creates one byte array 
		/// representing Constructed indefinite encoding.
		/// </summary>
		/// <returns>One byte byte-array, of value 0x80</returns>
		public static byte[] MakeIndefiniteLength() 
		{
			return new byte[] { (int) BitConstant.IndefiniteLength };
		}


		public bool IsIndefinite() 
		{
			/// must implement...
			/// 

			/*
			 *   provjeri jel dekodiran i vrati
			 *     return ((constructed) AND (prvi len-oktet je 0x80) )
			 *        
			 */

			return false;
		}



		public static byte[] ToSpecialBase128(int ulazInt) 
		{
			/* treba za:
			 *  ObjectIdentifier,
			 *  High-tag-number form
			 */

			/* 
			 * cut n paste iz MakeLenOctets()
			 */

			return new Byte[0];
		}

		
		public void ApplyClassNumber(EncodingClass classNumber) 
		{
			if (this.decQ) 
			{
				/// ako je BER raskodiran - sredi byte [0]...
				
				this.IdOctets[0]      |= (byte)classNumber;
			}

			if (this.encQ)
			{
				this.EncodedOctets[0] |= (byte) classNumber;
				this.decQ = false;


			}

			if (!(this.decQ || this.encQ))
			{
				throw new Exception("BERcode is not initialized!");
			}
		}

		public void MakeConstructed() 
		{
			this.constructedQ = true;

			if (this.decQ) 
			{
				this.IdOctets[0] |= (byte) BitConstant.ConstructedEncoding;
			}

			if (this.encQ) 
			{
				this.EncodedOctets[0] |= (byte) BitConstant.ConstructedEncoding;
			}
		}

#endregion

		
	}



	
	
	/// <summary>
	/// Constructor for a class responsible for 
	/// BER Primitive encoding.
	/// </summary>
	public class BERPrimitiveEncoding:   BEREncoding 
	{
		/// <summary>
		/// Jednostavni konstruktor za primitivni BER.
		/// </summary>
		/// <param name="tagNum">tag number</param>
		/// <param name="lenNum">length of contents</param>
		/// <param name="contOct">contents octets</param>
		public BERPrimitiveEncoding(int tagNum, int lenNum, byte[] contOct) 
		{
			this.encQ = false;
			
			this.IdOctets  = BEREncoding.MakeTagOctets(tagNum);
			this.LenOctets = BEREncoding.MakeLenOctets(lenNum);
			this.ContentsOctets = contOct;

			this.decQ = true;
		}

		public BERPrimitiveEncoding(int tagNum, byte[] contOct) 
		{
			int len=contOct.Length;
			this.encQ = false;

			this.IdOctets  = BEREncoding.MakeTagOctets(tagNum);
			this.LenOctets = BEREncoding.MakeLenOctets(len);
			this.ContentsOctets = contOct;

			this.decQ = true;
		}
	}

	
	/// <summary>
	/// Class responsible for constructed BER encoding
	/// </summary>
	public class BERConstructedEncoding: BEREncoding 
	{
		

		/// Novi konstruktori
		/// 

		public BERConstructedEncoding() 
		{
			decQ = encQ = false;
			this.constructedQ = true;
		}
		
		public BERConstructedEncoding(int tagNum, int lenNum, byte[] contOct) 
		{
			this.encQ = false;
			this.decQ = true;
			
			
			this.IdOctets  = BEREncoding.MakeTagOctets(tagNum);
			this.LenOctets = BEREncoding.MakeLenOctets(lenNum);

			this.ContentsOctets = contOct;

			this.MakeConstructed();		
		}

		public BERConstructedEncoding(int tagNum, byte[] contOct) 
		{
			this.encQ = false;
			
			
			this.IdOctets  = BEREncoding.MakeTagOctets(tagNum);

			this.MakeConstructed();		

			this.LenOctets = BEREncoding.MakeLenOctets(contOct.Length);
			this.ContentsOctets = contOct;

			this.decQ = true;
		}

		public BERConstructedEncoding(int tagNum, ArrayList BERLista) 
		{
			this.encQ = false;
			
			
			int len = BERLista.Count;
			Byte[] tmpBytes = new Byte[len];
			BERLista.ToArray().CopyTo(tmpBytes, 0);

			this.IdOctets = BEREncoding.MakeTagOctets(tagNum);
			this.MakeConstructed();

			this.LenOctets = BEREncoding.MakeLenOctets(len);

			this.ContentsOctets = tmpBytes;

			this.decQ = true;
		}

		public BERConstructedEncoding(int tagNum, byte[] contOct, EncodingClass class_number) 
		{
			this.encQ = false;
			
			
			this.IdOctets  = BEREncoding.MakeTagOctets(tagNum, class_number);

			this.MakeConstructed();		

			this.LenOctets = BEREncoding.MakeLenOctets(contOct.Length);
			this.ContentsOctets = contOct;

			this.decQ = true;
		}

	}

	
	/// <summary>
	/// BER class used for construction of 
	/// BER indefinite-length codes.
	/// </summary>
	public class BERIndefiniteEncoding:  BERConstructedEncoding
	{
			
		public new ArrayList ContentsOctets;

		
		/// <summary>
		/// Konstruktor za indefinite BER.
		/// Kasnije se sa rutinom AddContents(Byte[]) moze
		/// dodavati jos byteova. Na kraju treba pozvati
		/// MakeEndOfContents().
		/// </summary>
		/// <param name="tagOct">Tag okteti</param>
		/// <param name="contOct">Contents okteti (ili barem pocetak istih)</param>
		public BERIndefiniteEncoding(Byte[] tagOct, byte[] contOct) 
		{
			
			
			this.decQ = this.encQ = false;
			
			this.IdOctets = tagOct;

			/// Srediti!!!
			this.IdOctets[0] |= (byte) BitConstant.ConstructedEncoding;

			this.LenOctets = BEREncoding.MakeIndefiniteLength();
			this.ContentsOctets.AddRange(contOct);
		}


		/// <summary>
		/// Method used to "feed" indefinite BER. When done, 
		/// call MakeEndOfContents().
		/// </summary>
		/// <param name="contOctets"></param>
		public void AddContents(Byte[] contOctets) 
		{
			this.ContentsOctets.AddRange(contOctets);
		}

		

		public void MakeEndOfContents() 
		{
			this.ContentsOctets.AddRange(new byte[]{0x00, 0x00});
			this.decQ = true;
    	}

	}

	/// <summary>
	/// Perhaps unnecessary, but - very convenient, at least in the
	/// development phase.
	/// </summary>
	public class BERWriter : BinaryWriter 
	{
		public BERWriter(Stream fs) 
		{
			this.OutStream = fs;
		}

		public void Write(BEREncoding ber) 
		{
			this.Write(ber.GetBERCode());
		}

		public static void DumpHEX(BEREncoding ber) 
		{
			Console.Write("HEX: ");
			byte[] code = ber.GetBERCode();
			foreach (byte b in code) 
			{
				if(b<16) 
				{
					Console.Write("0");
				}
				Console.Write(b.ToString("X")+":");
			}
			Console.WriteLine();
		}

		public static void DumpHEX(ASN1Object ob) 
		{
			DumpHEX(ob.asDER());
		}

		public static void DumpDERToFile(ASN1Object ob, String filename) 
		{
			try 
			{
				FileStream fs = new FileStream(filename, FileMode.Create, FileAccess.Write);
				
				BERWriter br = new BERWriter(fs);
				br.Write(ob.asDER().GetBERCode());
				
				fs.Close();
				br.Close();
			}
			catch
			{
				Console.WriteLine("Failed at writing Data BER to file.");
			}
		}

	}

	

	public class BERReader : BinaryReader 
	{



		public BERReader(FileStream fs) : base(fs) {}

		/* 
		 * public BEREncoding CreateBEREncoding() 
		{
			try 
			{
				byte[] ber_bytes = this.ReadBytes((int) this.BaseStream.Length);
				BEREncoding ber = new BEREncoding(ber_bytes);

				return ber;			
			} 
			catch 
			{
				throw new Exception("Failed reading.");
			}
			  
		}
		*/

		public static BEREncoding CreateBEREncoding(Stream strm) 
		{
			try 
			{
				BinaryReader binReader = new BinaryReader(strm);
				byte[] ber_bytes = binReader.ReadBytes((int) strm.Length);
				binReader.Close();

				///BEREncoding ber = new BEREncoding(ber_bytes);

				return new BEREncoding(ber_bytes);
			}
			catch
			{
				throw new Exception("Reading failed.");
			}
		}

		public static BEREncoding CreateBEREncoding(string filename) 
		{
			using(Stream fs = new FileStream(filename, FileMode.Open)) 
			{
					return CreateBEREncoding(fs);
			}
		}
	}

//	try 
//{
//	FileStream fs = new FileStream(@"test.X501.Name.01.ber", FileMode.Open);
//	BinaryReader br = new BinaryReader(fs);
//	byte[] ber_bytes = br.ReadBytes((int)fs.Length);
//
//	BEREncoding ber = new BEREncoding(ber_bytes);
//	name.fromBER(ber);
//
//	fs.Close();
//}
//catch (Exception e)
//{
//Console.WriteLine(e.ToString());
//throw new Exception("Samples pallette failed at initializing .name!");
//}
	
	
	public class BERComparer:IComparer
	{
		#region IComparer Members

		public int Compare(object x, object y)
		{
			// TODO:  Add BERComparer.Compare implementation

			if ((x==null)||(y==null)) 
			{
				return 0;
			} 
			else 
			{
				return CompareBERs(x as byte[], y as byte[]);
			}
		}

		public int CompareBERs(byte[] x, byte[] y) 
		{
			int l_x = x.Length,
				l_y = y.Length,
				l;

			l = (l_x < l_y) ? l_x: l_y;

			/// sad po zajednickom dijelu usporedi BER kodove.
			
			for (int i=0; i<l; i++) 
			{
				if (x[i]<y[i]) return -1;
			}
            

			/// ako je manji dio jednak, onda je BER kod koji
			/// je kraci "manji" u ovom uredjaju.
			return (l_x-l);


		}

		#endregion

	}


}

	









