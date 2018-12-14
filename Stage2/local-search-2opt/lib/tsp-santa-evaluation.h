#include <stdio.h>
#include <stdlib.h> 
#include <math.h> 


#ifndef TSP_SANTA
#define TSP_SANTA

typedef struct{
  double x;
  double y; 
}City; 


typedef struct{
  int * tour;
  int  size; 
} Path; 
void free_path(Path path); 

typedef struct{ 
  City * cities; 
  int  size; 
  char * isprime; 
} Inst; 
void free_inst(Inst inst); 

// Función para cargar las ciudades 
//
Inst load_inst(char* filename); 

// Función para cargar el recorrido 
Path load_path(char* filename); 

// Función para calcular la distancia euclideana
// entre dos ciudades
double euc_dist(City city_a, City city_b); 

// Función para Calcular los números números primos 
char * sieve(long long max_value);

// Función que calcula el valor objetivo del recorrido 
double eval_path(Path path, Inst inst); 


#endif
