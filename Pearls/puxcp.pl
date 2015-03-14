
@ARGV = grep { -f && -T } glob("@ARGV");

foreach $e (@ARGV) {
	print $e,"\n";
	system "copy $e x.~x~";
	system "uxcp x.~x~";
	system "copy x.~x~ $e";
	unlink "x.~x~";
}
