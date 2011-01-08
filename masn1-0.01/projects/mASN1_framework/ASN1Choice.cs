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



	/// <summary>
	/// Abstract class - used as a base class for CHOICE
	/// structures.
	/// </summary>
	abstract public class ASN1Choice : ASN1Object 
	{
        /* naslijedjene klase trebaju dodati alternative u popis */

		public ASN1Type[] Alternatives;
		

		private ASN1Object chosen;
		public ASN1Object Chosen 
		{
			get 
			{
				return this.chosen;
			}
		}

		public ASN1Choice() 
		{
			this.RegisterTypes();

			/* odavde treba konstruirati TAG? TAG je potreban samo za Type klase */
		}

		abstract public void RegisterTypes();
				

		public void Assign(ASN1Object ob)
		{
			/// mozda ne bi bilo lose da se
			/// prije provjeri da li ovaj tip uopce smije
			/// biti u ovoj CHOICE strukturi... 
			/// 
			/// laters.
			
			this.chosen = ob;
			

			/// ??? 
			///  \/
			
			/// Ovo prespajanje treba napraviti tako da se moze 
			/// testirati MatchType... inace ovaj objekt ostaje 
			/// sa Tag == 0.
			//this.Tag = ob.Tag;

			/*
			 *  this.aTag = new MultiASN1TAG(ob.aTag)
			 * 
			 */
			
		}


		private void DecodeByTagNumber(int tagNum, BEREncoding ulazBER) 
		{
			/// BAAAD!!!
			foreach (ASN1Type tip in this.Alternatives) 
			{
				if (tagNum == tip.Tag) 
				{
					this.chosen = tip.CreateInstance(ulazBER);
					this.Tag = tagNum;
					return;
				}
			}

			throw new Exception("Unrecognized alternative in CHOICE!");
		}

		public override BEREncoding asDER()
		{
			return this.Chosen.asDER();
			// return new BEREncoding(TAG, this.Chosen.asDER());
		}

		public override void fromBER(BEREncoding ulazBER)
		{
			ulazBER.Decode();
			
			int tagNum = BEREncoding.DecodeTagNumber(ulazBER.GetIdOctets());

			DecodeByTagNumber(tagNum, ulazBER);	
		}


		public override string ToString()
		{
			return Chosen.ToString();
		}
	}


	abstract public class ASN1Choice_type:ASN1Type 
	{
		/// jel se tu ima sto posebno reci?
		/// CreateInstance treba prosvrljati
		/// i naci koji tip odgovara BER kodu..., 
		/// a ovo pak se ne zna ovdje.. 
		/// 
		/// Something to refactor!
		/// 

		public ASN1Choice_type() 
		{
			this.SkipMatch = true;
			this.Tags = new ArrayList();
		}

		protected ASN1Choice SubCreateInstance(ASN1Choice choiceObject, BEREncoding ulazBER) 
		{
			ulazBER.Decode();
			int my_tag = BEREncoding.DecodeTagNumber(ulazBER.GetIdOctets());
			
			foreach(ASN1Type tip in choiceObject.Alternatives) 
			{
				if (ASN1Type.MatchTag(my_tag, tip)) 
				{
					choiceObject.Assign(tip.CreateInstance(ulazBER));
					return choiceObject;
				}
			}
			
			/// Dolazak ovdje treba predstavljati
			/// gresku!
			return null;
		}

		public override ASN1Object CreateInstance(BEREncoding ulazBER)
		{
			return this.SubCreateInstance(this.Instance() as ASN1Choice, ulazBER);
		}


		public void SetTags(params int[] tags) 
		{
			foreach(int t in tags) 
			{
				this.Tags.Add(t);
			}
		}
	}





	/// <summary>
	/// Makes descendand constructors obsolete for this kind of
	/// CHOICE types.
	/// </summary>
	abstract public class SetSequenceChoice_type:ASN1Choice_type 
	{
		public SetSequenceChoice_type() 
		{
			this.SkipMatch = false;
			this.Tags.Add((int)ASN1TagNumber.SequenceTag);
			this.Tags.Add((int)ASN1TagNumber.SetTag);
		}
	}
	
		




}
