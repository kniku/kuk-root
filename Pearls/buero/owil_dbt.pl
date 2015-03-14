#!/usr/local/bin/perl
#SKINF: OWIL PRC Parser
#AUTHOR: kuk
# Create Table Logik: owil/owsql/owcrttab.c

# ===========
# loading libs
# ===========
use strict;
use Getopt::Std;
use Data::Dumper;

my ($kl_auf,%objekthash,$objektcount,$linecount,$filecount);
$Data::Dumper::Indent=1;

# ===========
# subroutines
# ===========
sub eval_cmdl
{
	Getopt::Std::getopts('vhc:i:');
	if ((not $ARGV[0]) || $Getopt::Std::opt_h) {
		print <<endOfMsg;
usage: $0 [-h] [-cCREORA-FILE] [-iDESCR-FILE] prj_file [storage_file]
where -h ... this help
      -v ... verbose
      -c ... write create table file
      -i ... write info (database description) file
Generiert aus dem OWIL prc File eine Datenbankbeschreibung wie der DBT
endOfMsg
		die "\n";
	}
}

sub read_line
{
	my ($listref)=@_;
	my ($line,$im_kommentar,$flag);

	$im_kommentar=0;
	while (1) {
		$line=shift @$listref;
		last if (not $line);
		chomp $line;
		next if ($line=~/^\s*\#/||$line=~/^\s*$/);	# Kommentarzeile od. leere Zeile
		# OWIL: bedingte Bloecke: Hack: Der else Zweig wird genommen ...
		if ($line=~/^\s*\S+\s*\{/) {
			$im_kommentar=1;
		} elsif ($line=~/^\s*\}/) {
			$im_kommentar=0;
			next;
		}
		next if ($im_kommentar);
		# Mehrzeilige Eintraege komplett einlesen...
		if ($line=~/\\\s*$/) {
			$line=$line.read_line($listref);
		}
#		print ">>$line<<\n";
		last;
	}
	return $line;
}

sub parse_rek
{
	my ($listref,$hashref,$callline,$prcfile)=@_;
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
		$main::kl_auf+=$line=~s/\[/\[/g;
		$main::kl_auf-=$line=~s/\]/\]/g;
		if ($line=~/^\s*(\w+)\s*\[/||$line=~/^\s*(\w+)\s+(\w+)\s*\[/) {
			# Neues Objekt beginnt...
			$objekttyp=$1;
			$objektname=$2;
#			print ">>OBJEKT<<>>$objektname<<\n";
			if (not $$hashref{$objekttyp}{$objektname?$objektname:$objekttyp}) {
				$$hashref{$objekttyp}{$objektname?$objektname:$objekttyp}={"--ID"=>$main::objektcount++,"--FILE"=>$prcfile,"--OBJ"=>$objekttyp};
			} elsif ($objektname) {
				print "WARNING: $objekttyp $objektname already in ",$$hashref{$objekttyp}{$objektname?$objektname:$objekttyp}{"--FILE"},"!\n";
			}
			parse_rek($listref,$$hashref{$objekttyp}{$objektname?$objektname:$objekttyp},$line,$prcfile);
		} elsif ($line=~/^\s*(\w+)\s*\=\s*(\S.*)/) {
			# Ressource gefunden...
			$resname=$1;
			$reswert=$2;
#			print ">>$resname<<>>$reswert<<\n";
			$reswert=~s/\"//;
			$reswert=~s/(\"\s*)$//;
#			if ($resname eq "ITDB925") {
#				print Dumper($hashref);
#				print $$hashref{$resname},"\n";
#			}
			if ($$hashref{$resname} && 
			($$hashref{"--OBJ"} eq "FOREIGNKEY")) {
				$resname.=sprintf("/%03d",++$$hashref{"--CNT"});
#				print ">>>$resname - $reswert\n";
			}
			$$hashref{$resname}=$reswert;
			if ($resname eq "FileName") {
				read_prc($reswert);
			}
		} elsif ($line=~/^\s*\]\s*$/) {
			# abschliessende Klammer ...
			last;
		} else {
			print "($main::kl_auf) $line --> ?????","\n";
		}
	}
}

sub read_prc
{
	my ($prcfile)=@_;
	my (@fl);
	print "reading $prcfile...\n" if ($Getopt::Std::opt_v);
	$main::filecount++;
	if (open(IN,"<$prcfile")) {
		@fl=<IN>;
		close IN;
		$main::linecount+=$#fl;
		parse_rek(\@fl,\%main::objekthash,undef,$prcfile);
	} else {
		warn "unable to open \"$prcfile\": $!\n";
	}
}

sub read_dbt
{
	my ($file);
	$file=$main::objekthash{"OWDBT"}{"OWDBT"}{"FnameDbRc"};
	if ($file && -f $file) {
		read_prc($file);
	}
}

sub write_doc
{
	my ($ref,$k,$kk,$vv,$v,$x,$pk,$chash,@clist,@tlist,$x,$i,$fk);
	open(OUT,">$Getopt::Std::opt_i") or die "$!\n";
	$ref=$main::objekthash{"SQLTABLE"};
	if ($ref) {
		@tlist = sort keys %$ref;
		foreach $k (@tlist) {
			$v=$$ref{$k};
			$x=$$v{"TableName"}?$$v{"TableName"}:$k;
			print OUT "TABLE $k [$x] ",$$v{"Descr"};
			$x=$$v{"TableSpace"}?$$v{"TableSpace"}:"???";
			print OUT " TableSpace=$x";
			$x=$$v{"TableSpaceIndex"}?$$v{"TableSpaceIndex"}:"???";
			print OUT ", TableSpaceIndex=$x";
			$x=$$v{"Count"}?$$v{"Count"}:"???";
			print OUT ", Count=$x";
			print OUT " (\n";
			$pk=$$v{"PrimaryKey"};
			$fk=$$v{"FOREIGNKEY"}{"FOREIGNKEY"};
			$chash=$$v{"COLUMN"};
			@clist = sort { $$chash{$a}{"--ID"} <=> $$chash{$b}{"--ID"} } keys %$chash;
			foreach $kk (@clist) {
				$vv=$$chash{$kk};
				print OUT $pk=~/$kk/?"p":" ";
				$x=0;
				foreach $i (values %$fk) {
					if ($i=~/$kk/) {
						$x=1;
					}
				}
				print OUT $x?"f":" ";
				$x=$$vv{"Flags"};
				# Default=NOT NULL !
				print OUT $x=~/SQLCOL_TYPE_NULL/?" ":"n";
				print OUT " $kk";
				$x=$$vv{"Type"}?$$vv{"Type"}:$main::objekthash{"SQLCOLUMN"}{$kk}{"Type"};
				$x=~s/SQL_//;
				print OUT sprintf(" %-10s",$x);
				$x=$$vv{"Length"}?$$vv{"Length"}:$main::objekthash{"SQLCOLUMN"}{$kk}{"Length"};
				print OUT sprintf(" (%3s",$x);
				$x=$$vv{"Scale"}?$$vv{"Scale"}:$main::objekthash{"SQLCOLUMN"}{$kk}{"Scale"};
				if ($x) {
					print OUT sprintf(",%2s",$x);
				} else {print OUT "   ";}
				print OUT " )";
				$x=$$vv{"Descr"}?$$vv{"Descr"}:$main::objekthash{"SQLCOLUMN"}{$kk}{"Descr"};
				print OUT " $x,";
				print OUT "\n";
			}
			print OUT ");\n\n";
		}
	}
	close OUT;
}

sub switch_type
{
	my ($type,$length,$scale)=@_;
#	return $type;
	$type=~s/SQL_//;
	$type="VARCHAR2" if $type eq "VARCHAR";
	$type="NUMBER" if $type eq "DECIMAL";
	undef $scale if $type eq "CHAR";
	if ($type eq "NUMERIC") {
		$type="NUMBER";
		$scale=0 if !defined $scale;
	}
	if ($type eq "INTEGER") {
		$type="NUMBER" ;
		$length=10;
		$scale=0;
	}
	# Hack: Hier musste eine Datenbank bzgl. UseStringDate=0/1 befragt werden
	if ($type eq "DATE") {
		$type="CHAR";
		$length=8;
	} elsif ($type eq "TIME") {
		$type="CHAR";
		$length=6;
	} elsif ($type eq "TIMESTAMP") {
		$type="CHAR";
		$length=14;
	}
	
	if ($length) {
		$type.="($length";
		$type.=",$scale" if defined $scale;
		$type.=")";
	}
	return $type;
}

sub write_creora
{
	my ($ref,$k,$kk,$vv,$v,$x,$pk,$chash,@clist,@tlist,$x,$i,$fk,$colnum);
	open(OUT,">$Getopt::Std::opt_c") or die "$!\n";
	$ref=$main::objekthash{"SQLTABLE"};
	if ($ref) {
		@tlist = sort keys %$ref;
		foreach $k (@tlist) {
			$v=$$ref{$k};
			next if $$v{"DontCreate"}==1;
#			$x=$$v{"TableName"}?$$v{"TableName"}:$k;
			print OUT "CREATE TABLE \"$k\"\n\t(";
			$pk=$$v{"PrimaryKey"};
			$chash=$$v{"COLUMN"};
			@clist = sort { $$chash{$a}{"--ID"} <=> $$chash{$b}{"--ID"} } keys %$chash;
			$colnum=0;
			foreach $kk (@clist) {
				print OUT ",\n\t" if $colnum>0;
				$colnum++;
				$vv=$$chash{$kk};
				print OUT "\"$kk\" ";
				$x=$$vv{"Type"}?$$vv{"Type"}:$main::objekthash{"SQLCOLUMN"}{$kk}{"Type"};
				$x=switch_type($x,$$vv{"Length"}?$$vv{"Length"}:$main::objekthash{"SQLCOLUMN"}{$kk}{"Length"},$$vv{"Scale"}?$$vv{"Scale"}:$main::objekthash{"SQLCOLUMN"}{$kk}{"Scale"});
				print OUT $x;
				$x=$$vv{"Flags"};
				# Default=NOT NULL !
				print OUT $x=~/SQLCOL_TYPE_NULL/?"":" NOT NULL";
			}
			print OUT ",\n\tCONSTRAINT PK_$k PRIMARY KEY\n\t($pk));\nCOMMIT;\n\n";
		}
		# Constraints anlegen ...
		foreach $k (@tlist) {
			$v=$$ref{$k};
			$fk=$$v{"FOREIGNKEY"}{"FOREIGNKEY"};
			if ($fk) {
				@clist = keys %$fk;
				foreach $i (@clist) {
					next if $i=~/^--/;
					undef $x;
					if ($i=~/\/\d+/) {
						($pk,$x)=$i=~/(.*)\/(\d+)/;
					} else {$pk=$i}
					$$fk{$i}=~/(.*):(.*)/;
					print OUT "ALTER TABLE \"$k\"\n\t ADD CONSTRAINT $k"."_$pk$x\n\tFOREIGN KEY($1)\n\tREFERENCES $pk ($2);\nCOMMIT;\n\n";
				}
			}
		}
	}
	close OUT;
}

# ===========
# MAIN Program
# ===========
eval_cmdl();
read_prc($ARGV[0]);
read_dbt();
read_prc($ARGV[1]) if ($ARGV[1]);
print "Files:   $main::filecount\n" if ($Getopt::Std::opt_v);
print "Lines:   $main::linecount\n" if ($Getopt::Std::opt_v);
print "Objects: $main::objektcount\n" if ($Getopt::Std::opt_v);
#print Dumper(%main::objekthash);
if ($Getopt::Std::opt_i) {
	write_doc() ;
	print "Description ($Getopt::Std::opt_i) generated\n";
}
if ($Getopt::Std::opt_c) {
	write_creora();
	print "create table stmts ($Getopt::Std::opt_c) generated\n";
}

__END__