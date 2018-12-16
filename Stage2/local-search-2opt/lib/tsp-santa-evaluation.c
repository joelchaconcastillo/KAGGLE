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


Inst load_inst(char* filename) { 
  Inst inst = {0,0,0}; 
  char trash[100];

  // Contar el número de ciudades 
  // El archivo contiene un header 
  inst.size = 0; 
  FILE *file = fopen(filename,"r"); 
  fscanf(file,"%s\n",trash);
  while(fscanf(file,"%s\n",trash) == 1){ 
    inst.size += 1; 
  }

  // Creamos el vector que contendrá las ciudades  
  inst.cities = (City*) malloc (inst.size * sizeof(City));
  
  // Leer las ciudades del archivo 
  rewind(file); 
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
  Path path = {0,0}; 
  char trash[100];
  
  // Obtener el número de elementos
  FILE *file = fopen(filename,"r"); 
  path.size = 0 ; 
  fscanf(file,"%s\n",trash);  
  while(fscanf(file,"%s\n",trash) == 1){ 
    path.size += 1; 
  } 
 
  // Crear el vector 
  path.tour = (int*) malloc(sizeof(int)*path.size); 

  // Leer el orden en que se visitan las ciudades. 
  rewind(file); 
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
    // Se penaliza cada 10 paso, si la etiqueta de la ciudad de 
    // la que parte no es un número primo. 
    if((((step+1)%10) == 0) && !inst.isprime[tour[step]] ){ 
      dist *= 1.1; 
    }
    score += dist; 
  }

  return score; 
} 

