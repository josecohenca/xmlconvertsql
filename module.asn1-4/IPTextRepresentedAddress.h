/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "CmccGPRSber130"
 * 	found in "module.asn1"
 * 	`asn1c -S/skeletons`
 */

#ifndef	_IPTextRepresentedAddress_H_
#define	_IPTextRepresentedAddress_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum IPTextRepresentedAddress_PR {
	IPTextRepresentedAddress_PR_NOTHING,	/* No components present */
	IPTextRepresentedAddress_PR_iPTextV4Address,
	IPTextRepresentedAddress_PR_iPTextV6Address
} IPTextRepresentedAddress_PR;

/* IPTextRepresentedAddress */
typedef struct IPTextRepresentedAddress {
	IPTextRepresentedAddress_PR present;
	union IPTextRepresentedAddress_u {
		OCTET_STRING_t	 iPTextV4Address;
		OCTET_STRING_t	 iPTextV6Address;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} IPTextRepresentedAddress_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IPTextRepresentedAddress;

#ifdef __cplusplus
}
#endif

#endif	/* _IPTextRepresentedAddress_H_ */
