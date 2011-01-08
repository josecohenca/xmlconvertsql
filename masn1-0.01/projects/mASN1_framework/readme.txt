Par stvari vezanih uz dizajn lib-a
--------------------------------------------------------

1. PARALELNE ASN1Object i ASN1Type HIJERARHIJE
     Buduci da o klasi ne mogu nista znati
     ako ne kreiram instancu, potrebna mi je jedna 
     hijerarhija za instance tipova, te jos jedna za
     same tipove, tj. hijerarhija klasa koja govore
     o tipovima, i mogu kreirati instance.
     

2. Zasto Init() metode?
     Odgovor je "Constructor inheritance". Kad bi 
     taj koncept postojao u C# - ne bi mi trebala Init() metoda.
     Ovako je ipak lakse. U suprotnom bi bilo puno teze
     napraviti alias tipova, kao npr. u PKCS-7:
     
         Data ::= OCTET STRING
         
     Ovako mogu napraviti
         
         public class Data : ASN1OctetString {}
             ...
         Data d = new Data;
         d.Init(octetString);
         
    Dakle, u kasnijem kodu imam dodan red vise u inicijalizaciji, 
    no bitno je lakse napraviti ASN.1 type alias.
