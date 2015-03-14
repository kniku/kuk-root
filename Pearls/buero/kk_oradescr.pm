#!/usr/bin/perl

package kk_oradescr;

# Library Version
$kk_oradescr::VERSION="01.000";

# Einlesen einer mit oradescr.pl erstellten Beschreibung in einen Hash
# Liefert eine Hashreferenz zurueck.
# Aufruf: parse_descr_file_into_hash(filename)
# ---------------------------------------------------------------- 
sub parse_descr_file_into_hash {
# ---------------------------------------------------------------- 
	my ($FILENAME)=@_;
	my %h,$table,$tabcnt;
	if (open(IN,"<$FILENAME")) {
		while (<IN>) {
			if (/Struktur von (\w+)/) {
				$table=$1;
				$h{$table}{"-columns"}=0;
				$h{$table}{"-index"}=++$tabcnt;
				$h{$table}{"-name"}=$table;
			} else {
				if ($table&&/^(\w+);\s*(\w+)\((\d*),(\d*),(\d*)\);\s*(\w)\s*$/o) {
					$h{$table}{"-columns"}++;				
					$h{$table}{"-descr"}{$1}={"-type"=>$2,
											"-name"=>$1,
											"-index"=>$h{$table}{"-columns"},
											"-width"=>$3,
											"-length"=>$4,
											"-scale"=>$5,
											"-null"=>$6};
				}
			}
		}
		close IN;
	} else {
		warn "$FILENAME nicht gefunden!\n";
	}
	return \%h;
}

# Durchlaufen aller Tabellen
# Aufruf: recurse_tables(\%tableliste_aus_parse_descr_file_into_hash,calldata)
# callbackformat: cb(\%tablehash,calldata)
# ---------------------------------------------------------------- 
sub recurse_tables {
# ---------------------------------------------------------------- 
	my ($rh,$CALLBACK,$CALLDATA)=@_;
	return 0 if (!$rh||!$CALLBACK);
	foreach $k (sort {$$rh{$a}{"-index"} <=> $$rh{$b}{"-index"}} keys %$rh) {
#		print $k," (",$$rh{$k}{"-index"},")\n";
		$CALLBACK->($$rh{$k},$CALLDATA);
	}
}

# Durchlaufen aller Spalten einer Tabelle
# Aufruf: recurse_columns(\%tablehash_aus_recurse_tables,calldata)
# callbackformat: cb(\%columnhash,calldata)
# ---------------------------------------------------------------- 
sub recurse_columns {
# ---------------------------------------------------------------- 
	my ($rh,$CALLBACK,$CALLDATA)=@_;
	return 0 if (!$rh||!$CALLBACK);
	$rh=$$rh{"-descr"};
	foreach $k (sort {$$rh{$a}{"-index"} <=> $$rh{$b}{"-index"}} keys %$rh) {
#		print $k," (",$$rh{$k}{"-index"},")\n";
		$CALLBACK->($$rh{$k},$CALLDATA);
	}
}

1;	# package return...

__END__

Kurzes Beispiel: ---------------------

use kk_oradescr;

sub cb_col {
	my ($rh,$calldata)=@_;
	print "\t",$$rh{"-name"},",\t",$$rh{"-type"},"\n";
}

sub cb_tab {
	my ($rh,$calldata)=@_;
	print $$rh{"-name"},"\n";
	kk_oradescr::recurse_columns($rh,\&cb_col);
}

$h=kk_oradescr::parse_descr_file_into_hash("dbdescr.out");
kk_oradescr::recurse_tables($h,\&cb_tab);

---------------------
