#!/usr/local/bin/perl
#SKINF: extrahiert aus QM Arbeitszeiten bestimmte Eintraege...

use Getopt::Std;

#Projektnummern:Zeitausgleich:		01-0003
#				Urlaub:				??
#				Arbeitsplatz:		11-0005


$SQLPLUS_OPT="qms/zoso\@ux05";

$FN_TMP="__kk_tmp.sql";
$FN_SPOOL="Arbeitszeiten.txt";

Getopt::Std::getopts('m:v:p:hs:');

$SQLPLUS=$opt_s?"sqlplus $opt_s \@$FN_TMP":"sqlplus $SQLPLUS_OPT \@$FN_TMP";
$MITARBEITER=$opt_m?$opt_m:"000003";
$STARTDATUM=$opt_v?$opt_v:"2002";
$PROJEKTKLAUSEL="and qmdb33s.QMASTPRjnr like '$opt_p%'" if $opt_p;

if (!$opt_h) {
	open (OUTF,">$FN_TMP")||die "oeffnen von temp. file $FN_TMP\n";
	print OUTF <<endOfStmt;

set pagesize 20000
set lines 293
spool $FN_SPOOL

select concat(concat(substr(qmdb33s.qmastdatum,7,2),'.'),
concat(concat(substr(qmdb33s.qmastdatum,5,2),'.'),
substr(qmdb33s.qmastdatum,1,4))) as Datum,
qmdb33s.qmastanzhh/60 as Stunden,'h:',
qmdb33s.qmcomnotiz as Beschreibung,qmdb33s.qmastprjnr as PrjNum,qmdb30.QMPRJBZEIC as Projekt,
qmdb33s.qmastttknr as tNum,
qmdb32.QMTTKBZEIC as Taetigkeit
from qmdb30,qmdb32,qmdb33s 
where qmdb33s.qmastpernr='$MITARBEITER' /*and qmdb33s.qmastdatyp=2*/
$PROJEKTKLAUSEL and
qmdb33s.qmastdatum>= '$STARTDATUM%' and
qmdb33s.QMMANNUMMR=qmdb32.QMMANNUMMR(+) and 
qmdb33s.QMASTtTKnr=qmdb32.QMTTKNUMMR(+) and 
qmdb33s.QMMANNUMMR=qmdb30.QMMANNUMMR(+) and 
qmdb33s.QMASTPRjnr=qmdb30.QMPRJNUMMR(+)
order by qmdb33s.qmastdatum,qmdb33s.qmastlfdnr;

spool off
exit;

endOfStmt

	close (OUTF);
	system ("$SQLPLUS");
	unlink ("$FN_TMP");
} else {
	print <<endOfMsg;
Aufruf: qmquery [-mMANR] [-pPJNR] [-vVON] [-h] [-sCONNECT]
wobei MANR...: Mitarbeiter (def.=000003,kuk)
      PJNR...: Projektnummer (def.=alle)
      VON....: 'Ab Datum' Maske: zB.: 1999 oder 199802 (def.=2001)
      CONNECT: oracle connectstring (def.: $SQLPLUS_OPT)
Ergebnis in $FN_SPOOL.
endOfMsg
}
