
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
    [ASN1Sequence ( Name = "GGSNPDPRecord", IsSet = true  )]
    public class GGSNPDPRecord : IASN1PreparedElement {
                    
	private CallEventRecordType recordType_ ;
	
        [ASN1Element ( Name = "recordType", IsOptional =  false , HasTag =  true, Tag = 0 , HasDefaultValue =  false )  ]
    
        public CallEventRecordType RecordType
        {
            get { return recordType_; }
            set { recordType_ = value;  }
        }
        
                
          
	private NetworkInitiatedPDPContext networkInitiation_ ;
	
        private bool  networkInitiation_present = false ;
	
        [ASN1Element ( Name = "networkInitiation", IsOptional =  true , HasTag =  true, Tag = 1 , HasDefaultValue =  false )  ]
    
        public NetworkInitiatedPDPContext NetworkInitiation
        {
            get { return networkInitiation_; }
            set { networkInitiation_ = value; networkInitiation_present = true;  }
        }
        
                
          
	private bool anonymousAccessIndicator_ ;
	
        private bool  anonymousAccessIndicator_present = false ;
	[ASN1Boolean( Name = "" )]
    
        [ASN1Element ( Name = "anonymousAccessIndicator", IsOptional =  true , HasTag =  true, Tag = 2 , HasDefaultValue =  false )  ]
    
        public bool AnonymousAccessIndicator
        {
            get { return anonymousAccessIndicator_; }
            set { anonymousAccessIndicator_ = value; anonymousAccessIndicator_present = true;  }
        }
        
                
          
	private IMSI servedIMSI_ ;
	
        [ASN1Element ( Name = "servedIMSI", IsOptional =  false , HasTag =  true, Tag = 3 , HasDefaultValue =  false )  ]
    
        public IMSI ServedIMSI
        {
            get { return servedIMSI_; }
            set { servedIMSI_ = value;  }
        }
        
                
          
	private GSNAddress ggsnAddress_ ;
	
        [ASN1Element ( Name = "ggsnAddress", IsOptional =  false , HasTag =  true, Tag = 4, IsImplicitTag = false  , HasDefaultValue =  false )  ]
    
        public GSNAddress GgsnAddress
        {
            get { return ggsnAddress_; }
            set { ggsnAddress_ = value;  }
        }
        
                
          
	private ChargingID chargingID_ ;
	
        [ASN1Element ( Name = "chargingID", IsOptional =  false , HasTag =  true, Tag = 5 , HasDefaultValue =  false )  ]
    
        public ChargingID ChargingID
        {
            get { return chargingID_; }
            set { chargingID_ = value;  }
        }
        
                
          
	private System.Collections.Generic.ICollection<GSNAddress> sgsnAddress_ ;
	
[ASN1SequenceOf( Name = "", IsSetOf = false  )]

    
        [ASN1Element ( Name = "sgsnAddress", IsOptional =  false , HasTag =  true, Tag = 6, IsImplicitTag = false  , HasDefaultValue =  false )  ]
    
        public System.Collections.Generic.ICollection<GSNAddress> SgsnAddress
        {
            get { return sgsnAddress_; }
            set { sgsnAddress_ = value;  }
        }
        
                
          
	private AccessPointNameNI accessPointNameNI_ ;
	
        [ASN1Element ( Name = "accessPointNameNI", IsOptional =  false , HasTag =  true, Tag = 7 , HasDefaultValue =  false )  ]
    
        public AccessPointNameNI AccessPointNameNI
        {
            get { return accessPointNameNI_; }
            set { accessPointNameNI_ = value;  }
        }
        
                
          
	private PDPType pdpType_ ;
	
        [ASN1Element ( Name = "pdpType", IsOptional =  false , HasTag =  true, Tag = 8 , HasDefaultValue =  false )  ]
    
        public PDPType PdpType
        {
            get { return pdpType_; }
            set { pdpType_ = value;  }
        }
        
                
          
	private PDPAddress servedPDPAddress_ ;
	
        private bool  servedPDPAddress_present = false ;
	
        [ASN1Element ( Name = "servedPDPAddress", IsOptional =  true , HasTag =  true, Tag = 9, IsImplicitTag = false  , HasDefaultValue =  false )  ]
    
        public PDPAddress ServedPDPAddress
        {
            get { return servedPDPAddress_; }
            set { servedPDPAddress_ = value; servedPDPAddress_present = true;  }
        }
        
                
          
	private System.Collections.Generic.ICollection<PDPAddress> remotePDPAddress_ ;
	
        private bool  remotePDPAddress_present = false ;
	
[ASN1SequenceOf( Name = "", IsSetOf = false  )]

    
        [ASN1Element ( Name = "remotePDPAddress", IsOptional =  true , HasTag =  true, Tag = 10, IsImplicitTag = false  , HasDefaultValue =  false )  ]
    
        public System.Collections.Generic.ICollection<PDPAddress> RemotePDPAddress
        {
            get { return remotePDPAddress_; }
            set { remotePDPAddress_ = value; remotePDPAddress_present = true;  }
        }
        
                
          
	private DynamicAddressFlag dynamicAddressFlag_ ;
	
        private bool  dynamicAddressFlag_present = false ;
	
        [ASN1Element ( Name = "dynamicAddressFlag", IsOptional =  true , HasTag =  true, Tag = 11 , HasDefaultValue =  false )  ]
    
        public DynamicAddressFlag DynamicAddressFlag
        {
            get { return dynamicAddressFlag_; }
            set { dynamicAddressFlag_ = value; dynamicAddressFlag_present = true;  }
        }
        
                
          
	private System.Collections.Generic.ICollection<ChangeOfCharCondition> listOfTrafficVolumes_ ;
	
[ASN1SequenceOf( Name = "", IsSetOf = false  )]

    
        [ASN1Element ( Name = "listOfTrafficVolumes", IsOptional =  false , HasTag =  true, Tag = 12 , HasDefaultValue =  false )  ]
    
        public System.Collections.Generic.ICollection<ChangeOfCharCondition> ListOfTrafficVolumes
        {
            get { return listOfTrafficVolumes_; }
            set { listOfTrafficVolumes_ = value;  }
        }
        
                
          
	private TimeStamp recordOpeningTime_ ;
	
        [ASN1Element ( Name = "recordOpeningTime", IsOptional =  false , HasTag =  true, Tag = 13 , HasDefaultValue =  false )  ]
    
        public TimeStamp RecordOpeningTime
        {
            get { return recordOpeningTime_; }
            set { recordOpeningTime_ = value;  }
        }
        
                
          
	private long duration_ ;
	[ASN1Integer( Name = "" )]
    
        [ASN1Element ( Name = "duration", IsOptional =  false , HasTag =  true, Tag = 14 , HasDefaultValue =  false )  ]
    
        public long Duration
        {
            get { return duration_; }
            set { duration_ = value;  }
        }
        
                
          
	private CauseForRecClosing causeForRecClosing_ ;
	
        [ASN1Element ( Name = "causeForRecClosing", IsOptional =  false , HasTag =  true, Tag = 15 , HasDefaultValue =  false )  ]
    
        public CauseForRecClosing CauseForRecClosing
        {
            get { return causeForRecClosing_; }
            set { causeForRecClosing_ = value;  }
        }
        
                
          
	private RECORDSequenceNumber recordSequenceNumber_ ;
	
        private bool  recordSequenceNumber_present = false ;
	
        [ASN1Element ( Name = "recordSequenceNumber", IsOptional =  true , HasTag =  true, Tag = 17 , HasDefaultValue =  false )  ]
    
        public RECORDSequenceNumber RecordSequenceNumber
        {
            get { return recordSequenceNumber_; }
            set { recordSequenceNumber_ = value; recordSequenceNumber_present = true;  }
        }
        
                
          
	private NodeID nodeID_ ;
	
        private bool  nodeID_present = false ;
	
        [ASN1Element ( Name = "nodeID", IsOptional =  true , HasTag =  true, Tag = 18 , HasDefaultValue =  false )  ]
    
        public NodeID NodeID
        {
            get { return nodeID_; }
            set { nodeID_ = value; nodeID_present = true;  }
        }
        
                
          
	private ManagementExtensions recordExtensions_ ;
	
        private bool  recordExtensions_present = false ;
	
        [ASN1Element ( Name = "recordExtensions", IsOptional =  true , HasTag =  true, Tag = 19 , HasDefaultValue =  false )  ]
    
        public ManagementExtensions RecordExtensions
        {
            get { return recordExtensions_; }
            set { recordExtensions_ = value; recordExtensions_present = true;  }
        }
        
                
          
	private APNSelectionMode apnSelectionMode_ ;
	
        private bool  apnSelectionMode_present = false ;
	
        [ASN1Element ( Name = "apnSelectionMode", IsOptional =  true , HasTag =  true, Tag = 21 , HasDefaultValue =  false )  ]
    
        public APNSelectionMode ApnSelectionMode
        {
            get { return apnSelectionMode_; }
            set { apnSelectionMode_ = value; apnSelectionMode_present = true;  }
        }
        
                
          
	private ListOfTrafficVolumesC listOfTrafficVolumesC_ ;
	
        [ASN1Element ( Name = "listOfTrafficVolumesC", IsOptional =  false , HasTag =  true, Tag = 27 , HasDefaultValue =  false )  ]
    
        public ListOfTrafficVolumesC ListOfTrafficVolumesC
        {
            get { return listOfTrafficVolumesC_; }
            set { listOfTrafficVolumesC_ = value;  }
        }
        
                
          
	private CallDurationC durationC_ ;
	
        [ASN1Element ( Name = "durationC", IsOptional =  false , HasTag =  true, Tag = 28 , HasDefaultValue =  false )  ]
    
        public CallDurationC DurationC
        {
            get { return durationC_; }
            set { durationC_ = value;  }
        }
        
                
          
	private MSISDN servedMSISDN_ ;
	
        private bool  servedMSISDN_present = false ;
	
        [ASN1Element ( Name = "servedMSISDN", IsOptional =  true , HasTag =  true, Tag = 29 , HasDefaultValue =  false )  ]
    
        public MSISDN ServedMSISDN
        {
            get { return servedMSISDN_; }
            set { servedMSISDN_ = value; servedMSISDN_present = true;  }
        }
        
                
          
	private ConsolidationResult consolidationResult_ ;
	
        [ASN1Element ( Name = "consolidationResult", IsOptional =  false , HasTag =  true, Tag = 30 , HasDefaultValue =  false )  ]
    
        public ConsolidationResult ConsolidationResult
        {
            get { return consolidationResult_; }
            set { consolidationResult_ = value;  }
        }
        
                
  
        public bool isNetworkInitiationPresent () {
            return this.networkInitiation_present == true;
        }
        
        public bool isAnonymousAccessIndicatorPresent () {
            return this.anonymousAccessIndicator_present == true;
        }
        
        public bool isServedPDPAddressPresent () {
            return this.servedPDPAddress_present == true;
        }
        
        public bool isRemotePDPAddressPresent () {
            return this.remotePDPAddress_present == true;
        }
        
        public bool isDynamicAddressFlagPresent () {
            return this.dynamicAddressFlag_present == true;
        }
        
        public bool isRecordSequenceNumberPresent () {
            return this.recordSequenceNumber_present == true;
        }
        
        public bool isNodeIDPresent () {
            return this.nodeID_present == true;
        }
        
        public bool isRecordExtensionsPresent () {
            return this.recordExtensions_present == true;
        }
        
        public bool isApnSelectionModePresent () {
            return this.apnSelectionMode_present == true;
        }
        
        public bool isServedMSISDNPresent () {
            return this.servedMSISDN_present == true;
        }
        

            public void initWithDefaults() {
            	
            }


            private static IASN1PreparedElementData preparedData = CoderFactory.getInstance().newPreparedElementData(typeof(GGSNPDPRecord));
            public IASN1PreparedElementData PreparedData {
            	get { return preparedData; }
            }

            
    }
            
}