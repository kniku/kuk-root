#SKINF: ergaenzt uebergebenen filenamen um die aktuelle zeit.

use Getopt::Std;

Getopt::Std::getopts('hsf');
if ($opt_h) {
	print <<endOfMsg;
usage: $0 [-hs] [files]
where -s ... set names (sonst nur nach stdout printen)
      -f ... force, bereits umbenannte Dateien nochmals aktualisieren
      -h ... diese Hilfe
Benennt Dateien um, indem die aktuelle Zeit angehaengt wird.
Format: "Datei, JJJJ.MM.DD,hh-mm-ss"
Normalerweise werden Dateien, die dieser Konvention genuegen
uebersprungen, ausser -f ist gesetzt...
endOfMsg
	die "\n";
}

@ARGV = grep {-f} glob("@ARGV");
print @ARGV;
($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)=localtime;
$mon++;
$year+=1900;

$t=sprintf("%4d.%02d.%02d,%02d-%02d-%02d",$year,$mon,$mday,$hour,$min,$sec);

foreach $e(@ARGV) {
	$n=$e;
	if ($e=~/, \d{4}\.\d{2}\.\d{2},\d{2}-\d{2}-\d{2}$/) {
		if ($opt_f) {
			print "(reset) ";
			$n=~s/, \d{4}\..*//;
		} else {
			print "(skip) $e...\n";
			next;
		}
	}
	print "$e->$n, $t";
	if ($opt_s) {
		$r=rename $e,$n.", $t";
		print "; rename: ",$r?"Ok":"FAILED","\n";
	} else {
		print "\n";
	}
}
