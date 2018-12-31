#ifndef _XOVER_H_
#define _XOVER_H_

#include "global.h"
#include "individual.h"

void ERX_individual(CIndividual &C, vector<set<int>> edges)
{
  //select start randomly...
  int city = rand()%NCITIES;

  vector<int> currentPath(NCITIES);
  vector<bool> selected(NCITIES, false);
  int start = -1;
  vector<int> indexDictionary(NCITIES), index(NCITIES);
  for(int i = 0; i < NCITIES; i++){indexDictionary[i] = i; index[i]=i;}
  for(int i = 0; i < NCITIES-1; i++)
  {
     currentPath[i] = city;
     selected[city]  =true; 
     if(city == 0) start = i;
     //check al edges of the i-city and get the minimum conncted edge
     priority_queue<pair<int, int> >recordsizes;
     for( set<int>::iterator it = edges[city].begin(); it != edges[city].end(); it++)
     {
	if(selected[*it]) continue; //avoid small cycles...
//	set<int>::iterator d = edges[*it].find(city);
//	if( d != edges[*it].end())
//	  edges[*it].erase(d);
	recordsizes.push(make_pair(-edges[*it].size(), *it));	
     }
     indexDictionary[index[index.size()-1]] = indexDictionary[city];
     iter_swap(index.begin()+indexDictionary[city], index.end()-1);
     index.pop_back();
//     edges[city].clear(); //it is not necessary at all..
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
   currentPath[NCITIES-1] = city;
   vector<int> check(NCITIES,0);
  for(int i = 0; i < NCITIES; i++)
   {
         C.path[i] = currentPath[(start+i)%NCITIES];
         C.inversePath[C.path[i]]=i;
	check[C.path[i]]++;
   }
  int sum = 0;
  for(int i = 0; i < NCITIES; i++)
	{
	   if(check[i]==0){cout << "bad "<<i <<endl; exit(0); }
	   if(check[i]==2){cout << "bad2 "<<i <<endl;exit(0); }
	}
}
//Edge Recombination Crossover 
void ERX_Symetric(
  CIndividual &P1, 
  CIndividual &P2, 
  CIndividual &C1, 
  CIndividual &C2) { 

  vector<set<int>> edges(NCITIES);
  
  //start node "santa house"..
  for(int i = 0; i < NCITIES; i++)
  {
	//first parent..
	edges[P1.path[i]].insert(P1.path[(i+1)%NCITIES]);
	edges[P1.path[i]].insert(P1.path[(i-1+NCITIES)%NCITIES]);

	//second parent..
	edges[P2.path[i]].insert(P2.path[(i+1)%NCITIES]);
	edges[P2.path[i]].insert(P2.path[(i-1+NCITIES)%NCITIES]);
  }

   ERX_individual(C1, edges);
   ERX_individual(C2, edges);
 
}
void ERX_Asymetric(
  CIndividual &P1, 
  CIndividual &P2, 
  CIndividual &C1, 
  CIndividual &C2) { 

  vector<set<int>> edges(NCITIES);
  
  //start node "santa house"..
  for(int i = 0; i < NCITIES; i++)
  {
	//first parent..
	edges[P1.path[i]].insert(P1.path[(i+1)%NCITIES]);

	//second parent..
	edges[P2.path[i]].insert(P2.path[(i+1)%NCITIES]);
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
  C.inversePath[C.path[0]]=0;
  checked[city] = true;

   indexDictionary[index[index.size()-1]] = indexDictionary[city];
   iter_swap(index.begin()+indexDictionary[city], index.end()-1);
   index.pop_back();
  for(int i = 1; i < NCITIES; i++)
  {
     city = parents[(i+1)%2]->path[(parents[(i+1)%2]->inversePath[city]+1 )%NCITIES];
     if(checked[city])
	city = index[rand()%index.size()];
     C.path[i] = city;
     C.inversePath[C.path[i]]=i;
     checked[city]=true;
     indexDictionary[index[index.size()-1]] = indexDictionary[city];
     iter_swap(index.begin()+indexDictionary[city], index.end()-1);
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

     indexDictionary[index[index.size()-1]] = indexDictionary[city];
     iter_swap(index.begin()+indexDictionary[city], index.end()-1);
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

void HGrex2opt(
  CIndividual &P1, 
  CIndividual &P2, 
  CIndividual &C ) { 

  vector<list<int>> Components;
   list<int> group;
  for(int i = 0; i < NCITIES; i++)
  {
     group.push_back(P1.path[i]);
     if(i < NCITIES-1)
     if( P1.path[i+1] != P2.path[P2.inversePath[P1.path[i]]+1])
     {
        Components.push_back(group);
	group.clear();
     }
  } 
  Components.push_back(group);


//	cout << Components.size()<<endl;
 int indexcomponent = 0;
 vector<bool> selectedComponent(Components.size(),false);
 //// ensambling no-connected edges
//	cout << Components.size()<<endl;
 for(int i = 0, index = 0; i < Components.size(); i++)
 {
   selectedComponent[indexcomponent] = true;
   while(!Components[indexcomponent].empty())
   {
     C.path[index] = Components[indexcomponent].front();
     C.inversePath[C.path[index]] = index;
     index++;
     Components[indexcomponent].pop_front();
   }
//	if( !(i%1000))
//	cout << i<<endl;
   //check the nearest city..
   double mind = DBL_MAX;
   for(int j = 0; j < Components.size(); j++)
    {
	if(selectedComponent[j]) continue;
	//select the city with best distances
	double d = distd(index-1, C.path[index-1], Components[j].front());
	if(mind > d) 
	{
	    mind = d;
	    indexcomponent = j;
	}
    }
 }
  

  vector<int> check(NCITIES,0);
  for(int i = 0; i < NCITIES; i++)
   {
	check[C.path[i]]++;
   }
  int sum = 0;
  for(int i = 0; i < NCITIES; i++)
	{
	   if(check[i]==0){cout << "bad "<<i <<endl;exit(0); }
	   if(check[i]>=2){cout << "bad2 "<<i <<endl;exit(0); }
	}
}
void HGrexNearest(
  CIndividual &P1, 
  CIndividual &P2, 
  CIndividual &C ) { 

  vector<bool> cityselected(NCITIES, false); 
  vector<list<int>> Components;
   list<int> group;
  for(int i = 0; i < NCITIES; i++)
  {
     group.push_back(P1.path[i]);
     if(i < NCITIES-1)
     if( P1.path[i+1] != P2.path[P2.inversePath[P1.path[i]]+1])
     {
        Components.push_back(group);
	group.clear();
     }
  } 
  Components.push_back(group);

 vector<int> inverseindexcomponent(NCITIES, -1);
 for(int i = 0; i < Components.size(); i++)
	{
	 cityselected[Components[i].front()] = true;
	 inverseindexcomponent[Components[i].front()] = i;
	}

//	cout << Components.size()<<endl;
 int indexcomponent = 0;
 vector<bool> selectedComponent(Components.size(),false);
 //// ensambling no-connected edges
//	cout << Components.size()<<endl;
 for(int i = 0, index = 0; i < Components.size(); i++)
 {
   selectedComponent[indexcomponent] = true;
   cityselected[Components[indexcomponent].front()] = false;
   while(!Components[indexcomponent].empty())
   {
     C.path[index] = Components[indexcomponent].front();
     C.inversePath[C.path[index]] = index;
     index++;
     Components[indexcomponent].pop_front();
   }
//	if( !(i%1000))
//	cout << i<<endl;
   //check the nearest city..
   indexcomponent = -1;
   int indexesk[51];
   int ksize = 50;
   double mind = DBL_MAX;
   for(int k = 0; k < ksize; k++)
  {
    for(int j = 0; j < NearestCities[C.path[index-1]].size(); j++)
    {
//     if(!cityselected[indexesk[k]])continue;
  //  int j = indexesk[k];
    if(selectedComponent[ j ]) continue;
        if( NearestCities[C.path[index-1]][k] != Components[j].front()) continue;
	//select the city with best distances
	double d = distd(index-1, C.path[index-1], j);
	if(mind > d) 
	{
	    mind = d;
	    //indexcomponent = inverseindexcomponent[j];
	    indexcomponent = j;
	}
     }
   }
	          if(indexcomponent == -1){

   double mind = DBL_MAX;
		    for(int j = 0; j < Components.size(); j++)
		    {
			if(selectedComponent[j]) continue;
			indexcomponent = j ; break;
			//select the city with best distances
			double d = distd(index-1, C.path[index-1], Components[j].front());
			if(mind > d) 
			{
			    mind = d;
			    indexcomponent = j;
			}
		    }
		}
 }
  

  vector<int> check(NCITIES,0);
  for(int i = 0; i < NCITIES; i++)
   {
	check[C.path[i]]++;
   }
  int sum = 0;
  for(int i = 0; i < NCITIES; i++)
	{
	   if(check[i]==0){cout << "bad "<<i <<endl;exit(0); }
	   if(check[i]>=2){cout << "bad2 "<<i <<endl;exit(0); }
	}
}

#endif
