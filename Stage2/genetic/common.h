#ifndef __COMMON_H_
#define __COMMON_H_

#include "global.h"

void readInstance(char *filename_instance){
    FILE *file = fopen(filename_instance, "r");
    int N = NCITIES;//n = 197769;
    double x, y;
    int id;
    char trash[100];
    fscanf(file, " %s\n", trash);
    while(N--){
        fscanf(file, "%d,%lf,%lf\n", &id, &x, &y);
        cities[id].id = id;
        cities[id].x = x;
        cities[id].y = y;
    }
    fclose(file);
}
void readNewSolution(char *fileSolution, int *path_segment, int *inversePath){
    //strcpy(fileSolution, (*argv)[1]);
    char trash[100];
    FILE *file = fopen(fileSolution, "r");
    fscanf(file, "%[^\n]\n", trash); //HEADER of six lines...
    fscanf(file, "%[^\n]\n", trash);
    fscanf(file, "%[^\n]\n", trash);
    fscanf(file, "%[^\n]\n", trash);
    fscanf(file, "%[^\n]\n", trash);
    fscanf(file, "%[^\n]\n", trash);
    //int l = atoi((*argv)[3]), r = atoi((*argv)[4]);
    for(int i = 0; i < NCITIES; i++) 
    {
	fscanf(file, "%d\n", path_segment + i);
	inversePath[path_segment[i]] = i;
	path_segment[i]--; //index zero..
    }
    fclose(file);
}
double distd(int pos, int c1, int c2){
    double d = (sqrt((cities[c1].x - cities[c2].x)*(cities[c1].x - cities[c2].x) + (cities[c1].y - cities[c2].y)*(cities[c1].y - cities[c2].y)));
    pos++; // Esta es la parte que agregue
    if(pos%10 == 0){
        if(isPrime[c1]) return d;
        else return 1.1*d;
    }
    else return d;
}
double evaluate(int *pathc){
    double len = 0.0;
    for(int i = 0; i < NCITIES; i++){
        int j = (i + 1)%NCITIES;
        len += distd(i, pathc[i], pathc[j]);
    }
    return len;
}
//void load_data(char filename[1024], vector< vector<double> > &data, int dim)
//{
//	std::ifstream readf(filename);
//	vector<double> vect = vector<double>(dim, 0);
//	while(!readf.eof())
//	{
//        for(int i=0; i<dim; i++)
//		{
//			readf>>vect[i];
//			//printf(" %f ", vect[i]);
//		}
//		data.push_back(vect);
//		//vect.clear();    // bug here. 
//	}
//
//	readf.close();    
//}
void sieve(long long upperbound){
    sieve_size = upperbound + 1;
    isPrime.set();
    isPrime[0] = isPrime[1] = false;
	
    for(long long i = 2; i < sieve_size; i++) if(isPrime[i]){
        for(long long j = i*i; j < sieve_size; j += i) isPrime[j] = false;
    }
}
#endif
