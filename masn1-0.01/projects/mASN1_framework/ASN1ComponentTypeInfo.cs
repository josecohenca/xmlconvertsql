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
	
	
	#region Definicija ASN1ComponentTypeInfo klase (potrebno za Sequence, Set)
	
	
	/// <summary>
	/// This is an ordered triple which is needed to hold information
	/// about SET's and SEQUENCE's members. 
	/// 
	/// Each component holds a type and possible OPTIONAL qualifier, 
	/// along with possible DEFAULT value.
	/// 
	/// Type component is mandatory, leaving a total of 4 possible 
	/// combinations of OPTIONAL and DEFAULT. Simplest case ofcourse
	/// is when both qualifiers are absent. These 4 cases result in
	/// 4 constructors of the class.
	/// </summary>
	public class ASN1ComponentTypeInfo 
	{

		/// <summary>
		/// <para>
		/// This is the essential member of ASN1ComponentTypeInfo class. Everything else in the class is about 
		/// functionality, or additional information about SEQUENCE's component type.
		/// </para>
		/// 
		/// <para>
		/// For example - whether this component is marked OPTIONAL, or it has a DEFAULT value etc. 
		/// </para>
		/// 
		/// </summary>
		public ASN1Type    Type;
		
		
		/// <summary>
		/// Moram iz ovog napraviti READONLY property
		/// </summary>
		public bool        isOptional;
		
		public ASN1Object  DefaultValue;

	

		public static bool OPTIONAL = true;  ///Ovo bi trebala biti konstanta...

		protected ASN1ComponentTypeInfo() 
		{
		}
	
		/// <summary>
		/// Basic constructor, type w/out qualifiers.
		/// </summary>
		/// <param name="tip">type of the component</param>
		public ASN1ComponentTypeInfo(ASN1Type tip) 
		{
			this.Type = tip;
			this.isOptional = false;
		}

		public ASN1ComponentTypeInfo(ASN1Type tip, bool optional) 
		{
			this.Type = tip;
			this.isOptional = optional;
		}

		public ASN1ComponentTypeInfo(ASN1Type tip, bool optional, ASN1Object defaultValue) 
		{
			this.Type = tip;
			this.isOptional = optional;
			this.DefaultValue = defaultValue;
		}

		public ASN1ComponentTypeInfo(ASN1Type tip, ASN1Object defaultValue)
		{
			this.Type = tip;
			this.isOptional = false;
			this.DefaultValue = defaultValue;
		}

		public ASN1ComponentTypeInfo(bool optional) 
		{
			this.isOptional = optional;
		}


		
		/// <summary>
		/// Delegates .CreateInstance to its .Type component.
		/// </summary>
		/// <param name="ulazBER"></param>
		/// <returns></returns>
		public virtual ASN1Object CreateInstance(BEREncoding ulazBER) 
		{
			return this.Type.CreateInstance(ulazBER);
		}





		public static ASN1ComponentTypeInfo Make(ASN1Type tip)
		{
			return new ASN1ComponentTypeInfo(tip);
		}

		public static ASN1ComponentTypeInfo Make(ASN1Type tip, ASN1Object defaultValue)
		{
			return new ASN1ComponentTypeInfo(tip, defaultValue);
		}

		public static ASN1ComponentTypeInfo Make(ASN1Type tip, bool optional)
		{
			return new ASN1ComponentTypeInfo(tip, optional);
		}

		public static ASN1ComponentTypeInfo Make(ASN1Type tip, bool optional, ASN1Object defaultValue)
		{
			return new ASN1ComponentTypeInfo(tip, optional, defaultValue);
		}


		/// <summary>
		/// This is advanced interface - this is what I would like...
		/// </summary>
		/// <param name="asnseq"></param>
		/// <param name="num"></param>
		/// <param name="tip"></param>
		public static void Make(ASN1Sequence asnseq, int num, ASN1Type tip)
		{
			asnseq.ComponentTypes[num] = ASN1ComponentTypeInfo.Make(tip);
		}
	
	}


	public class UNIQUE_TypeInfo:ASN1ComponentTypeInfo
	{
		public ASN1ObjectIdentifier uoid;
		public ASN1InformationObjectSet ioset;


		public UNIQUE_TypeInfo()
		{
			///this.uoid = new ASN1ObjectIdentifier();
			this.Type = new ASN1ObjectIdentifier_type();
		}

		public UNIQUE_TypeInfo(IParameterizedASN1Type parameterizedASN1Object):this()
		{
			ioset = parameterizedASN1Object.GetIOSet();
		}

		public UNIQUE_TypeInfo(ASN1InformationObjectSet inputIOSet):this()
		{
			this.ioset = inputIOSet;
		}
		
		public override ASN1Object CreateInstance(BEREncoding ulazBER)
		{
			//			ASN1ObjectIdentifier returnObject = new ASN1ObjectIdentifier();
			//			
			//			returnObject.fromBER(ulazBER);
			
			/*   now I have finished parameter. from this point on
			 *   I can set up whatever needed for CTX_DEP_TypeInfo...
			 * 
			 */
		
			this.uoid = new ASN1ObjectIdentifier();
			this.uoid.fromBER(ulazBER);

			///this.

			return this.uoid;
		}

	}


	/// <summary>
	/// <para>
	/// This class should initialize with a reference to an object
	/// that must be marked as UNIQUE in ASN.1 syntax. Here, this translates
	/// into initialization with mandatory UniqueObject. 
	/// </para>
	/// 
	/// <para>
	/// Most of functionality basicaly derives from the base class. Where needed, 
	/// UniqueObject is used.
	/// </para>
	/// </summary>
	public class CTX_DEP_TypeInfo:ASN1ComponentTypeInfo
	{
		/// <summary>
		/// The ONLY constructor for dependent TypeInfos.
		/// </summary>
		/// <param name="uniqueTypeInfo">THE TypeInfo this TypeInfo depends on.</param>
		public CTX_DEP_TypeInfo(UNIQUE_TypeInfo uniqueTypeInfo)
		{
			
//			this.uid = uniqueTypeInfo.uoid; // REMOVE LATER

			ASN1Any_type any = new ASN1Any_type(uniqueTypeInfo);
	
			this.Type=any;
		}

		private CTX_DEP_TypeInfo() 
		{
		}


		/// <summary>
		///  STATIC "factory"-pattern 'Make' methods....
		/// </summary>
		/// <param name="parameterizedType"></param>
		/// <returns></returns>
		public static ASN1ComponentTypeInfo Make(IParameterizedASN1Type parameterizedType)
		{
			UNIQUE_TypeInfo uniqueTypeInfo = parameterizedType.GetUNIQUE_TypeInfo();
			return new CTX_DEP_TypeInfo(uniqueTypeInfo);
		}

		public static ASN1ComponentTypeInfo Make(UNIQUE_TypeInfo uniqueTypeInfo)
		{
			return new CTX_DEP_TypeInfo(uniqueTypeInfo);
		}

		public static ASN1ComponentTypeInfo MakeNice(UNIQUE_TypeInfo uniqueTypeInfo) 
		{
			ASN1ComponentTypeInfo retValue = new CTX_DEP_TypeInfo();
			ASN1NiceAny_type niceAny = new ASN1NiceAny_type(uniqueTypeInfo);
			retValue.Type = niceAny;

			return retValue;
		}
		
	}

	

	
	
	
	#endregion



}