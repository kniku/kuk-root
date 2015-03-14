use DBI;
use Getopt::Std;
use Data::Dumper;

#default settings...
# $connect=user/passwd\@[host]
$connect="system/manager\@ux07";
$tablespace="data";
$tempspace="temp";

sub new_user
{
	username_again:
	print "enter username: ";
	$new_user=<stdin>;
	chomp $new_user;
	return if ($new_user=~/^\s*$/);
	if ($new_user=~/\s+/||$new_user=~/\W+/) {
		print "invalid username!\n";
		goto username_again;
	}
	
	print "enter description of $new_user: ";
	$new_descr=<stdin>;
	chomp $new_desc;
	
	print ": creating new user \"$new_user\"\n";
	$sth=$dbh->prepare("create user $new_user identified by $new_user ".
					   "default tablespace $tablespace ".
					   "temporary tablespace $tempspace");
	$sth->execute;
	$sth=$dbh->prepare("grant unlimited tablespace,connect to $new_user");
	$sth->execute;
	print ": writing description\n";
	$new_user=uc($new_user);
	$sth=$dbh->prepare("insert into eco_users (username,created_from,descr) ".
					   "values ('$new_user','".($ENV{USERNAME}?$ENV{USERNAME}:$ENV{USER})."','$new_descr')");
	$sth->execute;
	$dbh->commit;
}

sub drop_user
{
	username_again:
	print "enter username: ";
	$new_user=<stdin>;
	chomp $new_user;
	
	return if ($new_user=~/^\s*$/);
	if ($new_user=~/.*sys.*/i) {
		print ": system user! unable to drop!\n";
		return;
	}
	print ": dropping user \"$new_user\", wait...";
	$sth=$dbh->prepare("drop user $new_user cascade");
	$sth->execute;
	$new_user=uc($new_user);
	$sth=$dbh->prepare("delete from eco_users where username='$new_user'");
	$sth->execute;
	$dbh->commit;
	print "\b\b\b\b\b\b\bdone.  \n";
}

sub list_user
{
	print sprintf("%-25s ","USER"),"created ",sprintf(" %-5s ","from"),"description","\n";
	print sprintf("%-25s ","========================="),"========",sprintf(" %-5s ","====="),"==============","\n";
	$sth=$dbh->prepare(
		"select b.username,nvl(to_char(a.created),'not existant'),b.created_from,b.descr from dba_users a,eco_users b".
		" where a.username(+)=b.username".
		" union".
		" select a.username,to_char(a.created),nvl(b.created_from,'?'),nvl(b.descr,'no description') from dba_users a,eco_users b".
		" where a.username=b.username(+) order by 1");
	$sth->execute;
	while (@row=$sth->fetchrow_array) {
		print sprintf("%-25s ",$row[0]),$row[1],sprintf(" %-5s ",$row[2]),$row[3],"\n";
	}
	$dbh->rollback;
}

Getopt::Std::getopts('hc:');
if ($opt_h) {
	print <<endOfMsg;
usage: $0 [-h] [-c connect]
where -h ... this help (ask kuk)...
      -c ... connect string (def.=$connect)
endOfMsg
	die "\n";
}

$connect=$opt_c if ($opt_c);
($user,$pwd,$server)=$connect=~/(\S+)\/(\S+)\@(\S*)/;

# Datenbankconnect aufbauen...
print "connecting to oracle on [$server] as \"$user\", wait...";
$dbh=DBI->connect("DBI:Oracle:$server",$user,$pwd,{AutoCommit=>0});
if (not $dbh) {
   die "\nconnect failed: $DBI::errstr\n";
} else {
    print "\b\b\b\b\b\b\bconnection open...\n";
}

again:
print "> ";
$cmd=<stdin>;
chomp $cmd;
if ($cmd eq "cre") {
	new_user();
} elsif ($cmd eq "drop") {
	drop_user();
} elsif ($cmd eq "l") {
	list_user();
} elsif ($cmd eq "q") {
	# disconnect
	$dbh->disconnect;
	print "DB connection closed.\n";
	exit;
} else {
	print "unknown command!\nenter 'cre', 'drop', 'l'(ist) or 'q'(uit) to leave...\n";
}
goto again;

__END__
create table eco_users 
(username varchar2(30),
 created_from varchar2(3),
 descr varchar2(100),
 constraint PK primary key (username));

select b.username,nvl(to_char(a.created),'not existant'),b.created_from,b.descr from dba_users a,eco_users b
where a.username(+)=b.username
union
select a.username,to_char(a.created),nvl(b.created_from,'?'),nvl(b.descr,'no description') from dba_users a,eco_users b
where a.username=b.username(+) order by 1;


