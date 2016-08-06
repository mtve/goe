#! /usr/bin/perl

use warnings;
use strict;

sub slurp($) { open my ($f), $_[0] or die; local $/; <$f> }

my $goe = slurp "src.gol";

my $flag;
$|++;

AGAIN:
$flag = 0;
for (0..length($goe)-1) {
	print " $_";
	next if substr ($goe, $_, 1) =~ /\n|\?/;
	my $goe2 = $goe;
	substr ($goe2, $_, 1) = '?';
	open my ($f), "> dst.gol" or die;
	print $f $goe2;
	close $f;
	`perl ../gol_cnf.pl dst.gol >dst.cnf1`;
	`cryptominisat5 dst.cnf1 >dst.sol1`;
	next if slurp ("dst.sol1") !~ /UNSAT/;
	$goe = $goe2;

	print "\n" . localtime;
	open $f, "> src.gol1" or die;
	print $f $goe2;
	close $f;
	$flag = 1;
}
goto AGAIN if $flag;
