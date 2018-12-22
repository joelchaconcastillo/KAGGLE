/*==========================================================================
// //  Author: 
//     Description: 
//
// ===========================================================================*/

#include "algorithm.h"
//#include <omp.h>
void PrintHelp()
{
	cout << "Instructions:"<<endl;
	cout << "--Seed (299)" <<endl;
	cout << "--Px (0.9), is the Crossover probability" <<endl;
	cout << "--Pm (1/n), is the Mutation Probability " << endl;
	cout << "--Path ./RESULT, is the directory where will save results, inside should be POF (fronts) and POS (decision variable fronts) directories"<<endl;
	cout << "--n 100, is the number of individual by generation"<<endl;
	cout << "--nfes, 25000, is the number of function evaluations"<<endl;
	cout << "--Dist_factor 0.75 , initial valor of diversity D"<<endl;
	cout << "example: \"./Ejecutable --n 100 --nfes 2500000 --Path . --Dist_factor 0.1 \""<<endl;
}
void SetConfiguration(int argc, char*argv[])
{
	for(int i = 1; i < argc ; i++)
    	{
		string Terminal(argv[i]);
		if(Terminal == "--Seed")
			run = atoi(argv[++i]);
		else if(Terminal == "--Px")
			realx = atof(argv[++i]);
		else if(Terminal == "--Pm")
			realm= atof(argv[++i]);
		else if(Terminal == "--Path")
			strcpy(currentPATH, argv[++i]);
		else if(Terminal =="--n")
			pops= atoi(argv[++i]);
		else if(Terminal == "--nfes")
			max_nfes = atoi(argv[++i]);
		else if(Terminal == "--Dist_factor")
			Initial_lowest_distance_factor= atof(argv[++i])*sqrt(nvar);
		else if(Terminal == "--help" || Terminal == "--h")
			PrintHelp();
		else
		{
			cout << Terminal<<endl;
			cout << "Unknown Argument...";
			exit(0);
		}
	    }
}
int main(int argc, char *argv[])
{

	if(argc<2)
         {
	    
	    cout << "Unknown Argument.."<<endl;
	    PrintHelp();
	    exit(0);
	 }
	strcpy(currentPATH, ".");
	SetConfiguration(argc, argv);

	clock_t start, temp, finish;
	double  duration, last = 0;
	start = clock();

	std::fstream fout;
	char logFilename[1024];
	sprintf(logFilename, "%s/LOG/LOG_KAGGLE_%d.dat", currentPATH, run);
	fout.open(logFilename,std::ios::out);
	fout<<"Time: \n\n";
	GENETIC GENETIC;
	GENETIC.exec_emo(run);
	temp = clock();
	duration = (double)(temp - start) / CLOCKS_PER_SEC;
	last = duration;

	fout<<"\n\n";

	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	fout.close();
	return 0;

}
