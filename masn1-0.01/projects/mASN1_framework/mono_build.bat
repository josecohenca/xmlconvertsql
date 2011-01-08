mcs ASN1Simple.cs ASN1Base.cs ASN1BER.cs -target:library
mcs ASN1StringTypes.cs -r:ASN1Simple.dll -target:library

rem itd.