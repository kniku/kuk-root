#!/usr/bin/perl

package kk_owil_prc_reader;

my ($kl_auf,$objektcount);
my ($owil_define_list,@owil_rc_include_path);

# Library Version
$kk_owil_prc_reader::VERSION="01.000";
$kk_owil_prc_reader::VERBOSE=0;	# printing infos ?

sub read_line
{
	my ($listref)=@_;
	my ($line,$im_kommentar,$flag,$tmp);

	$im_kommentar=0;
	while (1) {
		$line=shift @$listref;
		last if (not $line);
		chomp $line;
		next if ($line=~/^\s*\#/||$line=~/^\s*$/);	# Kommentarzeile od. leere Zeile
		# OWIL: Bedingter Block, check gegen define_list ...
		if ($line=~/^\s*(\S+)\s*\{/) {	# Klammer auf gefunden...
			$tmp=".".$1.".";
			if ($kk_owil_prc_reader::owil_define_list!~/$tmp/) {	# Wenn define nicht gesetzt, Kommentar AN
				$im_kommentar=1;
			}
			next;
		} elsif ($line=~/^\s*\}/) {		# abschliessende Klammer gefunden...
			if ($line=~/\}\s*else/) {	# ein else gefunden ...
				$im_kommentar=$im_kommentar?0:1;	# Kommentarflag toggle ...
			} else {					# Klammer allein ohne else -> immer Kommentarende
				$im_kommentar=0;
			}
			next;
		}
		next if ($im_kommentar);

		# Mehrzeilige Eintraege komplett einlesen...
		if ($line=~/\\\s*$/) {
			$line=$line.read_line($listref);
		}
		last;
	}
	return $line;
}

sub parse_rek
{
	my ($reklevel,$listref,$hashref,$callline,$prcfile)=@_;
	my ($_kl_auf,$_kl_zu,$line,$objekttyp,$objektname,$resname,$reswert);

	if ($callline) {
		$_kl_auf=$callline=~s/\[/\[/g;
		$_kl_zu=$callline=~s/\]/\]/g;
		if ($_kl_auf==$_kl_zu) {
			return;
		}
	}
	while (1) {
		$line=read_line($listref);
		last if (not $line);
		undef $objekttyp;
		undef $objektname;
		$kk_owil_prc_reader::kl_auf+=$line=~s/\[/\[/g;
		$kk_owil_prc_reader::kl_auf-=$line=~s/\]/\]/g;
		if ($line=~/^\s*(\w+)\s*\[/||$line=~/^\s*(\w+)\s+(\w+)\s*\[/) {
			# Neues Objekt beginnt...
			$objekttyp=$1;
			$objektname=$2;
			$objektname.=":".$kk_owil_prc_reader::objektcount if ($line=~/MSKDESCR/);
			if (not $$hashref{$objekttyp}{$objektname?$objektname:$objekttyp}) {
				$$hashref{$objekttyp}{$objektname?$objektname:$objekttyp}={"--ID"=>$kk_owil_prc_reader::objektcount++,"--FILE"=>$prcfile,"--OBJ"=>$objekttyp};
			} elsif ($objektname) {
				print sprintf("% ".(($reklevel+1)*2).".".(($reklevel+1)*2)."s","-")."WARNING: $objekttyp $objektname already in ",$$hashref{$objekttyp}{$objektname?$objektname:$objekttyp}{"--FILE"},"!\n";
			}
			parse_rek($reklevel,$listref,$$hashref{$objekttyp}{$objektname?$objektname:$objekttyp},$line,$prcfile);
		} elsif ($line=~/^\s*(\w+)\s*\=\s*(\S.*)/) {
			# Ressource gefunden...
			$resname=$1;
			$reswert=$2;
			$reswert=~s/\"//;
			$reswert=~s/(\"\s*)$//;
			if ($$hashref{$resname} && 
			($$hashref{"--OBJ"} eq "FOREIGNKEY")) {
				$resname.=sprintf("/%03d",++$$hashref{"--CNT"});
			}
			$$hashref{$resname}=$reswert;
			if ($resname eq "FileName") {
				read_prc($reklevel+1,$reswert);
			}
		} elsif ($line=~/^\s*\]\s*$/) {
			# abschliessende Klammer ...
			last;
		} else {
			print sprintf("% ".(($reklevel+1)*2).".".(($reklevel+1)*2)."s","-")."($kk_owil_prc_reader::kl_auf) $line --> ?????","\n";
		}
	}
}

sub read_prc
{
	my ($reklevel,$prcfile,$owil_define_list,$owil_include_list)=@_;
	my (@fl,$inc_index,$prcfilepath);

	if ($reklevel==0) {
		if ($owil_define_list) {
			undef $kk_owil_prc_reader::owil_define_list;
			$kk_owil_prc_reader::owil_define_list=".".join(".",@$owil_define_list).".";
		}
		if ($owil_include_list) {
			undef @kk_owil_prc_reader::owil_include_list;
			@kk_owil_prc_reader::owil_include_list=@$owil_include_list;
		}
	}
	print sprintf("% ".(($reklevel+1)*2).".".(($reklevel+1)*2)."s","-")."processing $prcfile...\n" if ($VERBOSE);
	if ($prcfile=~/\.mlc/i) {
		print sprintf("% ".(($reklevel+1)*2).".".(($reklevel+1)*2)."s","-")."file type not supported - skipped...\n" if ($VERBOSE);
		return;
	}
	$kk_owil_prc_reader::filecount++;
	$inc_index=0;
	$prcfilepath=$prcfile;
	for (; ;) {
		if (open(IN,"<$prcfilepath")) {
			@fl=<IN>;
			close IN;
			print sprintf("% ".(($reklevel+1)*2).".".(($reklevel+1)*2)."s","-")."reading done\n" if ($VERBOSE);
			$kk_owil_prc_reader::linecount+=$#fl;
			parse_rek($reklevel,\@fl,\%kk_owil_prc_reader::objekthash,undef,$prcfile);
			last;
		} else {
			print sprintf("% ".(($reklevel+1)*2).".".(($reklevel+1)*2)."s","-")."open failed: " if ($VERBOSE);
			if ($kk_owil_prc_reader::owil_include_list[$inc_index]) {
				$prcfilepath=$kk_owil_prc_reader::owil_include_list[$inc_index++]."/".$prcfile;
				print "trying $prcfilepath...\n" if ($VERBOSE);
			} else {
				warn "unable to open \"$prcfile\": $!\n";
				last;
			}
		}
	}
}

return 1;

__END__

Kurzes Beispiel: ---------------------

use kk_owil_prc_reader;
use Data::Dumper;
$Data::Dumper::Indent=1;

my @defines=("tsapp");
my @includes=("f:/its/rc","f:/its/ts/tsapp");

$kk_owil_prc_reader::VERBOSE=1;	# printing infos
print "reader Version=",$kk_owil_prc_reader::VERSION,"\n";
kk_owil_prc_reader::read_prc(0,$ARGV[0],\@defines,\@includes);
print Dumper(%kk_owil_prc_reader::objekthash);

---------------------
