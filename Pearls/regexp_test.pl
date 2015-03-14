#!/usr/local/bin/perl
#SKINF: regexp tester ...

if (!defined $ARGV[0]) {
	die "usage: $0 [\"string\"]\n";
}
print "enter regexp's or ^C to exit...\n";
while (<STDIN>) {
	chomp;
	if ($ARGV[0]=~/$_/) {
		print "match, results as follows:\n";
		print "\$1: $1\n";
		print "\$2: $2\n";
		print "\$3: $3\n";
		print "\$4: $4\n";
		print "\$5: $5\n";
		print "\$6: $6\n";
		print "\$7: $7\n";
		print "\$8: $8\n";
		print "\$9: $9\n";
	} else {
		print "no match ...\n";
	}
}
