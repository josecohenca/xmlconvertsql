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


namespace mASN1_tests.SampleClasses 
{

	using mASN1;


	public class MyIOSet
	{
		public static ASN1InformationObjectSet ioset;
		public static ASN1ObjectIdentifier aux_oid1 = new ASN1ObjectIdentifier(1, 2, 3);
		public static ASN1ObjectIdentifier aux_oid2 = new ASN1ObjectIdentifier(2, 3, 4);

		static MyIOSet ()
		{
			TypeIdentifier ti1 = 
				new TypeIdentifier(aux_oid1, new ASN1Boolean_type());

			TypeIdentifier ti2 = 
				new TypeIdentifier(aux_oid2, ASN1Null.asn1type);

			ioset = new ASN1InformationObjectSet();
			
			ioset.Add(ti1);
			ioset.Add(ti2);
		}
	}

	
	/// <summary>
	/// Prefered - for TYPE IDENTIFIER sets.
	/// </summary>
	public class MyTypeIdSet
	{
		public static TypeIdentifierSet ioset = new TypeIdentifierSet();

		public static ASN1ObjectIdentifier aux_oid1 = new ASN1ObjectIdentifier(1, 2, 3);
		public static ASN1ObjectIdentifier aux_oid2 = new ASN1ObjectIdentifier(2, 3, 4);

		static MyTypeIdSet ()
		{
			ioset.Add(aux_oid1, new ASN1Boolean_type());
			ioset.Add(aux_oid2, ASN1Null.asn1type);
		}
	}

	
	/// <summary>
	/// <para>
	/// This class (inspired with X.509's AlgorithmIdentifier)
	/// will be a model for parametrized types here.
	/// </para>
	/// </summary>
	public class AlgorithmIdentifier:ASN1ParameterizedSequence
	{
		/* moj primjer (mala varijacija)
		 * 
		 * 
		 * AlgorithmIdentifier {ALGORITHM:IOSet} ::= SEQUENCE {
         *    algorithm   ALGORITHM.&id({IOSet}),
         *    parameters  ALGORITHM.&Type({IOSet}{@algorithm})  
	     *		}
		 * 
		 */
 
		public ASN1ObjectIdentifier algorithm;
		public ASN1Object           parameters;
	

		public AlgorithmIdentifier()
		{
			this.lateInit(MyTypeIdSet.ioset);
		}


		public override void RegisterComponents()
		{
			this.algorithm  = NextComponent() as ASN1ObjectIdentifier;
			this.parameters = NextComponent(); // notice the missing cast...
			// since the type cannot be resolved at compile-time this ASN1Object 
			// member has no cast.
		}

		public override void RegisterTypes()
		{
			this.ComponentTypes = new ASN1ComponentTypeInfo[2];
			
			RegUniqueType();
			RegDependentType();

															/* .. and this is what my code used to look like...
		
			ASN1Any_type any = new ASN1Any_type(this.parameterIOSet);
			any.Projection += new ASN1Any_type.ProjectionDelegate(TypeIdentifier.Project_type);
			any._depends_on = UNIQUE;
			
			this.ComponentTypes[1] = new ASN1ComponentTypeInfo(any, ASN1ComponentTypeInfo.OPTIONAL); 
															*/
		}
	}

	public class AlgorithmIdentifier_type: ASN1Sequence_type 
	{
		public override ASN1Object Instance()
		{
			return new AlgorithmIdentifier();
		}


	}





	/*
*   AttributeTypeAndDistinguishedValue ::= SEQUENCE {
*     type                  ATTRIBUTE.&id({SupportedAttributes}),
*     value                 ATTRIBUTE.&Type({SupportedAttributes}{@type}),
*     primaryDistinguished  BOOLEAN DEFAULT TRUE,
*     valuesWithContext
*       SET SIZE (1..MAX) OF
*         SEQUENCE {distingAttrValue
*                     [0]  ATTRIBUTE.&Type({SupportedAttributes}{@type})
*                       OPTIONAL,
*                   contextList       SET SIZE (1..MAX) OF Context} OPTIONAL
*   }
*/

	/// <summary>
	/// A complicated example - based on 
	/// X.501'
	/// AttributeTypeAndDistinguishedValue.
	/// 
	/// Non-important elements are stripped off here.
	/// </summary>
	public class ATADV: ASN1ParameterizedSequence 
	{
	 
		public ASN1ObjectIdentifier type;
		public ASN1Object             value;
		public ValuesWithContext      valuesWithContext;

	
		public ATADV()
		{
			this.lateInit(MyTypeIdSet.ioset);
		}


		public override void RegisterComponents()
		{
			this.type                 = NextComponent() as ASN1ObjectIdentifier;
			this.value                = NextComponent(); // as WHAT?
		    this.valuesWithContext    = NextComponent() as ValuesWithContext;

			/// povezi!!! 
			this.unique_TypeInfo.uoid = this.type;
		}

		public override void RegisterTypes()
		{
			this.ComponentTypes = new ASN1ComponentTypeInfo[3];

			RegUniqueType();
			RegDependentType();
			RegType(new ValuesWithContext_type(unique_TypeInfo), OPTIONAL);
		}
	}


	public class ValuesWithContext:ASN1SetOf
	{
		public ValuesWithContext(UNIQUE_TypeInfo u_ti):base(0,6)
		{
			///this.Max_Elements = 6;
			this.type = new ASN1Any_type(u_ti);
		}
        		
	}


	public class ValuesWithContext_type:ASN1DependentType   
	/* ovdje bi htio da klasa naslijedi i DependentType i SetOf_type... */
	{
		public ValuesWithContext_type(UNIQUE_TypeInfo uti):base(uti)
		{
			this.Tag = (int) ASN1TagNumber.SetTag;
		}

		public override ASN1Object Instance()
		{
			return new ValuesWithContext(this.dependsOnThis);
		}
	}



}