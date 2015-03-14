use lib "v:\\nt-tools\\perl\\lib\\site";
use Win32::AdminMisc;

#if (Win32::AdminMisc::GetUsers("$ARGV[0]","*",\@)) {
#	printf "%-20s    %s\n","Service","Description";
#	printf "%-20s    %s\n","=======","===========";
#	printf("%-20s -> %s\n",$y,$x) while (($x,$y)=each %h);
#} else {
#	print "GetServices failed for host [$ARGV[0]]...\n";
#}

Win32::AdminMisc::GetUsers($ARGV[0],0);
