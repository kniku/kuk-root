
use File::Find;
die "usage: chk_unicode [base_dir]\n" if ($#ARGV < 0);
$dir = $ARGV[0];

#find(\&callback, $dir);
find({ wanted => \&callback}, $dir);
sub callback() {
	if (-d) {
		print "processing directory: ",$File::Find::dir," ...\n";
	}
	return unless -f and /\.[ch]\s*$/;
#	print "Dir is $File::Find::dir, File name is $_\n\tFull path is $File::Find::name\n";
	chk_file($_);
}

sub chk_file() {
	my $f = shift;
	open IN, "<$f";	my @lines = <IN>; close IN;
	print "  ",$f,", ",$#lines," lines...";
	$content = join("", @lines);
	$xcontent=$content;
#	foreach $line (@lines) {
#		print $line;
#	}
#	print $content;
	print "\n";
	print "    memXXX/sizeof check: ";
	my $failures = 0;
#	while ($content =~ /\b(MemAlloc|MemAllocate|memcpy|memset)\b.*?\((.*?)\)/gs) {
	while ($content =~ /\b(MemAllocate)\b.*?\((.*?)\)/gs) {
		$x = $2;
		
		$pos = length($`);
		$lc = 0;
		$lc++ while ($xcontent =~ /$/s and length($`) < $pos);
		print "LINE: ".$lc;
#		print "[".length($`)."/".length($&)."/".length($')."]";
#		print "XXX".substr($content, length($`), 25)."XXX\n";
		
		
		$failures++ if not $x =~ /sizeof/;
	}
	if ($failures) { print "FAILED (".$failures." times)\n"; } else { print "ok\n"; }

}