#ifndef _RECOMBINATION_H_
#define _RECOMBINATION_H_

#include "global.h"
#include "individual.h"

 

typedef struct { 
  int id; 
  double val; 
} erx_tuple; 

int cmp_fun(const void *first, const void * second){ 
  erx_tuple a = ((erx_tuple*)first)[0];  
  erx_tuple b = ((erx_tuple*)second)[0]; 

  if(a.val == b.val){ return 0; }
  if(a.val < b.val) { return -1;} 
  if(a.val > b.val) { return 1; } 
}

typedef struct{
  int edge[2];
  int end;
  int size; 
}edge_list; 

typedef struct{ 
  edge_list * candidate;
  int end;
  int size;
}candidate_list; 

void free_candidate_list(candidate_list * list){ 
  if(list[0].candidate != NULL){ 
    free(list[0].candidate); 
  } 
  list[0].end = 0; 
  list[0].size = 0; 
} 


void init_candidate_list(candidate_list * list){ 
  list[0].candidate = (edge_list *) malloc(sizeof(edge_list)*(NCITIES)); 
  for(int i=0; i < NCITIES ; i += 1){ 
    for (int j=0; j < 2 ; j += 1){ 
      list[0].candidate[i].edge[j] = -1; 
    } 
    list[0].candidate[i].size = 0;
    list[0].candidate[i].end = 0; 
  } 
  list[0].size = NCITIES-1;
} 

void get_edge_list(const int * tour, candidate_list * list){ 
  fprintf(stdout,"get_edge_list\n");
  fflush(stdout);
   
  for(int i=0; i < NCITIES ; i+= 1) { 
    int pos = list[0].candidate[i].size ;
    list[0].candidate[i].edge[pos] = tour[i+1]; 
    list[0].candidate[i].size = pos + 1; 
  } 
} 

bool includes(edge_list * list, int edge){ 

  for(int i=0; i < list[0].size; i += 1){ 
    if (list[0].edge[i] == edge){ 
      return true; 
    } 
  } 

  return false; 
} 


bool push_edge(int edge, edge_list * list){ 
  bool push = true; 
  for(int i=0; i < list[0].size; i+= 1){ 
    if(!includes(list,edge)){ 
      push = false;  
    } 
  }
  
  if(push){ 
    list[0].edge[list[0].size] = edge; 
    list[0].size += 1; 
  }

  return push; 
} 

void merge_candidate_lists(
    candidate_list * merged  ,
    candidate_list * a_list,
    candidate_list * b_list){

  for(int i=0; i < NCITIES; i += 1) { 

    edge_list * merged_candidate = merged[0].candidate + i; 
    edge_list * a_candidate = a_list[0].candidate + i; 
    edge_list * b_candidate = b_list[0].candidate + i ; 

    for(int j=0; j < a_candidate[0].size; j+= 1) { 
      int edge = a_candidate[0].edge[j]; 
      push_edge(edge,merged_candidate); 
    } 

    for(int j=0; j < b_candidate[0].size; j+= 1) { 
      int edge = a_candidate[0].edge[j]; 
      push_edge(edge,merged_candidate); 
    } 
  } 

}


bool delete_in_list(int val, int * list, int * size){ 
  bool deleted = false;  
  for(int i=0; i < size[0]; i += 1){ 
    if (list[i] == val){ 
      list[i] = list[size[0]-1]; 
      list[size[0]-1] = val; 
      size[0] -= 1; 
      deleted = true; 
      break; 
    } 
  } 
  return deleted; 
} 

int random_int(int lower, int upper) { 
  return ( rand() % (upper - lower + 1)) + lower;
} 


void erx(candidate_list * neighbor_list ,CIndividual &C){ 
  
  fprintf(stdout,"reset pointers\n");
  fflush(stdout);

 
  // Reiniciar los punteros de la lista 
  for(int i=0; i < neighbor_list[0].size; i+=1) {
    neighbor_list[0].candidate[i].end = neighbor_list[0].candidate[i].size - 1;
  } 

  fprintf(stdout,"candidates\n");
  fflush(stdout);

  // Lista de ciudades candidatas 
  int candidates[NCITIES]; 
  for(int i=0; i < NCITIES; i += 1){ candidates[i] = i; } 
  /* int end = NCITIES-1; */ 
  int c_ptr = NCITIES-1; 
  
  int * new_tour = C.path; 
  int t_ptr = 0; 
  int x = 0; 


  /* int sorted_idexes[NCITIES]; */
  /* int sort_end = NCITIES -1; */

  /* erx_tuple sorting[end]; */ 
  /* for(int i=0; i < end; i += 1){ */ 
  /*   sorting[i].id = candidates[i]; */ 
  /*   sorting[i].val = neighbor_list[0].candidate[candidates[i]].size; */
  /* } */ 
  /* qsort(sorting,end,sizeof(erx_tuple),cmp_fun); */
  /* for(int i=0; i < end; i += 1) { */ 
    
  /* } */ 

  fprintf(stdout,"start loop\n");
  fflush(stdout);


  // Loop 
  while(true){ 
   
    // Agregar la ciudad al recorrido 
    new_tour[t_ptr] = x; 
    t_ptr += 1; 
    fprintf(stdout,"stopt criteria %d\n",t_ptr);
    fflush(stdout);


    if(t_ptr == NCITIES-1){ break; } 

    // borrar la ciudad candidata
    delete_in_list(x,candidates,&c_ptr);
    for(int i=0; i < NCITIES; i+=1 ){ 
      edge_list * candidate = &(neighbor_list[0].candidate[i]); 
      int * list = &(candidate[0].edge[0]); 
      delete_in_list(x,list,&(candidate[0].end)); 
    } 

    if (neighbor_list[0].candidate[x].size == 0) { 
      x = candidates[random_int(0,c_ptr)];
    } 
    else { 
      
      int rem_cand = neighbor_list[0].candidate[x].end;
      erx_tuple sorting[rem_cand]; 
      for(int i=0; i < rem_cand; i += 1){ 
        sorting[i].id = neighbor_list[0].candidate[x].edge[i]; 
        sorting[i].val = neighbor_list[0].candidate[sorting[i].id].end;
      } 
     
      qsort(sorting,rem_cand,sizeof(erx_tuple),cmp_fun);

      x = sorting[0].id;

    } 
  } 

  new_tour[NCITIES-1] = 0; 

  fprintf(stdout,"end loop\n");
  fflush(stdout);



} 

void xover_kaggle(CIndividual &P1, CIndividual &P2, CIndividual &C1, CIndividual &C2)
{

  fprintf(stdout,"Xover\n");
  fflush(stdout);
  // Generar la lista de candidatos 
  candidate_list neighbor_list; 
  init_candidate_list(&neighbor_list); 

  candidate_list a_list; 
  init_candidate_list(&a_list);
  candidate_list b_list; 
  init_candidate_list(&b_list); 


  // Conseguir las aristas de los recorridos 
  get_edge_list(P1.path,&a_list); 
  get_edge_list(P2.path,&b_list);

  // Combinar las aristas 
  //
  merge_candidate_lists(&neighbor_list,&a_list,&b_list); 
 
  fprintf(stdout,"start crossover\n");
  fflush(stdout);

  erx(&neighbor_list,C1);

  fprintf(stdout,"done\n");
  fflush(stdout);

  /* erx(&neighbor_list,C2); */

}
#endif
