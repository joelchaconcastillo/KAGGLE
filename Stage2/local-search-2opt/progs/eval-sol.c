#include "../lib/tsp-santa-evaluation.h" 

int main(int argc, char **argv){ 
  if(argc < 3){ 
    char *help = ""
      "Calcula el valor objetivo de un recorrido\n"
      " Uso:\n"
      "   %s cities tour\n"
      "\n"
      " cities: archivo con las ciudades\n"
      "   id,x,y\n"
      "   1,2.0,3.0\n"
      "   ... \n"
      " tour:   archivo con el recorrido\n"
      "   path\n"
      "   0\n"
      "   ...\n"
      "   0\n"; 
    printf(help,argv[0]);
    exit(0);
  } 
  

  // Cargar los archivos 
  char *inst_filename = argv[1]; 
  Inst inst = load_inst(inst_filename); 
  
  // Cargar el recorrido 
  char *path_filename   = argv[2]; 
  Path path = load_path(path_filename); 
  
  // Calcular el valor 
  printf("%lf\n",eval_path(path,inst)); 

  // Liberar memoria 
  free_inst(inst); 
  free_path(path);   
  return 0;  
} 
