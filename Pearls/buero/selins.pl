#!/usr/bin/perl


use Getopt::Std;
Getopt::Std::getopts('kdths:f:ci');
if ($opt_h) {
	print <<endOfMsg;
usage: $0 [configfile]

table select and insert after doing some modifications ...
reads modifications from a config file

format:
[tablename]
col1='new_value'
...
# optional where clause, e.g.:
__where=where mcode='100'
endOfMsg
	die "\n";
}

use Text::Wrap;
use lib "lib";
use objparse;

$CFGFILE=$ARGV[0]?$ARGV[0]:"selins.cfg";
$SPOOLFILE="dat/dbdescr.out";

sub parse_and_store_dbdescr {
	my ($DBF,$ERGF)=@_;
	my $CURTAB,@COLLIST,$TABCNT,$CURCOL,$CURWIDTH;
	my $CURTYPE,$CURNULLINFO,$COLCNT,@TABLE;

	open (IN,"<$DBF") or die "dateifehler beim oeffnen von $DBF ...\n";

	while (<IN>) {
		if (/(_DbDescr_TABELLENNAME: )(\w+)/) {
			# table found  
			if ($CURTAB) {
				$TABLE{$CURTAB}=join(',',@COLLIST);
				@COLLIST=();
			}
			$CURTAB=$2;
			$TABCNT++;
		} else {
			if (/(^ )(\w+)(\s+)(.*\s)(.{8}\s)(\w+)(\(.*\)$)/o) {
				# column found. get column info...
				$CURCOL=$2;
				$CURWIDTH=$7;
				$CURTYPE=$6;
				$CURNULLINFO=$5;
				$COLCNT++;
# specific check (hier: sucht nach varchar2(6) spalten)
				push @COLLIST,$CURCOL;
# specific check end ...
			}
		}
	}
	close IN;
	%TABLE;
}

sub baue_stmt {
	my ($TAB,$CLIST,$_CL,$W)=@_;
	my @COLS=split(/,/,$CLIST);
	my $S,$COMMA="";
	
	$S="insert into $TAB (";
	foreach (@COLS) {
		$S=$S.$COMMA.$_;
		$COMMA=', ';
	}
	$S=$S.") select ";
	$COMMA="";
	foreach (@COLS) {
		if ($$_CL{$_}) {
			$S=$S.$COMMA.$$_CL{$_};
		} else {
			$S=$S.$COMMA.$_;
		}
		$COMMA=', ';
	}
	$S=$S." from $TAB";
	if ($W) {
		$S=$S." $W";
	}
	$S.";";
}

%TABLIST=parse_and_store_dbdescr($SPOOLFILE,$OUTFILE1);
$rhOBJS=objparse::parse_objects($CFGFILE);
#kktools::print_parsed_objects($rhOBJS);

sub cb {
	my ($TAB,$rhCL)=@_;
	my %CL;

	$WH=$$rhCL{"__where"};
	undef %CL;
	while (($kk,$vv)=each %$rhCL) {
		next if ($kk=~/__/);
		$CL{$kk}=$vv;
	}
	if ($TABLIST{$TAB}) {
		print Text::Wrap::wrap("","",baue_stmt($TAB,$TABLIST{$TAB},\%CL,$WH)."\n");
		print "commit;\n";
	} else {
		print "rem ===> $TAB NOT found!\n"
	}
}

kktools::recurse_parsed_objects($rhOBJS,\&cb);
