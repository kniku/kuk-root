#!/usr/local/bin/perl
#SKINF: generiert eine Datenbankbeschreibung ...
#AUTHOR: kuk

use Getopt::Std;

$TEMPFILE="_temp.sql";			# temp. file, wird wieder geloescht
$DEFOUT="dbdescr.out";			# default output file
$DEFSQLOUT="dbdescr.sql";		# default sql output file (opt_o)

Getopt::Std::getopts('kdths:f:ceo');

if (!$opt_s||$opt_h) {
	print <<endOfMsg;
usage: $0 -s CONNECTSTRING [-hkctdeo] [-fFILE]
where -c ... count rows
      -t ... tables only (without description/foreign keys)
      -d ... tables with description (without foreign keys)
      -k ... tables plus foreign keys only
      -e ... columns in DDL order (NOT ordered by column_name)
      -o ... generate SQL script only (\"$DEFSQLOUT\" or to -f...)
creates a description of given database user (oracle only)
output in FILE or \"$DEFOUT\" (default)
endOfMsg
	die "\n";
}

$SQLPLUS="sqlplus -silent "."$opt_s";
$GET_COUNT=$opt_c?1:0;
$SPOOLFILE=$opt_f?$opt_f:"$DEFOUT";	# eigendlicher output ...
$DEFSQLOUT=$opt_f if $opt_f;

open (OUT,">$TEMPFILE") or die "dateifehler beim erzeugen von '$TEMPFILE'  ...\n";
print OUT "set termout off\nalter session set nls_sort=GERMAN;\nset pagesize 20000\nset lines 60\nspool $SPOOLFILE\nselect tname,tabtype from tab order by tname;\n";
print OUT "spool off\nrollback;\nexit\n";
close OUT;

print "===> Holen der Tabellenliste ... ";
system("$SQLPLUS \@$TEMPFILE");
print "Fertig.\n";

if (!$opt_t) {
	open (IN,"<$SPOOLFILE") or die "dateifehler beim oeffnen von '$SPOOLFILE' (sqlplus output) ...\n";
	open (OUT,">$TEMPFILE") or die "dateifehler beim erzeugen von '$TEMPFILE' ...\n";
	print OUT "set termout off\nalter session set nls_sort=GERMAN;\nset pagesize 20000\nset lines 60\nspool $SPOOLFILE\nselect count(*)||' Tabellen/Views' as \"Anzahl der Objekte\"from tab;\nselect * from tab order by tname;\n";
	if (!$opt_k) {
		while (<IN>) {
			if (/(\w+)(\s*TABLE)/) {
				if ($GET_COUNT==1) {
					print OUT "select count(*)||' Zeilen gefunden ($1)' as \"Zeilen in $1\" from $1;\n";
				}
				print OUT "select rpad(column_name||';',20,' ')||rpad(data_type||'('||Data_length||','||data_precision||','||data_scale||')'||';',20,' ')||nullable as \"Struktur von $1\" from user_tab_columns where table_name='$1' order by ".($opt_e?"column_id":"column_name").";\n";
			}
		}
	}
	print OUT "set lines 256\nselect constraint_name,constraint_type,table_name from user_constraints where constraint_type='R' order by constraint_name;\n" if !$opt_d;
	print OUT "spool off\nrollback;\nexit\n";
	close IN;
	close OUT;
	if (!$opt_o) {
		print "===> Auslesen der Datenbankbeschreibung ($SPOOLFILE) ... ";
		system("$SQLPLUS \@$TEMPFILE");
		print "Fertig.\n";
	}
}


if (!$opt_o) {
	open (OUT,">>$SPOOLFILE") or die "dateifehler beim schreiben auf '$SPOOLFILE' ...\n";
	$T=localtime;
	print OUT "\n== AUTO-GENERATED DATABASE DESCRIPTION OF $opt_s ($T)\n"; 
	print OUT "== FLAGS: ",$opt_e?"-e(EXACT)":"","\n"; 
	close OUT;
	unlink "$TEMPFILE";
} else {
	unlink "$SPOOLFILE";
	rename "$TEMPFILE", $DEFSQLOUT;
	print "===> $DEFSQLOUT generiert ... \n";
}


