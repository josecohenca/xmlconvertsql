namespace mASN1.framework.experimental  
{
	public abstract class Visitor 
	{
		abstract public void Visit_NULL    (mASN1CommonDenominator ob);
		abstract public void Visit_BOOLEAN (mASN1CommonDenominator ob);
		abstract public void Visit_INTEGER (mASN1CommonDenominator ob);

		//abstract public void Visit_CHOICE  (mASN1CommonDenominator ob);
		abstract public void Visit_SEQUENCE(mASN1CommonDenominator ob);


		abstract public void Visit_ANY(mASN1CommonDenominator ob);
		

		public virtual void Visit(mASN1CommonDenominator ob) 
		{
			if (ob!=null) 
			{
				ob.acceptVisitor(this);
			}
		}
	}

	public abstract class Decoder:Visitor 
	{
	}

	public abstract class Encoder:Visitor 
	{
	}
}


