# 
# zum Analysieren von Binaerfiles auf Vorkommen von Bytes
# prepare_csv
# par1: <input-filename>
# par2: optional, 0.....auch nicht vorkommende Bytes ausweisen
# par3: optional, csv...Output im csv-Format

use strict;
my $inpfname=(defined $ARGV[0])?$ARGV[0]:"nofile";
my $with_null=(defined $ARGV[1]&&$ARGV[1] eq '0')?1:0;
my $csv=(defined $ARGV[2]&&$ARGV[2] eq 'csv')?';':'';
my %BYTES;
my $count=0;
my $proz;
my $proz_min=100.0;
my $proz_max=0.0;

print "Input-Filename: $inpfname\n";

if ($with_null eq '1') {
	for (my $i=0; $i<256; $i++) {
		my $byte_ord=sprintf "%03d",$i;
		$BYTES{$byte_ord}=0;
	}
}

if (open (INPFILE,'<',"$inpfname")) {
	binmode(INPFILE);
	push (my @inpline, <INPFILE>);	
	my $inpstream=join("",@inpline);
	my $len=length($inpstream);
	for (my $pos=0; $pos<$len; $pos++) {
		my $byte=substr($inpstream,$pos,1);
		my $byte_ord=sprintf "%03d",ord($byte);
		
		if (defined ($BYTES{$byte_ord})) {
			$BYTES{$byte_ord}++;
		} else {
			$BYTES{$byte_ord}=1;
		}
	}
	foreach (sort keys %BYTES) {
		$proz = (($BYTES{$_}) / $len) * 100.0;
		if ($proz>$proz_max) {
			$proz_max=$proz;
		}
		if ($proz<$proz_min) {
			$proz_min=$proz;
		}
		my $c=chr($_);
		if (($_<32)||($_>126)) {
			$c='?';
		}
		printf "%3d.%s  [$_]%s    #%9d%s   =%s %8.4f %%%s   [%s]%s\n",
				++$count,$csv,$csv,
				$BYTES{$_},$csv,$csv,
				$proz,$csv,
				$c,$csv;
	}
	printf " TOTAL         #%9d   = 100.0000 %%   min:%7.4f %% / max:%7.4f %%\n", 
		$len, $proz_min, $proz_max;
	close (INPFILE);
} else {
	die "Cant open Inputfile $inpfname\n";
}
