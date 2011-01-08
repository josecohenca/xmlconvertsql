namespace mASN1.framework.experimental 
{
	using mASN1;
    
	using System.Collections;
	using System; // za Console.

	public class SimpleDER 
	{
		private byte[] derBytes;

		private int lenStart=0, valStart=0;

		public SimpleDER(byte[] derOctets) 
		{
			derBytes = derOctets;
			SetMarkers();
		}

		private void SetMarkers() 
		{
			byte[] bytes = derBytes;
			
			int i=0;
		
			byte b;

			#region TAG OCTETS
			b = bytes[i];
			if ((b & 0x1f) == 0x1f) 
			{	
				/// High Tag number form;
				do 
				{
					b = bytes[++i];
				} while (DERMethods.LongForm(b)); 
			}
			#endregion

			#region LENGTH OCTETS
			i++; 
			lenStart = i;
			b = bytes[i];

			if (DERMethods.LongForm(b)) 
			{  
				int lenlen = (int)(b & 0x7f);

				if (lenlen > 4) 
				{
					throw new Exception("Preveliki LENGTH BER koda!");
				}

				valStart = lenStart + lenlen + 1;
			}
			else
			{  
				valStart = lenStart + 1;
			}

			#endregion
		}


		private byte[] GetTagOctets() 
		{
			/* why would anybody need that? */
			byte[] retBytes = new byte[lenStart];

			Array.Copy(derBytes, 0, retBytes, 0, retBytes.Length);

			return retBytes;
		}

		private byte[] GetLengthOctets() 
		{
			byte[] retBytes = new byte[valStart - lenStart];

			Array.Copy(derBytes, lenStart, retBytes, 0, retBytes.Length);

			return retBytes;
		}

		public byte[] GetValueOctets() 
		{
			/* this could be needed.. */
			int start = DERMethods.FindStartOfValueOctets(0, derBytes);
			int lenOfValueOctets = derBytes.Length - start;
			byte[] returnArray = new byte[lenOfValueOctets];

			Array.Copy(derBytes, start, returnArray, 0, lenOfValueOctets);

			/* ?
			 * derBytes = null;
			 */

			return returnArray;
		}

		// find usage??
		public byte[] GetAllOctets() 
		{
			return derBytes;
		}


		public bool Match(ASN1TAG inputTag) 
		{
			/* probaj iz Tag-a kreirati "povrsinske" byte-ove i usporedi ih sa
			 * tag oktetima. */

			if (inputTag.IsMultiTag()) 
			{
				ASN1TAG[] tags = (inputTag as MultiASN1TAG).GetAllTags();

				foreach(ASN1TAG tag in tags) 
				{
					if (this.Match(tag)) 
					{
						return true;
					}
				}
				return false;
			}
			else 
			{
				SimpleASN1TAG simpleTag = inputTag.ExtractSimpleOuterTag();
				
				return this.MatchWithSimpleTag(simpleTag);
			}
		}

		protected bool IsConstructed() 
		{
			//?
			return false;
		}
		
		private bool MatchWithSimpleTag(SimpleASN1TAG simpleTag) 
		{
			byte[] tmpTagOctets = DERMethods.MakeTagOctets(simpleTag);

			for(int i=0; i<this.lenStart; i++) 
			{
				if (tmpTagOctets[i]!=this.derBytes[i]) { 
					return false;
				}
			}
			return true;
		}

		public int Length 
		{
			get 
			{
				return derBytes.Length;
			}
		}

		
		public SimpleDER[] Unwrap()
		{
					
			ArrayList DERList = new ArrayList();
			if (true) //TODO: test za 'constructed'
			{
				int bytesProcessed = DERMethods.FindStartOfValueOctets(0, derBytes);

				int totalLen = this.Length;
				int lastIndex;
				
				while ( bytesProcessed < totalLen ) 
				{
					lastIndex = bytesProcessed;

					int len = DERMethods.ScanBERLength(bytesProcessed, derBytes);
					 
					bytesProcessed += len;
				
					byte[] tmpByteArray = new byte[len];
					Array.Copy(derBytes, bytesProcessed, tmpByteArray,0, len);
				
					DERList.Add(new SimpleDER(tmpByteArray));
				}

				return (DERList.ToArray(this.GetType()) as SimpleDER[]);

			}
			else 
			{
				// not constructed.. unwrap returns what?
			}
		}
	}
}