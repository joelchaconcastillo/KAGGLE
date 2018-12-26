#ifndef _XOVER_H_
#define _XOVER_H_

#include "global.h"
#include "individual.h"


void ERX_individual(CIndividual &C, vector<set<int>> &edges)
{
  vector<bool> selected(NCITIES, false);
  vector<int> edgescount(NCITIES, 4); // In the symetric case each node has four edges availables..
  //select start randomly...
  int currentindex = rand()%NCITIES;
  int currentPath[NCITIES];
  int start = -1;
  for(int i = 0; i < NCITIES; i++)
  {
     currentPath[i] = currentindex;
     if(currentindex == 0) start = i;
     selected[currentindex] = true;
     //check al edges of the i-city and get the minimum conncted edge
     priority_queue< pair<int, int > > recordsizes; 
     for(set<int>::iterator it = edges[currentindex].begin(); it != edges[currentindex].end(); it++)
     {
	if( selected[*it] )continue;
	recordsizes.push(make_pair(-edgescount[*it], *it));
	edgescount[*it]--;
     }
     if( !recordsizes.empty() )//check that an index is available if it is -1 then it means that it doesn't have an edge
     {
	int size = recordsizes.top().first;
	vector<int> choices;
        while( recordsizes.top().first == size)
	{
	   choices.push_back(recordsizes.top().second); 
	   recordsizes.pop();
	}
        currentindex= choices[rand()%choices.size()];
     }else{  cout << "error.." <<endl; exit(0);}
  }  
  //fix path...
  for(int i = 0; i < NCITIES; i++)
   {
	 C.path[i] = currentPath[start+i];
	//update inversepath..
	 C.inversePath[C.path[i]]=i;
   }
}

void ERX_Symetric(
  CIndividual &P1, 
  CIndividual &P2, 
  CIndividual &C1, 
  CIndividual &C2) { 

  //Store the edges of both parents...
  vector< set<int> > edges(NCITIES);
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
void AEX(
  CIndividual &P1, 
  CIndividual &P2, 
  CIndividual &C1, 
  CIndividual &C2) { 

}

void HGrex(
  CIndividual &P1, 
  CIndividual &P2, 
  CIndividual &C1, 
  CIndividual &C2) { 

}




#endif
