#!/usr/bin/perl
# Description: Get SVN log from Collabnet and pipe it into a csv file.
#
# @Author Casey Ralls


# USAGE:
# perl logmake.pl <repository_name>


# Define Repositories. 
#
# You can add extra repositories to the tool here!

@repos = ("lj-ice-installer","kodiak","csl-laserjetservice");


# Lets define some variables to work with

my $repo = "https://$ARGV[0].csd200a.com/svn/$ARGV[0]/trunk";
my $from_date = '{'.$ARGV[1].'}';

if($ARGV[2]) {
	my $to_date = '{'.$ARGV[2].'}';
} else {
	my $to_date = 'HEAD';
}

# Define the arguments for the command line

@args = ("svn","log","$repo","-v","-r$from_date:HEAD","--xml");


# Lets check to see which repositories are being called and get the log into an
# array from Collabnet for formatting and later writing to a csv file. 

if ((@ARGV) && ($ARGV[0]) && ($ARGV[1])) {
	$xml = `svn log $repo -v -r$from_date:HEAD --xml`;
} else {
	die("
		-------------------------------------------------------------------------
		USAGE: perl logmake.pl <repository_name> <from_date> <to_date (Optional)>
		
		Currenly installed repositories are:
			@repos

		Dates are formatted like:
			yyyy-mm-dd
		------------------------------------------------------------------------

	");
}


# Write the xml data to a file

$file = "svn.xml";
open FH, "+>", $file or die $!;
print FH "$xml";
close FH;

# Convert the xml to csv for viewing in excel

@args = ("xml2csv.exe","svn.xml","svn.csv","revision,author,date,msg,path","-Q");
system(@args);


# Open the resulting csv file and format it so that files share a single cell.

my $sub = ',,,,"';
$file = "svn.csv";
open FH, "+<", $file or die $!;
@filearr = <FH>;
my $i=0;
for(@filearr) {
	if(!index($filearr[$i],$sub)) {
		$filearr[$i] = substr $filearr[$i], 5;
		$previ = $i-1;
		substr($filearr[$previ], -2, 1) = '';
	}
$i++;
}
close FH;
open FH, "+>", $file or die $!;
print FH @filearr;
close FH;

# Copy the csv file to a share

@args = ("copy","svn.csv","\\\\pls-drops\\atlantissw\\developerbuilds\\logmake");
system(@args);

