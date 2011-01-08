include ../platform.mk

all : message.xml

message.xml: message.dat
	$(ASN2XML) message.dat -schema Test.asn -o message.xml


clean:
	$(RM) message.xml
