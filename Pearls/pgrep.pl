#!/usr/local/bin/perl
#SKINF: fgrep mit perl reg expr und binary mode...

use Getopt::Std;

Getopt::Std::getopts('hp:e:iqob');
if (!$opt_p||$opt_h) {
	print <<endOfMsg;
usage: pgrep [-iqoh][-e ENDPATTERN] -p PATTERN files
where -q ... quit/count only
      -i ... case insensitive
      -o ... source lines only
      -b ... binary mode (first occurence of pattern)
endOfMsg
	die "\n";
}

if ($opt_b) {
	@ARGV = grep { -f } glob("@ARGV");
} else {
	@ARGV = grep { -f && -T } glob("@ARGV");
}

$CNT=0;

if ($opt_b) {
	binmode IN;
	while (<>) {
		$H=$opt_i?/$opt_p/ig:/$opt_p/g;
		if ($H) {
			$CNT++;
			print $ARGV,"\n";
			close ARGV; # aktuelles file fertig
		}
	}
} else {
	while (<>) {
		$H=$opt_i?/$opt_p/io:/$opt_p/o;
		if ($H) {
			print "$ARGV $.\t $_" if !$opt_q&&!$opt_o;
			print "$_" if !$opt_q&&$opt_o;
			$IN=1 if $opt_e;
			$CNT++;
		} elsif ($IN) {
			print "$ARGV $.\t $_" if !$opt_q&&!$opt_o;
			print "$_" if !$opt_q&&$opt_o;
			$E=$opt_i?/$opt_e/io:/$opt_e/o;
			undef $IN if ($E);;
		}
		if (eof) {
			close ARGV;
			undef $IN;
		}
	}
}

print "matches found: $CNT\n" if !$opt_o;
print "\n" if $opt_o;

