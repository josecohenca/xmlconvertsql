namespace mASN1.framework.experimental 
{

	public class ComponentTypeInfo 
	{

		/// <summary>
		/// <para>
		/// This is the essential member of ComponentTypeInfo class. Everything else in the class is about 
		/// functionality, or additional information about SEQUENCE's component type.
		/// </para>
		/// 
		/// <para>
		/// For example - whether this component is marked OPTIONAL, or it has a DEFAULT value etc. 
		/// </para>
		/// 
		/// </summary>
		public ExperimentalType    Type;
		
		
		/// <summary>
		/// Moram iz ovog napraviti READONLY property
		/// </summary>
		public bool        isOptional;
		
		public ExperimentalObject  DefaultValue;

	

		public static bool OPTIONAL = true;  ///Ovo bi trebala biti konstanta...

		protected ComponentTypeInfo() 
		{
		}
	
		/// <summary>
		/// Basic constructor, type w/out qualifiers.
		/// </summary>
		/// <param name="tip">type of the component</param>
		public ComponentTypeInfo(ExperimentalType tip) 
		{
			this.Type = tip;
			this.isOptional = false;
		}

		public ComponentTypeInfo(ExperimentalType tip, bool optional) 
		{
			this.Type = tip;
			this.isOptional = optional;
		}

		public ComponentTypeInfo(ExperimentalType tip, bool optional, ExperimentalObject defaultValue) 
		{
			this.Type = tip;
			this.isOptional = optional;
			this.DefaultValue = defaultValue;
		}

		public ComponentTypeInfo(ExperimentalType tip, ExperimentalObject defaultValue)
		{
			this.Type = tip;
			this.isOptional = false;
			this.DefaultValue = defaultValue;
		}

		public ComponentTypeInfo(bool optional) 
		{
			this.isOptional = optional;
		}





		public static ComponentTypeInfo Make(ExperimentalType tip)
		{
			return new ComponentTypeInfo(tip);
		}

		public static ComponentTypeInfo Make(ExperimentalType tip, ExperimentalObject defaultValue)
		{
			return new ComponentTypeInfo(tip, defaultValue);
		}

		public static ComponentTypeInfo Make(ExperimentalType tip, bool optional)
		{
			return new ComponentTypeInfo(tip, optional);
		}

		public static ComponentTypeInfo Make(ExperimentalType tip, bool optional, ExperimentalObject defaultValue)
		{
			return new ComponentTypeInfo(tip, optional, defaultValue);
		}


		/// <summary>
		/// This is advanced interface - this is what I would like...
		/// </summary>
		/// <param name="asnseq"></param>
		/// <param name="num"></param>
		/// <param name="tip"></param>
		public static void Make(SEQUENCE asnseq, int num, ExperimentalType tip)
		{
			asnseq.TypeInfos[num] = ComponentTypeInfo.Make(tip);
		}
	
	}


//	public class UNIQUE_TypeInfo:ComponentTypeInfo
//	{
//		public ExperimentalObjectIdentifier uoid;
//		public ASN1InformationObjectSet ioset;
//
//
//		public UNIQUE_TypeInfo()
//		{
//			///this.uoid = new ExperimentalObjectIdentifier();
//			this.Type = new ExperimentalObjectIdentifier_type();
//		}
//
//		public UNIQUE_TypeInfo(IParameterizedExperimentalType parameterizedExperimentalObject):this()
//		{
//			ioset = parameterizedExperimentalObject.GetIOSet();
//		}
//
//		public UNIQUE_TypeInfo(ASN1InformationObjectSet inputIOSet):this()
//		{
//			this.ioset = inputIOSet;
//		}
//		
//		public override ExperimentalObject CreateInstance(BEREncoding ulazBER)
//		{
//			//			ExperimentalObjectIdentifier returnObject = new ExperimentalObjectIdentifier();
//			//			
//			//			returnObject.fromBER(ulazBER);
//			
//			/*   now I have finished parameter. from this point on
//				 *   I can set up whatever needed for CTX_DEP_TypeInfo...
//				 * 
//				 */
//		
//			this.uoid = new ExperimentalObjectIdentifier();
//			this.uoid.fromBER(ulazBER);
//
//			///this.
//
//			return this.uoid;
//		}
//
//	}
//
//
//	/// <summary>
//	/// <para>
//	/// This class should initialize with a reference to an object
//	/// that must be marked as UNIQUE in ASN.1 syntax. Here, this translates
//	/// into initialization with mandatory UniqueObject. 
//	/// </para>
//	/// 
//	/// <para>
//	/// Most of functionality basicaly derives from the base class. Where needed, 
//	/// UniqueObject is used.
//	/// </para>
//	/// </summary>
//	public class CTX_DEP_TypeInfo:ComponentTypeInfo
//	{
//		/// <summary>
//		/// The ONLY constructor for dependent TypeInfos.
//		/// </summary>
//		/// <param name="uniqueTypeInfo">THE TypeInfo this TypeInfo depends on.</param>
//		public CTX_DEP_TypeInfo(UNIQUE_TypeInfo uniqueTypeInfo)
//		{
//			
//			//			this.uid = uniqueTypeInfo.uoid; // REMOVE LATER
//
//			ASN1Any_type any = new ASN1Any_type(uniqueTypeInfo);
//	
//			this.Type=any;
//		}
//
//		private CTX_DEP_TypeInfo() 
//		{
//		}
//
//
//		/// <summary>
//		///  STATIC "factory"-pattern 'Make' methods....
//		/// </summary>
//		/// <param name="parameterizedType"></param>
//		/// <returns></returns>
//		public static ComponentTypeInfo Make(IParameterizedExperimentalType parameterizedType)
//		{
//			UNIQUE_TypeInfo uniqueTypeInfo = parameterizedType.GetUNIQUE_TypeInfo();
//			return new CTX_DEP_TypeInfo(uniqueTypeInfo);
//		}
//
//		public static ComponentTypeInfo Make(UNIQUE_TypeInfo uniqueTypeInfo)
//		{
//			return new CTX_DEP_TypeInfo(uniqueTypeInfo);
//		}
//
//		public static ComponentTypeInfo MakeNice(UNIQUE_TypeInfo uniqueTypeInfo) 
//		{
//			ComponentTypeInfo retValue = new CTX_DEP_TypeInfo();
//			ASN1NiceAny_type niceAny = new ASN1NiceAny_type(uniqueTypeInfo);
//			retValue.Type = niceAny;
//
//			return retValue;
//		}
//		
//	}
}