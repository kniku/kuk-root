die "usage $0 [filepattern]\n" if !$ARGV[0];
foreach (grep {-f && (-M $_<=3)} glob("@ARGV")) {
	if (open(IN,"<$_")) {
		for ($line=0;$line<100&&!eof(IN);$line++) {$buf[++$#buf]=<IN>;}
		close IN;
		foreach $eintrag (@buf) {
			if (!$wer) {($wer)=($eintrag=~/from:(.*)/i)};
			last if ($wer&&$wer!~/ecofinance\./i&&$wer!~/zoso\./i);
			if ($wer&&$eintrag=~/x-mailer:(.*)/i) {
				print "$_: Boesewicht=$wer\n" if ($1=~/outlook/i);
				last;
			}
		}
	}
	undef $wer;
	undef @buf;
}
