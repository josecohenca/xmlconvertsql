namespace mASN1.framework.experimental 
{
	using mASN1;
    
	using System.Collections;
	using System; // za Console.WriteLine

	using System.IO;


	public class DERWriter
	{
		private Stream strm_;
		private BinaryWriter binWriter;


		public DERWriter(Stream strm) 
		{
			strm_=strm;
			binWriter = new BinaryWriter(strm_);
		}

		public Stream BaseStream 
		{
			get 
			{
				return strm_;
			}
		}

		public void Write(TagValueNode node) 
		{
			binWriter.Write(node.TagOctets_);
			binWriter.Write(node.LengthOctets_);

			if (node.IsLeaf()) 
			{
				binWriter.Write(node.ValueOctets_);
			}
			else 
			{
				foreach (TagValueNode nod in node.Nodes) 
				{
					Write(nod);
				}
			}
		}
		

		public void Close()
		{
			binWriter.Flush();
			binWriter.Close();
			strm_.Close();
		}

		
	
	}

	public class DERDumper 
	{
		private BinaryWriter binWriter;

		public DERDumper(Stream strm) 
		{ 
			binWriter = new BinaryWriter(strm);
		}


		public void DumpNodeToStream(TagValueNode node) 
		{
			binWriter.Write(node.TagOctets_);
			binWriter.Write(node.LengthOctets_);

			if (node.IsLeaf()) 
			{
				binWriter.Write(node.ValueOctets_);
			}
			else 
			{
				foreach (TagValueNode nod in node.Nodes) 
				{
					DumpNodeToStream(nod);
				}
			}
		}

		//public 
	}


}
