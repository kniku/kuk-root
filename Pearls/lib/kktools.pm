#!/usr/local/bin/perl

package kktools;
use Cwd;

# Library Version
$kktools::VERSION="01.001";

# default backup-file extension
# kann bei bedarf ueberschrieben werden.
$kktools::BAK_EXTENSION=".backup";

# liefert den Wochentag als string zum gegebenen wday (0-6)
# aufruf: wdayname($I)
# ---------------------------------------------------------------- 
sub wdayname {
# ---------------------------------------------------------------- 
	my %WDAY=(	0=>"Sonntag",
				1=>"Montag",
				2=>"Dienstag",
				3=>"Mittwoch",
				4=>"Donnerstag",
				5=>"Freitag",
				6=>"Samstag");
	if (defined($_[0])) {
		return $WDAY{$_[0]};
	}
}

# erfragen der backupextension
# ---------------------------------------------------------------- 
sub get_backup_extension {
# ---------------------------------------------------------------- 
	my $ext=$kktools::BAK_EXTENSION;
#	my $ext=$kktools::BAK_EXTENSION_DEF;
#	if ($kktools::BACKUP_EXT) {
		# overruling default...
#		$ext=$kktools::BACKUP_EXT;
#	}
	return $ext;
}

# erzeugt backup fuer gegebenen file.
# rv: 1....ok
#     0....skipped
#     -1...fehler
# ---------------------------------------------------------------- 
sub make_file_backup {
# ---------------------------------------------------------------- 
	my ($FILE)=@_;
	my $I,$ext=get_backup_extension;

	if ($FILE!~/$ext$/) {	# keine backups backupen...
		# erzeuge eine sicherungskopie...
		$I=system("cp $FILE $FILE$ext");
		if ($I==0) {
			# backup Ok.
			return 1;
		} else {
			print " - [$FILE]: backup [$FILE$ext] konnte nicht erzeugt werden!\n";
			return -1;
		}
	} else {
		print " - skipped [$FILE]...\n";
		return 0;
	}
}

# versuch geg. filenamen auf backup extension zu renamen...
# return wie make_backup_file ()
# ---------------------------------------------------------------- 
sub rename_to_backup {
# ---------------------------------------------------------------- 
	my ($FILE)=@_;
	my $I,$ext=get_backup_extension;

	if ($FILE!~/$ext$/) {	# keine backups backupen...
		# erzeuge eine sicherungskopie...
		if (-f "$FILE$ext") {
			print " - rename [$FILE]: [$FILE$ext] existiert!\n";
			return -1;
		}
		$I=rename $FILE, "$FILE$ext";
		if ($I==1) {
			# backup Ok.
			return 1;
		} else {
			print " - [$FILE]: rename auf [$FILE$ext] fehlgeschlagen!\n";
			return -1;
		}
	} else {
		print " - skipped [$FILE]...\n";
		return 0;
	}
}

# rek_dir kann mit einem callback (mit \&func) und einem
# calldata aufgrufen werden. rek_dir sorgt dafuer, das der callback
# fuer jedes verzeichnis innerhalb des gegebenen aufgerufen wird.
# der callback erhaelt das aktuelle Verzeichnis und calldata als parameter
# ---------------------------------------------------------------- 
sub recurse_dir {
# ---------------------------------------------------------------- 
	my ($DIR,$CALLBACK,$CALLDATA)=@_;
	my ($ADIR,@LIST,$CWD,$WD);


	$CWD=cwd;
	if (chdir($DIR)) {
	
		$WD=cwd;

		if (opendir(DIR,".")) {
			@LIST=grep {(-d $_)&&($_ ne "..")&&($_ ne ".")} readdir(DIR);
			closedir(DIR);

			$CALLBACK->($WD,$CALLDATA);

			foreach $ADIR (@LIST) {
				recurse_dir("$ADIR",$CALLBACK,$CALLDATA);
			}
		} else {
			warn "unable to open \"$DIR\"\n";
		}
		chdir($CWD);	
	} else {
		warn "couldn't chdir to \"$DIR\"\n";
	}

}

# rek_list kann mit einem callback (mit \&func) und einem
# calldata aufgrufen werden. rek_list sorgt dafuer, das der callback
# fuer jedes verzeichnis der uebergebenen Liste aufgerufen wird.
# der callback erhaelt das aktuelle Verzeichnis und calldata als parameter
# ---------------------------------------------------------------- 
sub recurse_list {
# ---------------------------------------------------------------- 
	my ($DIRLIST,$CALLBACK,$CALLDATA)=@_;
	my ($ADIR,$WD,$CWD);

	$CWD=cwd;
	foreach $ADIR (@$DIRLIST) {
		if (chdir($ADIR)) {
			$WD=cwd;
			$CALLBACK->($WD,$CALLDATA);
		} else {
			warn "couldn't chdir to \"$ADIR\"\n";
		}
		if (!chdir($CWD)) {
			warn "couldn't chdir back to \"$CWD\"\n";
		}
	}
}

# rek_dir kann mit einem callback (mit \&func) und einem
# calldata aufgrufen werden. rek_dir sorgt dafuer, das der callback
# fuer jedes verzeichnis aufgerufen wird, welches im $DIRFILE angegeben ist.
# der callback erhaelt das aktuelle Verzeichnis und calldata als parameter
# Zeilenformat Configfile: DIR[+] wobei das + rekusiv bedeutet...
# ---------------------------------------------------------------- 
sub recurse_dirfile {
# ---------------------------------------------------------------- 
	my ($DIRFILE,$CALLBACK,$CALLDATA)=@_;
	my ($LINE,@DIRLIST,@REK,$AREK,$CWD);

	if (open(INF,"<$DIRFILE")) {

		# Dateienliste auslesen und in DIRLIST ablegen...

		while ($LINE=<INF>) {
			chomp($LINE);				# entferne \n
			if (!($LINE=~/^\s*#/)) {	# keine Kommentarzeilen oder Leerzeilen aufnehmen
				($LINE,$AREK)=($LINE=~/(\S+?)(\+*$)/);	# Rekursionen erwuenscht?
				if ($LINE=~/\S+/) {		# Keine Leerzeilen...
					if ($AREK ne "") {
						push(@REK,1);
					} else {
						push(@REK,0);
					}
					push(@DIRLIST,$LINE);	# Zeile in Liste einfuegen
				}
			}
		}
		close(INF);

		# Liste durchlaufen und Callback aufrufen
	
		foreach $LINE (@DIRLIST) {
			if (shift(@REK)==1) {
				recurse_dir($LINE,$CALLBACK,$CALLDATA);
			} else {
				$CWD=cwd;
				if (chdir($LINE)) {
					$CALLBACK->($LINE,$CALLDATA);
					chdir($CWD);
				} else {
					warn "unable to chdir to \"$LINE\"\n";
				}
			}
		}

	} else {
		warn "unable to open \"$DIRFILE\": $_\n";
	}

}

# walk_files kann mit einem callback (mit \&func) und einem
# calldata aufgrufen werden. walk_files sorgt dafuer, das der callback
# fuer jeden file innerhalb des gegebenen verzeichnisses aufgerufen wird.
# der callback erhaelt das aktuelle Verzeichnis, den aktuellen file 
# und calldata als parameter
# ---------------------------------------------------------------- 
sub walk_files {
# ---------------------------------------------------------------- 
	my ($DIR,$CALLBACK,$CALLDATA)=@_;
	my ($ADIR,@LIST,$CWD,$WD);


	$CWD=cwd;
	if (chdir($DIR)) {
		$WD=cwd;
		if (opendir(DIR,".")) {
			@LIST=grep {(-f $_)} readdir(DIR);
			closedir(DIR);
			foreach $AFILE (@LIST) {
				$CALLBACK->($WD,$AFILE,$CALLDATA);
			}
		} else {
			warn "unable to open \"$DIR\"\n";
		}
		chdir($CWD);
	} else {
		warn "couldn't chdir to \"$DIR\"\n";
	}
}

1;	# package return...








