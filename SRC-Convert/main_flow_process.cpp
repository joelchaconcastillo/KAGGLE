//= ===========================================================================
// Author       : Emmanuel Romero Ruiz, Joel Chacón
// Problem Name : 
// Description  : In this script is selected a regement of the KAGGLE-TSP problem, which is solved with Dynamic Programming or a heuristic.
// Arguments: cities_files, path (format KAGGLE), start node, end node, path result file..
//============================================================================
#include <bits/stdc++.h>
#include <iostream>
#include <numeric>
#include <cstring>
#include <cstdlib>
#include <cstdio>
//#include <filesystem>
using namespace std;
#define REP(i, n) for (int i = 0; i < int(n); i++)
#define forn(i,n) for (int i = 0; i < int(n); i++)
#define FOR(i, a, b) for (int i = a; i < int(b); i++)
#define FOREACH(i, v) for (__typeof((v).begin()) i = (v).begin(); i != (v).end(); i++)
#define SIZE(v) ((int)(v).size())
#define zero(v) memset(v, 0, sizeof(v))
#define pb push_back
#define mp make_pair
#define st first
#define nd second
#define ll long long
#define pii pair<int, int>
#define NCITIES 197769
#define DP 100
#define LKH_REPLACEMENT 101
#define LKH3 102
#include "common.hpp"
#include "LKH_Replacement.hpp"
#include "LKH3.hpp"
#include "LS_DP.hpp"
void readParams(int *argc, char ***argv)
{
    strcpy(filename_instance, (*argv)[1]);
    strcpy(filename_oldSolution, (*argv)[2]);
     l = atoi((*argv)[3]);
    len = atoi((*argv)[4]);
    strcpy(filename_newSolution, (*argv)[5]);
    seed = atoi((*argv)[6]);
    r = len + l - 1; // the code recives the left index and the pivot..

}
int main(int argc, char **argv){


  ///////Prepare data and read information
    sieve(400000);// printf("End Sieve\n");
    readParams(&argc, &argv);
    readSolution();
    readInstance();
    strcpy(currentDirectory, "/home/joel.chacon/KAGGLE/git/KAGGLE/SRC-Convert");
    //int IMPROVEMENT = LKH3;//LKH_REPLACEMENT;

     int IMPROVEMENT = DP;// LKH_REPLACEMENT;
    switch(IMPROVEMENT)
    {
       case DP:
         localSearch();
       break;
       case LKH_REPLACEMENT:
	LKH_Replacement();
       break;
       case LKH3:
        LKH3_Procedure();
       break;
    }

    saveNewSolution();
    printf("Score of old path: %f\n",evaluate(path));
    printf("Score of new path: %f",evaluate(nPath));
}
