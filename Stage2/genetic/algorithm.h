/*==========================================================================
// //  Author:
//     Description: 
//
// ===========================================================================*/


#ifndef __EVOLUTION_H_
#define __EVOLUTION_H_

#include <bits/stdc++.h>
#include <queue>
#include <iomanip>
#include <cfloat>
#include "global.h"
#include "common.h"
#include "individual.h"
#include "recomb.h"
#include "xover.h"
#include "localsearch.h"
#include "localsearch2.h"

class GENETIC
{

public:
	GENETIC();
	virtual ~GENETIC();

	void init_population();                  // initialize the population

	void evol_population();                                      
	void exec_emo(int run);

	void save_front(char savefilename[1024]);       
	void save_pos(char savefilename[1024]);

        void binary_tournament_selection(vector<CIndividual > &population, vector<CIndividual> &child_pop);

        void recombination(vector<CIndividual> &child_pop);
	void alternativerecombination(vector<CIndividual> &child_pop);
	void evaluate(vector<CIndividual> &child_pop);

	void improvement(vector<CIndividual> &child_pop);
        void update_diversity_factor();


	int distance(vector<int> &a, vector<int> &b);
	void diversity_replacement(vector<CIndividual> &population, vector<CIndividual> &child_pop);

	vector <CIndividual> population;
	vector<CIndividual> child_pop;	// memory solutions
public:
//
//	// algorithm parameters
	int nfes;
};

GENETIC::GENETIC()
{
cities = new city[NCITIES+1];
}

GENETIC::~GENETIC()
{
 delete [] cities;
}

int GENETIC::distance(vector<int> &a, vector<int> &b)
{
   int dist = 0;
   for(int i = 0; i < nvar; i++)
      dist += (a[i] == b[i])?0:1;
   return dist;
}
void GENETIC::init_population() //read the initial paths..
{
    FILE * file = fopen("list_initial_pop.txt", "r");
    //read a list of initial files...
	char filename_path[100];
	int i = 0; 
	while( fscanf(file, "%s\n", filename_path) != EOF && i < pops)
	{
		cout << "reading path " << filename_path<<" as individual "<<i<<endl;
		CIndividual indiv;
		// Randomize and evaluate solution
		indiv.init(filename_path);
		indiv.obj_eval();
		cout << indiv.cost<<endl;
		// Save in the population
		population.push_back(indiv);
		nfes++;
		i++;
	}
	if( i < pops) pops = i;
	child_pop = population;
   fclose(file);
}
void GENETIC::evol_population()
{
	update_diversity_factor();
        //binary tournament selction procedure
        binary_tournament_selection(population, child_pop);
        recombination(child_pop); 
//        alternativerecombination(child_pop); 
	//evaluation..
	evaluate(child_pop);
	//cout << "improvement parents"<<endl;
//	improvement(population);
//	cout << "improvement offspring"<<endl;
	improvement(child_pop);
	diversity_replacement(population, child_pop);
	
}
void GENETIC::improvement(vector<CIndividual> &child_pop)
{
//	LS1(child_pop);
	for(int i = 0; i < child_pop.size(); i++)
          cout << "individual.. " << i <<" before improvement.. "<< std::setprecision(9)<< child_pop[i].cost<<endl;
        // #pragma omp parallel for	
	for(int i = 0; i < child_pop.size(); i++)
	{
	   fast2opt(child_pop[i]);
	}
	for(int i = 0; i < child_pop.size(); i++)
          cout << "individual " << i <<" after improvement.. "<< std::setprecision(9)<< child_pop[i].cost<<endl;
   
}
void GENETIC::evaluate(vector<CIndividual> &child_pop)
{
//   #pragma omp parallel for	
   for(int i = 0; i < child_pop.size(); i++)
	child_pop[i].obj_eval();
}
void GENETIC::diversity_replacement(vector<CIndividual> &population, vector<CIndividual> &child_pop)
{
  vector< CIndividual *>  survivors, candidates, penalized;
  

  priority_queue < pair <int, CIndividual*> > pq;
  for(int i = 0 ; i < population.size(); i++)
  {
	  pq.push( make_pair(-population[i].cost, &population[i]));
	  pq.push( make_pair(-child_pop[i].cost, &child_pop[i]));
  }

  
  while(!pq.empty() &&  survivors.size() < population.size())
  {
     pair<int, CIndividual*> data = pq.top();
     pq.pop();
    
	if(survivors.size() >= pops)
	break;
	
	bool flag = true;
     for(int i = 0 ; i < survivors.size(); i++)
	{
	     double d = distance(survivors[i]->path, data.second->path);
		if(d < lowestDistanceFactor)
		{
		   flag = false;
		   break;
		}
	}
	if(!flag)
	{
	   penalized.push_back(data.second);   
	}
	else
	{
	   survivors.push_back(data.second);
	}
  }

     vector<int> v_distances(penalized.size(), INT_MAX);
     for(int i = 0 ;  i < penalized.size(); i++)
	{
	   for(int j = 0; j< survivors.size(); j++)
	   {
	      v_distances[i] = min(v_distances[i], distance( penalized[i]->path, survivors[j]->path));
	   }
	}

     while(survivors.size() < population.size())
     {
	double maxd = -INFINITY;
	int maxindex = -1;

	//find the max farthest point from survivor to children....
        for(int i = 0 ; i < penalized.size(); i++)
	{
		if( v_distances[i] > maxd)
	  	{
			maxd = v_distances[i];
			maxindex = i;
	  	}
	}
	//update the modified values..
        for(int i = 0 ; i < penalized.size(); i++)
        {
	   if(i==maxindex) continue;
	   v_distances[i] = min(v_distances[i] , distance(penalized[maxindex]->path, penalized[i]->path )  );
        }
	
        survivors.push_back(penalized[maxindex]);	
	iter_swap(penalized.begin()+maxindex, penalized.end()-1);
	penalized.pop_back();
	iter_swap(v_distances.begin()+maxindex, v_distances.end()-1);
	v_distances.pop_back();
     }
  
  vector<CIndividual> tmpPopulation;
  for(int i = 0 ; i < population.size(); i++)
  {
	 tmpPopulation.push_back(*survivors[i]); //note: this is necessary because we are working with memmory address
  }
   population = tmpPopulation;
}
//updates the lowest distance factor of the diversity explicitly promoted
void GENETIC::update_diversity_factor()
{
	double ratio = ((double) nfes)/max_nfes;
	lowestDistanceFactor = Initial_lowest_distance_factor - Initial_lowest_distance_factor*(ratio/0.9);
}
void GENETIC::recombination(vector<CIndividual> &child_pop)
{
   vector<CIndividual> child_pop2 = child_pop;
//	 #pragma omp parallel for	
   for(int i = 0; i < child_pop.size(); i+=2)
    {
       int indexa = int(rnd_uni(&rnd_uni_init)*pops);
       int indexb = int(rnd_uni(&rnd_uni_init)*pops);	
	//crossover
	  //mating nearest parents to indexa.....
//	  priority_queue< pair< int, int>> pq;
//	for(int j = 0; j < child_pop2.size(); j++)
//	{
//	   pq.push( make_pair(-distance(child_pop2[indexa].path, child_pop2[j].path), j));
//	} 
//	while(pq.top().first == 0 && !pq.empty()){ pq.pop(); 	indexb = pq.top().second;} //if distance is 0..
	//xover_kaggle(child_pop2[indexa], child_pop2[indexb], child_pop[i], child_pop[i+1]);
//	ERX_Symetric(child_pop2[indexa], child_pop2[indexb], child_pop[i], child_pop[i+1]);
//	AEX(population[indexa], population[indexb], child_pop[i], child_pop[i+1]);
	HGrex2opt(child_pop2[indexa], child_pop2[indexb], child_pop[i]);
       indexa = int(rnd_uni(&rnd_uni_init)*pops);
       indexb = int(rnd_uni(&rnd_uni_init)*pops);	
	HGrex2opt(child_pop2[indexa], child_pop2[indexb], child_pop[i+1]);
//	HGrex(child_pop2[indexa], child_pop2[(indexb+1)%pops], child_pop[i+1]);
   child_pop[i].obj_eval();
   child_pop[i+1].obj_eval();
cout <<"op "<< std::setprecision(9)<< child_pop[i].cost<<endl;
cout <<"op " <<std::setprecision(9)<< child_pop[i+1].cost<<endl;
    }
}
void GENETIC::alternativerecombination(vector<CIndividual> &child_pop)
{
   vector<CIndividual> child_pop2 = child_pop;
//	 #pragma omp parallel for	
   for(int i = 0; i < child_pop.size(); i+=2)
    {
       int indexa = int(rnd_uni(&rnd_uni_init)*pops);
       int indexb = int(rnd_uni(&rnd_uni_init)*pops);	

	while( distance(child_pop2[indexa].path, child_pop2[indexb].path) == 0)
	{
       		indexb = int(rnd_uni(&rnd_uni_init)*pops);	
	}
       xover_kaggle(child_pop2[indexa], child_pop2[indexb], child_pop[i], child_pop[i+1]);

	child_pop[i].obj_eval();
	child_pop[i+1].obj_eval();
     for(int p = 0; p < 10; p++)
     {   	
	CIndividual A=child_pop[i];
	CIndividual B=child_pop[i+1];
        indexa = int(rnd_uni(&rnd_uni_init)*pops);
///	while( distance(child_pop2[indexa].path, child_pop2[indexb].path) == 0)
///	{
///       		indexb = int(rnd_uni(&rnd_uni_init)*pops);	
///	}
	//crossover
	  //mating nearest parents to indexa.....
	  priority_queue< pair< int, int>> pq;
	for(int j = 0; j < child_pop2.size(); j++)
	{
	   pq.push( make_pair(-distance(child_pop2[indexa].path, child_pop2[j].path), j));
	} 
	while(pq.top().first == 0 && !pq.empty()){ pq.pop(); 	indexb = pq.top().second;} //if distance is 0..
       //xover_kaggle(child_pop2[indexa], child_pop2[indexb], child_pop[i], child_pop[i+1]);
	xover_kaggle(child_pop2[indexa], child_pop2[indexb], A, B);
	A.obj_eval();
	B.obj_eval();
//
	if( A.cost < child_pop[i].cost  ) child_pop[i] = A;
	if( B.cost < child_pop[i+1].cost && distance(A.path, B.path) > 0 ) child_pop[i+1] = B;
//	ERX_Symetric(population[indexa], population[indexb], child_pop[i], child_pop[i+1]);
//	AEX(population[indexa], population[indexb], child_pop[i], child_pop[i+1]);
//	HGrex(population[indexa], population[indexb], child_pop[i]);

     }
cout << std::setprecision(9)<< child_pop[i].cost<<endl;
cout << std::setprecision(9)<< child_pop[i+1].cost<<endl;
    }
}
void GENETIC::binary_tournament_selection(vector<CIndividual > &population, vector<CIndividual> &child_pop)
{
   for(int i = 0; i < population.size(); i++)
	{
	   int indexa = int(rnd_uni(&rnd_uni_init)*pops);
	   int indexb = int(rnd_uni(&rnd_uni_init)*pops);
	   if(population[indexa].cost < population[indexb].cost)
	      child_pop[i] = population[indexa];
	   else if(population[indexa].cost > population[indexb].cost)
	      child_pop[i] = population[indexb];
	   else 
	   {
	      child_pop[i] = (rnd_uni(&rnd_uni_init) < 0.5  )? population[indexa] : population[indexb];
	   }	
	}
}
void GENETIC::exec_emo(int run)
{
       char filename1[5024];
       char filename2[5024];
		seed = run;
	seed = (seed + 23)%1377;
	rnd_uni_init = -(long)seed;

        sieve(400000);// printf("End Sieve\n");
	char instance[100];
	strcpy(instance, "cities.csv");
	readInstance(instance);

	nfes      = 0;
	init_population(); //Initialize individuals...

	sprintf(filename1,"%s/POS/kaggle_path_RUN%d_seed_%d.dat_bounded",currentPATH,run, seed);
	sprintf(filename2,"%s/POF/kaggle_costs_RUN%d_seed_%d.dat_bounded",currentPATH,run, seed);

	while(nfes < max_nfes )
	{
		evol_population();
		nfes += pops;
	    if( !(nfes % (int)(0.1*max_nfes)  ))
	    {
	      cout << "nfes... "<< nfes <<endl;
              save_front(filename2); //save the objective space information
		cout << "best... " << std::setprecision(9)<< population[0].cost<<endl;
	    }
	}
	save_pos(filename1); //save the decision variable space information
        save_front(filename2); //save the objective space information
	population.clear();
}
void GENETIC::save_front(char saveFilename[1024])
{

    std::fstream fout;
	//fout.open(saveFilename,std::ios::out);
	fout.open(saveFilename,fstream::app|fstream::out );
	for(int n=0; n<pops; n++)
	{
			fout<<population[n].cost<<"  ";
		fout<<"\n";
	}
	fout.close();
}
void GENETIC::save_pos(char saveFilename[1024])
{
    std::fstream fout;
	//fout.open(saveFilename,std::ios::out);
	fout.open(saveFilename, fstream::app|fstream::out);
	for(int n=0; n<pops; n++)
	{
		for(int k=0;k<nvar;k++)
			fout<<population[n].path[k] << "  ";
		fout<<"\n";
	}
	fout.close();
}



#endif
