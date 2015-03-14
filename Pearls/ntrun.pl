#!/usr/bin/perl
# SKINF: NT-Tool zur Kommandozeilenexpansion von
# wildcards...

if (!$ARGV[0]||!$ARGV[1]) {
	die "usage: ntrun [cmd] [params]\n";
}

#@_ARGV=grep {-f&&-T} glob("@ARGV");
@_ARGV=grep {-f} glob("@ARGV");
print "Aufruf von $ARGV[0] mit expandierten Parameteren ($#_ARGV)...\n";
system "$ARGV[0] @_ARGV" if ($#_ARGV>0);
