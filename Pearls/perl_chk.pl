#!/usr/local/bin/perl

# Check, ob installierter Perl Interpreter fuer
# kuk ausreicht... ;-)

BEGIN {
	@modlist=("5.0.0","Config","Config::IniFiles","Getopt::Std","Cwd","lwp");
	print "$^X, version=$]\n";
	foreach $mod (@modlist) {
		if (not eval("require $mod")) {
			print "*** Modul \"$mod\" not found!\n";
			$error=1;
		} else {
			print "check \"$mod\", ok\n";
		}
	}
	$error=$error?"IS NOT":"IS";
	print "== $error ready for kuk ==\n";
}



