using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Icdr.Conn
{
    public class ModEum
    {
        //枚举字段
        public enum CauseForRecClosing
        {
            normalRelease = 0,
            abnormalRelease = 4,
            volumeLimit = 16,
            timeLimit = 17,
            sGSNChange = 18,
            maxChangeCond = 19
        }

        //枚举字段
        public enum ChangeCondition
        {
            qoSChange = 0,
            tariffTime = 1,
            recordClosure = 2
        }

        //枚举字段
        public enum Diagnostics
        {
            gsm0408Cause=0,	//	[0] INTEGER,
            gsm0902MapErrorValue=1,//.[1] INTEGER,
            ccittQ767Cause=2	,//	[2] INTEGER,
            networkSpecificCause=3,//	[3] ManagementExtension,
            manufacturerSpecificCause=4//		[4] ManagementExtension

        }
        //枚举字段
       public enum  APNSelectionMode
        {
	        mSorNetworkProvidedSubscriptionVerified=0,
	        mSProvidedSubscriptionNotVerified=1,
	        networkProvidedSubscriptionNotVerified=2
        }
       //枚举字段
        public enum  ConsolidationResult 
        {
	        normal=0,             //			(0),
	        notNormal=1,             //			(1),
	        forInterSGSNConsolidation=2,//		(2),
 	        reachLimit=3,            //			(3),
	        onlyOneCDRGenerated=4         //	(4)
        }
        public enum CallEventRecordType
        {
            sgsnPDPRecord = 18,
            ggsnPDPRecord = 19,
            sgsnMMRecord = 20,
            sgsnSMORecord = 21,
            sgsnSMTRecord = 22,
            ggsnPDPRecordwap = 70
        }
        public enum QoSDelay
        {
            delayClass1 = 1,
            delayClass2 = 2,
            delayClass3 = 3,
            delayClass4 = 4
        }

        public enum QoSMeanThroughput
        {
            bestEffort = 0,
            mean100octetPh = 1,
            mean200octetPh = 2,
            mean500octetPh = 3,
            mean1000octetPh = 4,
            mean2000octetPh = 5,
            mean5000octetPh = 6,
            mean10000octetPh = 7,
            mean20000octetPh = 8,
            mean50000octetPh = 9,
            mean100000octetPh = 10,
            mean200000octetPh = 11,
            mean500000octetPh = 12,
            mean1000000octetPh = 13,
            mean2000000octetPh = 14,
            mean5000000octetPh = 15,
            mean10000000octetPh = 16,
            mean20000000octetPh = 17,
            mean50000000octetPh = 18
        }
        public enum QoSPeakThroughput
        {
            unspecified = 0,
            upTo100OctetPs = 1,
            upTo200OctetPs = 2,
            upTo400OctetPs = 3,
            upTo800OctetPs = 4,
            upTo1600OctetPs = 5,
            upTo3200OctetPs = 6,
            upTo6400OctetPs = 7,
            upTo12800OctetPs = 8,
            upTo25600OctetPs = 9
        }
        public enum QoSPrecedence
        {
            unspecified = 0,
            highPriority = 1,
            normalPriority = 2,
            lowPriority = 3
        }
        public enum QoSReliability
        {
            unspecifiedReliability = 0,
            acknowledgedGTP = 1,
            unackGTPAcknowLLC = 2,
            unackGTPLLCAcknowRLC = 3,
            unackGTPLLCRLC = 4,
            unacknowUnprotectedData = 5
        }
    }
}
