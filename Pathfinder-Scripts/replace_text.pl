#! /usr/bin/perl -w

if($#ARGV<2)
{
	print "Used to replace a text by another in a file. The texts are taken from files.\n";
	print "Usage: <script.pl> <file.dat> <file_with_search_text> <file_with_replacement_text>\n";
	exit;
}

#open (IN, "+<$ARGV[0]");
$search = slurp($ARGV[1]);
$replace = slurp($ARGV[2]);

#@file = <IN>;

#seek IN,0,0;

#foreach $file (@file){
#$file =~ s/$search/$replace/g;
#print IN $file;
#}
#close IN;

$file = slurp($ARGV[0]);

$file =~ s/$search/$replace/gs;
open (IN, ">$ARGV[0]");
print IN $file;

exit;

# Slurp the given filename into a single variable
# Usage: slurp 'filename'
sub slurp
{
	local(@args) = @_;
	local(*IN);
	local($holdTerminator) = $/;
	undef $/;
	open (IN, "+<$args[0]");
	my $buf = <IN>;
	$/ = $holdTerminator;
	my @lines = split /$holdTerminator/, $buf;
	$buf = "init";
	$buf = join $holdTerminator, @lines;
	return $buf;
}
