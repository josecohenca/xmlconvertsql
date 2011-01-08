//namespace mASN1.framework.experimental
//{
//	using mASN1;
//	using System.Collections;
//
//	public interface IByteArray 
//	{
//		byte[] ToByteArray();
//
//		byte[][] GetByteArrays();
//	}
// 
//
//	public class ByteArray:IByteArray 
//	{
//		private byte[] bytes;
//		
//
//
//		public ByteArray(byte[] inputBytes) 
//		{
//			this.bytes = inputBytes;
//		}
//
//		#region IByteArray Members
//
//		public byte[] ToByteArray()
//		{
//			// TODO:  Add ByteArray.ToByteArray implementation
//			return bytes;
//		}
//
//		public byte[][] GetByteArrays() 
//		{
//			return new byte[][]{bytes, bytes, bytes};
//		}
//
//		#endregion
//	}
//
//	
//
//	abstract public class DERNode 
//	{
//		protected byte[] T_;
//		protected byte[] L_;
//
//		protected IByteArray V;
//	}
//	
//
//
//	
//
//
//}