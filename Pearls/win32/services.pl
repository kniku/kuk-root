use Win32::Service;

die "usage: $0 [host] [srevicename]\n" if (!$ARGV[0]);

if (Win32::Service::GetServices("$ARGV[0]",\%h)) {
	printf "%-20s    %s\n","Service","Description";
	printf "%-20s    %s\n","=======","===========";
	printf("%-20s -> %s\n",$y,$x) while (($x,$y)=each %h);
} else {
	print "GetServices failed for host [$ARGV[0]]...\n";
}

if ($ARGV[1]) {
	print "\nstatus of service \"$ARGV[1]\" on $ARGV[0]:\n"; 
	Win32::Service::GetStatus("$ARGV[0]","$ARGV[1]",\%status);
	printf("%-30s -> %s\n",$x,$y) while (($x,$y)=each %status);
}
