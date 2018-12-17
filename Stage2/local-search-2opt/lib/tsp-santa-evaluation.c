#include "tsp-santa-evaluation.h" 

void free_inst(Inst inst){
  if(inst.cities != 0){ free(inst.cities); }
  if (inst.isprime != 0){ free(inst.isprime); }  
} 

void free_path(Path path){ 
  if(path.tour){ free(path.tour);   } 
} 

int random_int(int lower, int upper) { 
  return ( rand() % (upper - lower + 1)) + lower;
} 


Path clone_path(Path source){
  Path clone = source; 
  clone.tour = (int*) malloc(sizeof(int)*source.size); 
  for(int i=0; i < clone.size; i+=1){ 
    clone.tour[i] = source.tour[i]; 
  } 
  return clone; 
}

void path_copy(Path source, Path dest){ 
  dest.size = source.size; 
  for(int i=0; i < dest.size; i+=1){ 
    dest.tour[i] = source.tour[i]; 
  } 
} 


int get_lines_num(char * filename){ 
  FILE *file = fopen(filename,"r"); 
  char trash[1024];
  int lines_num = 0; 
  while(fscanf(file,"%s\n",trash) == 1){ 
    lines_num += 1; 
  }
  fclose(file);  
  return lines_num; 
} 

Inst load_inst(char* filename) { 
  Inst inst = {0,0,0}; 

  // Contar el número de ciudades 
  // El archivo contiene un header 
  inst.size = get_lines_num(filename) - 1; 

  // Creamos el vector que contendrá las ciudades  
  inst.cities = (City*) malloc (inst.size * sizeof(City));
  
  // Leer las ciudades del archivo 
  FILE *file = fopen(filename,"r"); 
  char trash[100];
  fscanf(file,"%s\n",trash);  
  for(int i=0; i < inst.size; i += 1 ){ 
    int id; 
    fscanf(file,"%d,%lf,%lf\n",&id,&(inst.cities[i].x),&(inst.cities[i].y));
  }  
  fclose(file);

  // Calcular el arreglo para comprobar si la etiqueta es número primo o no 
  inst.isprime = sieve(inst.size); 
  
  return inst;
} 


Path load_path(char* filename) { 
  Path path; 
  
  // Obtener el número de elementos
  path.size = get_lines_num(filename) - 1; 
 
  // Crear el vector 
  path.tour = (int*) malloc(sizeof(int)*path.size); 

  // Leer el orden en que se visitan las ciudades. 
  FILE *file = fopen(filename,"r"); 
  char trash[100];
  fscanf(file,"%s\n",trash);  
  for(int i=0; i < path.size; i += 1 ){ 
    fscanf(file,"%d",&(path.tour[i]));
  }  
  
  fclose(file);
  return path;
} 


void export_path(Path path,FILE *file){ 
  // Se imprime el header
  fprintf(file,"Path\n"); 

  // Se imprime el recorrido 
  for(int i=0; i < path.size; i+=1) { 
    fprintf(file,"%d\n",path.tour[i]); 
  }
} 


double euc_dist(City city_a,City city_b){ 
  double x = (city_a.x - city_b.x); 
  double y = (city_a.y - city_b.y); 
  return (sqrt(x*x + y*y));
} 



bool * sieve(long long max_val) {  
  bool * isprime = (bool*) malloc(sizeof(bool)*(max_val+1));
  for(long long i=0; i <= max_val; i+=1){ 
    isprime[i] = true; 
  }
   
  isprime[0] = isprime[1] = 0; 
  for(long long i = 0 ; i <= sqrt(max_val); i+=1 ){ 
    if(isprime[i]) { 
      for(long long j = i*i; j <= max_val; j+= i) { 
        if((j % i) == 0) { 
          isprime[j] = false;
        }  
      } 
    } 
  } 
  
  return isprime;
} 


double eval_path(Path path, Inst inst){ 
  double score = 0.0; 
  int * tour = path.tour; 
  City * cities = inst.cities; 

  for(int step=0; step < inst.size; step+=1){ 

    double dist = euc_dist(cities[tour[step]],cities[tour[step+1]]); 
    if((((step+1)%10) == 0) && !inst.isprime[tour[step]] ){ 
      dist *= 1.1; 
    }
    score += dist; 
  }

  return score; 
} 

