require HTTP::Request;
require LWP::UserAgent;

die "Aufruf: translate [string]\nOnline šbersetzung ber http://dict.leo.org/\n" if (not $ARGV[0]);

$ua = LWP::UserAgent->new;
$request = HTTP::Request->new(GET => 'http://dict.leo.org/?search='.join(' ',@ARGV).'&searchLoc=0&relink=on&deStem=standard&lang=de');
$ua->proxy(['http', 'ftp'], 'http://proxy.ecofinance.com:8080/');
$response = $ua->request($request);

if ($response->is_success&&$response->content=~/\<\/STRONG\>      \<TABLE(.*)\<\/TABLE\>/) {
	$line=$1;$last_kind="f";
	while ($line=~/search\=(.*?)\&p\=\/(\w)/g) {
		print ($2 eq "g"?" => ":"\n") if ($last_kind ne $2);
		print "$1 ";
		$last_kind=$2;
	}
}
