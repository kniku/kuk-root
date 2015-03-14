#!/usr/local/bin/perl
#use lib '/big/logins/kkurt/Pearls/lib';
use lib 'f:/kuk/pearls/lib';
use kktools;
use Cwd;
 
$START_DIR=".";			# Startverzeichnis der rekursiven Dateisuche...
$CREATE_BACKUPS=1;		# Flag, ob Backups erwuenscht...
$EXT_COPY=".backup";	# backup extension
$VERBOSE=1;				# Statusmeldungen erwuenscht...?

$fCOUNT=0,$fCOUNT_MCH,$fCOUNT_CHG=0,$fCOUNT_OK=0,$fCOUNT_ERR=0;
$lCOUNT=0,$fCOUNT_MCH,$lCOUNT_CHG=0,$lCOUNT_OK=0,$lCOUNT_ERR=0;

# IN:  Verzeichnis, Filename (Strings)
# OUT: 1/0 (file matcht/matcht nicht
sub file_filter {
	my ($DIR,$FILE)=@_;
	my $rv=0;
	
	if ($FILE=~/\.y$/) {
		$rv=1;
	}
	return $rv;
}

sub cb_file {
	my ($DIR,$FILE,$CALLDATA)=@_;
	my $RVCP,$LINE;

	$fCOUNT++;
	
	# file filter...
	if ($FILE!~/$EXT_COPY$/&&	# keine backups backupen...
		file_filter($DIR,$FILE)) {
		$FILE="$DIR/$FILE";
		print "processing $FILE...\n" if $VERBOSE;

		$fCOUNT_MTC++;
		$fCOUNT_CHG++;

		# erzeuge eine sicherungskopie...
		$RVCP=rename $FILE,$FILE.$EXT_COPY;
		
		if ($RVCP!=0) {
			print " - $FILE backup'd to $FILE$EXT_COPY ...\n" if $VERBOSE&&$CREATE_BACKUPS;
			# backup Ok. bearbeite file...
			if (open (IN,"<$FILE$EXT_COPY")) {
				if (open (OUT,">$FILE")) {

					# zeilenweises abarbeiten/modifizieren...

					while (<IN>) {
						$LINE=$_;
						$lCOUNT++;
						# user processing goes here ...
						$lCOUNT_MTC++;
						$lCOUNT_OK++;
						$lCOUNT_CHG++;
						# end user processing ...
						print OUT "$LINE";
					}

					close OUT;
					print " - $FILE: fertig...\n" if $VERBOSE;
					$fCOUNT_OK++;
				} else {
					print " - $FILE: konnte nicht schreibend geoeffnet werden!\n";
					$fCOUNT_ERR++;
				}
				close IN;
			} else {
				print " - $FILE$EXT_COPY: konnte nicht lesend geoeffnet werden!\n";
				$fCOUNT_ERR++;
			}
			if (!$CREATE_BACKUPS) {
				unlink $FILE.$EXT_COPY;
			}
		} else {
			print " - $FILE: backup konnte nicht erzeugt werden!\n";
			$fCOUNT_ERR++;
		}
	} else {
		print "skipped ($FILE)...\n" if $VERBOSE;
	}
	
}

sub cb_dir {
	my ($DIR,$CALLDATA)=@_;
	kktools::walk_files($DIR,\&cb_file,$CALLDATA);
}

$CALLDATA=join(" ",@ARGV);

kktools::recurse_dir($START_DIR,\&cb_dir,$CALLDATA);
print "result :\ttotal\tmatch\tchanges\tsuccess\terrors\n";
print "files  :\t$fCOUNT\t$fCOUNT_MTC\t$fCOUNT_CHG\t$fCOUNT_OK\t$fCOUNT_ERR\n";
print "lines  :\t$lCOUNT\t$lCOUNT_MTC\t$lCOUNT_CHG\t$lCOUNT_OK\t$lCOUNT_ERR\n";

