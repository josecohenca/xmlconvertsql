#!/usr/bin/perl

# for the duration of the script, we want to have the local directory included
# in the $PATH
if ($^O =~ /Win/) {
   $ENV{"PATH"} = $ENV{"PATH"} . ';.;..\..\bin';
}
else { 
   $ENV{"PATH"} = $ENV{"PATH"} . ':.:../../bin';
}

select STDERR; $| = 1;
select STDOUT; $| = 1;

@dirlist = (`ls .`);

foreach $filename (@dirlist) {
    $filename =~ s/\n$//;

    if (-d $filename) {
        chdir ($filename) || dir ("chdir to $filename failed. $!");

        if (-e "message.dat") {
            print "$filename\n";
            print `asn2xml message.dat -schema *.asn -o temp.xml`;

            if (-e 'temp.xml') {
                # validate with xmllint
                print `xmllint temp.xml --noout`;
                unlink ('temp.xml');
            }
        }

        chdir ('..');
    }
}
