#! /usr/bin/perl

use warnings;
use strict;

sub clause {
	my @a = @_;

	print "\tsat->add_lit ($_);\n" for @a, 0;
}

sub imp { clause (map (-$_, @_[0..$#_-1]), $_[-1]) } # $_0 & $_1 .. => $_-1

sub gen {
	my $old = 'PAR(y,x)';
	my $new = 'CHI(y,x)';
	my @nei = qw(
		PAR(y-1,x-1) PAR(y-1,x) PAR(y-1,x+1)
		PAR(y,x-1)              PAR(y,x+1)
		PAR(y+1,x-1) PAR(y+1,x) PAR(y+1,x+1)
        );
	my @d8 = 0..7;

	print "\t/* <=1 -> D (8) */\n";
	for my $i (@d8) {
		imp (map (-$nei[$_], grep $_ != $i, @d8), -$new);
	}

	print "\t/* 2 & D -> D (28) */\n";
	print "\t/* 2 & A -> A (28) */\n";
	for my $i (@d8) {
	for my $j (@d8) { next if $j <= $i;
		my @f = map /$i|$j/ ? $nei[$_] : -$nei[$_], @d8;
		imp (@f, -$old, -$new);
		imp (@f,  $old,  $new);
	}}

	print "\t/* 3 -> A (56) */\n";
	for my $i (@d8) {
	for my $j (@d8) { next if $j <= $i;
	for my $k (@d8) { next if $k <= $j;
		my @f = map /$i|$j|$k/ ? $nei[$_] : -$nei[$_], @d8;
		imp (@f, $new);
	}}}

	print "\t/* >=4 -> D (70) */\n";
	for my $i (@d8) {
	for my $j (@d8) { next if $j <= $i;
	for my $k (@d8) { next if $k <= $j;
	for my $l (@d8) { next if $l <= $k;
		imp (@nei[$i,$j,$k,$l], -$new);
	}}}}
}

gen ();
