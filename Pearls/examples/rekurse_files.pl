
use File::Find;
use Getopt::Std;

sub wanted {
	if ($File::Find::name=~/(test*)/i&&
		-f $File::Find::name) {
		push @LIST,$File::Find::name;
	}
}

Getopt::Std::getopts('h');

File::Find::find({wanted=>\&wanted,no_chdir=>1},'.');

foreach $EMF (@LIST) {
	print "-----------[$EMF]-----------\n";
	if (open(IN,"<$EMF")) {
		@f=<IN>;
		close IN;
		if (open(OUT,">$EMF")) {
			foreach $e (@f) {			
				print OUT $e;
			}
			close(OUT);
		}
	} else {
		warn "reading $EMF: $!\n";
	}
}
