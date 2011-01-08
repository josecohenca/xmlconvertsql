namespace mASN1
{
	using mASN1.framework.experimental;

	abstract public class TagTable 
	{
		public static  SimpleASN1TAG BOOLEAN				= new SimpleASN1TAG(ASN1TagNumber.BooleanTag);
		public static  SimpleASN1TAG INTEGER				= new SimpleASN1TAG(ASN1TagNumber.IntegerTag);
		public static  SimpleASN1TAG BITSTRING				= new SimpleASN1TAG(ASN1TagNumber.BitStringTag);
		public static  SimpleASN1TAG OCTETSTRING			= new SimpleASN1TAG(ASN1TagNumber.OctetStringTag);

		public static  SimpleASN1TAG NULL					= new SimpleASN1TAG(ASN1TagNumber.NullTag); 
		public static  SimpleASN1TAG OBJECT_IDENTIFIER		= new SimpleASN1TAG(ASN1TagNumber.ObjectIdentifierTag); 
	
		public static  SimpleASN1TAG REAL					= new SimpleASN1TAG(ASN1TagNumber.RealTag); 
	
		public static  SimpleASN1TAG UTF8STRING			    = new SimpleASN1TAG(ASN1TagNumber.UTF8StringTag); 
		public static  SimpleASN1TAG RELATIVE_OID			= new SimpleASN1TAG(ASN1TagNumber.RelativeOID); 

		public static  SimpleASN1TAG SEQUENCE		= new StructuredSimpleASN1TAG(ASN1TagNumber.SequenceTag); 
		public static  SimpleASN1TAG SET			= new StructuredSimpleASN1TAG(ASN1TagNumber.SetTag); 

		
		//public SimpleASN1TAG OBJECT_DESCRIPTOR = new SimpleASN1TAG();
		//public SimpleASN1TAG EXTERNAL          = new SimpleASN1TAG();
		//public SimpleASN1TAG INSTANCE_OF       = new SimpleASN1TAG();
		//public SimpleASN1TAG ENUMERATED  = new SimpleASN1TAG();
		//public SimpleASN1TAG EMBEDED_PDV = new SimpleASN1TAG(); //11

	}

}