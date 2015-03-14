#!/usr/local/bin/perl

package objparse;

# Library Version
$objparse::VERSION="01.001";

# globale variablen

# gross/klein (objekte) ignorieren?
# falls 1, werden alle objekte in grossbuchstaben
# gespeichert...
$objparse::ignore_case_object=0;

# gross/klein (variablen) ignorieren?
# falls 1, werden alle variablen in grossbuchstaben
# gespeichert...
$objparse::ignore_case_var=0;

# parst einen konfig-file und legt objekte in einem hash ab
# format: [OBJNAME]
#         var1=wert1
#         var2=wert2
#         var3          (hier wird var3 mit dem Wert 1 (TRUE) angelegt)
#         [OBJECT2] ... usf.
# taucht ein objekt mehrfach auf, so wird es zu einem gemerged,
# wobei idente variablen ueberschrieben werden!
# liefert eine hash-refernz zurueck
# IN: reffilename
# zB: $refH=parse_objects("test.dat");
# ---------------------------------------------------------------- 
sub parse_objects {
# ---------------------------------------------------------------- 
	my $CFGFILE=pop(@_);
	my %hOBJ,$NO;
   
	if (!open (IN,"<$CFGFILE")) {
	    return;
	}
	$hOBJ{"--OBJDAT"}={"--VALID"=>1};
	$hOBJ{"--OBJDAT"}={"--CFGFILE"=>$CFGFILE};
	while (<IN>) {
		next if ($_=~/\s*\#/);
		if ($_=~/^\s*\[*\s*--/) {
			print "$CFGFILE ($.): invalid object/variablename found: $_";
			next;
		}
		if ($_=~/\s*\[(\S+)\]\s*/) {
			# new object found
			$NO=$1;
			$NO=uc($NO) if ($objparse::ignore_case_object);
			if (not defined $hOBJ{$NO}) {
#				print "new object: $NO\n";
				${$hOBJ{"--OBJDAT"}}{"--OBJCNT"}++;
				if (${$hOBJ{"--OBJDAT"}}{"--OBJLIST"}) {
					${$hOBJ{"--OBJDAT"}}{"--OBJLIST"}=${$hOBJ{"--OBJDAT"}}{"--OBJLIST"}.";$NO";
				} else {
					${$hOBJ{"--OBJDAT"}}{"--OBJLIST"}=${$hOBJ{"--OBJDAT"}}{"--OBJLIST"}."$NO";
				}
				$hOBJ{$NO}={"--VALID"=>1};
			} else {
#				print "reuse object: $NO\n";
			}
			$CUROBJ=$NO;
		} elsif ($_=~/\s*(\S.*?)\s*\=\s*(.*).*$/&&!($_=~/\#/)) {
			# variable found
			$NO=$1;
			if ($CUROBJ) {
        			$NO=uc($NO) if ($objparse::ignore_case_var);
				if (not defined ${$hOBJ{$CUROBJ}}{$NO}) {
					${$hOBJ{$CUROBJ}}{"--VARCNT"}++;
					if (${$hOBJ{$CUROBJ}}{"--VARLIST"}) {
						${$hOBJ{$CUROBJ}}{"--VARLIST"}=${$hOBJ{$CUROBJ}}{"--VARLIST"}.";$NO";
					} else {
						${$hOBJ{$CUROBJ}}{"--VARLIST"}=${$hOBJ{$CUROBJ}}{"--VARLIST"}."$NO";
					}
				}
				${$hOBJ{$CUROBJ}}{$NO}=$2;			
			}
		} elsif (/(\S+.*?)\s*$/) {
			$NO=$1;
			if ($CUROBJ) {
        			$NO=uc($NO) if ($objparse::ignore_case_var);
				if (not defined ${$hOBJ{$CUROBJ}}{$NO}) {
					${$hOBJ{$CUROBJ}}{"--VARCNT"}++;
					if (${$hOBJ{$CUROBJ}}{"--VARLIST"}) {
						${$hOBJ{$CUROBJ}}{"--VARLIST"}=${$hOBJ{$CUROBJ}}{"--VARLIST"}.";$NO";
					} else {
						${$hOBJ{$CUROBJ}}{"--VARLIST"}=${$hOBJ{$CUROBJ}}{"--VARLIST"}."$NO";
					}
				}
				${$hOBJ{$CUROBJ}}{$NO}=1;
			}		    
		}
	}
	close IN;
	\%hOBJ;
}

# printet einen ueber parse_objects angelegten hash...
# IN: hashreferenz
# ---------------------------------------------------------------- 
sub print_parsed_objects {
# ---------------------------------------------------------------- 
	my $rhO=pop @_;
	my @L,$k,$v,$kk,$vv;

	print "list of objects found in \"".${$rhO->{"--OBJDAT"}}{"--CFGFILE"}."\":\n";
	@L=split(";",${$rhO->{"--OBJDAT"}}{"--OBJLIST"});
	foreach $k (@L) {
		$v=$rhO->{$k};
		print "OBJECT \"$k\" [\n";
		while (($kk,$vv)=each %$v) {
			next if ($kk=~/^--/);
			print "  \"$kk\" => \"$vv\"\n";
		}
		print "]\n";
	}
}

# wie oben, es werden aber auch interne Eintraege
# rausgeschrieben...
# ---------------------------------------------------------------- 
sub _print_parsed_objects {
# ---------------------------------------------------------------- 
	my $rhO=pop @_;
	my $k,$v,$kk,$vv;

	print "(internal) list of objects found in \"".${$rhO->{"--OBJDAT"}}{"--CFGFILE"}."\":\n";
	while (($k,$v)=each %$rhO) {
		print "OBJECT \"$k\" [\n";
		while (($kk,$vv)=each %$v) {
			print "  \"$kk\" => \"$vv\"\n";
		}
		print "]\n";
	}
}

# ruft einen geg. callback fuer jedes object auf.
# IN: hashref,callback
# callbackformat: cb($objname,\%objdescription)
# die 'internen' objekte/variablen werden vorher entfernt!
# (beginnen mit --)
# ---------------------------------------------------------------- 
sub recurse_parsed_objects {
# ---------------------------------------------------------------- 
	my ($rhO,$CALLBACK)=@_;
	my %hVO,$k,$v,$kk,$vv,@L;

	@L=split(";",${$rhO->{"--OBJDAT"}}{"--OBJLIST"});
#	print "objectlist: @L\n";
	foreach $k (@L) {
#		print "key: \"$k\"\n";
		$v=$rhO->{$k};
		next if (!$v||($kk=~/--/&&$kk!=~/^--\d+--/));
		undef %hVO;
		while (($kk,$vv)=each %$v) {
			next if ($kk=~/^--/);
			$hVO{$kk}=$vv;
		}
		if ($k!~/^--/) {
#			print "calling callback for object \"$k\"\n";
			$CALLBACK->($k,\%hVO) if ($CALLBACK);
		}
	}
}
# walk durch die Variablen eines best. Objekts.
# in: hashref,objektname,callback
# callback call: cb(objektname,key,value)
# ---------------------------------------------------------------- 
sub recurse_parsed_variables {
# ---------------------------------------------------------------- 
	my ($rhO,$ONAME,$CALLBACK)=@_;
	my $v,$kk,$vv,@LL;
	
        $ONAME=uc($ONAME) if ($objparse::ignore_case_object);
	$v=$rhO->{$ONAME};
	if ($v) {
		@LL=split(";",$v->{"--VARLIST"});
#		print "varlist: @LL\n";
		foreach $kk (@LL) {
			next if ($kk=~/--/&&$kk!=~/^--\d+--/);
			$vv=$v->{$kk};
			$CALLBACK->($ONAME,$kk,$vv);
		}
	}
}

# liefert objektbeschreibung als hashreferenz zuueck
# IN: hashref,objektname
# die 'internen' objekte/variablen werden vorher entfernt!
# (beginnen mit --)
# ---------------------------------------------------------------- 
sub get_object_descr {
# ---------------------------------------------------------------- 
	my ($rhO,$ONAME)=@_;
	my %hVO,$v,$kk,$vv;

#	print "searching $ONAME...\n";
        $ONAME=uc($ONAME) if ($objparse::ignore_case_object);
	$v=$rhO->{$ONAME};
	if ($v) {
		while (($kk,$vv)=each %$v) {
			next if ($kk=~/--/&&$kk!=~/^--\d+--/);
			$hVO{$kk}=$vv;
#			print "$ONAME: $kk->$vv\n";
		}
	}
	\%hVO;
}

# sucht nach einer bestimmten varaiblen in einem
# bestimmten objekt und liefert der wert zurueck
# ---------------------------------------------------------------- 
sub get_object_variable {
# ---------------------------------------------------------------- 
	my ($rhO,$ONAME,$VNAME)=@_;
	my %hVO,$v,$kk,$vv;

        $ONAME=uc($ONAME) if ($objparse::ignore_case_object);
        $VNAME=uc($VNAME) if ($objparse::ignore_case_var);
	$v=$rhO->{$ONAME};
	if ($v) {
		return $v->{"$VNAME"};
	}
}

1;	# package return...

