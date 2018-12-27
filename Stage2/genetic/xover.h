#ifndef _XOVER_H_
#define _XOVER_H_

#include "global.h"
#include "individual.h"

void ERX_individual(CIndividual &C, vector<set<int>> edges)
{
  //select start randomly...
  int city = rand()%NCITIES;
  int currentPath[NCITIES];
  int start = -1;
  vector<int> indexDictionary(NCITIES), index(NCITIES);
  for(int i = 0; i < NCITIES; i++){indexDictionary[i] = i; index[i]=i;}
  for(int i = 0; i < NCITIES; i++)
  {
     currentPath[i] = city;
     if(city == 0) start = i;
     //check al edges of the i-city and get the minimum conncted edge
     priority_queue<pair<int, int> >recordsizes;
     for( set<int>::iterator it = edges[city].begin(); it != edges[city].end(); it++)
     {
	set<int>::iterator d = edges[*it].find(city);
	if(d != edges[*it].end())edges[*it].erase(d);
	recordsizes.push(make_pair(-edges[*it].size(), *it));	
     }
     iter_swap(index.begin()+indexDictionary[city], index.end()-1);
     indexDictionary[index[index.size()-1]] = indexDictionary[city];
     index.pop_back();

     if( !recordsizes.empty() )
     {
	int size = recordsizes.top().first;
	vector<int> choices;
        while( recordsizes.top().first == size && !recordsizes.empty())
	{
	   choices.push_back(recordsizes.top().second); 
	   recordsizes.pop();
	}
        city= choices[rand()%choices.size()];
     }else{ city= index[rand()%index.size()]; }
  }  
   vector<int> check(NCITIES,0);
  for(int i = 0; i < NCITIES; i++)
   {
         C.path[i] = currentPath[(start+i)%NCITIES];
         C.inversePath[C.path[i]]=i;
	check[C.path[i]]++;
   }
  int sum = 0;
  for(int i = 0; i < NCITIES; i++)sum += check[i];
	cout <<"SUM "<<sum<<endl;
}
//Edge Recombination Crossover 
void ERX_Symetric(
  CIndividual &P1, 
  CIndividual &P2, 
  CIndividual &C1, 
  CIndividual &C2) { 

  vector<set<int>> edges(NCITIES);
  
  //Store the edges of both parents...
//  vector< set<int> > edges(NCITIES);
  //start node "santa house"..
  for(int i = 0; i < NCITIES; i++)
  {
	//first parent..
	edges[i].insert(P1.path[(i+1)%NCITIES]);
	edges[i].insert(P1.path[(i-1+NCITIES)%NCITIES]);
	//second parent..
	edges[i].insert(P2.path[(i+1)%NCITIES]);
	edges[i].insert(P2.path[(i-1+NCITIES)%NCITIES]);
  }

   ERX_individual(C1, edges);
   ERX_individual(C2, edges);
 
}


void AEX_individual(CIndividual &P1, CIndividual&P2, CIndividual &C)
{
  vector<bool> checked(NCITIES, false);
  vector<int> indexDictionary(NCITIES), index(NCITIES);
  for(int i = 0; i < NCITIES; i++){indexDictionary[i] = i; index[i]=i;}

  //choose the first arc...
  vector< CIndividual *>parents(2);
  parents[0] = &P1;
  parents[1] = &P2;

  int city = parents[0]->path[0];
  C.path[0] = city;
  checked[city] = true;

  iter_swap(index.begin()+indexDictionary[city], index.end()-1);
  indexDictionary[city] = indexDictionary[index.size()-1];
  index.pop_back();

  for(int i = 1; i < NCITIES; i++)
  {
     city = parents[(i+1)%2]->path[(parents[(i+1)%2]->inversePath[city]+1 )%NCITIES];
     if(checked[city])
	city = index[rand()%index.size()];
     C.path[i] = city;
     C.inversePath[C.path[i]]=i;
     checked[city]=true;
     iter_swap(index.begin()+indexDictionary[city], index.end()-1);
     indexDictionary[index[index.size()-1]] = indexDictionary[city];
     index.pop_back();
  }
}
//Alternating Edges Crossover 
void AEX(
  CIndividual &P1, 
  CIndividual &P2, 
  CIndividual &C1, 
  CIndividual &C2) { 
   AEX_individual(P1, P2, C1);
   AEX_individual(P2, P1, C2);
}
void HGrex(
  CIndividual &P1, 
  CIndividual &P2, 
  CIndividual &C ) { 

  vector<bool> checked(NCITIES, false);
  vector<int> indexDictionary(NCITIES), index(NCITIES);
  for(int i = 0; i < NCITIES; i++){indexDictionary[i] = i; index[i]=i;}
  int city = 0;// rand()%NCITIES;
  for(int i = 0; i < NCITIES; i++)
  {
     C.path[i] = city; 
     C.inversePath[C.path[i]]=i;
     checked[city] = true;

     iter_swap(index.begin()+indexDictionary[city], index.end()-1);
     indexDictionary[index[index.size()-1]] = indexDictionary[city];
     index.pop_back();
     if( checked[P1.path[(P1.inversePath[city]+1)%NCITIES ]] && !checked[P2.path[(P2.inversePath[city]+1)%NCITIES ]]  )
	city = P2.path[(P2.inversePath[city]+1)%NCITIES ];
     else if( !checked[P1.path[(P1.inversePath[city]+1)%NCITIES ]] && checked[P2.path[(P2.inversePath[city]+1)%NCITIES ]]  )
	city = P1.path[(P1.inversePath[city]+1)%NCITIES ];
     else if( !checked[P1.path[(P1.inversePath[city]+1)%NCITIES ]] && !checked[P2.path[(P2.inversePath[city]+1)%NCITIES ]]  )
     {
	int dest1city = P1.path[(P1.inversePath[city]+1)%NCITIES ];
	int dest2city = P2.path[(P2.inversePath[city]+1)%NCITIES ];
	if( distd(i, city, dest1city )  < distd(i, city,dest2city ) ) city = dest1city;
	else city = dest2city;
     }
       if(checked[city] && !index.empty())
	{
	   city = index[rand()%index.size()];    
	}
  }
}




#endif
