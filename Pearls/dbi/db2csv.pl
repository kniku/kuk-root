#!/usr/local/bin/perl

use DBI;
use Getopt::Std;

#default settings...
$driver="Oracle";
$connect="ux07";
$user="itsakt";
$pwd="itsakt";

Getopt::Std::getopts('ahvw:u:p:c:d:');
if ($opt_h||!$ARGV[0]) {
	print <<endOfMsg;
fetch some tables from a database into CSV files...
usage: $0 [-hva] [-u user] [-p pwd] [-c connect] [-d driver] [-w where_clause] table_list
where -v ... verbose
      -a ... append to existing files
      -w ... where clause
             stuff the '%' character, e.g. "firma='100' and blgnr like '%%10'"
      -d ... DB driver (def.=$driver)
      -u ... DB user (def.=$user)
      -p ... DB pwd (def.=$pwd)
      -c ... connect string (def.=$connect)
      -h ... this help (ask kuk)...
endOfMsg
	die "\n";
}

@tables=@ARGV;

$driver=$opt_d if ($opt_d);
$connect=$opt_c if ($opt_c);
$user=$opt_u if ($opt_u);
$pwd=$opt_p if ($opt_p);

# Datenbankconnect aufbauen...
print "connecting to user \"$user\" at $connect, wait...";
$dbh=DBI->connect("DBI:$driver:$connect",$user,$pwd,{AutoCommit=>0});
if (not $dbh) {
   die "\nconnect failed: $DBI::errstr\n";
} else {
    print "\r\"$driver\" connection to user \"$user\" at $connect open...\n";
}

#CSV connect aufbauen...
$dbh_csv=DBI->connect("DBI:CSV:");
die "\n" if (!$dbh_csv);
print "CSV connection open...\n";

foreach $table (@tables) {
	print "--- fetching [$table] ...\n";
	$sth=$dbh->prepare("select * from $table".($opt_w?" where $opt_w":""));
	next if (!$sth);
	$sth->execute;
	undef $cnt; undef $csv_open;
	while (@row=$sth->fetchrow_array) {
		if (!$csv_open) {
			$i=0;$tabledesc=$qm="";
			while ($sth->{NAME}->[$i]) {
				if ($opt_v) {
					print $sth->{NAME}->[$i]," (",
						$dbh->type_info($sth->{TYPE}->[$i])->{TYPE_NAME},
						"[".($dbh_csv->type_info($sth->{TYPE}->[$i])?$dbh_csv->type_info($sth->{TYPE}->[$i])->{TYPE_NAME}:"???")."]",
						"(",$sth->{PRECISION}->[$i],",",$sth->{SCALE}->[$i],"))",
						$sth->{NULLABLE}->[$i]?", NULL":", NOT NULL",
						"\n";
				}
				$x=$dbh_csv->type_info($sth->{TYPE}->[$i]);
				if (!$x) {
					$x="INTEGER";
				} else {
					$x=$dbh_csv->type_info($sth->{TYPE}->[$i])->{TYPE_NAME}."(".$sth->{PRECISION}->[$i].")";
				}
				$tabledesc.=($tabledesc?",":"").$sth->{NAME}->[$i]." ".$x;
				$qm.=($qm?",":"")."?";
				$i++;
			}
			$dbh_csv->do("DROP TABLE $table") if (!$opt_a&&(-f $table));
			$dbh_csv->do("CREATE TABLE $table ($tabledesc)") if (not -f $table);
			$csv_open=1;
		}
		$dbh_csv->do("INSERT INTO $table VALUES ($qm)", undef,@row);
		$cnt++;
		print "\rrows: $cnt";
	}
	print "\n";
}

# disconnect
$dbh->rollback;
print "DB connection closed, result=",$dbh->disconnect,"\n";
print "CSV connection closed, result=",$dbh_csv->disconnect,"\n";

