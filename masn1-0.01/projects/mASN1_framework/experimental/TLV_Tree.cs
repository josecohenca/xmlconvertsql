namespace mASN1.framework.experimental
{
	// OBSOLETE: Ocu ovo van. Nova fora je TagValueNode


	using mASN1;
	using System.Collections;

	public interface IReturnTLV 
	{
		ArrayList ReturnTLV();
	}

	abstract public class TLVNode:IReturnTLV 
	{
		public    SimpleASN1TAG TAG;
		protected ArrayList     Children;

		protected int innerLength;
		protected int outerLength;

		protected TLVNode() {}

		protected TLVNode(SimpleASN1TAG tag, ArrayList children) 
		{
			TAG = tag;
			Children = children;
		}

		/// <summary>
		/// Forms Tag, Length and Value octets on the node.
		/// </summary>
		/// <returns></returns>
		public abstract ArrayList ReturnTLV();

		public static TLVNode Create(ASN1TAG tag, byte[] octets) 
		{
			// UNDONE
			// innerLength
			int tmpLen=octets.Length;

			return null;
		}

	
		public static TLVNode Create(ASN1TAG tag, ArrayList children) 
		{
			// innerLength
			int tmpLen=0;
			foreach(TLVNode t in children) 
			{
				tmpLen += t.outerLength;
			}



			//UNDONE
			return null;
	
		}
	}

	
	public class TLVValueNode:TLVNode 
	{
			
		private byte[] _ValueOctets;

	
		public TLVValueNode(SimpleASN1TAG tag, byte[] valueOctets) 
		{
			TAG = tag;
			_ValueOctets = valueOctets;
			innerLength = valueOctets.Length;
		}

		public override ArrayList ReturnTLV() 
		{
			byte[] T = DERMethods.MakeTagOctets(TAG);
			byte[] L;
			ICollection V = _ValueOctets;
			
			L = DERMethods.MakeLengthOctets(V);

			ArrayList returnList = new ArrayList();
			
			returnList.AddRange(T);
			returnList.AddRange(L);
			returnList.AddRange(V);

			return returnList;
		}
	}

	
	public class MiddleDERNode:TLVNode 
	{
		public MiddleDERNode(SimpleASN1TAG tag, ArrayList children):base(tag, children) {}

		public override ArrayList ReturnTLV() 
		{			
			int valLength = 0;

			byte[] T = DERMethods.MakeTagOctets(TAG);
			byte[] L;
			ArrayList V=new ArrayList();

			foreach(TLVNode node in this.Children) 
			{
				ArrayList tmp = node.ReturnTLV();
				

                V.AddRange((node.ReturnTLV() as ICollection));                                
			}

			L = DERMethods.MakeLengthOctets(V);

			ArrayList returnList = V;
			returnList.InsertRange(0, L);
			returnList.InsertRange(0, T);

			return returnList;
		}
	}

	public class TLVInfo 
	{
		public byte[] T;
		public byte[] L;

		public int innerLen;
		public int outerLen;


	}


	
}