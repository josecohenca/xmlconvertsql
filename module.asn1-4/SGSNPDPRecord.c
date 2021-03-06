/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "CmccGPRSber130"
 * 	found in "module.asn1"
 * 	`asn1c -S/skeletons`
 */

#include <asn_internal.h>

#include "SGSNPDPRecord.h"

static asn_TYPE_member_t asn_MBR_sgsnAddress_7[] = {
	{ ATF_POINTER, 0, 0,
		-1 /* Ambiguous tag (CHOICE?) */,
		0,
		&asn_DEF_GSNAddress,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_sgsnAddress_tags_7[] = {
	(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_sgsnAddress_specs_7 = {
	sizeof(struct sgsnAddress),
	offsetof(struct sgsnAddress, _asn_ctx),
	2,	/* XER encoding is XMLValueList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_sgsnAddress_7 = {
	"sgsnAddress",
	"sgsnAddress",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_sgsnAddress_tags_7,
	sizeof(asn_DEF_sgsnAddress_tags_7)
		/sizeof(asn_DEF_sgsnAddress_tags_7[0]), /* 2 */
	asn_DEF_sgsnAddress_tags_7,	/* Same as above */
	sizeof(asn_DEF_sgsnAddress_tags_7)
		/sizeof(asn_DEF_sgsnAddress_tags_7[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_sgsnAddress_7,
	1,	/* Single element */
	&asn_SPC_sgsnAddress_specs_7	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_listOfTrafficVolumes_18[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_ChangeOfCharCondition,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_listOfTrafficVolumes_tags_18[] = {
	(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_listOfTrafficVolumes_specs_18 = {
	sizeof(struct listOfTrafficVolumes),
	offsetof(struct listOfTrafficVolumes, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_listOfTrafficVolumes_18 = {
	"listOfTrafficVolumes",
	"listOfTrafficVolumes",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_listOfTrafficVolumes_tags_18,
	sizeof(asn_DEF_listOfTrafficVolumes_tags_18)
		/sizeof(asn_DEF_listOfTrafficVolumes_tags_18[0]) - 1, /* 1 */
	asn_DEF_listOfTrafficVolumes_tags_18,	/* Same as above */
	sizeof(asn_DEF_listOfTrafficVolumes_tags_18)
		/sizeof(asn_DEF_listOfTrafficVolumes_tags_18[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_listOfTrafficVolumes_18,
	1,	/* Single element */
	&asn_SPC_listOfTrafficVolumes_specs_18	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_recordSequenceNumber_24[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_RECORDSequenceNumber,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_recordSequenceNumber_tags_24[] = {
	(ASN_TAG_CLASS_CONTEXT | (21 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_recordSequenceNumber_specs_24 = {
	sizeof(struct recordSequenceNumber),
	offsetof(struct recordSequenceNumber, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_recordSequenceNumber_24 = {
	"recordSequenceNumber",
	"recordSequenceNumber",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_recordSequenceNumber_tags_24,
	sizeof(asn_DEF_recordSequenceNumber_tags_24)
		/sizeof(asn_DEF_recordSequenceNumber_tags_24[0]) - 1, /* 1 */
	asn_DEF_recordSequenceNumber_tags_24,	/* Same as above */
	sizeof(asn_DEF_recordSequenceNumber_tags_24)
		/sizeof(asn_DEF_recordSequenceNumber_tags_24[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_recordSequenceNumber_24,
	1,	/* Single element */
	&asn_SPC_recordSequenceNumber_specs_24	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_SGSNPDPRecord_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, recordType),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CallEventRecordType,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"recordType"
		},
	{ ATF_POINTER, 2, offsetof(struct SGSNPDPRecord, networkInitiation),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NetworkInitiatedPDPContext,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"networkInitiation"
		},
	{ ATF_POINTER, 1, offsetof(struct SGSNPDPRecord, anonymousAccessIndicator),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"anonymousAccessIndicator"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, servedIMSI),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IMSI,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"servedIMSI"
		},
	{ ATF_POINTER, 1, offsetof(struct SGSNPDPRecord, servedIMEI),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IMEI,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"servedIMEI"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, sgsnAddress),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		0,
		&asn_DEF_sgsnAddress_7,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"sgsnAddress"
		},
	{ ATF_POINTER, 4, offsetof(struct SGSNPDPRecord, msNetworkCapability),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MSNetworkCapability,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"msNetworkCapability"
		},
	{ ATF_POINTER, 3, offsetof(struct SGSNPDPRecord, routingArea),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RoutingAreaCode,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"routingArea"
		},
	{ ATF_POINTER, 2, offsetof(struct SGSNPDPRecord, locationAreaCode),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LocationAreaCode,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"locationAreaCode"
		},
	{ ATF_POINTER, 1, offsetof(struct SGSNPDPRecord, cellIdentity),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CellId,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"cellIdentity"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, chargingID),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ChargingID,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"chargingID"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, ggsnAddressUsed),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_GSNAddress,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"ggsnAddressUsed"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, accessPointNameNI),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AccessPointNameNI,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"accessPointNameNI"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, pdpType),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PDPType,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"pdpType"
		},
	{ ATF_POINTER, 1, offsetof(struct SGSNPDPRecord, servedPDPAddress),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_PDPAddress,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"servedPDPAddress"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, listOfTrafficVolumes),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		0,
		&asn_DEF_listOfTrafficVolumes_18,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"listOfTrafficVolumes"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, recordOpeningTime),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TimeStamp,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"recordOpeningTime"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, duration),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CallDuration,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"duration"
		},
	{ ATF_POINTER, 1, offsetof(struct SGSNPDPRecord, sgsnChange),
		(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SGSNChange,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"sgsnChange"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, causeForRecClosing),
		(ASN_TAG_CLASS_CONTEXT | (19 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CauseForRecClosing,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"causeForRecClosing"
		},
	{ ATF_POINTER, 5, offsetof(struct SGSNPDPRecord, recordSequenceNumber),
		(ASN_TAG_CLASS_CONTEXT | (21 << 2)),
		0,
		&asn_DEF_recordSequenceNumber_24,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"recordSequenceNumber"
		},
	{ ATF_POINTER, 4, offsetof(struct SGSNPDPRecord, nodeID),
		(ASN_TAG_CLASS_CONTEXT | (22 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NodeID,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"nodeID"
		},
	{ ATF_POINTER, 3, offsetof(struct SGSNPDPRecord, recordExtensions),
		(ASN_TAG_CLASS_CONTEXT | (23 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ManagementExtensions,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"recordExtensions"
		},
	{ ATF_POINTER, 2, offsetof(struct SGSNPDPRecord, apnSelectionMode),
		(ASN_TAG_CLASS_CONTEXT | (25 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_APNSelectionMode,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"apnSelectionMode"
		},
	{ ATF_POINTER, 1, offsetof(struct SGSNPDPRecord, accessPointNameOI),
		(ASN_TAG_CLASS_CONTEXT | (26 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AccessPointNameOI,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"accessPointNameOI"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, listOfTrafficVolumesC),
		(ASN_TAG_CLASS_CONTEXT | (27 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ListOfTrafficVolumesC,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"listOfTrafficVolumesC"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, durationC),
		(ASN_TAG_CLASS_CONTEXT | (28 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CallDurationC,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"durationC"
		},
	{ ATF_POINTER, 1, offsetof(struct SGSNPDPRecord, servedMSISDN),
		(ASN_TAG_CLASS_CONTEXT | (29 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MSISDN,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"servedMSISDN"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SGSNPDPRecord, consolidationResult),
		(ASN_TAG_CLASS_CONTEXT | (30 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ConsolidationResult,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"consolidationResult"
		},
};
static ber_tlv_tag_t asn_DEF_SGSNPDPRecord_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (17 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_SGSNPDPRecord_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* recordType at 67 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* networkInitiation at 68 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* anonymousAccessIndicator at 69 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* servedIMSI at 70 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* servedIMEI at 71 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* sgsnAddress at 72 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* msNetworkCapability at 73 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* routingArea at 74 */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* locationAreaCode at 75 */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* cellIdentity at 76 */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* chargingID at 77 */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 }, /* ggsnAddressUsed at 78 */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 12, 0, 0 }, /* accessPointNameNI at 79 */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 13, 0, 0 }, /* pdpType at 80 */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 14, 0, 0 }, /* servedPDPAddress at 81 */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 15, 0, 0 }, /* listOfTrafficVolumes at 82 */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 16, 0, 0 }, /* recordOpeningTime at 83 */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 17, 0, 0 }, /* duration at 84 */
    { (ASN_TAG_CLASS_CONTEXT | (18 << 2)), 18, 0, 0 }, /* sgsnChange at 85 */
    { (ASN_TAG_CLASS_CONTEXT | (19 << 2)), 19, 0, 0 }, /* causeForRecClosing at 86 */
    { (ASN_TAG_CLASS_CONTEXT | (21 << 2)), 20, 0, 0 }, /* recordSequenceNumber at 87 */
    { (ASN_TAG_CLASS_CONTEXT | (22 << 2)), 21, 0, 0 }, /* nodeID at 88 */
    { (ASN_TAG_CLASS_CONTEXT | (23 << 2)), 22, 0, 0 }, /* recordExtensions at 89 */
    { (ASN_TAG_CLASS_CONTEXT | (25 << 2)), 23, 0, 0 }, /* apnSelectionMode at 90 */
    { (ASN_TAG_CLASS_CONTEXT | (26 << 2)), 24, 0, 0 }, /* accessPointNameOI at 91 */
    { (ASN_TAG_CLASS_CONTEXT | (27 << 2)), 25, 0, 0 }, /* listOfTrafficVolumesC at 92 */
    { (ASN_TAG_CLASS_CONTEXT | (28 << 2)), 26, 0, 0 }, /* durationC at 93 */
    { (ASN_TAG_CLASS_CONTEXT | (29 << 2)), 27, 0, 0 }, /* servedMSISDN at 94 */
    { (ASN_TAG_CLASS_CONTEXT | (30 << 2)), 28, 0, 0 } /* consolidationResult at 96 */
};
static uint8_t asn_MAP_SGSNPDPRecord_mmap_1[(29 + (8 * sizeof(unsigned int)) - 1) / 8] = {
	(1 << 7) | (0 << 6) | (0 << 5) | (1 << 4) | (0 << 3) | (1 << 2) | (0 << 1) | (0 << 0),
	(0 << 7) | (0 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (0 << 1) | (1 << 0),
	(1 << 7) | (1 << 6) | (0 << 5) | (1 << 4) | (0 << 3) | (0 << 2) | (0 << 1) | (0 << 0),
	(0 << 7) | (1 << 6) | (1 << 5) | (0 << 4) | (1 << 3)
};
static asn_SET_specifics_t asn_SPC_SGSNPDPRecord_specs_1 = {
	sizeof(struct SGSNPDPRecord),
	offsetof(struct SGSNPDPRecord, _asn_ctx),
	offsetof(struct SGSNPDPRecord, _presence_map),
	asn_MAP_SGSNPDPRecord_tag2el_1,
	29,	/* Count of tags in the map */
	asn_MAP_SGSNPDPRecord_tag2el_1,	/* Same as above */
	29,	/* Count of tags in the CXER map */
	0,	/* Whether extensible */
	(unsigned int *)asn_MAP_SGSNPDPRecord_mmap_1	/* Mandatory elements map */
};
asn_TYPE_descriptor_t asn_DEF_SGSNPDPRecord = {
	"SGSNPDPRecord",
	"SGSNPDPRecord",
	SET_free,
	SET_print,
	SET_constraint,
	SET_decode_ber,
	SET_encode_der,
	SET_decode_xer,
	SET_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_SGSNPDPRecord_tags_1,
	sizeof(asn_DEF_SGSNPDPRecord_tags_1)
		/sizeof(asn_DEF_SGSNPDPRecord_tags_1[0]), /* 1 */
	asn_DEF_SGSNPDPRecord_tags_1,	/* Same as above */
	sizeof(asn_DEF_SGSNPDPRecord_tags_1)
		/sizeof(asn_DEF_SGSNPDPRecord_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_SGSNPDPRecord_1,
	29,	/* Elements count */
	&asn_SPC_SGSNPDPRecord_specs_1	/* Additional specs */
};

