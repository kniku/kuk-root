#!/usr/local/bin/perl

use Tk;

print "tk version=$Tk::VERSION\n";
$top=MainWindow->new();
$but=$top->Button(-text=>"Hello World",-command=>sub {exit 0});
$but->pack();
MainLoop;
