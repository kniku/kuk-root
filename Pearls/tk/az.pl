#!/usr/local/bin/perl

use Tk;
use Win32;

$test=0;

if ($test) {
	$ifile="e:\\tmp\\az-test.txt";
	$statfile="e:\\tmp\\azstat-test.txt";
	$timeout=1*1000;
} else {
	$ifile="F:\\kuk\\az-Skript-Daten\\az.txt";
	$statfile="F:\\kuk\\az-Skript-Daten\\azstat.txt";
	$timeout=15*1000;
}

# restart(1) -> kompletter restart
# restart(2) -> restart mit gleicher selektion
sub restart {
	$lastgeometry=$top->geometry;
	$lastgeometry=~s/\d+x\d+//;
	$again=shift;
	$top->destroy;
}

sub write_statline {
	my ($l,$msg)=@_;
	my $T=localtime;
	chomp($l);chomp($msg);
	open(IN,">>$statfile") or die "\n";
	print IN "$T: ",$l," ($msg)\n";
	close IN;
}

sub save_list {
	open(IN,">$ifile") or die "\n";
	print IN @lines;
	print IN "#eof";
	close IN;
}

# element callbacks
sub cb_timer {
	my $y=$$lastselection->cget('text');
	$acttime=int $acttime+$timeout/1000;
	$y=~s/\((\d+)\)/($acttime)/;
	$lasttimer=$$lastselection->after($timeout,[\&cb_timer]);
	$$lastselection->configure(text=>$y);
	$lines[$lastindex]=~s/\((\d+)\)/($acttime)/ if ($lastindex>=0);
}

sub cb_button {
	my ($w)=shift;
	my $y=$$w->cget('text');
	if ($lastselection!=$w) {
		$$lastselection->configure(relief=>raised,background=>gray,activebackground=>lightgray) if ($lastselection);
		$lasttimer->cancel() if ($lasttimer);
		$y=~/\((\d+)\)/;
		$acttime=int $1;
		$$w->configure(relief=>sunken,background=>lightgreen,activebackground=>lightgreen);
		$lastselection=$w;
		$lastindex=shift;
		$lasttimer=$$w->after($timeout,[\&cb_timer]);
	}
}

sub cb_done {
	my ($w,$i)=@_;
	my $l=$lines[$i];
	write_statline($l,"done");
	@lines=grep($_ ne $l,@lines);    
	save_list();
	restart(1);
}

sub cb_reset {
	my ($w,$i)=@_;
	write_statline($lines[$i],"reset");
	$lines[$i]=~s/\((\d+)\)/(0)/;
	save_list();
	restart(2);
}

sub cb_edit {
	$lasttimer->cancel();
	save_list();
	system("notepad $ifile");
	restart(1);
}

sub cb_edit_stat {
	system("start notepad $statfile");
}

#$lastgeometry="244x252+66+66";
$lastgeometry="+0+0";

do {

	$lasttimer=0;
	if (!defined $again || $again==1) {
		$lastindex=-1;
	}
	$lastselection=0;
	$acttime=0;
	$again=0;
	
	# read entire file ...
	open(IN,"<$ifile") or die "\n";
	@lines=grep(!/^\s*\#/,<IN>);
	close IN;
	
	# create UI
	$top=MainWindow->new;
	$top->title("Arbeitszeiten");
	$top->geometry($lastgeometry);

	$p1=$top->Frame->pack(-fill=>x);
	$p2=$p1->Frame->pack(-fill=>x,-side=>left);
	$p2->Label(-text=>"Data File:")->pack();
	$p2->Label(-text=>"Stat File:")->pack();

	$p2=$p1->Frame->pack(-expand=>1,-fill=>x,-side=>left);
	$p2->Entry(-textvariable=>\$ifile,-state=>disabled,-background=>lightgray)->pack(-expand=>1,-fill=>x);
	$p2->Entry(-textvariable=>\$statfile,-state=>disabled,-background=>lightgray)->pack(-expand=>1,-fill=>x);

	$p2=$p1->Frame->pack(-fill=>x,-side=>left);
	$p2->Button(-command=>[\&cb_edit],-text=>"-->Edit",-foreground=>blue,-activeforeground=>red)->pack(-fill=>x,-expand=>0);
	$p2->Button(-command=>[\&cb_edit_stat],-text=>"-->View",-foreground=>blue,-activeforeground=>red)->pack(-fill=>x,-expand=>0);

	$p1=$top->Frame->pack(-fill=>x);
	$i=0;
	foreach $f (@lines) {
		next if ($f=~/^\s*\#/||$f=~/^\s*&/);
		$e=$f;
		chomp($e);
		$p1=$top->Frame->pack(-fill=>x);
		$b=\$p1->Button(-text=>$e,-background=>gray,-activeforeground=>darkgreen)->pack(-side=>left,-fill=>x,-expand=>1);
		$$b->configure(command=>[\&cb_button,$b,$i]);
		$$b->invoke() if ($lastindex==$i);
		$b=\$p1->Button(-text=>"Save",-background=>lightgray,-foreground=>blue,-activeforeground=>red)->pack(-side=>left,-fill=>x,-expand=>0);
		$$b->configure(command=>[\&cb_reset,$b,$i]);
		$b=\$p1->Button(-text=>"Done",-background=>lightgray,-foreground=>blue,-activeforeground=>red)->pack(-side=>left,-fill=>x,-expand=>0);
		$$b->configure(command=>[\&cb_done,$b,$i]);
		$i++;
	}
	$p1=$top->Frame->pack(-fill=>x);
	$b=\$p1->Button(-text=>"Sonstiges (0)",-background=>lightgray,-activeforeground=>darkgreen)->pack(-side=>top,-fill=>x,-expand=>0);
	$$b->configure(command=>[\&cb_button,$b,-1]);
	$$b->invoke() if ($lastindex==-1);
	MainLoop;
	$lasttimer->cancel() if ($lasttimer);
	$loop++;
} while ($again);

save_list();
