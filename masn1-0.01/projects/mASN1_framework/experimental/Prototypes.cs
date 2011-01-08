namespace mASN1.framework.experimental 
{
	abstract public class mASN1CommonDenominator:ASN1Object 
	{
		public override BEREncoding asDER()
		{
			// TODO: baci ovo van
			throw new System.NotImplementedException();
		}

		abstract public void acceptVisitor(Visitor v);

		public void TAG_IMPLICITLY(int num) 
		{
			this.TAG = new IMPLICITTag(num, this.TAG);
		}

		public void TAG_EXPLICITLY(int num) 
		{
			this.TAG = new EXPLICITTag(num, this.TAG);
		}

	}

	
	abstract public class ExperimentalObject:mASN1CommonDenominator 
	{
		
	
	}
	

	abstract public class ExperimentalType:mASN1CommonDenominator
	{
		public abstract ExperimentalObject Instance();		
	}

	
	
//	//abstract public class ExperimentalType:mASN1CommonDenominator 
//	{
//	}

	#region FUNDAMENTAL CLASSES for hierarchy of instances
	
	public class BOOLEAN:ExperimentalObject 
	{
		public bool boolValue;

		public BOOLEAN() 
		{
			TAG = TagTable.BOOLEAN;
		}

		public override void acceptVisitor(Visitor v)	{v.Visit_BOOLEAN(this);}
	}


	
	
	public class NULL:ExperimentalObject 
	{
		public NULL() 
		{
			TAG = TagTable.NULL;
		}

		public override void acceptVisitor(Visitor v)
		{
			v.Visit_NULL(this);
		}

	}

	
	public class INTEGER:ExperimentalObject 
	{
		public override void acceptVisitor(Visitor v)	{v.Visit_INTEGER(this);}

	}




	abstract public class SEQUENCE:ExperimentalObject 
	{
		public ExperimentalObject[]    Components;
		public ComponentTypeInfo[] TypeInfos;

		public SEQUENCE() 
		{
			this.TAG = TagTable.SEQUENCE;
		}

		public override void acceptVisitor(Visitor v)
		{
			v.Visit_SEQUENCE(this);
		}
	}
	

	#endregion

	#region FUNDAMENTAL CLASSES for hierarchy of types

	abstract public class SEQUENCE_type:ExperimentalType 
	{
		public SEQUENCE_type() 
		{
			this.TAG = TagTable.SEQUENCE;
		}

		public override void acceptVisitor(Visitor v)
		{
			v.Visit_SEQUENCE(this);
		}
	}
	
	
	public class BOOLEAN_type:ExperimentalType 
	{
		public BOOLEAN_type() 
		{
			this.TAG = TagTable.BOOLEAN;
		}

		public override void acceptVisitor(Visitor v)
		{
			v.Visit_BOOLEAN(this);
		}

		public override ExperimentalObject Instance()
		{
			return new BOOLEAN();
		}
	}


	
	#endregion
	
	public class e0_BOOLEAN:BOOLEAN 
	{
		public e0_BOOLEAN  () 
		{
			TAG_EXPLICITLY(0);
		}
	}
	
	public class e0_BOOLEAN_type:BOOLEAN_type 
	{
		public e0_BOOLEAN_type() 
		{
			TAG_EXPLICITLY(0);
		}

		public override ExperimentalObject Instance()
		{
			return new e0_BOOLEAN();
		}
	}
	
	
	public class i1_BOOLEAN:BOOLEAN 
	{
		public i1_BOOLEAN () 
		{
			TAG_IMPLICITLY(1);
		}
	}

	public class i1_BOOLEAN_type:BOOLEAN_type 
	{
		public i1_BOOLEAN_type() 
		{
			TAG_IMPLICITLY(1);
		}

		public override ExperimentalObject Instance()
		{
			return new i1_BOOLEAN();
		}
	}

	
	public class test1Sequence:SEQUENCE 
	{
		public BOOLEAN bool_;
		public NULL    null_;

		public test1Sequence()
		{
			this.Components = new ExperimentalObject[2];
        }
	}


	


}