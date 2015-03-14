
use File::Find;

sub wanted {
	# falls keine rekursive verzeichnisabarbeitung erwuenscht:
	# 1, sonst 0 am anfang der if klausel...
	if (0 && not $File::Find::dir eq ".") {	
      	$File::Find::prune = 1;
      	return;
	}
	# spezifischer filefilter (hier nur Test auf File)
	if (-f $File::Find::name) {
		push @LIST,$File::Find::name;
	}
}

File::Find::find({wanted=>\&wanted,no_chdir=>1},'.');

foreach $EMF (@LIST) {
	#split Pfad<->File
	($path,$file)=$EMF=~/(.*[\\\/])(.*?)$/;
	print "-----------[$path$file]-----------\n";
}
