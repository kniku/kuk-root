#!/usr/local/bin/perl
#SKINFO: rc,c to mlc input...

# Kommandozeilenoptionen checken...

use Getopt::Std;
use MD5;

Getopt::Std::getopts('ho:v');
if ($opt_h||!$opt_o) {
	print <<endOfMsg;
usage: $0 [-hl] [-i pattern] [-o outputfile] files...
-v...detailiertes Log
-h...diese Hilfe (ask kuk)
Die Angabe der files ist wilcardfaehig.
Achtung: kein BACKSLASH in den Pfaden verwenden!
Merge-Modus: Ist "outfile" vorhanden, so werden die darin enthaltenen
Id's mitverwendet (vorrangig, bestehende Uebersetzungen bleiben
erhalten). Ggf. werden nicht benoetigte ID's in "outfile.nu" abgelegt
(wird dann ebenfalls dazugelesen).
endOfMsg
	die "\n";
}

# Wildcards selbst aufloesen (nicht jede shell kann das zb DOS)
$CL=join(" ",@ARGV);
@ARGV = grep { -f && -T && !/guitemp\.rc/i} glob("@ARGV");

# falls outputfile bereits vorhanden,
# einlesen und Liste aufbauen...
read_outputfile($opt_o);
read_outputfile("$opt_o.nu");


# Inputfiles Zeilenweise durchgehen und
# Liste der Msg-Id's anlegen...
$curLines=$curIds=$curIdsdbl=0;
while (<>) {
	if ($ARGV ne $curFile) {
		# Neuen File begonnen...
		$curFile=$ARGV;
		$mode=$curFile=~/\.[chi]/?1:2;
		print "processing \"$curFile\" (mode=$mode)...\n";
	}
	$curLines++;
	if ($mode==1) {mode_c($_)}
	elsif ($mode==2) {mode_rc($_)}
	if (eof) {
		# Aktuellen File abschliessen...
		close ARGV;
		print "-> lines: ",$curLines,", Id's: ",$curIds," davon ",$curIdsdbl," bereits in der Liste oder doppelt.\n";
		$curLines=$curIds=$curIdsdbl=0;
	}
}

# Outputfile oeffnen...
open OUT,">$opt_o" or die "Fehler beim Anlegen von $opt_o\n";
open OUT2,">$opt_o.nu" or die "Fehler beim Anlegen von $opt_o.nu\n";
print "Files \"$opt_o\", \"$opt_o.nu\" (nicht benoetigte Eintraege) angelegt, schreibe Liste ...\n";

# Header rausschreiben...
$T=localtime;
print OUT <<endOfHeader;
# -  
# -  Automatisch generiertes -Multilanguage- File
# -  
# -  Quelle: $CL
# -  Ziel:   $opt_o
# -  
# -  Erstellt am $T
# -  
# -  !!!  BITTE NICHT EDITIEREN  !!!
# -  
# -  .output rc   # - 
# -  .language    # - 
# -  eng          # - 
# -  ger          # - 
# -  .fset ansi   # - 
# -  .assign      # - 
# -  .default=ger # - 

endOfHeader
print OUT2 "Derzeit nicht benutzte ID's...\n\n";

# Liste der gefundenen Eintraege durchgehen...
$md5=new MD5;
print "OUTPUT:\n" if $opt_v;
while (($k,$v)=each(%list)) {
	next if ($k eq "");		# 'leere' labels ignorieren...
	print "Key->Id: \"$k\"->" if $opt_v;
	$m=$k;
	$m=~s/ //g;				# leerzeichen raus
	$m=~s/^\d//g;			# 1. Ziffer raus
	if ($m!~/\w/) {
		print " nur Sonderzeichen, ignoriert...\n";
		next;
	}
if (0) {
	$m=$k;
	$m=~s/ |\\n|\\t/_/g;	# white space -> _
	$m=~s/\W/X/g;			# sonderzeichen -> X
	$m=~s/^\d/X/;			# Ziffer am Anfang -> X
} else {
	$md5->reset;
	$md5->add($k);
	$m="MD5_".$md5->hexdigest;
}
	print "\"$m\"" if $opt_v;
	if ($$v{"-cnt"}==0) {
		print OUT2 ".msg $m\n";  
		print OUT2 "ger:\"$k\"\n";
		print OUT2 "eng:\"",$$v{"-eng"},"\"\n\n";
		print " (not used)\n" if $opt_v;
	} else {
		print OUT "# - Eintrag: ",$$v{"-old"}?"Alt":"Neu",", cnt=",$$v{"-cnt"},"\n";  
		print OUT "# - Orig.  : ",$$v{"-orig"},"->\"$k\"\n";
		print OUT "# - gef. in: ",$$v{"-position"},"\n";  
		print OUT ".msg $m\n";  
		print OUT "ger:\"$k\"\n";
		print OUT "eng:\"",$$v{"-eng"},"\"\n\n";
		print " (used)\n" if $opt_v;
	}
}

# Outputfile schliessen...
close OUT;
close OUT2;

print "---Fertig---\nGesamt: ",$c_all,", Neu: ",$c_new,", Alt: ",$c_old,"\n";
print "Achtung: 'leere' Labels gefunden (",$list{""}{"-cnt"},")\n" if ($list{""}{"-cnt"}>0);

# --- ende main ---

# RC Mode...
sub mode_rc 
{
	my ($l)=@_;
	my $orig;
	
	chomp($l);
	$l=~s/^\s+//;
	if ($l=~/LS\s*\(/) {
		print "$ARGV ($.): \"$l\"-> ignored...\n" if $opt_v;	
		return 0;
	}
	if ($l=~/^\s*(Label|Title|PopHelp|OwOptmDescr\d*|ListLabel|LbcHeader|LbcFooter|LbcBody)\s*=\s*\"(.*)\"\s*/) {
		$orig=$l;
		print "$ARGV ($.): \"$orig\"->" if $opt_v;
		$l=$2;
		$l=~s/\\b\s*\{.*}\s*//; # bitmap-labels zerlegen...
		$curIds++;
		print "\"$l\"" if $opt_v;
		if (exists($list{$l})) {
			$list{$l}{"-cnt"}++;
			$curIdsdbl++;
			print " (existiert)\n" if $opt_v;
		} else {
			$list{$l}{"-cnt"}=1;
			$list{$l}{"-eng"}="???";
			$list{$l}{"-position"}="$ARGV ($.)";
			$list{$l}{"-orig"}="$orig";
			$c_all++;
			$c_new++;
			print " (neu)\n" if $opt_v;
		}
	}
}

# C Mode...
sub mode_c 
{
	my ($l)=@_;
}

sub read_outputfile
{
	my ($f)=@_;
	my $key;
	
	if (open(IN,"<$f")) {
		undef $key;
		print "file \"$f\" gefunden, lese file...\n";
		while (<IN>) {
			if (/ger:\"(.*)\"/) {
				$key=$1;
			} elsif (defined($key)&&/^(\S+):\"(.*)\"/) {
				# Uebersetzung gefunden, reinhaengen...
				$list{$key}{"-cnt"}=0;				
				$list{$key}{"-old"}=1;				
				$list{$key}{"-$1"}=$2;
				$list{$key}{"-position"}="$f ($.)";
				$c_all++;
				$c_old++;
				print "old: $f ($.): $1\[$key\]\=\"$2\"\n" if $opt_v;
			}
		}
		close IN;
	}
}
