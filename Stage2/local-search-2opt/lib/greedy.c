#include "greedy.h"


Path nearest_neighbors(Inst inst){ 

  Path path; 
  path.size = inst.size + 1; 
  path.tour = (int*) malloc(sizeof(int)*path.size); 
  int * tour = path.tour; 

  int  nearest = 0;  
  double  nearest_dist = 1/0.0; 

  int * candidates = (int*) malloc(sizeof(int)*inst.size); 
  for(int i=0; i < inst.size; i += 1){ 
    candidates[i] = i;  
  } 

  tour[0] = tour[inst.size] = 0; 


  for(int step =1; step < inst.size; step += 1){ 
      
    fprintf(stderr,"step: %d, tour: %d\n",step,tour[step-1]); 
    nearest_dist = 1/0.0; 

    // Encontrar las k ciudades vecinas más cercanas de entre los candidatos
    for(int i=step; i < inst.size-1; i+=1){ 
      int dist = euc_dist(inst.cities[tour[step]],inst.cities[candidates[i]]); 
      if (dist < nearest_dist){ 
        nearest_dist = dist; 
        nearest = i; 
      } 
    }
    int aux = candidates[step]; 
    candidates[step] = candidates[nearest]; 
    candidates[nearest] = aux; 

    tour[step] = candidates[step]; 
  } 

  free(candidates); 
  return path; 
} 
