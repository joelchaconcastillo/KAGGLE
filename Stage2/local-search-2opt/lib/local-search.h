#ifndef TSP_SANTA_LOCAL_SEARCH
#define TSP_SANTA_LOCAL_SEARCH

#include "tsp-santa-evaluation.h"
#include <time.h> 
#include <stdbool.h>

typedef struct{
  int rows; 
  int cols; 
  int ** ids;
} IdsMatrix; 

void free_idsmatrix(IdsMatrix mat); 



typedef struct{
  int a_pos; 
  int b_pos; 
  int a_index; 
  int b_index; 
  IdsMatrix nearest; 
  int * indexes; 
}Neighborhood; 

void free_neighborhood(Neighborhood ng); 

// Cargar los ids de las ciudades vecinas
IdsMatrix load_nearest_cities(Inst inst);

// Regresa un número aleatorio en el rango entre [lower, upper]
int random_int(int lower, int upper); 


// Inicializa la vecindad 
Neighborhood create_neighborhood(Inst inst);

// Actualiza la vecindad al siguiente vecino 
bool next_two_opt_neighbor(const Path path, Neighborhood * ng); 

// Reinicia la vecindad
void reset_neighborhood(Neighborhood * ng); 

// Realiza una búsqueda local con vecindad 2opt
Path two_opt_local_search(Inst inst, Path init_sol); 

// Invierte el segmento 
void two_opt_move(Path path,Neighborhood ng);



#endif
