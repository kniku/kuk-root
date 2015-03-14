#!/usr/local/bin/perl

use Getopt::Std;
$A=$#ARGV+1;
print "Anzahl der Parameter: $A\n";
# nur fuer UNIX!!!
#Getopt::Std::getopts('abf:c',\%OPTS);
#while (($key,$value) = each %OPTS) {
#	print "$key\t= $value\n";
#}

# funzt auch unter WIN32
# ein : hinter einem Parameter bedeutet: da muss auch ein wert angegeben sein
# die anderen sind einfache schalter ...
#Getopt::Std::getopts('abf:c');

# diese variablen wurden autom. befuellt
print "$opt_a, $opt_b, $opt_f\n";
$A=$#ARGV+1;
print "Anzahl der Parameter (nach getopt): $A\n";


use Getopt::Long;

#__END__

my @inclib = ();
#mit mehreren Werten:
GetOptions ("XXX=s" => \@inclib);
print @inclib,"\n";

