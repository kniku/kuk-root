
use Getopt::Std;
Getopt::Std::getopts('b:p:');

use strict;

use Win32::OLE;
use Win32::OLE::Const 'Microsoft Word';

my $Word = Win32::OLE->new('Word.Application');

$Word->{'Visible'} = 0; 
$Word->Documents->Add;

my $build=$Getopt::Std::opt_b;
my $patch=$Getopt::Std::opt_p;
my $begin=0;

print "Build: $build, Patch: $patch";

$Word->Documents->Open("v:\\mad\\update\\main.doc");
$Word->ActiveDocument->Content->Copy;
$Word->Documents("main.doc")->Close;
$Word->Selection->Collapse(Direction=>wdCollapseEnd);
$Word->Selection->Paste;

open(FH,"<update.txt");

while(<FH>) {

	chomp($_);
	if(/### ab build $build ###/) {
		if ($begin==1) {
			$begin=0;	
		}
		else {
			$begin=1;
			$build++;
		}
	}
	if($begin==1&&!/^\*/) {
		if(/U#(\d{1,3})#/) {
			if(-e "U$1.doc") {
				$Word->Documents->Open("v:\\mad\\update\\U$1.doc");
				$Word->ActiveDocument->Content->Copy;
				$Word->Documents("U$1.doc")->Close;
				$Word->Selection->Collapse(Direction=>wdCollapseEnd);
				$Word->Selection->Paste;
			}
			else {
				$Word->ActiveDocument->Content->InsertAfter("\nU$1.doc existiert nicht !");
			}
		}
	}
}

close (FH);

$Word->{'Visible'} = 1; 
