#!/usr/bin/perl

open (INFILE, "ASN1VE-Manual.html") ||
    die ("Unable to open ASN1VE-Manual.html");

$outfn = "../asnvesrc/help.string";

`rm -f $outfn`;

open (OUTFILE, ">".$outfn);

print OUTFILE "static const char* helpTextArray[] = {\n";

while (<INFILE>) {
    chomp;
    print OUTFILE "\"";
    s/^\s+//;
    s/\\/\\\\/g;
    s/\n//g;
    s/\"/\\\"/g;
    s/$//g;
    print OUTFILE $_;
    print OUTFILE "\",\n";
}

print OUTFILE "};\n";
close OUTFILE;
