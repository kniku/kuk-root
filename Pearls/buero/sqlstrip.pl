#!/usr/local/bin/perl
use Getopt::Std;
Getopt::Std::getopts('hs:i:o:d:');

# 0. check options, dump help msg...
if ($opt_h||!$opt_i||!$opt_s||!$opt_o) {
	print <<endOfMsg;
usage: $0 [-h] -sSQLFILE -iITDB.TXT -oSQLOUT [-dDIFF]
       verwirft alle sql Kommandos in SQLFILE bzgl. Tables, welche
       in ITDB.TXT nicht vorkommen. Ergebnis in SQLOUT.
       Im (opt.) DIFF werden die verworfenen Reste weggeschrieben.
       Die Ausgabe (log) vieleicht auf file umleiten 
       (1. sehr lang und 2. aufschlussreich) ...
endOfMsg
	die "\n";
}

# 1. parsing tables, create tlist ...
open F,"<$opt_i" or die "file error: $opt_i";
while (<F>) {
	push @tlist,$1 if (/^TABLE\s+(\S+)/);
}
close F;
print "tables found ($opt_i): $#tlist\n";

# 2. reading sql-file...
open F,"<$opt_s" or die "file error: $opt_s";
$B=join '',<F>;
close F;

# 3. find stmts, extract tablename(s) and look tlist for it...
# open needed files...
open F,">$opt_o" or die "file error: $opt_o";
if ($opt_d) {
	open D,">$opt_d" or die "file error: $opt_d";
	print D "rem $0 created diff-file. src=[$opt_s], target=[$opt_o]\nrem =====================================\n";
}
print F "rem $0 created file. src=[$opt_s]\nrem =====================================\n";
while ($B=~/(^.*?)(^\s*(update|alter|select|drop|create|insert|delete).*?\;\s*?\n)/sgmic) {
	print "prefix=[$1]\nstmt=[$2]\n";
	$prefix=$1;	$stmt=$2;
	# try to extract the tablename from the various types of stmts...
	# don't remove 'unknown' stmts...
	$match="without check"; $table="?";
	if ($stmt=~/^\s*(update\s+|select.*from\s+|alter\s+table\s+|
				drop\s+table\s+|create\s+table\s+|insert\s+into\s+|
				delete\s+from\s+)"??(\w+)"??/ix) {
		$table=$2;
		# correct tmp-table stuff (*temp, *_t)
		print "table=[$table]";
		if ($table=~/(.*?)(temp|_t)/i) {
			$table=$1;
		}
		print "==>[$table]\n";
		# check tablename found against tlist...
		$match=0;
		foreach (@tlist) {
			if ($table=~/\b$_\b/i) {
				$match=$_;
				last;
			}
		}	
	}
	if ($match) {
		# table ok. write all the stuff...
		print "table [$table] match with [$match]\n";
		print F "$prefix$stmt";
		$CMATCH++;
	} else {
		# ambiguous stmt. write prefix without the stmt...
		print "table [$table] not found\n";
		print F "$prefix";
		print D "$stmt\n";
		$CNOT++;
	}
}
# if there's something left from the last pattern matching -
# - write it out...
$B=~/\G(.*$)/sg;
print F $1;

close F;
close D;

print "------------------------\nmatch: $CMATCH\nkills: $CNOT\n";
