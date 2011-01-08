/* Do not modify this file.                                                   */
/* It is created automatically by the ASN.1 to Wireshark dissector compiler   */
/* packet-CmccGPRSber130-fn.c                                                 */
/* F:/bin/trunk-1.4/tools/asn2wrs.py G:/xmlconvertsql/ICDR_EDGE/ICDR_EDGE/bin/Debug/asn1/CmccGPRSber130.asn1 */


/* --- Module CmccGPRSber130 --- --- ---                                      */


static const value_string CmccGPRSber130_CallEventRecordType_vals[] = {
  {  18, "sgsnPDPRecord" },
  {  19, "ggsnPDPRecord" },
  {  20, "sgsnMMRecord" },
  {  21, "sgsnSMORecord" },
  {  22, "sgsnSMTRecord" },
  { 0, NULL }
};

static guint32 CmccGPRSber130_CallEventRecordType_value_map[5+0] = {18, 19, 20, 21, 22};

static int
dissect_CmccGPRSber130_CallEventRecordType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
                                     5, NULL, FALSE, 0, CmccGPRSber130_CallEventRecordType_value_map);

  return offset;
}



static int
dissect_CmccGPRSber130_NetworkInitiatedPDPContext(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_boolean(tvb, offset, actx, tree, hf_index, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_BOOLEAN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_boolean(tvb, offset, actx, tree, hf_index, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_IMSI(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       NO_BOUND, NO_BOUND, FALSE, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_IMEI(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       8, 8, FALSE, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_OCTET_STRING_SIZE_4(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       4, 4, FALSE, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_OCTET_STRING_SIZE_16(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       16, 16, FALSE, NULL);

  return offset;
}


static const value_string CmccGPRSber130_IPBinaryAddress_vals[] = {
  {   0, "iPBinV4Address" },
  {   1, "iPBinV6Address" },
  { 0, NULL }
};

static const per_choice_t CmccGPRSber130_IPBinaryAddress_choice[] = {
  {   0, &hf_CmccGPRSber130_iPBinV4Address, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_OCTET_STRING_SIZE_4 },
  {   1, &hf_CmccGPRSber130_iPBinV6Address, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_OCTET_STRING_SIZE_16 },
  { 0, NULL, 0, NULL }
};

static int
dissect_CmccGPRSber130_IPBinaryAddress(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
                                 ett_CmccGPRSber130_IPBinaryAddress, CmccGPRSber130_IPBinaryAddress_choice,
                                 NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_OCTET_STRING_SIZE_7_15(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       7, 15, FALSE, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_OCTET_STRING_SIZE_15_45(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       15, 45, FALSE, NULL);

  return offset;
}


static const value_string CmccGPRSber130_IPTextRepresentedAddress_vals[] = {
  {   2, "iPTextV4Address" },
  {   3, "iPTextV6Address" },
  { 0, NULL }
};

static const per_choice_t CmccGPRSber130_IPTextRepresentedAddress_choice[] = {
  {   2, &hf_CmccGPRSber130_iPTextV4Address, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_OCTET_STRING_SIZE_7_15 },
  {   3, &hf_CmccGPRSber130_iPTextV6Address, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_OCTET_STRING_SIZE_15_45 },
  { 0, NULL, 0, NULL }
};

static int
dissect_CmccGPRSber130_IPTextRepresentedAddress(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
                                 ett_CmccGPRSber130_IPTextRepresentedAddress, CmccGPRSber130_IPTextRepresentedAddress_choice,
                                 NULL);

  return offset;
}


static const value_string CmccGPRSber130_IPAddress_vals[] = {
  {   0, "iPBinaryAddress" },
  {   1, "iPTextRepresentedAddress" },
  { 0, NULL }
};

static const per_choice_t CmccGPRSber130_IPAddress_choice[] = {
  {   0, &hf_CmccGPRSber130_iPBinaryAddress, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_IPBinaryAddress },
  {   1, &hf_CmccGPRSber130_iPTextRepresentedAddress, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_IPTextRepresentedAddress },
  { 0, NULL, 0, NULL }
};

static int
dissect_CmccGPRSber130_IPAddress(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
                                 ett_CmccGPRSber130_IPAddress, CmccGPRSber130_IPAddress_choice,
                                 NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_GSNAddress(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_CmccGPRSber130_IPAddress(tvb, offset, actx, tree, hf_index);

  return offset;
}


static const per_sequence_t CmccGPRSber130_SEQUENCE_OF_GSNAddress_sequence_of[1] = {
  { &hf_CmccGPRSber130_sgsnAddress_item, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_GSNAddress },
};

static int
dissect_CmccGPRSber130_SEQUENCE_OF_GSNAddress(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_sequence_of(tvb, offset, actx, tree, hf_index,
                                      ett_CmccGPRSber130_SEQUENCE_OF_GSNAddress, CmccGPRSber130_SEQUENCE_OF_GSNAddress_sequence_of);

  return offset;
}



static int
dissect_CmccGPRSber130_MSNetworkCapability(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       1, 1, FALSE, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_RoutingAreaCode(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       1, 1, FALSE, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_LocationAreaCode(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       2, 2, FALSE, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_CellId(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       2, 2, FALSE, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_ChargingID(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
                                                            0U, 4294967295U, NULL, FALSE);

  return offset;
}



static int
dissect_CmccGPRSber130_AccessPointNameNI(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       1, 63, FALSE, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_PDPType(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       2, 2, FALSE, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_AddressString(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       1, 20, FALSE, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_ETSIAddress(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_CmccGPRSber130_AddressString(tvb, offset, actx, tree, hf_index);

  return offset;
}


static const value_string CmccGPRSber130_PDPAddress_vals[] = {
  {   0, "iPAddress" },
  {   1, "eTSIAddress" },
  { 0, NULL }
};

static const per_choice_t CmccGPRSber130_PDPAddress_choice[] = {
  {   0, &hf_CmccGPRSber130_iPAddress, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_IPAddress },
  {   1, &hf_CmccGPRSber130_eTSIAddress, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_ETSIAddress },
  { 0, NULL, 0, NULL }
};

static int
dissect_CmccGPRSber130_PDPAddress(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
                                 ett_CmccGPRSber130_PDPAddress, CmccGPRSber130_PDPAddress_choice,
                                 NULL);

  return offset;
}


static const value_string CmccGPRSber130_QoSReliability_vals[] = {
  {   0, "unspecifiedReliability" },
  {   1, "acknowledgedGTP" },
  {   2, "unackGTPAcknowLLC" },
  {   3, "unackGTPLLCAcknowRLC" },
  {   4, "unackGTPLLCRLC" },
  {   5, "unacknowUnprotectedData" },
  { 0, NULL }
};


static int
dissect_CmccGPRSber130_QoSReliability(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
                                     6, NULL, FALSE, 0, NULL);

  return offset;
}


static const value_string CmccGPRSber130_QoSDelay_vals[] = {
  {   1, "delayClass1" },
  {   2, "delayClass2" },
  {   3, "delayClass3" },
  {   4, "delayClass4" },
  { 0, NULL }
};

static guint32 CmccGPRSber130_QoSDelay_value_map[4+0] = {1, 2, 3, 4};

static int
dissect_CmccGPRSber130_QoSDelay(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
                                     4, NULL, FALSE, 0, CmccGPRSber130_QoSDelay_value_map);

  return offset;
}


static const value_string CmccGPRSber130_QoSPrecedence_vals[] = {
  {   0, "unspecified" },
  {   1, "highPriority" },
  {   2, "normalPriority" },
  {   3, "lowPriority" },
  { 0, NULL }
};


static int
dissect_CmccGPRSber130_QoSPrecedence(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
                                     4, NULL, FALSE, 0, NULL);

  return offset;
}


static const value_string CmccGPRSber130_QoSPeakThroughput_vals[] = {
  {   0, "unspecified" },
  {   1, "upTo100OctetPs" },
  {   2, "upTo200OctetPs" },
  {   3, "upTo400OctetPs" },
  {   4, "upTo800OctetPs" },
  {   5, "upTo1600OctetPs" },
  {   6, "upTo3200OctetPs" },
  {   7, "upTo6400OctetPs" },
  {   8, "upTo12800OctetPs" },
  {   9, "upTo25600OctetPs" },
  { 0, NULL }
};


static int
dissect_CmccGPRSber130_QoSPeakThroughput(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
                                     10, NULL, FALSE, 0, NULL);

  return offset;
}


static const value_string CmccGPRSber130_QoSMeanThroughput_vals[] = {
  {   0, "bestEffort" },
  {   1, "mean100octetPh" },
  {   2, "mean200octetPh" },
  {   3, "mean500octetPh" },
  {   4, "mean1000octetPh" },
  {   5, "mean2000octetPh" },
  {   6, "mean5000octetPh" },
  {   7, "mean10000octetPh" },
  {   8, "mean20000octetPh" },
  {   9, "mean50000octetPh" },
  {  10, "mean100000octetPh" },
  {  11, "mean200000octetPh" },
  {  12, "mean500000octetPh" },
  {  13, "mean1000000octetPh" },
  {  14, "mean2000000octetPh" },
  {  15, "mean5000000octetPh" },
  {  16, "mean10000000octetPh" },
  {  17, "mean20000000octetPh" },
  {  18, "mean50000000octetPh" },
  { 0, NULL }
};


static int
dissect_CmccGPRSber130_QoSMeanThroughput(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
                                     19, NULL, FALSE, 0, NULL);

  return offset;
}


static const per_sequence_t CmccGPRSber130_QoSInformation_sequence[] = {
  { &hf_CmccGPRSber130_reliability, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_QoSReliability },
  { &hf_CmccGPRSber130_delay, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_QoSDelay },
  { &hf_CmccGPRSber130_precedence, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_QoSPrecedence },
  { &hf_CmccGPRSber130_peakThroughput, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_QoSPeakThroughput },
  { &hf_CmccGPRSber130_meanThroughput, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_QoSMeanThroughput },
  { NULL, 0, 0, NULL }
};

static int
dissect_CmccGPRSber130_QoSInformation(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
                                   ett_CmccGPRSber130_QoSInformation, CmccGPRSber130_QoSInformation_sequence);

  return offset;
}



static int
dissect_CmccGPRSber130_DataVolumeGPRS(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_integer(tvb, offset, actx, tree, hf_index, NULL);

  return offset;
}


static const value_string CmccGPRSber130_ChangeCondition_vals[] = {
  {   0, "qoSChange" },
  {   1, "tariffTime" },
  {   2, "recordClosure" },
  { 0, NULL }
};


static int
dissect_CmccGPRSber130_ChangeCondition(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
                                     3, NULL, FALSE, 0, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_TimeStamp(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       9, 9, FALSE, NULL);

  return offset;
}


static const per_sequence_t CmccGPRSber130_ChangeOfCharCondition_sequence[] = {
  { &hf_CmccGPRSber130_qosRequested, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_QoSInformation },
  { &hf_CmccGPRSber130_qosNegotiated, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_QoSInformation },
  { &hf_CmccGPRSber130_dataVolumeGPRSUplink, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_DataVolumeGPRS },
  { &hf_CmccGPRSber130_dataVolumeGPRSDownlink, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_DataVolumeGPRS },
  { &hf_CmccGPRSber130_changeCondition, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_ChangeCondition },
  { &hf_CmccGPRSber130_changeTime, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_TimeStamp },
  { NULL, 0, 0, NULL }
};

static int
dissect_CmccGPRSber130_ChangeOfCharCondition(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
                                   ett_CmccGPRSber130_ChangeOfCharCondition, CmccGPRSber130_ChangeOfCharCondition_sequence);

  return offset;
}


static const per_sequence_t CmccGPRSber130_SEQUENCE_OF_ChangeOfCharCondition_sequence_of[1] = {
  { &hf_CmccGPRSber130_listOfTrafficVolumes_item, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_ChangeOfCharCondition },
};

static int
dissect_CmccGPRSber130_SEQUENCE_OF_ChangeOfCharCondition(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_sequence_of(tvb, offset, actx, tree, hf_index,
                                      ett_CmccGPRSber130_SEQUENCE_OF_ChangeOfCharCondition, CmccGPRSber130_SEQUENCE_OF_ChangeOfCharCondition_sequence_of);

  return offset;
}



static int
dissect_CmccGPRSber130_CallDuration(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_integer(tvb, offset, actx, tree, hf_index, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_SGSNChange(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_boolean(tvb, offset, actx, tree, hf_index, NULL);

  return offset;
}


static const value_string CmccGPRSber130_CauseForRecClosing_vals[] = {
  {   0, "normalRelease" },
  {   4, "abnormalRelease" },
  {  16, "volumeLimit" },
  {  17, "timeLimit" },
  {  18, "sGSNChange" },
  {  19, "maxChangeCond" },
  { 0, NULL }
};

static guint32 CmccGPRSber130_CauseForRecClosing_value_map[6+0] = {0, 4, 16, 17, 18, 19};

static int
dissect_CmccGPRSber130_CauseForRecClosing(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
                                     6, NULL, FALSE, 0, CmccGPRSber130_CauseForRecClosing_value_map);

  return offset;
}



static int
dissect_CmccGPRSber130_RecordNumber(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_integer(tvb, offset, actx, tree, hf_index, NULL);

  return offset;
}


static const per_sequence_t CmccGPRSber130_SEQUENCE_OF_RecordNumber_sequence_of[1] = {
  { &hf_CmccGPRSber130_recordNumberList_item, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_RecordNumber },
};

static int
dissect_CmccGPRSber130_SEQUENCE_OF_RecordNumber(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_sequence_of(tvb, offset, actx, tree, hf_index,
                                      ett_CmccGPRSber130_SEQUENCE_OF_RecordNumber, CmccGPRSber130_SEQUENCE_OF_RecordNumber_sequence_of);

  return offset;
}


static const per_sequence_t CmccGPRSber130_RECORDSequenceNumber_sequence[] = {
  { &hf_CmccGPRSber130_gsnAddress, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_GSNAddress },
  { &hf_CmccGPRSber130_recordNumberList, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_SEQUENCE_OF_RecordNumber },
  { NULL, 0, 0, NULL }
};

static int
dissect_CmccGPRSber130_RECORDSequenceNumber(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
                                   ett_CmccGPRSber130_RECORDSequenceNumber, CmccGPRSber130_RECORDSequenceNumber_sequence);

  return offset;
}


static const per_sequence_t CmccGPRSber130_SEQUENCE_OF_RECORDSequenceNumber_sequence_of[1] = {
  { &hf_CmccGPRSber130_recordSequenceNumber_item, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_RECORDSequenceNumber },
};

static int
dissect_CmccGPRSber130_SEQUENCE_OF_RECORDSequenceNumber(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_sequence_of(tvb, offset, actx, tree, hf_index,
                                      ett_CmccGPRSber130_SEQUENCE_OF_RECORDSequenceNumber, CmccGPRSber130_SEQUENCE_OF_RECORDSequenceNumber_sequence_of);

  return offset;
}



static int
dissect_CmccGPRSber130_NodeID(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       1, 20, FALSE, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_ManagementExtension(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_null(tvb, offset, actx, tree, hf_index);

  return offset;
}


static const per_sequence_t CmccGPRSber130_ManagementExtensions_set_of[1] = {
  { &hf_CmccGPRSber130_ManagementExtensions_item, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_ManagementExtension },
};

static int
dissect_CmccGPRSber130_ManagementExtensions(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_set_of(tvb, offset, actx, tree, hf_index,
                                 ett_CmccGPRSber130_ManagementExtensions, CmccGPRSber130_ManagementExtensions_set_of);

  return offset;
}


static const value_string CmccGPRSber130_APNSelectionMode_vals[] = {
  {   0, "mSorNetworkProvidedSubscriptionVerified" },
  {   1, "mSProvidedSubscriptionNotVerified" },
  {   2, "networkProvidedSubscriptionNotVerified" },
  { 0, NULL }
};


static int
dissect_CmccGPRSber130_APNSelectionMode(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
                                     3, NULL, FALSE, 0, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_AccessPointNameOI(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       1, 37, FALSE, NULL);

  return offset;
}


static const per_sequence_t CmccGPRSber130_ChangeOfCharConditionC_sequence[] = {
  { &hf_CmccGPRSber130_dataVolumeGPRSUplink, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_DataVolumeGPRS },
  { &hf_CmccGPRSber130_dataVolumeGPRSDownlink, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_DataVolumeGPRS },
  { NULL, 0, 0, NULL }
};

static int
dissect_CmccGPRSber130_ChangeOfCharConditionC(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
                                   ett_CmccGPRSber130_ChangeOfCharConditionC, CmccGPRSber130_ChangeOfCharConditionC_sequence);

  return offset;
}


static const per_sequence_t CmccGPRSber130_ListOfTrafficVolumesC_sequence[] = {
  { &hf_CmccGPRSber130_peak_01, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_ChangeOfCharConditionC },
  { &hf_CmccGPRSber130_offPeak_01, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_ChangeOfCharConditionC },
  { NULL, 0, 0, NULL }
};

static int
dissect_CmccGPRSber130_ListOfTrafficVolumesC(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
                                   ett_CmccGPRSber130_ListOfTrafficVolumesC, CmccGPRSber130_ListOfTrafficVolumesC_sequence);

  return offset;
}



static int
dissect_CmccGPRSber130_INTEGER(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_integer(tvb, offset, actx, tree, hf_index, NULL);

  return offset;
}


static const per_sequence_t CmccGPRSber130_CallDurationC_sequence[] = {
  { &hf_CmccGPRSber130_peak , ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_INTEGER },
  { &hf_CmccGPRSber130_offPeak, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_INTEGER },
  { NULL, 0, 0, NULL }
};

static int
dissect_CmccGPRSber130_CallDurationC(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
                                   ett_CmccGPRSber130_CallDurationC, CmccGPRSber130_CallDurationC_sequence);

  return offset;
}



static int
dissect_CmccGPRSber130_MSISDN(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       NO_BOUND, NO_BOUND, FALSE, NULL);

  return offset;
}


static const value_string CmccGPRSber130_ConsolidationResult_vals[] = {
  {   0, "normal" },
  {   1, "notNormal" },
  {   2, "forInterSGSNConsolidation" },
  {   3, "reachLimit" },
  {   4, "onlyOneCDRGenerated" },
  { 0, NULL }
};


static int
dissect_CmccGPRSber130_ConsolidationResult(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_enumerated(tvb, offset, actx, tree, hf_index,
                                     5, NULL, FALSE, 0, NULL);

  return offset;
}


static const per_sequence_t CmccGPRSber130_SGSNPDPRecord_set[] = {
  { &hf_CmccGPRSber130_recordType, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_CallEventRecordType },
  { &hf_CmccGPRSber130_networkInitiation, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_NetworkInitiatedPDPContext },
  { &hf_CmccGPRSber130_anonymousAccessIndicator, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_BOOLEAN },
  { &hf_CmccGPRSber130_servedIMSI, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_IMSI },
  { &hf_CmccGPRSber130_servedIMEI, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_IMEI },
  { &hf_CmccGPRSber130_sgsnAddress, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_SEQUENCE_OF_GSNAddress },
  { &hf_CmccGPRSber130_msNetworkCapability, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_MSNetworkCapability },
  { &hf_CmccGPRSber130_routingArea, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_RoutingAreaCode },
  { &hf_CmccGPRSber130_locationAreaCode, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_LocationAreaCode },
  { &hf_CmccGPRSber130_cellIdentity, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_CellId },
  { &hf_CmccGPRSber130_chargingID, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_ChargingID },
  { &hf_CmccGPRSber130_ggsnAddressUsed, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_GSNAddress },
  { &hf_CmccGPRSber130_accessPointNameNI, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_AccessPointNameNI },
  { &hf_CmccGPRSber130_pdpType, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_PDPType },
  { &hf_CmccGPRSber130_servedPDPAddress, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_PDPAddress },
  { &hf_CmccGPRSber130_listOfTrafficVolumes, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_SEQUENCE_OF_ChangeOfCharCondition },
  { &hf_CmccGPRSber130_recordOpeningTime, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_TimeStamp },
  { &hf_CmccGPRSber130_duration_01, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_CallDuration },
  { &hf_CmccGPRSber130_sgsnChange, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_SGSNChange },
  { &hf_CmccGPRSber130_causeForRecClosing, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_CauseForRecClosing },
  { &hf_CmccGPRSber130_recordSequenceNumber_02, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_SEQUENCE_OF_RECORDSequenceNumber },
  { &hf_CmccGPRSber130_nodeID, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_NodeID },
  { &hf_CmccGPRSber130_recordExtensions, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_ManagementExtensions },
  { &hf_CmccGPRSber130_apnSelectionMode, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_APNSelectionMode },
  { &hf_CmccGPRSber130_accessPointNameOI, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_AccessPointNameOI },
  { &hf_CmccGPRSber130_listOfTrafficVolumesC, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_ListOfTrafficVolumesC },
  { &hf_CmccGPRSber130_durationC, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_CallDurationC },
  { &hf_CmccGPRSber130_servedMSISDN, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_MSISDN },
  { &hf_CmccGPRSber130_consolidationResult, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_ConsolidationResult },
  { NULL, 0, 0, NULL }
};

static int
dissect_CmccGPRSber130_SGSNPDPRecord(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_set(tvb, offset, actx, tree, hf_index,
                              ett_CmccGPRSber130_SGSNPDPRecord, CmccGPRSber130_SGSNPDPRecord_set);

  return offset;
}


static const per_sequence_t CmccGPRSber130_SEQUENCE_OF_PDPAddress_sequence_of[1] = {
  { &hf_CmccGPRSber130_remotePDPAddress_item, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_PDPAddress },
};

static int
dissect_CmccGPRSber130_SEQUENCE_OF_PDPAddress(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_sequence_of(tvb, offset, actx, tree, hf_index,
                                      ett_CmccGPRSber130_SEQUENCE_OF_PDPAddress, CmccGPRSber130_SEQUENCE_OF_PDPAddress_sequence_of);

  return offset;
}



static int
dissect_CmccGPRSber130_DynamicAddressFlag(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_boolean(tvb, offset, actx, tree, hf_index, NULL);

  return offset;
}


static const per_sequence_t CmccGPRSber130_GGSNPDPRecord_set[] = {
  { &hf_CmccGPRSber130_recordType, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_CallEventRecordType },
  { &hf_CmccGPRSber130_networkInitiation, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_NetworkInitiatedPDPContext },
  { &hf_CmccGPRSber130_anonymousAccessIndicator, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_BOOLEAN },
  { &hf_CmccGPRSber130_servedIMSI, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_IMSI },
  { &hf_CmccGPRSber130_ggsnAddress, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_GSNAddress },
  { &hf_CmccGPRSber130_chargingID, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_ChargingID },
  { &hf_CmccGPRSber130_sgsnAddress, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_SEQUENCE_OF_GSNAddress },
  { &hf_CmccGPRSber130_accessPointNameNI, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_AccessPointNameNI },
  { &hf_CmccGPRSber130_pdpType, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_PDPType },
  { &hf_CmccGPRSber130_servedPDPAddress, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_PDPAddress },
  { &hf_CmccGPRSber130_remotePDPAddress, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_SEQUENCE_OF_PDPAddress },
  { &hf_CmccGPRSber130_dynamicAddressFlag, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_DynamicAddressFlag },
  { &hf_CmccGPRSber130_listOfTrafficVolumes, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_SEQUENCE_OF_ChangeOfCharCondition },
  { &hf_CmccGPRSber130_recordOpeningTime, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_TimeStamp },
  { &hf_CmccGPRSber130_duration, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_INTEGER },
  { &hf_CmccGPRSber130_causeForRecClosing, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_CauseForRecClosing },
  { &hf_CmccGPRSber130_recordSequenceNumber, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_RECORDSequenceNumber },
  { &hf_CmccGPRSber130_nodeID, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_NodeID },
  { &hf_CmccGPRSber130_recordExtensions, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_ManagementExtensions },
  { &hf_CmccGPRSber130_apnSelectionMode, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_APNSelectionMode },
  { &hf_CmccGPRSber130_listOfTrafficVolumesC, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_ListOfTrafficVolumesC },
  { &hf_CmccGPRSber130_durationC, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_CallDurationC },
  { &hf_CmccGPRSber130_servedMSISDN, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_MSISDN },
  { &hf_CmccGPRSber130_consolidationResult, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_ConsolidationResult },
  { NULL, 0, 0, NULL }
};

static int
dissect_CmccGPRSber130_GGSNPDPRecord(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_set(tvb, offset, actx, tree, hf_index,
                              ett_CmccGPRSber130_GGSNPDPRecord, CmccGPRSber130_GGSNPDPRecord_set);

  return offset;
}


static const per_sequence_t CmccGPRSber130_ChangeLocation_sequence[] = {
  { &hf_CmccGPRSber130_locationAreaCode, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_LocationAreaCode },
  { &hf_CmccGPRSber130_routingAreaCode, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_RoutingAreaCode },
  { &hf_CmccGPRSber130_cellId, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_CellId },
  { &hf_CmccGPRSber130_changeTime, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_TimeStamp },
  { NULL, 0, 0, NULL }
};

static int
dissect_CmccGPRSber130_ChangeLocation(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_sequence(tvb, offset, actx, tree, hf_index,
                                   ett_CmccGPRSber130_ChangeLocation, CmccGPRSber130_ChangeLocation_sequence);

  return offset;
}


static const per_sequence_t CmccGPRSber130_SEQUENCE_OF_ChangeLocation_sequence_of[1] = {
  { &hf_CmccGPRSber130_changeLocation_item, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_ChangeLocation },
};

static int
dissect_CmccGPRSber130_SEQUENCE_OF_ChangeLocation(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_sequence_of(tvb, offset, actx, tree, hf_index,
                                      ett_CmccGPRSber130_SEQUENCE_OF_ChangeLocation, CmccGPRSber130_SEQUENCE_OF_ChangeLocation_sequence_of);

  return offset;
}


static const value_string CmccGPRSber130_Diagnostics_vals[] = {
  {   0, "gsm0408Cause" },
  {   1, "gsm0902MapErrorValue" },
  {   2, "ccittQ767Cause" },
  {   3, "networkSpecificCause" },
  {   4, "manufacturerSpecificCause" },
  { 0, NULL }
};

static const per_choice_t CmccGPRSber130_Diagnostics_choice[] = {
  {   0, &hf_CmccGPRSber130_gsm0408Cause, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_INTEGER },
  {   1, &hf_CmccGPRSber130_gsm0902MapErrorValue, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_INTEGER },
  {   2, &hf_CmccGPRSber130_ccittQ767Cause, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_INTEGER },
  {   3, &hf_CmccGPRSber130_networkSpecificCause, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_ManagementExtension },
  {   4, &hf_CmccGPRSber130_manufacturerSpecificCause, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_ManagementExtension },
  { 0, NULL, 0, NULL }
};

static int
dissect_CmccGPRSber130_Diagnostics(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
                                 ett_CmccGPRSber130_Diagnostics, CmccGPRSber130_Diagnostics_choice,
                                 NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_LocalSequenceNumber(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_constrained_integer(tvb, offset, actx, tree, hf_index,
                                                            0U, 4294967295U, NULL, FALSE);

  return offset;
}


static const per_sequence_t CmccGPRSber130_SGSNMMRecord_set[] = {
  { &hf_CmccGPRSber130_recordType, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_CallEventRecordType },
  { &hf_CmccGPRSber130_servedIMSI, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_IMSI },
  { &hf_CmccGPRSber130_servedIMEI, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_IMEI },
  { &hf_CmccGPRSber130_sgsnAddress_01, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_GSNAddress },
  { &hf_CmccGPRSber130_msNetworkCapability, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_MSNetworkCapability },
  { &hf_CmccGPRSber130_routingArea, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_RoutingAreaCode },
  { &hf_CmccGPRSber130_locationAreaCode, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_LocationAreaCode },
  { &hf_CmccGPRSber130_cellIdentity, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_CellId },
  { &hf_CmccGPRSber130_changeLocation, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_SEQUENCE_OF_ChangeLocation },
  { &hf_CmccGPRSber130_recordOpeningTime, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_TimeStamp },
  { &hf_CmccGPRSber130_duration_01, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_CallDuration },
  { &hf_CmccGPRSber130_sgsnChange, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_SGSNChange },
  { &hf_CmccGPRSber130_causeForRecClosing, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_CauseForRecClosing },
  { &hf_CmccGPRSber130_diagnostics, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_Diagnostics },
  { &hf_CmccGPRSber130_recordSequenceNumber_01, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_INTEGER },
  { &hf_CmccGPRSber130_nodeID, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_NodeID },
  { &hf_CmccGPRSber130_recordExtensions, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_ManagementExtensions },
  { &hf_CmccGPRSber130_localSequenceNumber, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_LocalSequenceNumber },
  { NULL, 0, 0, NULL }
};

static int
dissect_CmccGPRSber130_SGSNMMRecord(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_set(tvb, offset, actx, tree, hf_index,
                              ett_CmccGPRSber130_SGSNMMRecord, CmccGPRSber130_SGSNMMRecord_set);

  return offset;
}



static int
dissect_CmccGPRSber130_RecordingEntity(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_CmccGPRSber130_AddressString(tvb, offset, actx, tree, hf_index);

  return offset;
}



static int
dissect_CmccGPRSber130_MessageReference(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_octet_string(tvb, offset, actx, tree, hf_index,
                                       NO_BOUND, NO_BOUND, FALSE, NULL);

  return offset;
}



static int
dissect_CmccGPRSber130_SMSResult(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_CmccGPRSber130_Diagnostics(tvb, offset, actx, tree, hf_index);

  return offset;
}


static const per_sequence_t CmccGPRSber130_SGSNSMORecord_set[] = {
  { &hf_CmccGPRSber130_recordType, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_CallEventRecordType },
  { &hf_CmccGPRSber130_servedIMSI, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_IMSI },
  { &hf_CmccGPRSber130_servedIMEI, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_IMEI },
  { &hf_CmccGPRSber130_servedMSISDN, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_MSISDN },
  { &hf_CmccGPRSber130_msNetworkCapability, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_MSNetworkCapability },
  { &hf_CmccGPRSber130_serviceCentre, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_AddressString },
  { &hf_CmccGPRSber130_recordingEntity, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_RecordingEntity },
  { &hf_CmccGPRSber130_locationArea, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_LocationAreaCode },
  { &hf_CmccGPRSber130_routingArea, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_RoutingAreaCode },
  { &hf_CmccGPRSber130_cellIdentity, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_CellId },
  { &hf_CmccGPRSber130_messageReference, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_MessageReference },
  { &hf_CmccGPRSber130_originationTime, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_TimeStamp },
  { &hf_CmccGPRSber130_smsResult, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_SMSResult },
  { &hf_CmccGPRSber130_recordExtensions, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_ManagementExtensions },
  { &hf_CmccGPRSber130_nodeID, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_NodeID },
  { &hf_CmccGPRSber130_localSequenceNumber, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_LocalSequenceNumber },
  { NULL, 0, 0, NULL }
};

static int
dissect_CmccGPRSber130_SGSNSMORecord(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_set(tvb, offset, actx, tree, hf_index,
                              ett_CmccGPRSber130_SGSNSMORecord, CmccGPRSber130_SGSNSMORecord_set);

  return offset;
}


static const per_sequence_t CmccGPRSber130_SGSNSMTRecord_set[] = {
  { &hf_CmccGPRSber130_recordType, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_CallEventRecordType },
  { &hf_CmccGPRSber130_servedIMSI, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_IMSI },
  { &hf_CmccGPRSber130_servedIMEI, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_IMEI },
  { &hf_CmccGPRSber130_servedMSISDN, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_MSISDN },
  { &hf_CmccGPRSber130_msNetworkCapability, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_MSNetworkCapability },
  { &hf_CmccGPRSber130_serviceCentre, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_AddressString },
  { &hf_CmccGPRSber130_recordingEntity, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_RecordingEntity },
  { &hf_CmccGPRSber130_locationArea, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_LocationAreaCode },
  { &hf_CmccGPRSber130_routingArea, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_RoutingAreaCode },
  { &hf_CmccGPRSber130_cellIdentity, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_CellId },
  { &hf_CmccGPRSber130_originationTime, ASN1_NO_EXTENSIONS     , ASN1_NOT_OPTIONAL, dissect_CmccGPRSber130_TimeStamp },
  { &hf_CmccGPRSber130_smsResult, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_SMSResult },
  { &hf_CmccGPRSber130_recordExtensions, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_ManagementExtensions },
  { &hf_CmccGPRSber130_nodeID, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_NodeID },
  { &hf_CmccGPRSber130_localSequenceNumber, ASN1_NO_EXTENSIONS     , ASN1_OPTIONAL    , dissect_CmccGPRSber130_LocalSequenceNumber },
  { NULL, 0, 0, NULL }
};

static int
dissect_CmccGPRSber130_SGSNSMTRecord(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_set(tvb, offset, actx, tree, hf_index,
                              ett_CmccGPRSber130_SGSNSMTRecord, CmccGPRSber130_SGSNSMTRecord_set);

  return offset;
}


static const value_string CmccGPRSber130_CallEventRecord_vals[] = {
  {   0, "sgsnPDPRecord" },
  {   1, "ggsnPDPRecord" },
  {   2, "sgsnMMRecord" },
  {   3, "sgsnSMORecord" },
  {   4, "sgsnSMTRecord" },
  { 0, NULL }
};

static const per_choice_t CmccGPRSber130_CallEventRecord_choice[] = {
  {   0, &hf_CmccGPRSber130_sgsnPDPRecord, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_SGSNPDPRecord },
  {   1, &hf_CmccGPRSber130_ggsnPDPRecord, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_GGSNPDPRecord },
  {   2, &hf_CmccGPRSber130_sgsnMMRecord, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_SGSNMMRecord },
  {   3, &hf_CmccGPRSber130_sgsnSMORecord, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_SGSNSMORecord },
  {   4, &hf_CmccGPRSber130_sgsnSMTRecord, ASN1_NO_EXTENSIONS     , dissect_CmccGPRSber130_SGSNSMTRecord },
  { 0, NULL, 0, NULL }
};

static int
dissect_CmccGPRSber130_CallEventRecord(tvbuff_t *tvb _U_, int offset _U_, asn1_ctx_t *actx _U_, proto_tree *tree _U_, int hf_index _U_) {
  offset = dissect_per_choice(tvb, offset, actx, tree, hf_index,
                                 ett_CmccGPRSber130_CallEventRecord, CmccGPRSber130_CallEventRecord_choice,
                                 NULL);

  return offset;
}

