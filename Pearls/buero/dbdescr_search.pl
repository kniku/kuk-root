#!/usr/local/bin/perl
#SKINFO: durchsuchen von dbdescr output ...

$SPOOLFILE="dat/dbdescr.out";
$OUTFILE1="dbdescr_search.out";

open (IN,"<$SPOOLFILE") or die "dateifehler beim oeffnen von $SPOOLFILE ...\n";
open (OUT1,">$OUTFILE1") or die "dateifehler beim erzeugen von $OUTFILE1 ...\n";
print "erzeuge '$OUTFILE1' ...\n";

while (<IN>) {
	if (/(_DbDescr_TABELLENNAME: )(\w+)/) {
		# table found  
		$CURTAB=$2;
		$TABCNT++;
	} else {
		if (/(^ )(\w+)(\s+)(.*\s)(.{8}\s)(\w+)\((.*)\)$/o) {
# ------------------------------- -------- ----
# FXITSKONTR                               CHAR(4)
# MCODE                           NOT NULL VARCHAR2(3)
			# column found. get column info...
			$CURCOL=$2;
			$CURWIDTH=$7;
			$CURTYPE=$6;
			$CURNULLINFO=$5;
			$COLCNT++;
# specific check (hier: sucht nach varchar2(6) spalten)
			if ($CURWIDTH=~/\(6\)/o&&$CURTYPE eq "VARCHAR2") {
				print OUT1 "$CURTAB:\t$CURCOL\t$CURTYPE$CURWIDTH ($CURNULLINFO)\n";
				$MCOLCNT++;
				if ($LASTTAB ne $CURTAB) {
					$MTABCNT++;
					$LASTTAB=$CURTAB;
				}
			}
# specific check end ...
		}
	}
}

close OUT1;
close IN;
print "== $TABCNT tables with $COLCNT columns found.\n";
print "== $MTABCNT tables, $MCOLCNT columns are matching.\n";




