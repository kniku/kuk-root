#!/usr/local/bin/perl

foreach (grep {-f||-d} glob("@ARGV")) {
	$_=~/(.\/)*(\S+)/;
	print "renaming \"$_\" to \"$1\U$2\", ".(rename "$_","$1\U$2")."\n";
}

