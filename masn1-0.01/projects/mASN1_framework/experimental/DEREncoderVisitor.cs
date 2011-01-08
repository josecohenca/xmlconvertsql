namespace mASN1.framework.experimental 
{
	using mASN1;
    
	using System.Collections;
	using System; // za Console.

	public class DEREncoder:Encoder
	{
		private enum state
		{
			MISSING_COMPONENT,
			START_OF_SEQUENCE,
			START_OF_SEQUENCE_OF,
			START_OF_SET
		}

		private Stack stack;

		public DEREncoder() 
		{
			stack = new Stack();
		}

	
		private void Push(object o) 
		{
			if (o!=null) {	stack.Push(o);}
		}

		
		public override void Visit(mASN1CommonDenominator ob) 
		{
			if (ob!=null)
			{
                ob.acceptVisitor(this);				
			}
			else
			{
                this.Push(state.MISSING_COMPONENT);				
			}
		}



		public override void Visit_NULL(mASN1CommonDenominator ob)
		{
			byte[] value_Octets=new byte[]{};
		
			TagValueNode node = TagValueNode.Create(ob.TAG, value_Octets);
			Push(node);
		}


		public override void Visit_BOOLEAN(mASN1CommonDenominator ob)
		{
			byte boolCode = 0x00;
			BOOLEAN b = (ob as BOOLEAN);
			if (b.boolValue) boolCode = 0xff;
			byte[] value_Octets=new byte[]{boolCode};

			TagValueNode node = TagValueNode.Create(b.TAG, value_Octets);
			Push(node);
		}

		public override void Visit_INTEGER(mASN1CommonDenominator ob)
		{
			throw new NotImplementedException();
		}

		public override void Visit_ANY(mASN1CommonDenominator ob)
		{
			/// a tricky one? 
			throw new NotImplementedException();
		}

		public override void Visit_SEQUENCE(mASN1CommonDenominator seq)
		{
			//markiraj pocetak trpanja SEQUENCE komponenti
			Push(state.START_OF_SEQUENCE);

			SEQUENCE s = (seq as SEQUENCE);

			// obradi(potrpaj) children-objekte na stack
			foreach(ExperimentalObject eo in s.Components) 
			{
				/// TODO: Before visiting must check if non-optional are missing.
				Visit(eo);				
			}

			/* sad Pop() until START_OF_SEQUENCE, pazi na obrnuti redoslijed */

			ArrayList TVList = new ArrayList();
			TagValueNode node;
			while( (node=stack.Pop() as TagValueNode)!=null ) 
			{
				TVList.Insert(0, node);
            }


			node = TagValueNode.Create(s.TAG, TVList);

			// TODO: jos bi trebalo markirati da se SEQUENCE kodira "constructed".
			// mozda bi to trebalo tutnuti u SEQUENCE tag i sve zajedno preimenovati
			// u TagInfo. Or something.

			Push(node);			
		}



		// FUJ. Ovo van. Sve te stvari treba potrpati u .Encode().
		public TagValueNode ShowResult() 
		{
			TagValueNode node = stack.Pop() as TagValueNode;
			TagValueNode expandedNode = TagValueNode.Expand(node);

			return expandedNode;
		}

		public void Encode(ExperimentalObject ob)  
		{

			this.Visit(ob);
//			this.Visit(ob);
//
//			TLVNode node = stack.Pop() as TLVNode;
//			ArrayList byteList = node.ReturnTLV();

			
			//TODO:
			//throw new NotImplementedException();
		}
	}

}