#!/usr/local/bin/perl

#SKINF: bricht lange zeilen um.
$L=128;
while (<STDIN>) {
	print "$1\n" while (/(.{1,$L})/g);
}
