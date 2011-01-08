using System;

namespace Asn1Processor
{
    /// <summary>
    /// Define ASN.1 tag constants.
    /// </summary>
    /// 
    public class Asn1Tag
    {
        /// <summary>
        /// Tag mask constant value.
        /// </summary>
        public const byte TAG_MASK              = 0x1F;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte BOOLEAN 			    = 0x01;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte INTEGER 			    = 0x02;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte BIT_STRING			= 0x03;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte OCTET_STRING		    = 0x04;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte TAG_NULL			    = 0x05;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte OBJECT_IDENTIFIER	    = 0x06;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte OBJECT_DESCRIPTOR	    = 0x07;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte EXTERNAL			    = 0x08;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte REAL				    = 0x09;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte ENUMERATED			= 0x0a;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte UTF8_STRING			= 0x0c;

        /// <summary>
        /// Relative object identifier.
        /// </summary>
        public const byte RELATIVE_OID          = 0x0d;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte SEQUENCE			    = 0x10;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte SET 				    = 0x11;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte NUMERIC_STRING		= 0x12;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte PRINTABLE_STRING 	    = 0x13;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte T61_STRING			= 0x14;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte VIDEOTEXT_STRING 	    = 0x15;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte IA5_STRING			= 0x16;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte UTC_TIME 			    = 0x17;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte GENERALIZED_TIME 	    = 0x18;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte GRAPHIC_STRING		= 0x19;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte VISIBLE_STRING		= 0x1a;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte GENERAL_STRING		= 0x1b;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte UNIVERSAL_STRING	    = 0x1C;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte BMPSTRING		        = 0x1E;	/* 30: Basic Multilingual Plane/Unicode string */

        /// <summary>
        /// Constructor.
        /// </summary>
        public Asn1Tag()
        {
        }
    };

	/// <summary>
	/// Define ASN.1 tag class constants.
	/// </summary>
	/// 
	public class Asn1TagClasses
    {
        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte CLASS_MASK        = 0xc0;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte UNIVERSAL			= 0x00;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte CONSTRUCTED 		= 0x20;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte APPLICATION 		= 0x40;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte CONTEXT_SPECIFIC	= 0x80;

        /// <summary>
        /// Constant value.
        /// </summary>
        public const byte PRIVATE 			= 0xc0;

        /// <summary>
        /// Constructor.
        /// </summary>
        public Asn1TagClasses()
        {
        }
    };

}
