#/usr/local/bin/perl

use lib "d:/work/pearls/lib";
require "find.pl";
use Getopt::Std;
use kktools;

@ARGV=grep{-f}glob"@ARGV";

$OLDROOT="v\\|\\/nt-tools";
$NEWROOT="d\|\/tools";

sub wanted {
	my $dir,$file;

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

foreach $EMF (@LIST) {
	#trenne pfad und name...
	$EMF=~/(.*)\/(.*?$)/;
	print "found: [$2] in [$1]\n";
	if (kktools::rename_to_backup($EMF)==1) {
#	if (0) {
		if (open (IN,"<$EMF".kktools::get_backup_extension)) {
			if (open(OUT,">$EMF")) {
				while (<IN>) {
					s/$OLDROOT/$NEWROOT/ig;
					print OUT $_;
				}
				close OUT;
			}			
			close IN;
		}
	}
}


