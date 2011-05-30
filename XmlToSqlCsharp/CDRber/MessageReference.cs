
//
// This file was generated by the BinaryNotes compiler.
// See http://bnotes.sourceforge.net 
// Any modifications to this file will be lost upon recompilation of the source ASN.1. 
//

using System;
using org.bn.attributes;
using org.bn.attributes.constraints;
using org.bn.coders;
using org.bn.types;
using org.bn;

namespace GPRSber {


    [ASN1PreparedElement]
    [ASN1BoxedType ( Name = "MessageReference") ]
    public class MessageReference: IASN1PreparedElement {
    
            private byte[] val = null;

            [ASN1OctetString( Name = "MessageReference") ]            
            
            public byte[] Value
            {
                get { return val; }
                set { val = value; }
            }            
            
            public MessageReference() {
            }

            public MessageReference(byte[] value) {
                this.Value = value;
            }            
            
            public MessageReference(BitString value) {
                this.Value = value.Value;
            }                        

            public void initWithDefaults()
	    {
	    }


            private static IASN1PreparedElementData preparedData = CoderFactory.getInstance().newPreparedElementData(typeof(MessageReference));
            public IASN1PreparedElementData PreparedData {
            	get { return preparedData; }
            }

    }
            
}