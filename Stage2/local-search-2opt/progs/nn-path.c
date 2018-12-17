#include "../lib/greedy.h" 

int main(int argc, char **argv){ 

  if(argc < 2){ 
    char * help = ""
      "Este programa genera un recorrido de forma codiciosa tomando la ciudad\n"
      "más cercana y como la siguiente ciudad a visitar\n"
      "\n"
      "Uso:\n"
      "  %s  cities.csv \n" 
      "\n"
      "    cities: Archivo que contiene las ciudades\n" 
      "\n"; 
    printf(help,argv[0]); 
    return 1; 
  } 

  char * inst_filename = argv[1]; 
  Inst inst = load_inst(inst_filename); 

  Path nn_path = nearest_neighbors(inst); 
  export_path(nn_path,stdout); 

  free_path(nn_path); 


  return 0; 
} 
