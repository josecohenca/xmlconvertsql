# this makefile generates embedded help file for Asn1Viewer

all: ../asnvesrc/help.string

../asnvesrc/help.string: ASN1VE-Manual.html help.pl
	perl help.pl

clean :
	rm -f ../asnvesrc/help.string
