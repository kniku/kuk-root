#!/usr/local/bin/perl

use Tk;
use Cwd;
require "find.pl";

sub wanted {
	if (
		$name=~/\/\D\d+$/||				# X1234 .. uemacs
		uc($name)=~/\.BAK$/||			# *.bak .. bak-files
		$name=~/\/\.#.*\d+\.\d+/||		# #.*N.NN  cvs (merge)
		$name=~/~$/||					# *.*~ ... emacs
		$name=~/sqlnet.log/||			# oracle fehlerlogs
		$name=~/\/#.*#$/				# #*.*# .. nt-emacs
		) {		
			$list->insert($I++,$name);
    	}
}

sub remove {
	my $F,$J=0,$MAX=$list->index('end');
	while ($J<$MAX) {
		$F=$list->get($J++);
		$list->delete($J-1);
		if (unlink $F) {
			$F=$F." ... ok.";
		} else {
			$F=$F." ... fehler!.";
		}
		$list->insert($J-1,$F);
	}
}

sub listclick {
	$list->deleteSelected();
}

sub scan {
	$b1->configure(-cursor=>watch);
	$I=0;
	$list->delete(0,'end');
	&find($e1->get());
	$b1->configure(-cursor=>arrow);
}

$E1=cwd;

$top=MainWindow->new;
$top->title("Xdelbak");
$p1=$top->Frame->pack(-fill=>x);
$p1->Label(-text=>"directory:")->pack(-side=>left);
$e1=$p1->Entry(-textvariable=>\$E1)->pack(-side=>right,-expand=>yes,-fill=>x);
$list=$top->Scrolled("Listbox",-width=>40,-height=>20,-setgrid=>1,-scrollbars=>e)->pack(-fill=>both,-expand=>yes);
$list->bind("<Button-1>"=>sub {listclick});
$p1=$top->Frame->pack(-fill=>x);
$p1->Button(-text=>"exit",-command=>sub {exit 0})->pack(-side=>right,-fill=>x,-expand=>yes);
$p1->Button(-text=>"remove",-command=>sub {remove})->pack(-side=>right,-fill=>x,-expand=>yes);
$b1=$p1->Button(-text=>"scan",-command=>sub {scan})->pack(-side=>right,-fill=>x,-expand=>yes);
$e1->focus;
$e1->SetCursor('end');

#&find('.');
MainLoop;
