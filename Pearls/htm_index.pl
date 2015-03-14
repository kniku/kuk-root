#/usr/local/bin/perl

use lib "h:/pearls/lib";
require "find.pl";
use Getopt::Std;
use kktools;
use Cwd;

($ROOTDIR)=Cwd::cwd=~/(.*?)\/??$/;

$IDXFILE="local_index.htm";

sub wanted {
	my $dir,$file;

	$cnt++;
	print "\rfind: $cnt";

	# ACHTUNG: $_ darf hier nicht veraendert werden!

	# falls keine rekursive verzeichnisabarbeitung erwuenscht:
	if (0 && not $File::Find::dir eq ".") {	
      	$File::Find::prune = 1;
     	return;
	}

	# trenne pfad und name...
	# wir befinden uns naemlich im verzeichnis $dir !
	$File::Find::name=~/(.*)\/(.*?$)/;
	$dir=$1;
	$file=$2;

	# spezifischer filefilter...
	# hier wird nur auf 'file' geprueft
#	if ($dir eq "./dat/tmp"&& 	# nur aus dat/tmp lesen...
	if (
		$file=~/\.html*$/&& 		# nur htm(l) files...
		-f $file) {
		push @LIST,$File::Find::name;
    }
}


find('.');
print "\n";

open OUT,">$IDXFILE" or die "fileerror\n";
# ---------------------------
print OUT <<endOfHeader;
<HTML> 
<BASE HREF="">
<HEAD> 
<TITLE>Liste lokaler HTML Dateien...
</TITLE> 
</HEAD>
<BODY>
<STRONG>Liste der gefundenen HTML Dateien in [$ROOTDIR]:</STRONG><P>
<UL>
endOfHeader
# ---------------------------
$cnt=0;
foreach $EMF (@LIST) {
	$EMF=~s/\./$ROOTDIR/;
	#trenne pfad und name...
	($dir,$file)=$EMF=~/(.*)\/(.*?$)/;
	open IN,"<$EMF";
	$title="?";
	while (<IN>) {
		if (/<TITLE>(.*)/) {
			$title=$1;
			$title=~s/[\<\>]/\?/g;
			last;
		}
	}
	close IN;
	if (not $dir eq $lastdir) {
		print OUT "<P><STRONG>$dir</STRONG>\n";
		$lastdir=$dir;
	}
	$cnt++;
	print "\rindexing: $cnt";
	print OUT "<LI><A HREF=\"$EMF\">$EMF</A>: $title\n";
}
# ---------------------------
print OUT <<endOfFooter;
</UL>
</BODY>
</HTML>
endOfFooter
# ---------------------------

close OUT;
print "\n";
