#!/usr/local/bin/perl

use DBI;
use Getopt::Std;

Getopt::Std::getopts('hcd:');
if ($opt_h) {
	print <<endOfMsg;
CSV SQL Shell...
usage: $0 [-hc][-d CSV-dir] commandlist
where -c ... execute commandlist (batch mode)
      -d ... CSV directory (def.=.)
      -h ... this help (ask kuk)...
endOfMsg
	die "\n";
}

$dir=$opt_d?$opt_d:".";

#CSV connect aufbauen...
$dbh_csv=DBI->connect("DBI:CSV:f_dir=$dir");
die "\n" if (!$dbh_csv);
print "CSV-SQL Shell v1.0 (",($opt_c?"batch":"interactive")." mode)\nCSV connection \"$dir\" open, type help for some hints... \n";

if (!$opt_c) {
	# interactive mode ...
	print "CSVSQL>";
	while (<STDIN>) {
		chomp;s/;$//;
		last if (/\s*exit/);
		if (/\s*help/) {
			print "commands:\n exit ... end session\n",
			      " dir  ... list files/tables\n",
				  " help ... this help\n";
		} elsif (/\s*dir/) {
			system("ls $dir");
		} else {
			doit($_);
		}
		print "CSVSQL>";
	}
} else {
	# batch mode ...
	foreach $stmt (@ARGV) {
		print "executing $stmt ...\n";
		doit($stmt);
	}
}

# disconnect
print "CSV connection closed, result=",$dbh_csv->disconnect,"\n";

sub doit {
	my ($stmt)=@_;
	my $sth,$cnt,@row;

	if ($stmt) {
		$sth=$dbh_csv->prepare($stmt);
		if ($sth) {
			$cnt=$sth->execute;
			if (defined $cnt) {
				print "execution result: $cnt\n";
				$cnt=0;
				if ($stmt=~/select/) {
					while (@row=$sth->fetchrow_array) {
						print join(",",@row),"\n";
						$cnt++;
					}
					print "$cnt rows selected\n";
				}
			}
		}
	}
}
