#!/usr/bin/perl
require 5.002;
use strict;
use Socket;
use Getopt::Std;
use Config;

sub umlaute4nt
{
	my $s=@_[0];
	if ($Config{'osname'}=~/mswin/i) {
		$s=~s/ö/\x94/g;
		$s=~s/ä/\x84/g;
		$s=~s/ü/\x81/g;
		$s=~s/Ö/\x99/g;
		$s=~s/Ä/\x8e/g;
		$s=~s/Ü/\x9a/g;
		$s=~s/ß/\xe1/g;
	}
	return $s;
}

my ($MAGIC_NUMBER,$VERSION,$line,$host, $port, $iaddr, $paddr, $proto,$user);
my ($timestamp,$mode,$pattern,$flag,$vname,$name,$telfa,$teldw,$telpr,$telp2,$thypr,$kubez,$gesch,$thyfa,$inout,$ts,$kfzkz,$info);

# Hier muss der ecoserv Host und Port eingetragen werden
# Defaulteinstellungen, mit den Parameteren -p PORT bzw. -s SERVER uebersteuerbar
$host = 'localhost';
$port = 7881;
$MAGIC_NUMBER="es3.5";
$VERSION="6.0";

Getopt::Std::getopts('p:s:f:ikeu:m:t:n');
$host=$Getopt::Std::opt_s if $Getopt::Std::opt_s;
$port=$Getopt::Std::opt_p if $Getopt::Std::opt_p;
$user=$Getopt::Std::opt_u if $Getopt::Std::opt_u;
$mode=$Getopt::Std::opt_m if $Getopt::Std::opt_m;
$timestamp=$Getopt::Std::opt_t if $Getopt::Std::opt_t;

$mode=$ARGV[0] if not $mode;	# old style call
$pattern=$Getopt::Std::opt_m?$ARGV[0]:$ARGV[1];

if (!$mode || ($mode ne "in" && $mode ne "out" && $mode ne "upnr" && $mode ne "info" &&
				$mode ne "qmi" && $mode ne "init" && $mode ne "pre-in" && $mode ne "pre-out")) {
	print <<endOfMsg;
	
$0, v$VERSION

USAGE: $0 [-s SERVER] [-p PORT] [-u USERNAME] [-m MODE] [-i] [-e] [-k] [-n] [-f Firma] [-t PrelogTimestamp] [in|out|upnr|info|qmi|pre-in|pre-out] [Info(max.30) oder Pattern(qmi)]
where -p ... ecoserv port (def=$port)
      -s ... ecoserv host (def=$host)
      -u ... login (def=from environment)
      -m ... mode
              in ... push IN entry
              out .. push OUT entry
              upnr . update number wanted
              info . req. ecoserv status (ping)
              qmi .. QM request
              init . force ecoserv to reread configuration

     flags for mode qmi:
      -i ... intern (employees only - filtered by full name or login)
      -e ... extern (customers only - filtered by full name)
      -k ... kuerzel (employees only - filtered by login)
      -f ... firma (customers only - filtered by company name)
      -n ... nummer (employees only - filtered by car number)
    
     flags for mode pre-in, pre-out:
      -t ... timestamp - prelog timestamp, format: YYYYMMDDhhmmss)
endOfMsg
	die "\n";
}

if (!$user) {
	$user=$ENV{"USER"}?$ENV{"USER"}:$ENV{"USERNAME"};
	die "unable to determine your login!\n" if not $user;
}

# sending request
$proto = getprotobyname('tcp');
$iaddr = inet_aton($host) or die "Kein Host: $host";
$paddr = sockaddr_in($port,$iaddr);
socket(SOCK, PF_INET, SOCK_STREAM, $proto) or die "socket: $!";
connect(SOCK,$paddr) or die "connect: $!";

if ($mode eq "upnr") {
	#UPN:uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu:
	send(SOCK,sprintf("$MAGIC_NUMBER:UPN:%-30s:",$user),0);
	while ($line = <SOCK>) {
		if ($line=~/^\s*(\d+)\s*$/) {
			print "Hallo ",$user,", \nDein Update-Key lautet: U\#",$line,"\#\n";
		}
	}
	close (SOCK) or die "close: $!"; 
} elsif ($mode eq "info") {
	#INF:uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu:
	send(SOCK,sprintf("$MAGIC_NUMBER:INF:%-30s:",$user),0);
	while ($line = <SOCK>) {
		print "[".$line."]\n";
		if ($line=~/\:(.*?)\:(.*?)\:(.*?)\:(.*?)$/) {
			print "Hallo ",$user,", \nthis is ecoserv V$1, uptime: $2 hours ($3 requests, $4 active threads) and still alive ...\n";
		}
	}
	close (SOCK) or die "close: $!"; 
} elsif ($mode eq "init") {
	#INI:uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu:
	send(SOCK,sprintf("$MAGIC_NUMBER:INI:%-30s:",$user),0);
	close (SOCK) or die "close: $!"; 
} elsif ($mode eq "qmi") {
	undef $flag;
	if ($Getopt::Std::opt_n) {
		$flag="I";
		$mode=$pattern;
		$pattern="";
	} else {
		$flag="E" if $Getopt::Std::opt_e;
		$flag="F" if $Getopt::Std::opt_f;
		$flag="I" if $Getopt::Std::opt_i;
		$flag="K" if $Getopt::Std::opt_k;
		$flag="A" if !$flag;
		$pattern=~s/\*/%/g;
		$pattern=~s/\?/_/g;
	}
	#QMI:uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu:I|F|K:Pattern(30):Firma(30)
	send(SOCK,sprintf("$MAGIC_NUMBER:QMI:%-30s:%s:%-30s:%-30s",$user,$flag, $pattern,($flag eq "F")?$Getopt::Std::opt_f:"?"),0);
	while ($line = <SOCK>) {
#		print "[$line]\n";
		if (($flag,$vname,$name,$telfa,$teldw,$telpr,$telp2,$thypr,$thyfa,$gesch,$kubez,$kfzkz,$inout,$ts,$info)=$line=~
			/^(.*?)\x02(.*?)\x02(.*?)\x02(.*?)\x02(.*?)\x02(.*?)\x02(.*?)\x02(.*?)\x02(.*?)\x02(.*?)\x02(.*?)\x02(.*?)\x02(.*?)\x02(.*?)\x02(.*?)$/) {
			if ($Getopt::Std::opt_n && $mode ne "" && $kfzkz !~ /$mode/i) { next; }
			print "-------------------------------------------------------------------------------\n";
			if ($flag eq "I") {
				$line=""; 
				if ($ts=~/\w+/) {
					$line="nicht " if $inout eq "OU";
					$line.="im Haus seit $ts";
				} else {
					$line="wahrscheinlich nicht im Haus";
				}
				print umlaute4nt("** $name, $vname: $kubez, $line\n");
				print "Notiz          : \"$info\"\n" if ($info=~/\w+/);
				print "Tel. Firma     : $telfa\n" if ($telfa=~/\w+/);
				print "Interne DW     : $teldw\n" if ($teldw=~/\w+/);
				print "Tel. Privat    : $telpr\n" if ($telpr=~/\w+/);
				print "Tel. Privat2   : $telp2\n" if ($telp2=~/\w+/);
				print "Privathandy    : $thypr\n" if ($thypr=~/\w+/);
				print "Firmenhandy    : $thyfa\n" if ($thyfa=~/\w+/);
				print "KFZ Kennzeichen: $kfzkz\n" if ($kfzkz=~/\w+/);
			} else {
				print umlaute4nt(">> ".($gesch eq "M"?"Hr.":($gesch eq "W"?"Fr.":"??"))." $name, $vname\n");
				print "Tel 1 : $teldw\n" if ($teldw=~/\w+/);
				print "Tel 2 : $telpr\n" if ($telpr=~/\w+/);
				print "Handy : $telp2\n" if ($telp2=~/\w+/);
				print "Fax   : $thypr\n" if ($thypr=~/\w+/);
				print "e-Mail: $thyfa\n" if ($thyfa=~/\w+/);
			}
		}
	}
	print "-------------------------------------------------------------------------------\n";
	close (SOCK) or die "close: $!"; 
} elsif ($mode eq "pre-in" or $mode eq "pre-out") {
	#<in l="LOGIN" [i=INFO] [pre="1"] [t="20081005121500"]/>
#	printf SOCK "<%s pre=\"1\" l=\"%s\" t=\"%s\"%s/>",
#			$mode eq "pre-in"?"in":"out",$user, $timestamp,
#			$pattern ? " i=\""+$pattern+"\"" : "";
	$line = sprintf("<%s pre=\"1\" l=\"%s\" t=\"%s\"%s/>",
			$mode eq "pre-in"?"in":"out",$user, $timestamp,
			($pattern ? " i=\"".$pattern."\"" : ""));
#	print $line;
	send(SOCK, $line,0);
	while ($line = <SOCK>) {
		printf "ES: [%s]", $line
	}
	close (SOCK) or die "close: $!"; 
} else {
	#ENT:uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu:I:K:text(30))
	$line = sprintf("$MAGIC_NUMBER:ENT:%-30s:%s:K:%-30.30s",
			$user,$mode eq "in"?"I":"O",$pattern?$pattern:" ");
	send(SOCK, $line,0);
	while ($line = <SOCK>) {
		printf "ES: [%s]", $line
	}
	close (SOCK) or die "close: $!"; 
}
