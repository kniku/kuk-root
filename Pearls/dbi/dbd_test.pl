#!/usr/local/bin/perl

use DBI;

print 'file version=$Id: dbd_test.pl 27 2004-09-07 12:34:03Z kuk $'."\n";

print "dbi version=$DBI::VERSION\n";
print "supported drivers: ".join(",",DBI->available_drivers)."\n";

if (!$ARGV[3]) {
	print "usage: $0 driver [connect] [user] [pwd]\ne.g.: $0 Oracle ux01.zoso.at zoso zoso\n";
	print "possible connections via $ARGV[0]:\n",join("\n",DBI->data_sources($ARGV[0])),"\n" if ($ARGV[0]);
	die "\n";
}

$driver=$ARGV[0];
$connect=$ARGV[1];
$user=$ARGV[2];
$pwd=$ARGV[3];

# connect aufbauen...
$dbh=DBI->connect("DBI:$driver:$connect",$user,$pwd,{AutoCommit=>0});
print "driver info: ",$dbh->{Driver}->{Name},", ",$dbh->{Driver}->{Version},", ",$dbh->{Driver}->{Attribution},"\n";
if (not $dbh) {
   die "connect failed: $DBI::errstr\n";
} else {
    print "\"$driver\" connection to user \"$user\" at $connect open...\n";
}

# tables auslesen...
#@tables=$dbh->tables;
#print "tables:\n",join("\n",@tables),"\n";

# Beispiel: select...
print "assumption: ITS Database -> fetching some companies (itdb901)...\n";
print "---------------\n";
$sth=$dbh->prepare("select firma,bzeic from itdb901 where mcode=? and rownum<6");
$sth->execute("100");
while (@row=$sth->fetchrow_array) {
	print "@row\n";
}
print "---------------\n";

# Beispiel: insert...
#$sth=$dbh->prepare("insert into itdb901 (mcode,firma,bzeic) values (?,?,?)");
#$sth->execute("KUK","xxx","kurtis test");

$dbh->rollback;

# disconnect
print "connection closed, result=",$dbh->disconnect,"\n";
