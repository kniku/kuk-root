#!/usr/local/bin/perl
# liest datei und erzeugt daraus output, wo idente zeile count mal 
# mit einfachem zaehler versehen dupliziert wird. (schnell mal 1000 inserts...)

use Getopt::Std;

$USAGE="dup -f file -c count -l len -o offset";

Getopt::Std::getopts('l:c:o:f:');
$opt_l or die "$USAGE\n";
$opt_c or die "$USAGE\n";
$opt_o or die "$USAGE\n";
$opt_f or die "$USAGE\n";

$L=$opt_l;
$C=$opt_c;
$O=$opt_o;
$F=$opt_f;

open (FIN,"$F")||die "open error: $_\n";
while (<FIN>) {
	$_=~/(.{$O})(.{$L})(.*)/;
	$PRE=$1;
	$POST=$3;
	for ($I=0;$I<$C;$I++) {
		print "$PRE";
		printf ("%0".$L."d",$I);
		print "$POST\n";
	}
	last;
}
close (FIN);


