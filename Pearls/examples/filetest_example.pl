#!/usr/local/bin/perl
use Getopt::Std;

($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
       $atime,$mtime,$ctime,$blksize,$blocks)
                  = stat("$ARGV[0]");
                  
print "dev:\t$dev\nino:\t$ino\nmode:\t$mode\nnlink:\t$nlink\nuid:\t$uid\n";
print "gid:\t$gid\nrdev:\t$rdev\nsize:\t$size\natime:\t$atime\n";
print "mtime:\t$mtime\nctime:\t$ctime\nblksize:\t$blksize\nblocks:\t$blocks\n";
