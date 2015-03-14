#!/usr/local/bin/perl

#SKINFO: durchsucht itdb.txt ...

$SPOOLFILE="dat/itdb.txt";
$OUTFILE1="erg/itdb_search.out";

open (IN,"<$SPOOLFILE") or die "dateifehler beim oeffnen von $SPOOLFILE ...\n";
open (OUT1,">$OUTFILE1") or die "dateifehler beim erzeugen von $OUTFILE1 ...\n";
print "lese $SPOOLFILE, erzeuge '$OUTFILE1' ...\n";

while (<IN>) {
	if (/(TABLE )(\w+)/) {
		$CURTAB=$2;
	} else {
		chomp;
		$LINE=$_;
		if (/(.{4})(\b\w+\b).*(,\s*$)/) {
			$CURCOL=$2;
# specific check...
			if (1) {
				$MATCHCNT++;
				print OUT1 "$CURTAB: $LINE\n";
			}
# end
		}
	}
}

close OUT1;
close IN;
print "$MATCHCNT matches found...\n";
