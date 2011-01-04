using System;
using org.bn;
using org.bn.coders;
using org.bn.types;
using csUnit;
//using test.org.bn.coders.test_asn;
//using test.org.bn.utils;
using System.Collections.Generic;
using GPRSber;
//using csUnit;
//using test.org.bn.coders.test_asn;

namespace gprsBer

{
    public class BERCoderTestUtils : CoderTestUtilities
    {
        public  byte[] createDataSeqBytes()
        {
            Console.WriteLine("file input");
            //string file = Console.ReadLine();
            System.IO.FileStream fs = new System.IO.FileStream(@"c:\ggsn.der", System.IO.FileMode.Open);
            byte[] data = new byte[fs.Length];
            return data;
        }
    }
    public abstract class CoderTestUtilities
    {
        public virtual MSISDN  createMSISDN()
        {
            MSISDN msisdn = new MSISDN();
            return msisdn;
        }
    }

    [TestFixture]
    public abstract class DecoderTest
    {
        private CoderTestUtilities coderTestUtils;

        public DecoderTest(string testName, CoderTestUtilities coderTestUtils)
        {
            this.coderTestUtils = coderTestUtils;
        }

        protected abstract IDecoder newDecoder();
                                IDecoder decoder = newDecoder();
            System.IO.MemoryStream stream =
              new System.IO.MemoryStream(coderTestUtils.createMSISDN);
            //CoderTestUtilities. val = decoder.decode<CoderTestUtilities>(stream);
            //Assert.Equals(val, coderTestUtils.);

    }
    public class BERDecoderTest : DecoderTest
    {
        private   CoderFactory coderFactory = new CoderFactory();


        public BERDecoderTest(System.String sTestName)
            : base(sTestName, new BERCoderTestUtils())
        {
        }

        public BERDecoderTest(System.String sTestName, CoderTestUtilities coderTestUtils)
            : base(sTestName, coderTestUtils)
        {
        }


        protected    override IDecoder newDecoder()
        {
            return coderFactory.newDecoder("BER");
        }
    }
    class Program 
    {       
        static void Main(string[] args)
        {

           
          //  DecoderTest test;
          //  test .
          //BERCoderTestUtils u=new BERCoderTestUtils ();
          //BERDecoderTest b = new BERDecoderTest("MSISDN", u);
          //  IDecoder decoder = b.e
           
          //  //System.IO.MemoryStream stream = new System.IO.MemoryStream(data );
          //  TagClasses tag = decoder.decode<TagClasses>(stream);
          //  MSISDN msisdn = decoder.decode<MSISDN>(stream);
          //  Console.WriteLine(tag .ToString () );
          //  Console.ReadKey();

        }
    }
}