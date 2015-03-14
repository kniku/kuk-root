
$tmpfile="analyse.sql";
$sqlplus="sqlplus zoso/zoso\@ux01.zoso.at \@$tmpfile";
$tablist="'ITDB901','ITDB501','ITDB820'";

open(OUT,">$tmpfile") or die "\n";
print OUT <<endOfText;
set pagesize 2000
spool $tmpfile.spool
select * from tab where tname in ($tablist);
exit;
endOfText

close OUT;

system($sqlplus);

open(IN,"<$tmpfile.spool") or die "\n";
open(OUT,">$tmpfile") or die "\n";
print OUT "set lines 500\nset pagesize 2000\n\n";
while (<IN>) {
	if (/(\S+)\s+TABLE\s+/) {
		$t=$1;
		print OUT "analyze table $t compute statistics;\n" if $tablist=~/$t/;
	}
}
close IN;
unlink "$tmpfile.spool";

print OUT <<endOfStmt;

select 'Table '||a.table_name||' in '||a.tablespace_name||': Extents='||
b.extents||', Rows='||a.num_rows||', '||a.num_rows*a.AVG_ROW_LEn||
' Bytes von '||
b.bytes||'; ~'||(b.bytes-(a.num_rows*a.AVG_ROW_LEn))/AVG_ROW_LEn||
' rows; chain='||a.chain_cnt||'.'
from user_segments b,user_tables a where
b.segment_name in ($tablist) and b.segment_name=a.table_name;

endOfStmt

close OUT;

