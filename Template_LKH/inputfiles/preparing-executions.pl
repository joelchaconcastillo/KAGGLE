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
	my $population = $configuration2[1];
	my $executions = $configuration2[2];
	for(my $Sed = 1; $Sed <= $executions; $Sed++) ##Realizar 35 ejecuciones con distintas semilla de cada instancia..
	{
	      my $file = "input_".$time."_".$population."_".$executions."_".$Sed;
	      my $fout;
	      open($fout, '>' ,$file);
	      print $fout "PROBLEM_FILE = /home/joel.chacon/KAGGLE/Phase1-Optimizer/data/kagglesanta2018.197769.tsp-informat\n";
	      print $fout "RUNS = 100000000000000\n";
	      print $fout "OUTPUT_TOUR_FILE=//home/joel.chacon/KAGGLE/Phase1-Optimizer/Ready-Phase1/report_".$time."_".$population."_".$executions."_".$Sed.".output\n";
	      print $fout "MAX_TRIALS=25\n";
	      print $fout "TOTAL_TIME_LIMIT = $time\n";
	      print $fout "TRACE_LEVEL = 0\n";
	      print $fout "POPULATION_SIZE = $population\n";
	      print $fout "INIT_P = 0.5\n";
	      print $fout "CANDIDATE_FILE=/home/joel.chacon/KAGGLE/Phase1-Optimizer/tmp/lkh-kagglesanta\n";
	      print $fout "PI_FILE=/home/joel.chacon/KAGGLE/Phase1-Optimizer/tmp/lkh-kagglesanta.pifile\n";
	      print $fout "MOVE_TYPE=5\n";
	      print $fout "SEED=$Sed\n";
	      close($fout);
	}
}
##creating tasks file

my $file = "execution-file";
my $fout;
open($fout, '>' ,$file);
foreach my $configuration (@Conf)
{
	my @configuration2 = split ' ', $configuration;
	my $time = $configuration2[0];
	my $population = $configuration2[1];
	my $executions = $configuration2[2];
	for(my $Sed = 1; $Sed <= $executions; $Sed++) ##Realizar 35 ejecuciones con distintas semilla de cada instancia..
	{
	      my $file = "input_".$time."_".$population."_".$executions."_".$Sed;
	       
	      print $fout "/home/joel.chacon/KAGGLE/Phase1-Optimizer/LKH /home/joel.chacon/KAGGLE/Phase1-Optimizer/inputfiles/$file > /home/joel.chacon/KAGGLE/Phase1-Optimizer/inputfiles/$file.stdout\n";
	}
}
