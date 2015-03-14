#!/usr/local/bin/perl
require "find.pl";
use Getopt::Std;

sub wanted {
	if (
		$name=~/\/\D\d+$/||				# X1234 .. uemacs
		uc($name)=~/\.BAK$/||			# *.bak .. bak-files
		$name=~/\/\.#.*\d+\.\d+/||		# #.*N.NN  cvs (merge)
		$name=~/~$/||					# *.*~ ... emacs
		$name=~/sqlnet.log/||			# oracle fehlerlogs
		$name=~/\/#.*#$/				# #*.*# .. nt-emacs
		) {		
			
	    	print("$name\n");
			push @LIST,$name;
		
    	}
}

print "Sollen folgende Dateien wirklich geloescht werden?\n";
&find('.');
print "j/n (j):";
$key = uc(getc(STDIN));
if ($key eq "\n"||$key eq "J") {
	foreach $EMF (@LIST) {
		print "loesche $EMF";
		(unlink "$EMF")==1?print ", ok.\n":print " - fehlgeschlagen!\n";
	}
} else {
	print "canceled...\n";
}

exit;

