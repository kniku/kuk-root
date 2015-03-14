use lib "lib";
use kktools;
use objparse;

$y=kktools::get_backup_extension;
print "kktools-version =$kktools::VERSION\n";
print "objparse-version=$objparse::VERSION\n";
print "backup extension=$y\n";
