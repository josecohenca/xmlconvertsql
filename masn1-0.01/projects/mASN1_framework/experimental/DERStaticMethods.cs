namespace mASN1.framework.experimental
{
	using mASN1;

	using System;
	using System.Collections;

	abstract public class DERMethods 
	{
	
		public static byte[] MakeTagOctets(SimpleASN1TAG tag) 
		{
			
			/// treba odrediti koliki je int.
			/// Za ("tagnumber<32") dovoljan je prvi oktet,
			/// (to je 5 raspolozivih bitova)
	
			byte[] returnByteArray;
				
			if (tag.tagValue < 0x1f) 
			{
				returnByteArray = new byte[]{Convert.ToByte(tag.tagValue)};
			}
			else
			{	
				/* nisam siguran da sam ikad usao u ovu granu koda... */
				
				/// high tag number form
				/// zapisan u bazi 128, pocevsi od drugog byte-a.d
				/// Prvi byte je oblika "---1 1111"
				/// Svi osim zadnjeg od drugog nadalje su oblika
				/// "0--- ----"
				/// Zadnji je oblika
				/// "1--- ----"
				/// 

				/// Zasad pretpostavljam da je tag number MaxInt
				int ulazTagNumber = tag.tagValue;

				ArrayList b = new ArrayList();
				
				//HACK: jednom to ljepse srediti..
				b.Insert(0, Convert.ToByte((ulazTagNumber & 0x0000007f) | 0x80));  // ovo je zadnji...
				b.Insert(0, Convert.ToByte((ulazTagNumber>> 7) & (0x7f))); 
				b.Insert(0, Convert.ToByte((ulazTagNumber>>14) & (0x7f)));
				b.Insert(0, Convert.ToByte((ulazTagNumber>>21) & (0x7f)));
				b.Insert(0, Convert.ToByte((ulazTagNumber>>28) & (0x7f)));
				
				while ((byte)b[0]==0)  
				{
					b.RemoveAt(0);
				}

				b.Insert(0, (byte)0x1f);


				// Ovo je najjednostavniji nacin - koliko znam - za prepisati
				// ArrayList u odgovarajuce polje.
				returnByteArray = b.ToArray(typeof(byte)) as byte[];
			}

			
			/// apply encoding class number:
	
			returnByteArray[0] |= (byte) tag.encodingClass;

			if (tag.ConstructedBitOn) 
			{
				returnByteArray[0]|= (byte) 0x20;
			}

			return returnByteArray;
		}

		
		public static byte[] MakeLengthOctets(ICollection V) 
		{			
			return MakeLengthOctets(V.Count);
		}

	
		public static byte[] MakeLengthOctets(int ulazLenNumber) 
		{
			/// za konkretnu duljinu postoje dva oblika:
			/// short form    i      long form.
			/// Short form pokriva duljine od 0..127, 
			/// Long form pokriva dovoljno velike brojeve..d
		
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
				byte[] tmpArray = new byte[len+1];
				
				tmpArray[0]=Convert.ToByte(len + 128);

				for ( i=0; i < len; i++) 
				{
					tmpArray[i+1] = (byte)(q.Dequeue());
				}

				return (  tmpArray  );
			}
		}

		/// <summary>
		/// Long form for tag and length octets have 8-th bit set to 1.
		/// </summary>
		/// <param name="b"></param>
		/// <returns></returns>
		public static bool LongForm(byte b) 
		{
			return ((b & 0x80) == 0x80);
		}

		
		/// <summary>
		/// Scans total length of DER/BER code that starts
		/// at start position.
		/// </summary>
		/// <param name="start"></param>
		/// <param name="bytes"></param>
		/// <returns></returns>
		public static int ScanBERLength (int start, byte[] bytes) 
		{
			int i=start, hl=0;
		

			byte b;

			#region TAG OCTETS
			b = bytes[i];
			if ((b & 0x1f) == 0x1f) 
			{	
				/// High Tag number form;
				do 
				{
					i++; hl++;
					b = bytes[i];
				} while (LongForm(b)); 
			}
//			else 
//			{	
//				/// Low Tag number form;
//				i++; hl++;
//				b = bytes[i];
//			}

			i++; hl++;
			b = bytes[i];

			#endregion

			#region LENGTH OCTETS
			/// procitaj 'Length' oktete
			/// 
			hl++;

			if (LongForm(b)) 
			{  
				int lenlen;
				
				/// first byte in long form tells how many digits
				/// in base 256 does this BER have. Since this 
				/// implementation assumes maxInt lengths...
				lenlen = (int)(b & 0x7f);

				if (lenlen > 4) 
				{
					// TODO: mASN1Exception - Unsupported BER Length...
					throw new Exception("Preveliki LENGTH BER koda!");
				}

				hl += lenlen;

				

				int tmpLen=0;
				
				for (int j=1; j<=lenlen; j++) 
				{
					tmpLen <<=8;
					tmpLen |= (int) (bytes[i+j]);
				}

				i+=lenlen;

				return (hl+tmpLen);
			}
			else
			{  ///ShortForm (za duljine manje od 128.				
				return (hl + (int) b);
			}

			#endregion
		}

		

		public static int FindStartOfLengthOctets(int start, byte[] bytes) 
		{
			return 0;
		}

		public static int FindStartOfValueOctets(int start, byte[] bytes) 
		{
			int i=start;
		

			byte b = bytes[i];

			if ((b & 0x1f) == 0x1f) 
			{	
				/// High Tag number form;
				do 
				{  
					i++; 
					b = bytes[i];
				} while (LongForm(b)); 
			}
			
			
			i++; 
			b = bytes[i];
			
 
			/// prevrti 'Length' oktete
			if (LongForm(b)) 
			{  
				int lenlen;
				
				/// first byte in long form tells how many digits
				/// in base 256 does this BER have. Since this 
				/// implementation assumes maxInt lengths...
				lenlen = (int)(b & 0x7f);

				if (lenlen > 4) 
				{
					// TODO: mASN1Exception - Unsupported BER Length...
					throw new Exception("Preveliki LENGTH BER koda!");
				}

				i += lenlen + 1;
			} 
			else 
			{
				i++;
			}

		


			return i;
		
		}
	}

}