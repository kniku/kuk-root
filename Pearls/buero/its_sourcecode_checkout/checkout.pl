use Cwd;
require "config.pl";

print "\n* checking out ITS/CPS sources...";
print "\n* configuration (see config.pl):";
print "\n    target directory: ".$TARGET_DIRECTORY;
print "\n    ITS Classic/Web:\n      Revision: ".$REVISION_ITS."\n      Owil:     ".$REVISION_ITS_OWIL;
print "\n    CPS Classic/Web:\n      Revision: ".$REVISION_CPS."\n      Owil:     ".$REVISION_CPS_OWIL;
print "\n    Customer: ";
for $key ( keys %CUSTOMERS ) {
	print $key." " if $CUSTOMERS{$key};
}
print "\n\n* press ENTER to continue...";<STDIN>;
print "\n-----------------------------------------------------\n";

$starting_directory = getcwd();

print "* creating/changing directory: \"".$TARGET_DIRECTORY."\" ...\n";
mkdir $TARGET_DIRECTORY;
chdir $TARGET_DIRECTORY;

if (is_windoof()) { $pipestr = " 2>>stderr.log >>stdout.log"; }
else { $pipestr = " >> & stdouterr.log"; }

$cwd = getcwd();
$ENV{"CVS_PASSFILE"} = os_path($cwd."\\.cvspass");
print "* creating temporary .cvspass file ...\n";
open OUT,">.cvspass";
print OUT "/1 :pserver:build\@cvs:2401/cvsroot/f_e Aub?'eKooR\n";
print OUT "/1 :pserver:build\@cvs:2401/cvsroot/its Aub?'eKooR\n";
close OUT;

if ($REVISION_ITS and $REVISION_ITS_OWIL) {
	$current_dir = "its_classic";
	print "* creating directory: ", $current_dir, " ...\n";
	mkdir $current_dir;
	if (-d $current_dir) {
		print "    checking out: \"f_e/owext\"      -> \"".$current_dir."/owil_ext\" ...\n";
		system("cvs -d:pserver:build\@cvs:/cvsroot/f_e exp -r ".$REVISION_ITS_OWIL." -d ".os_path($current_dir."\\owil_ext")." owext".$pipestr);
		print "    checking out: \"f_e/owbin\"      -> \"".$current_dir."/owil_prod\" ...\n";
		system("cvs -d:pserver:build\@cvs:/cvsroot/f_e exp -r ".$REVISION_ITS_OWIL." -d ".os_path($current_dir."\\owil_prod")." owbin".$pipestr);
		print "    checking out: \"f_e/owil\"       -> \"".$current_dir."/owil_src\" ...\n";
		system("cvs -d:pserver:build\@cvs:/cvsroot/f_e exp -r ".$REVISION_ITS_OWIL." -d ".os_path($current_dir."\\owil_src")." owil".$pipestr);
		print "    checking out: \"its/its\"        -> \"".$current_dir."/its_src\" ...\n";
		system("cvs -d:pserver:build\@cvs:/cvsroot/its exp -r ".$REVISION_ITS." -d ".os_path($current_dir."\\its_src")." its".$pipestr);
	}

	$current_dir = "its_web";
	print "* creating directory: ", $current_dir, " ...\n";
	mkdir $current_dir;
	if (-d $current_dir) {
		print "    checking out: \"f_e/jafaext\"    -> \"".$current_dir."/jafa_ext\" ...\n";
		system("cvs -d:pserver:build\@cvs:/cvsroot/f_e exp -r ".$REVISION_ITS_OWIL." -d ".os_path($current_dir."\\jafa_ext")." jafaext".$pipestr);
		print "    checking out: \"f_e/jafa\"       -> \"".$current_dir."/jafa\" ...\n";
		system("cvs -d:pserver:build\@cvs:/cvsroot/f_e exp -r ".$REVISION_ITS_OWIL." -d ".os_path($current_dir."\\jafa")." jafa".$pipestr);
		print "    checking out: \"its/itsweb\"     -> \"".$current_dir."/itsweb\" ...\n";
		system("cvs -d:pserver:build\@cvs:/cvsroot/its exp -r ".$REVISION_ITS." -d ".os_path($current_dir."\\itsweb")." itsweb".$pipestr);
		if ($CUSTOMERS{"COBA"}) {
			print "    checking out: \"its/cobaitsweb\" -> \"".$current_dir."/cobaitsweb\" ...\n";
			system("cvs -d:pserver:build\@cvs:/cvsroot/its exp -r ".$REVISION_ITS." -d ".os_path($current_dir."\\cobaitsweb")." cobaitsweb".$pipestr);
		} else {
			print "    checking out: \"its/ecoitsweb\"  -> \"".$current_dir."/ecoitsweb\" ...\n";
			system("cvs -d:pserver:build\@cvs:/cvsroot/its exp -r ".$REVISION_ITS." -d ".os_path($current_dir."\\ecoitsweb")." ecoitsweb".$pipestr);
		}
	}
}

if ($REVISION_CPS and $REVISION_CPS_OWIL) {
	$current_dir = "cps_classic";
	print "* creating directory: ", $current_dir, " ...\n";
	mkdir $current_dir;
	if (-d $current_dir) {
		print "    checking out: \"f_e/owext\"      -> \"".$current_dir."/owil_ext\" ...\n";
		system("cvs -d:pserver:build\@cvs:/cvsroot/f_e exp -r ".$REVISION_CPS_OWIL." -d ".os_path($current_dir."\\owil_ext")." owext".$pipestr);
		print "    checking out: \"f_e/owbin\"      -> \"".$current_dir."/owil_prod\" ...\n";
		system("cvs -d:pserver:build\@cvs:/cvsroot/f_e exp -r ".$REVISION_CPS_OWIL." -d ".os_path($current_dir."\\owil_prod")." owbin".$pipestr);
		print "    checking out: \"f_e/owil\"       -> \"".$current_dir."/owil_src\" ...\n";
		system("cvs -d:pserver:build\@cvs:/cvsroot/f_e exp -r ".$REVISION_CPS_OWIL." -d ".os_path($current_dir."\\owil_src")." owil".$pipestr);
		print "    checking out: \"its/its\"        -> \"".$current_dir."/cps_src\" ...\n";
		system("cvs -d:pserver:build\@cvs:/cvsroot/its exp -r ".$REVISION_CPS." -d ".os_path($current_dir."\\cps_src")." its".$pipestr);
	}
	$current_dir = "cps_web";
	print "* creating directory: ", $current_dir, " ...\n";
	mkdir $current_dir;
	if (-d $current_dir) {
		print "    checking out: \"its/cpsweb_common\"        -> \"".$current_dir."/cpsweb_common\" ...\n";
		system("cvs -d:pserver:build\@cvs:/cvsroot/its exp -r ".$REVISION_CPS." -d ".os_path($current_dir."\\cpsweb_common")." cpsweb_common".$pipestr);
		if ($CUSTOMERS{"WLB"}) {
			print "    checking out: \"its/cpsweb_wlb\"        -> \"".$current_dir."/cpsweb_wlb\" ...\n";
			system("cvs -d:pserver:build\@cvs:/cvsroot/its exp -r ".$REVISION_CPS." -d ".os_path($current_dir."\\cpsweb_wlb")." cpsweb_wlb".$pipestr);
		} else {
			print "    checking out: \"its/cpsweb_eco\"        -> \"".$current_dir."/cpsweb_eco\" ...\n";
			system("cvs -d:pserver:build\@cvs:/cvsroot/its exp -r ".$REVISION_CPS." -d ".os_path($current_dir."\\cpsweb_eco")." cpsweb_eco".$pipestr);
		}
	}
}


print "* removing temporary .cvspass file ...\n";
unlink ".cvspass";

print "-----------------------------------------------------\n";
print "* verifying:";
print "\n    owext (ITS) ...         ";
if (-e os_path("its_classic\\owil_ext\\prd\\lib\\wnt\\owcrypto.lib")) { print "Ok"; } else { print "FAILED"; }
print "\n    owbin (ITS) ...         ";
if (-e os_path("its_classic\\owil_prod\\bin\\wnt\\dbc.exe")) { print "Ok"; } else { print "FAILED"; }
print "\n    owil (ITS) ...          ";
if (-e os_path("its_classic\\owil_src\\base\\buser.h")) { print "Ok"; } else { print "FAILED"; }
print "\n    its ...                 ";
if (-e os_path("its_classic\\its_src\\shared\\sys\\itsys.h")) { print "Ok"; } else { print "FAILED"; }

print "\n    owext (CPS) ...         ";
if (-e os_path("cps_classic\\owil_ext\\prd\\lib\\wnt\\owcrypto.lib")) { print "Ok"; } else { print "FAILED"; }
print "\n    owbin (CPS) ...         ";
if (-e os_path("cps_classic\\owil_prod\\bin\\wnt\\dbc.exe")) { print "Ok"; } else { print "FAILED"; }
print "\n    owil (CPS) ...          ";
if (-e os_path("cps_classic\\owil_src\\base\\buser.h")) { print "Ok"; } else { print "FAILED"; }
print "\n    cps ...                 ";
if (-e os_path("cps_classic\\cps_src\\shared\\sys\\itsys.h")) { print "Ok"; } else { print "FAILED"; }

print "\n    itsweb (ITSWEB) ...     ";
if (-e os_path("its_web\\itsweb\\src\\com\\ecofinance\\itsweb\\bl\\handler\\HListLPErfMatrix.java")) { print "Ok"; } else { print "FAILED"; }
print "\n    jafaext (ITSWEB) ...    ";
if (-e os_path("its_web\\jafa_ext\\lib\\xerces.jar")) { print "Ok"; } else { print "FAILED"; }
print "\n    jafa (ITSWEB) ...       ";
if (-e os_path("its_web\\jafa\\src\\com\\ecofinance\\jafa\\bl\\record\\Records.java")) { print "Ok"; } else { print "FAILED"; }
if ($CUSTOMERS{"COBA"}) {
	print "\n    cobaitsweb (ITSWEB) ... ";
	if (-e os_path("its_web\\cobaitsweb\\xmldialogs.xml")) { print "Ok"; } else { print "FAILED"; }
} else {
	print "\n    ecoitsweb (ITSWEB) ...  ";
	if (-e os_path("its_web\\ecoitsweb\\xmldialogs.xml")) { print "Ok"; } else { print "FAILED"; }
}

print "\n    cpsweb_common (CPSWEB) ... ";
if (-e os_path("cps_web\\cpsweb_common\\build.xml")) { print "Ok"; } else { print "FAILED"; }
if ($CUSTOMERS{"WLB"}) {
	print "\n    cpsweb_wlb (CPSWEB) ... ";
	if (-e os_path("cps_web\\cpsweb_wlb\\build.xml")) { print "Ok"; } else { print "FAILED"; }
} else {
	print "\n    cpsweb_eco (CPSWEB) ... ";
	if (-e os_path("cps_web\\cpsweb_eco\\build.xml")) { print "Ok"; } else { print "FAILED"; }
}

print "\n* leaving directory: \"".$TARGET_DIRECTORY."\" ...\n";
chdir $starting_directory;
print "* Done.\n";


sub is_windoof() {
	return $^O =~ /win/i;
}

sub os_path() {
	my $r = shift;
	if (is_windoof()) { $r =~ tr!/!\\!; } else { $r =~ tr!\\!/!; }
	return $r
}