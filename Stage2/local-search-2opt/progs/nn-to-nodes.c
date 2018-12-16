#include "../lib/tsp-santa-evaluation.h" 

typedef struct{
  int id; 
  double value; 
}tuple; 

int cmp_fun(const void *first, const void * second){ 
  tuple a = ((tuple*)first)[0];  
  tuple b = ((tuple*)second)[0]; 

  if(a.value == b.value){ return 0; }
  if(a.value < b.value) { return -1;} 
  if(a.value > b.value) { return 1; } 
} 

int main(int argc, char **argv) { 

  if(argc < 3){ 
    
    char *help = ""
      "Calcula los 'cnt' vecinos más cercanos en distancia euclideana para\n"
      "para cada ciudad\n" 
      "\n" 
      "Se generan un archivo por cada ciudad que contiene por linea la etiqueta\n"
      "de todas las ciudades vecinas\n"
      "\n"
      "Uso:\n"
      "  %s cities cnt [dir]\n"
      "\n"
      "    cities: Archivo con las ciudades\n"
      "    cnt:    Número de vecinos a considerar\n"
      "    dir:    Ruta al directorio donde se guardan los archivos\n" 
      "\n"; 

    printf(help,argv[0]); 
    return 0; 
  } 

  char *inst_filename = argv[1]; 
  Inst inst = load_inst(inst_filename); 
  
  int cnt = atoi(argv[2]); 

  char *dir = "near-cities"; 
  if(argc == 4){ 
    dir = argv[3]; 
  } 

  tuple nearest[cnt]; 
  for(int i=0; i < cnt; i+= 1){
    nearest[i].value = 1/0.0; 
  } 

  // Por cada ciudad encontrar las 'n' ciudades más cercanas
  /* tuple * sorted = (tuple*) malloc(sizeof(tuple)*inst.size); */ 
  for(int from=0; from < inst.size; from += 1){ 
    if( from % 1000)
    fprintf(stdout,"\r%lf%",((double)from/inst.size)*100);  
    fflush(stdout);

    // Calcular la distancia para cada ciudad 
    for(int to=0; to < inst.size; to += 1) { 
      if(to == from){continue;} 
      tuple candidate; 
      candidate.id = to; 
      candidate.value = euc_dist(inst.cities[from],inst.cities[to]); 

      if(nearest[cnt-1].value > candidate.value){ 
        nearest[cnt-1] = candidate ;
        qsort(nearest,cnt,sizeof(tuple),cmp_fun); 
      } 
    } 
     
    // Escribir a un archivo las 'cnt' ciudades más cercanas 
    char filename[100];
    sprintf(filename,"%s/nearest-to-%d",dir,from); 
    FILE *file = fopen(filename,"w+"); 
    for(int to=0; to < cnt; to += 1){ 
      fprintf(file,"%d\n",nearest[to]);
    } 
    fclose(file);
  } 

  // Housekeeping 
  free_inst(inst); 
  return 0; 
} 
