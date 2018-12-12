/*
Modified by Carlos Segura to implement the method described in

@inproceedings{Segura:2015:NDE:2739480.2754802,
 author = {Segura, Carlos and Botello Rionda, Salvador and Hern\'{a}ndez Aguirre, Arturo and Valdez Pe\~{n}a, S. Ivvan},
 title = {A Novel Diversity-based Evolutionary Algorithm for the Traveling Salesman Problem},
 booktitle = {Proceedings of the 2015 on Genetic and Evolutionary Computation Conference},
 series = {GECCO '15},
 year = {2015},
 isbn = {978-1-4503-3472-3},
 location = {Madrid, Spain},
 pages = {489--496},
 numpages = {8},
 url = {http://doi.acm.org/10.1145/2739480.2754802},
 doi = {10.1145/2739480.2754802},
 acmid = {2754802},
 publisher = {ACM},
 address = {New York, NY, USA},
 keywords = {diversity preservation, evolutionary algorithms, memetic algorithms, traveling salesman problem},
} 

*/ 


#include "LKH.h"
#include "Genetic.h"
#include <sys/time.h>
#include <math.h>
/*
 * This file contains the main function of the program.
 */

extern int K;

void setMoveType(int m){
	printf("Fija moveType a %d\n", m);
	MoveType = m;
   SubsequentMoveType = MoveType;
    K = MoveType >= SubsequentMoveType
        || !SubsequentPatching ? MoveType : SubsequentMoveType;
    if (PatchingC > K)
        PatchingC = K;
    if (PatchingA > 1 && PatchingA >= PatchingC)
        PatchingA = PatchingC > 2 ? PatchingC - 1 : 1;
    if (NonsequentialMoveType == -1 ||
        NonsequentialMoveType > K + PatchingC + PatchingA - 1)
        NonsequentialMoveType = K + PatchingC + PatchingA - 1;
    if (PatchingC >= 1 && NonsequentialMoveType >= 4) {
        BestMove = BestSubsequentMove = BestKOptMove;
        if (!SubsequentPatching && SubsequentMoveType <= 5) {
            MoveFunction BestOptMove[] =
                { 0, 0, Best2OptMove, Best3OptMove,
                Best4OptMove, Best5OptMove
            };
            BestSubsequentMove = BestOptMove[SubsequentMoveType];
        }
    } else {
        MoveFunction BestOptMove[] = { 0, 0, Best2OptMove, Best3OptMove,
            Best4OptMove, Best5OptMove
        };
        BestMove = MoveType <= 5 ? BestOptMove[MoveType] : BestKOptMove;
        BestSubsequentMove = SubsequentMoveType <= 5 ?
            BestOptMove[SubsequentMoveType] : BestKOptMove;
    }

}

int main(int argc, char *argv[])
{
    GainType Cost, OldOptimum;
    double Time, LastTime = GetTime();
		struct timeval actualTime; 
		gettimeofday(&actualTime, NULL);
		double InitialTime = ((double) (actualTime.tv_sec) * 1.0e6 + (double) (actualTime.tv_usec))/1.0e6;

    /* Read the specification of the problem */
    if (argc >= 2)
        ParameterFileName = argv[1];
    ReadParameters();
    MaxMatrixDimension = 10000;
    ReadProblem();

    if (SubproblemSize > 0) {
        if (DelaunayPartitioning)
            SolveDelaunaySubproblems();
        else if (KarpPartitioning)
            SolveKarpSubproblems();
        else if (KCenterPartitioning)
            SolveKCenterSubproblems();
        else if (KMeansPartitioning)
            SolveKMeansSubproblems();
        else if (RohePartitioning)
            SolveRoheSubproblems();
        else if (MoorePartitioning || SierpinskiPartitioning)
            SolveSFCSubproblems();
        else
            SolveTourSegmentSubproblems();
        return EXIT_SUCCESS;
    }
    AllocateStructures();
    CreateCandidateSet();
    InitializeStatistics();

		int maxMoveType = MoveType;
		int initialMoveType = 3;
		int m = initialMoveType;
		setMoveType(m);

    if (Norm != 0)
        BestCost = PLUS_INFINITY;
    else {
        /* The ascent has solved the problem! */
        Optimum = BestCost = (GainType) LowerBound;
        UpdateStatistics(Optimum, GetTime() - LastTime);
        RecordBetterTour();
        RecordBestTour();
        WriteTour(OutputTourFileName, BestTour, BestCost);
        WriteTour(TourFileName, BestTour, BestCost);
        Runs = 0;
    }
		SRandom(Seed);

    /* Find a specified number (Runs) of local optima */
		int generation = 0;
		int initialDistance = 0;
    for (Run = 1; Run <= Runs; Run++) {
        if (TraceLevel >= 1)
          PrintPopulation();

        LastTime = GetTime();
        /* Genetic algorithm */
        if (PopulationSize < MaxPopulationSize) {//Filling the initial population
        	Cost = FindTour();      /* using the Lin-Kernighan heuristic */
	        if (Cost < BestCost) {
            BestCost = Cost;
            RecordBetterTour();
            RecordBestTour();
				struct timeval actualTime; 
				gettimeofday(&actualTime, NULL);
				double currentTime = ((double) (actualTime.tv_sec) * 1.0e6 + (double) (actualTime.tv_usec))/1.0e6;
						 printf("Time: %lf -> ", currentTime - InitialTime);

            WriteTour(OutputTourFileName, BestTour, BestCost);
            WriteTour(TourFileName, BestTour, BestCost);
        	}
        	if (!HasFitness(Cost)) {//Tal vez cambiarlo por calculo de distancias (ver si ya esta en la poblacion)
          	AddToPopulation(Cost);
          }
				}

        Time = fabs(GetTime() - LastTime);
        UpdateStatistics(Cost, Time);
        if (TraceLevel >= 1 && Cost != PLUS_INFINITY) {
            printff("Run %d: Cost = " GainFormat, Run, Cost);
            if (Optimum != MINUS_INFINITY && Optimum != 0)
                printff(", Gap = %0.4f%%",
                        100.0 * (Cost - Optimum) / Optimum);
            printff(", Time = %0.2f sec. %s\n\n", Time,
                    Cost < Optimum ? "<" : Cost == Optimum ? "=" : "");
        }
        if (PopulationSize == MaxPopulationSize) {//Generacion
					generation++;
						printf("Generacion %d\n", generation);
					//Create offspring
					for (int i = 0; i < PopulationSize; i++){
     		   	Node *N;
        	  int Parent1, Parent2;
         	 	Parent1 = BinaryTournament();
          	do
          		Parent2 = BinaryTournament();
          	while (Parent2 == Parent1);
          	ApplyCrossover(Parent1, Parent2);
          	N = FirstNode;
          	do {
            	int d = C(N, N->Suc);
            	AddCandidate(N, N->Suc, d, INT_MAX);
            	AddCandidate(N->Suc, N, d, INT_MAX);
            	N = N->InitialSuc = N->Suc;
          	} while (N != FirstNode);
						Cost = FindTour();//LocalSearch

						for (int j = 0; j < PopulationSize; j++) {
                GainType OldCost = Cost;
                Cost = MergeTourWithIndividual(j);
            }


		        if (Cost < BestCost) {
  	          BestCost = Cost;
    	        RecordBetterTour();
     	       RecordBestTour();
				struct timeval actualTime; 
				gettimeofday(&actualTime, NULL);
				double currentTime = ((double) (actualTime.tv_sec) * 1.0e6 + (double) (actualTime.tv_usec))/1.0e6;
						 printf("Time: %lf ->", currentTime - InitialTime);
     	       WriteTour(OutputTourFileName, BestTour, BestCost);
     	       WriteTour(TourFileName, BestTour, BestCost);
      	  	}

						AddOffspring(Cost, i + PopulationSize);
        	}
					struct timeval actualTime; 
					gettimeofday(&actualTime, NULL);
					if (generation == 1){
						int maxDistance = 0;
						for (int i = 0; i < MaxPopulationSize * 2; i++){
							for (int j = 0; j < MaxPopulationSize * 2; j++){
								int d = getDistance(i, j);
								if (d > maxDistance){
									maxDistance = d;
								}
							}
						}
						initialDistance = maxDistance * initP;
					}
					double CurrentTime = ((double) (actualTime.tv_sec) * 1.0e6 + (double) (actualTime.tv_usec))/1.0e6;
					//int penaltyDistance = initP * Dimension - initP * Dimension * (CurrentTime - InitialTime) / TotalTimeLimit;
					int penaltyDistance = initialDistance - initialDistance * (CurrentTime - InitialTime) / TotalTimeLimit;
					printf("Penaliza: %d\n", penaltyDistance);
					ReplacementPhase(penaltyDistance);
					int previousM = m;
					m = initialMoveType + (maxMoveType + 1 - initialMoveType) * (CurrentTime - InitialTime) / TotalTimeLimit;
					m = min(m, maxMoveType);
					if (previousM != m){
						setMoveType(m);
					}
					fflush(stdout);
				}
				struct timeval actualTime; 
				gettimeofday(&actualTime, NULL);
				double CurrentTime = ((double) (actualTime.tv_sec) * 1.0e6 + (double) (actualTime.tv_usec))/1.0e6;
				if (CurrentTime - InitialTime > TotalTimeLimit){
					Runs = Run;	
				}
    }
    PrintStatistics();
    return EXIT_SUCCESS;
}
