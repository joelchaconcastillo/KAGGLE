#Con este script se genera la lista de ejecuciones....
#!/usr/bin/perl
use strict;
use warnings;
use Getopt::Long;
my $Instance=0;
my $Sed=0;

#time - size_population - number_executions
my @Conf =(
"86400 10 30", #days: 1
"172800 10 30", #days: 2
"345600 10 10", #days: 4
"345600 20 10", #dats: 4
"691200 10 10", #dats: 8
"691200 30 10", #dats: 8
"1382400 10 10", #dats: 16
"1382400 30 10", #dats: 16
"1382400 50 10", #dats: 16
"1900800 10 10", #dats: 22
"1900800 30 10", #dats: 22
"1900800 50 10", #dats: 22
"2073600 50 10" #dats: 24
);
	foreach my $configuration (@Conf)
{
	my @configuration2 = split ' ', $configuration;
	my $time = $configuration2[0];
	my $population =  $configuration2[1];
	my $executions = $configuration2[2];

	print "$time $population $executions\n";
	for(my $Sed = 1; $Sed <= $executions; $Sed++) ##Realizar 35 ejecuciones con distintas semilla de cada instancia..
	{
	      my $command = "cat ../Results/report_".$time."_".$population."_".$executions."_".$Sed.".output | ./evaluator-tsp";
	      print `$command`."\n";
	}
}
	print "\nPenalizations\n";
foreach my $configuration1 (@Conf)
{
	my @configuration2 = split ' ', $configuration1;
	my $time = $configuration2[0];
	my $population = $configuration2[1];
	my $executions = $configuration2[2];
	print "$time $population $executions\n";

	for(my $Sed = 1; $Sed <= $executions; $Sed++) ##Realizar 35 ejecuciones con distintas semilla de cada instancia..
	{
	      my $command = "cat ../Results/report_".$time."_".$population."_".$executions."_".$Sed.".output | ./evaluator-penalizations";
	      print `$command`."\n";
	}
}
