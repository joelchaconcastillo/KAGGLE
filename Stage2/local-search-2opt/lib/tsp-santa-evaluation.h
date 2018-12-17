
#ifndef TSP_SANTA
#define TSP_SANTA

#include <stdio.h>
#include <stdlib.h> 
#include <math.h> 
#include <stdbool.h> 

// Representación de una ciudad, con posición x,y
typedef struct{
  double x;
  double y; 
}City; 

// Representación de un recorrido como un arreglo de n+1 enteros 
// que contiene en cada posición la etiqueta de la ciudad que se visita 
// [0 .... 0]
//
typedef struct{
  int * tour;
  int  size; 
} Path; 
void free_path(Path path); 

// Representación del problema: 
// 
// cities: arreglo de ciudades, su posición en el arreglo corresponde a la 
//         etiqueta. 
// size:   cantidad de ciudades
// isprime: arreglo de booleanos que indica si es número es primo. 
//
typedef struct{ 
  City * cities; 
  int  size; 
  bool * isprime; 
} Inst; 
void free_inst(Inst inst); 

// Imprime en el archivo el recorrido 
void export_path(Path path,FILE *file); 

// Función para cargar las ciudades 
Inst load_inst(char* filename); 

// Función para cargar el recorrido 
Path load_path(char* filename); 

// Función para calcular la distancia euclideana entre dos ciudades
double euc_dist(City city_a, City city_b); 

// Función para Calcular los números números primos 
bool * sieve(long long max_value);

// Función que calcula el valor objetivo del recorrido 
double eval_path(Path path, Inst inst); 

// Generar un número en el rango [a,b] de forma aleatoria 
int random_int(int lower,int upper); 

// Crea una copia profunda de un recorrido
Path  clone_path(Path source); 

// Copia el contenido de 'source' en 'dest' 
void path_copy(Path source, Path des); 

// Cuenta la cantidad de lineas en un archivo
int get_lines_num(char * filename); 


#endif
