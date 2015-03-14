#!/usr/local/bin/perl
#use lib '/big/logins/kkurt/PerlSkripts/lib';
use lib 'u:/PerlSkripts/lib';
use kktools;
use Cwd;

@LIST=(	'shared/fmk',
		'shared/sql',
		'shared/sys',
		'shared/ink',
		'shared/dsp',
		'shared/gui',
		'shared/abb',
		'shared/zzt',
		'shared/zfd',
		'shared/liq',
		'ts/tssrc',
		'hs/hssrc',
		'cm/cmsrc',
		'zv/zvsrc',
		'fx/fxsrc',
		'da/dasrc',
		'zf/zfsrc',
		'me/mesrc',
		'gm/gmsrc');

sub cb {
	my ($DIR,$CALLDATA)=@_;
	system("$CALLDATA") if $CALLDATA;
}

$CALLDATA=join(" ",@ARGV);
kktools::recurse_list(\@LIST,\&cb,$CALLDATA);
