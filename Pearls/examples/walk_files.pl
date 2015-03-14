#!/usr/local/bin/perl
#use lib '/big/logins/kkurt/PerlSkripts/lib';
use lib '/big/logins/kkurt/Pearls/lib';
use kktools;
use Cwd;
 
@LIST=('examples','xxx','lib');

sub cb_file {
	my ($DIR,$FILE,$CALLDATA)=@_;
	my ($CDMSG);

	$CDMSG=$CALLDATA?"with $CALLDATA":"without any calldata";
	print "- $DIR/$FILE\n";
}

sub cb_dir {
	my ($DIR,$CALLDATA)=@_;
	my ($CDMSG);

	$CDMSG=$CALLDATA?"with $CALLDATA":"without any calldata";
	print "dir-CALLBACK called for $DIR $CDMSG ...\n";
	kktools::walk_files($DIR,\&cb_file,$CALLDATA);
}

$CALLDATA=join(" ",@ARGV);

kktools::recurse_dir("/big/logins/kkurt/Pearls",\&cb_dir,$CALLDATA);
#kktools::recurse_dirfile("dirlist",\&cb_dir,$CALLDATA,0);
#kktools::recurse_list(\@LIST,\&cb_dir,$CALLDATA);
#kktools::walk_files("/big/logins/kkurt/Pearls",\&cb_file,$CALLDATA);
