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
#include <strings.h>

int min(int a, int b){
	return (a < b)?a:b;
}

void swap(int **a, int **b){
	int *tmp = *a;
	*a = *b;
	*b = tmp;
}

void swapIndividual(int i, int j){
	GainType tmp = Fitness[i];
	Fitness[i] = Fitness[j];
	Fitness[j] = tmp;
	int *P;
	P = connections[i];
	connections[i] = connections[j];
	connections[j] = P;
  P = Population[i];
  Population[i] = Population[j];
  Population[j] = P;
}

int getRandomInteger0_N(int n){
	return (int) ((n + 1.0)*rand()/(RAND_MAX+1.0));
}

int getDistance(int i, int j){
	int common = 0;
	for (int k = 1; k <= Dimension; k++){
		int first = Population[i][k-1];
		int second = Population[i][k];
		if ((connections[j][first] == second) || (connections[j][second] == first)){
			common++;
		}
	}
	return Dimension - common;
}

#define NON_DOMINANCES_EQUALS 1
#define FIRST_DOMINATES 2
#define SECOND_DOMINATES 3
#define NON_DOMINANCES 4


//Select MaxPopulationSize from the 2 * MaxPopulationSize individuals
void ReplacementPhase(int distanceToPenalize){
	//Calculate all distances (ojo, no precalcular todo esto solo es para checar)
	GainType modifiedFitness[MaxPopulationSize*2];
	memcpy(modifiedFitness, Fitness, sizeof(modifiedFitness));

	/*
	int distances[MaxPopulationSize*2][MaxPopulationSize*2];
	for (int i = 0; i < MaxPopulationSize * 2; i++){
		for (int j = 0; j < MaxPopulationSize * 2; j++){
			distances[i][j] = getDistance(i, j);
			printf("Distancia entre %d y %d: %d\n", i, j, distances[i][j]);
		}
	}

	for (int i = 0; i < MaxPopulationSize * 2; i++){
		for (int j = 0; j < MaxPopulationSize * 2; j++){
			if (distances[i][j] != distances[j][i]){
				printf("%d y %d\n", distances[i][j], distances[j][i]);
				printf("Error interno");
				exit(-1);
			}
		}
	}*/


	int survivors[MaxPopulationSize*2];
	bzero(survivors, sizeof(survivors));
	int dcn[MaxPopulationSize*2];
	for (int i = 0; i < MaxPopulationSize * 2; i++){
		dcn[i] = INT_MAX;
	}

	//Search the best
	int bestValue = Fitness[0];
	int bestIndexes[MaxPopulationSize*2];
	int bestIndexesSize = 1;
	bestIndexes[0] = 0;
	for (int i = 1; i < MaxPopulationSize * 2; i++){
		if (Fitness[i] < bestValue){
			bestValue = Fitness[i];
			bestIndexesSize = 1;
			bestIndexes[0] = i;
		} else if (Fitness[i] == bestValue){
			bestIndexes[bestIndexesSize++] = i;
		}
	}
	int bestIndex = bestIndexes[getRandomInteger0_N(bestIndexesSize-1)];

	survivors[bestIndex] = 1;

	int lastSurvivor = bestIndex;
	int currentSurvivors = 1;
	while(currentSurvivors != MaxPopulationSize){
		//Update distances
		for (int i = 0; i < MaxPopulationSize * 2; i++){
			if (survivors[i]){
				continue;
			} else {
				dcn[i] = min(dcn[i], getDistance(i, lastSurvivor));
			}
		}

		//Penalize
		for (int i = 0; i < MaxPopulationSize * 2; i++){
			if (survivors[i]){
				continue;
			} else {
				if (dcn[i] <= distanceToPenalize){
					modifiedFitness[i] = INT_MAX;
				}
			}
		}

		/*for (int i = 0; i < MaxPopulationSize * 2; i++){
			printf("%d: ", i);
			if (survivors[i]){
				printf("survivor\n");
			} else {
				printf("%d %lld\n", dcn[i], modifiedFitness[i]);
			}
		}*/


		//Mark non-dominated
		int dominated[MaxPopulationSize * 2];
		bzero(dominated, sizeof(dominated));
		int nonDominatedIndexes[MaxPopulationSize * 2];
		int nonDominatedSize = 0;
		for (int i = 0; i < MaxPopulationSize * 2; i++){
			if ((survivors[i]) || (dominated[i])){
				continue;
			}
			for (int j = i + 1; j < MaxPopulationSize * 2; j++){
				if ((survivors[j]) || (dominated[j])){
					continue;
				}
				int dominance;
				if ((dcn[i] == dcn[j]) && (modifiedFitness[i] == modifiedFitness[j])){
					dominance = NON_DOMINANCES_EQUALS;
				} else if ((dcn[i] >= dcn[j]) && (modifiedFitness[i] <= modifiedFitness[j])){
					dominance = FIRST_DOMINATES;
				} else if ((dcn[j] >= dcn[i]) && (modifiedFitness[j] <= modifiedFitness[i])){
					dominance = SECOND_DOMINATES;
				} else {
					dominance = NON_DOMINANCES;
				}
				if (dominance == FIRST_DOMINATES){
					dominated[j] = 1;
				} else if (dominance == SECOND_DOMINATES) {
					dominated[i] = 1;
					break;
				} else if (dominance == NON_DOMINANCES_EQUALS){
					dominated[j] = 1;
				}
			}
			if (!dominated[i]){
				nonDominatedIndexes[nonDominatedSize++] = i;
			}
		}
		/*printf("No dominados\n");
		for (int i = 0; i < nonDominatedSize; i++){
			printf("%d\n", nonDominatedIndexes[i]);
		}
		printf("Fin no dominados\n");*/

		lastSurvivor = nonDominatedIndexes[getRandomInteger0_N(nonDominatedSize-1)];

		survivors[lastSurvivor] = 1;
		currentSurvivors++;
	}

	int currentPos = 0;
	for (int i = 0; i < 2 * PopulationSize; i++){
		if (survivors[i]){//Place in currentPos
			swapIndividual(i, currentPos);
			currentPos++;
		}
	}

}

//Add children in the given position
void AddOffspring(GainType Cost, int position){
		//printf("Posicion: %d\n", position);
    int *P = Population[position];
    Node *N = FirstNode;
    Fitness[position] = Cost;
    P = Population[position];
    int i = 1;
    do{
        P[i++] = N->Id;
    } while ((N = N->Suc) != FirstNode);
    P[0] = P[Dimension];
		for (int i = 1; i <= Dimension; i++){
			int first = Population[position][i-1];
			int second = Population[position][i];
			connections[position][first] = second;
		}
}

/*
 * The AddToPopulation function adds the current tour as an individual to 
 * the population. The fitness of the individual is set equal to the cost
 * of the tour. The population is kept sorted in increasing fitness order.
 */

void AddToPopulation(GainType Cost)
{
    int i, *P;
    Node *N;

    if (!Population) {
        assert(Population =
               (int **) malloc(MaxPopulationSize * 2 * sizeof(int *)));
        assert(connections =
               (int **) malloc(MaxPopulationSize * 2 * sizeof(int *)));

        for (i = 0; i < MaxPopulationSize * 2; i++){
            assert(Population[i] =
                   (int *) malloc((1 + Dimension) * sizeof(int)));
						assert(connections[i] =
                   (int *) malloc((1 + Dimension) * sizeof(int)));

				}
        assert(Fitness =
               (GainType *) malloc(MaxPopulationSize * 2 * sizeof(GainType)));
    }
    for (i = PopulationSize; i >= 1 && Cost < Fitness[i - 1]; i--) {
        Fitness[i] = Fitness[i - 1];
				P = connections[i];
				connections[i] = connections[i-1];
				connections[i-1] = P;
        P = Population[i];
        Population[i] = Population[i - 1];
        Population[i - 1] = P;
    }
    Fitness[i] = Cost;
    P = Population[i];
    N = FirstNode;
    int pos = 1;
    do
        P[pos++] = N->Id;
    while ((N = N->Suc) != FirstNode);
    P[0] = P[Dimension];
		for (int j = 1; j <= Dimension; j++){
			int first = Population[i][j-1];
			int second = Population[i][j];
			connections[i][first] = second;
		}

    PopulationSize++;
}

/*
 * The ApplyCrossover function applies a specified crossover operator to two 
 * individuals.
 */

void ApplyCrossover(int i, int j)
{
    int *Pi, *Pj, k;

    Pi = Population[i];
    Pj = Population[j];
    for (k = 1; k <= Dimension; k++) {
        NodeSet[Pi[k - 1]].Suc = &NodeSet[Pi[k]];
        NodeSet[Pj[k - 1]].Next = &NodeSet[Pj[k]];
    }
    if (TraceLevel >= 1)
        printff("Crossover(%d,%d)\n", i + 1, j + 1);
    /* Apply the crossover operator */
    Crossover();
}

#define Free(s) { free(s); s = 0; }

/*
 * The FreePopulation function frees the memory space allocated to the 
 * population.
 */

void FreePopulation()
{
    if (Population) {
        int i;
        for (i = 0; i < MaxPopulationSize; i++)
            Free(Population[i]);
        Free(Population);
        Free(Fitness);
    }
    PopulationSize = 0;
}

/*
 * The HasFitness function returns 1 if the population contains an
 * individual with fitness equal to a given tour cost; otherwise 0.
 *
 * Since the population is sorted in fitness order the test may be
 * made by binary search.
 */

int HasFitness(GainType Cost)
{
    int Low = 0, High = PopulationSize - 1;
    while (Low < High) {
        int Mid = (Low + High) / 2;
        if (Fitness[Mid] < Cost)
            Low = Mid + 1;
        else
            High = Mid;
    }
    return High >= 0 && Fitness[High] == Cost;
}

/*
 * Random01 is an auxiliary function for computing a random double number
 * in the range [0;1).
 */

static double Random01()
{
    return ((double) Random()) / INT_MAX;
}

int BinaryTournament(){
	int r1 = getRandomInteger0_N(MaxPopulationSize); 
	int r2 = getRandomInteger0_N(MaxPopulationSize);
	if (Fitness[r1] < Fitness[r2]){
		return r1;
	} else {
		return r2;
	}
}

/*
 * The LinearSelection function is used to select an individual with 
 * random linear bias towards the best members of the population.
 * The parameter Bias is a number between 1.0 and 2.0.
 *
 * See
 *     Darrell Whitley,
 *     The GENITOR algorithm and selection pressure:
 *     Why rank-based allocation of reproductive trials is best. 
 *     Proceedings of the Third International Conference on Genetic Algorithms,
 *     1989.
 */

int LinearSelection(int Size, double Bias)
{
    return (int) (Size *
                  (Bias -
                   sqrt((Bias * Bias - 4 * (Bias - 1) * Random01()))) /
                  2 / (Bias - 1));
}

/*
 * The MergeTourWithIndividual function attempts to find a short tour by
 * merging the current tour with a specified inddividual of the population.
 * The merging algorithm is the iterative partial transcription algrithm
 * described by Mobius, Freisleben, Merz and Schreiber.
 */

GainType MergeTourWithIndividual(int i)
{
    int *Pi, k;

    assert(i >= 0 && i < PopulationSize);
    Pi = Population[i];
    for (k = 1; k <= Dimension; k++)
        NodeSet[Pi[k - 1]].Next = &NodeSet[Pi[k]];
    return MergeWithTour();
}

/*
 * The PrintPopulation function prints the cost and gap to optimum for
 * each individual of the population.
 */

void PrintPopulation()
{
    int i;
    printff("Population:\n");
    for (i = 0; i < PopulationSize; i++) {
        printff("%3d: " GainFormat, i + 1, Fitness[i]);
        if (Optimum != MINUS_INFINITY && Optimum != 0)
            printff(", Gap = %0.4f%%",
                    100.0 * (Fitness[i] - Optimum) / Optimum);
        printff("\n");
    }
}

/*
 * The ReplaceIndividualWithTour function replaces a given individual in 
 * the population by an indidual that represents the current tour.
 * The population is kept sorted in increasing fitness order.
 */

void ReplaceIndividualWithTour(int i, GainType Cost)
{
    int j, *P;
    Node *N;

    assert(i >= 0 && i < PopulationSize);
    Fitness[i] = Cost;
    P = Population[i];
    N = FirstNode;
    for (j = 1; j <= Dimension; j++) {
        P[j] = N->Id;
        N = N->Suc;
    }
    P[0] = P[Dimension];
    while (i >= 1 && Cost < Fitness[i - 1]) {
        Fitness[i] = Fitness[i - 1];
        Population[i] = Population[i - 1];
        i--;
    }
    Fitness[i] = Cost;
    Population[i] = P;
    while (i < PopulationSize - 1 && Cost > Fitness[i + 1]) {
        Fitness[i] = Fitness[i + 1];
        Population[i] = Population[i + 1];
        i++;
    }
    Fitness[i] = Cost;
    Population[i] = P;
}

/* 
 * The DistanceToIndividual returns the number of different edges between 
 * the tour (given by OldSuc) and individual i. 
 */

static int DistanceToIndividual(int i) { 
    int Count = 0, j, *P = Population[i];
    Node *N;
    
    for (j = 0; j < Dimension; j++) {
        N = &NodeSet[P[j]];
        (N->Next = &NodeSet[P[j + 1]])->Prev = N;
    }
    N = FirstNode;
    do
        if (N->OldSuc != N->Next && N->OldSuc != N->Prev)
            Count++;
    while ((N = N->OldSuc) != FirstNode);
    return Count;
}

/*
 * The ReplacementIndividual function returns the individual to be 
 * replaced with the current tour. The function implements the 
 * replacement strategy (CD/RW) proposed in
 *
 *      M. Lozano, F. Herrera, and J. R. Cano,
 *      Replacement strategies to preserve useful diversity in
 *      steady-state genetic algorithms.
 *      Information Sciences 178 (2008) 4421–4433.
 */

int ReplacementIndividual(GainType Cost) {
    int i, j, d, *P;
    int MinDist = INT_MAX, CMin = PopulationSize - 1;
    Node *N = FirstNode;
    while ((N = N->OldSuc = N->Suc) != FirstNode);
    for (i = PopulationSize - 1; i >= 0 && Fitness[i] > Cost; i--) {
        if ((d = DistanceToIndividual(i)) < MinDist) {
            CMin = i;
            MinDist = d;
        }
    }
    if (CMin == PopulationSize - 1)
        return CMin;
    P = Population[CMin];
    for (j = 0; j < Dimension; j++)
        NodeSet[P[j]].OldSuc = &NodeSet[P[j + 1]];
    for (i = 0; i < PopulationSize; i++)
        if (i != CMin && (d = DistanceToIndividual(i)) < MinDist)
            return PopulationSize - 1;
    return CMin;
}
