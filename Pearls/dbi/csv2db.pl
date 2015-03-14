#!/usr/local/bin/perl

use DBI;
use Getopt::Std;

#default settings...
$driver="Oracle";
$connect="ux01";
$user="zoso";
$pwd="zoso";
$mode="rollback";

Getopt::Std::getopts('ahvw:u:p:c:d:m:');
if ($opt_h||!$ARGV[0]) {
	print <<endOfMsg;
fetch some tables from a database into CSV files...
usage: $0 [-hva] [-u user] [-p pwd] [-c connect] [-d driver] [-w where_clause] [-m mode] table_list
where -v ... verbose
      -a ... append to existing files
      -w ... where clause
             stuff the '%' character, e.g. "firma=100 and blgnr like '%%10'"
      -d ... DB driver (def.=$driver)
      -u ... DB user (def.=$user)
      -p ... DB pwd (def.=$pwd)
      -c ... connect string (def.=$connect)
      -m ... mode: commit or rollback (def.=$mode)
      -h ... this help (ask kuk)...
endOfMsg
	die "\n";
}

@tables=@ARGV;

$driver=$opt_d if ($opt_d);
$connect=$opt_c if ($opt_c);
$user=$opt_u if ($opt_u);
$pwd=$opt_p if ($opt_p);
$mode=$opt_m if ($opt_m);

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
	$sth=$dbh_csv->prepare("select * from $table".($opt_w?" where $opt_w":""));
	next if (!$sth);
	$sth->execute;
	undef $cnt; undef $init_done;
	while (@row=$sth->fetchrow_array) {
		if (not $init_done) {
			$i=0;$qm="";$init_done=1;
			while ($sth->{NAME}->[$i]) {
				$qm.=($qm?",":"")."?";
				$i++;
			}
			print "$qm \n";
		}
		$dbh->do("INSERT INTO $table VALUES ($qm)", undef,@row);
		$cnt++;
		print "\rrows: $cnt";
	}
	print "\n";
}

# disconnect
print "$mode transaction...\n";
$dbh->commit if ($mode eq "commit");
$dbh->rollback if ($mode eq "rollback");
print "DB connection closed, result=",$dbh->disconnect,"\n";
print "CSV connection closed, result=",$dbh_csv->disconnect,"\n";

