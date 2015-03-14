
use Getopt::Std;

Getopt::Std::getopts('td:');

$tmpfile="__x.txt";

die "no CVS directory found!\n" if (not (-d "cvs" || -d "CVS"));
print "\ninvoking cvs, wait ...\n";
print "\r===============================================================================\n";

if (!$opt_t && !$opt_d) {
	$std_call=1;
}

if ($std_call) {
	system("cvs status 2>nul 1> $tmpfile");
	open(IN,"<$tmpfile");
	@lines=<IN>;
	close IN;
	unlink $tmpfile;
		
	foreach $e (@lines) {
		if ($e=~/File: (.*\S+)\s+Status: (\S.*)/) {
			print_last();
			$file=$1;$status=$2;
		}
		if ($e=~/Working revision:\s+(\S+)/) {
			$wrev=$1;
		}
		if ($e=~/Repository revision:\s+(\S+)\s+(.*)/) {
			$rrev=$1;$path=$2;
		}
	}
	print_last();
} elsif ($opt_t) {
	print "Existierende Tags in dieser Sandbox:\n";
	print "-------------------------------------------------------------------------------\n";
	chdir($ENV{"ITSROOT"}) or die "ITSROOT nicht definiert!\n";
	system("cvs status -v its.txt 2>nul 1> $tmpfile");
	@lines=read_tmp_file();
	foreach $e (@lines) {
		if ($e=~/Existing Tags/) {
			$taglist_found=1;
		} elsif ($taglist_found && $e=~/\s+(\S+)\s+\(/) {
			push @lines2,$1;
		}
	}
	foreach $e (sort {$a cmp $b} @lines2) {
		print "$e\n";
	}
} elsif ($opt_d) {
	print "files mit Aenderungen seit $opt_d:\n";
	print "-------------------------------------------------------------------------------\n";
	system("cvs diff -r$opt_d --brief 2>nul 1> $tmpfile");
	@lines=read_tmp_file();
	foreach $e (@lines) {
		chomp $e;
		if ($e=~/RCS file\:\s*(\S+)/) {
			$x=$1;
			$x=~s/.*its\///;
			$x=~s/,v//;
		} elsif ($e=~/--brief (.*)/) {
			push @lines2,$x."\t".$1;
		}
	}
	foreach $e (@lines2) {
		print "$e\n";
	}
}

print "===============================================================================\n";

sub print_last
{
	if ($file&&$status ne "Up-to-date") {
		($shortpath)=$path=~/.*\/(\w+\/)/;
		printf("%-20s$status\t(rev.:$wrev, rep. rev.:$rrev)\n",$shortpath.$file." ");
		undef $file;
	}
}

sub read_tmp_file
{
	my @lines;
	open(IN,"<$tmpfile");
	@lines=<IN>;
	close IN;
	unlink $tmpfile;
	return @lines;
}
