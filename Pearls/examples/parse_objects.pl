#!/usr/bin/perl
#SKINFO: parsing configfile and store objects...
use lib "../lib";
use objparse;

$CFGFILE="../dat/inp/parse_objects.cfg";

sub cb_var {
	my ($ONAME,$VNAME,$VVALUE)=@_;
	print "cb_v: $ONAME: $VNAME=$VVALUE\n";
}

sub cb_obj {
	my ($ONAME,$rVHash)=@_;
	print "cb: $ONAME\n";
	objparse::recurse_parsed_variables($rhOBJS,$ONAME,\&cb_var);
}

print "=============================================\n";
print "modul objparse.pm, version $objparse::VERSION\n";
print "=============================================\n";
$rhOBJS=objparse::parse_objects($CFGFILE);
objparse::print_parsed_objects($rhOBJS);
#objparse::_print_parsed_objects($rhOBJS);
#objparse::recurse_parsed_objects($rhOBJS,\&cb_obj);
#print "var found: ".objparse::get_object_variable($rhOBJS,"O2","cv")."\n";




