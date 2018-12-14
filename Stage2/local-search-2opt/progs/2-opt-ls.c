#include "../lib/local-search.h"

int main(int argc, char **argv) { 
  srand(time(0)); 

  if(argc < 3){ 
    char *help = ""
      "Aplica la búsqueda local simple con vecindad 2opt a un recorrido inicial\n"
      "\n" 
      "Requiere de que exista un conjunto de archivos en la ruta \"out\" con el\n"
      "nombre\n"
      "  nearest-to-[id]\n"
      "\n"
      "donde 'id' indica la ciudad a considerar. Todos los archivos tienen la \n"
      "la misma cantidad de elementos, cada uno representando la etiqueta de la\n"
      "ciudad que es cercana a 'id',separados por salto de linea.\n" 
      "\n"
      "Uso:\n"
      "  %s cities path"
      "\n"
      "\n"; 
    printf(help,argv[0]);
    exit(0);
  } 


  // Cargar el problema 
  char * inst_filename = argv[1]; 
  Inst inst = load_inst(inst_filename); 

  // Cargar la solución inicial; 
  char * path_filename = argv[2]; 
  Path path = load_path(path_filename); 

  puts ("iniciando");
  Path local_optima = two_opt_local_search(inst,path); 

  puts("Se ha terminado la vecindad");

  free_path(path);
  free_inst(inst); 
  free_path(local_optima); 

  return 0; 
} 
