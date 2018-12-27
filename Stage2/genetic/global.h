#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <memory.h>
#include <vector>
#include <cassert>
#include <algorithm>
#include "random.h"

#define NCITIES 197769
const int MAX = 1001000;


using namespace std;

struct city{
    int id;
    //long long x, y;
    double x, y;
};

bitset<1001000> isPrime;
long long sieve_size;
//city cities[MAX];
city *cities;
vector<int> primeIds;

//------------- Parameters in test instance ------------------
int pops = 100; //population size
int nvar  = NCITIES;
char    currentPATH[500];//filname path

long int  max_nfes = 10000; //The function evaluation criteria is prefered than generations..
//------------- Parameters in random number ------------------
int     seed    = 177; //Default seed...
long    rnd_uni_init;        

double Initial_lowest_distance_factor=0.2*nvar, lowestDistanceFactor; 

//------------- Parameters in VSD-MOEA
double  realx=0.9,  realm = 1.0/nvar;    // crossover, mutation, selection probabilities
int run;

#endif
