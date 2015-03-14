#!/usr/local/bin/perl

use DBI;

print 'file version=$Id$'."\n";

print "dbi version=$DBI::VERSION\n";
print "supported drivers: ".join(",",DBI->available_drivers)."\n";
