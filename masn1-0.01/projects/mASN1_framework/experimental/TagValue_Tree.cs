namespace mASN1.framework.experimental
{
	using mASN1;
	using System.Collections;



	/// <summary>
	/// This class will represent a tree structure that Visit() method 
	/// in DERVisitor creates. Another method (?) xpands this tree by DFSing it into a
	/// TLV tree which can be dumped it to MemoryStream.
	/// </summary>
	public abstract class TagValueNode 
	{
        protected ASN1TAG Tag;
		public ArrayList Nodes;
		protected byte[] ValueOctets;

		protected byte[] LengthOctets;
		protected byte[] TagOctets;

		protected int outerLen;
		protected int innerLen;


		#region Properties
		public byte[] TagOctets_ 
		{
			get 
			{
				return TagOctets;
			}
		}

		public byte[] LengthOctets_ 
		{
			get  
			{
				return LengthOctets;
			}
		}

		public byte[] ValueOctets_ 
		{
			get 
			{
				return ValueOctets;
			}
		}

		#endregion

		protected TagValueNode(ASN1TAG inputTag) 
		{
			Tag = inputTag;
		}

		public abstract bool IsLeaf();

		public static TagValueNode Create(ASN1TAG inputTag, byte[] valueOctets) 
		{
			return new TagValueLeafNode(inputTag, valueOctets);
		}

		public static TagValueNode Create(ASN1TAG inputTag, ArrayList nodes) 
		{
			return new TagValueMiddleNode(inputTag, nodes);
		}


		private static TagValueNode SimplifyTag(TagValueNode node) 
		{
			if (node.Tag.IsExplicit()) 
			{
				EXPLICITTag    exTag = node.Tag as EXPLICITTag;
				SimpleASN1TAG  smTag = exTag.ExtractSimpleOuterTag();

				node.Tag = exTag.underTag;

				TagValueNode tmpNode = SimplifyTag(node);
		
				// node := (novi node)->node...

				ArrayList nodes = new ArrayList();
				nodes.Add(node);
				return Create(smTag, nodes);

			}

			if (node.Tag.IsImplicit()) 
			{
				IMPLICITTag    imTag = node.Tag as IMPLICITTag;
				SimpleASN1TAG  smTag = imTag.ExtractSimpleOuterTag();

				node.Tag = imTag.underTag;
				node = SimplifyTag(node);
				node.Tag = smTag;

				return node;
			}

			return null;
		}

		/// <summary>
		/// <para>
		/// This method traverses TagValue tree and expands all explicit and implicit tags and 
		/// turns them into simple tags.
		/// </para>
		/// <para>
		/// This is preparation before binary dump. There are no length octets - but now 
		/// when all value octets are expanded they can be easily calculated on the fly.</para>
		/// </summary>
		/// <param name="node">a node representing a TagValue tree</param>
		/// <returns>expanded corresponding tree</returns>
		public static TagValueNode Expand(TagValueNode node) 
		{

			if (!node.Tag.IsSimple())  { node = SimplifyTag(node); }

			if (node.IsLeaf()) 
			{
				node.innerLen = node.ValueOctets.Length;
			}
			else 
			{
				int valueLength = 0;
				for(int i=0; i<node.Nodes.Count; i++) 
				{
					node.Nodes[i] = Expand(node.Nodes[i] as TagValueNode);
					valueLength += (node.Nodes[i] as TagValueNode).outerLen;
				}
				node.innerLen = valueLength;
			}
			
			/// FinishUpNode...
			/// begin
			node.LengthOctets = DERMethods.MakeLengthOctets(node.innerLen);
			node.TagOctets    = DERMethods.MakeTagOctets(node.Tag as SimpleASN1TAG);
			node.outerLen = node.innerLen + node.LengthOctets.Length + node.TagOctets.Length;
			/// end 

			return node;
		}
	}

	
	public class TagValueLeafNode:TagValueNode 
	{
		public TagValueLeafNode(ASN1TAG inputTag, byte[] valueOctets):base(inputTag) 
		{
			ValueOctets = valueOctets;
		}

		public override bool IsLeaf()
		{
			return true;
		}

	}

	
	public class TagValueMiddleNode:TagValueNode 
	{
		public TagValueMiddleNode(ASN1TAG inputTag, ArrayList nodes):base(inputTag) 
		{
			Nodes = nodes;
		}

		public override bool IsLeaf()
		{
			return false;
		}

	}



	
}