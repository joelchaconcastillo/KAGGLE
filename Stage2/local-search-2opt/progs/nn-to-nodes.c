#include "../lib/tsp-santa-evaluation.h" 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 


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

  // Obtener el número de núcleos y calcular la repartición de las ciudades 
  long cpu_cores      = sysconf(_SC_NPROCESSORS_ONLN);
  int  tasks_per_core = inst.size / cpu_cores;
  int  * start        = int * malloc(sizeof(int) * cpu_cores);
  int  * end          = int * malloc(sizeof(int) * cpu_cores);

  for(int core = 0; core < cpu_cores; core += 1){ 
    start[core] = (core*tasks_per_core); 
    end[cpu_cores-1 - core] = inst.size - 1 - start[core]; 
  }
  end[cpu_cores] = inst.size; 



  // Generamos los procesos 
  int   id  = -1;
  pid_t pid = 0;
  for(int core = 0 ; core < cpu_cores; core+=1) {
    pid = fork(); 
    if (pid == 0){ 
      id = core; 
      break; 
    }  
  }
 
  if(pid != 0 ){ 
    // El nodo padre debe esperar a los hijos. 
    pid_t wpid; 
    int status; 
    while(wpid = wait(&status) > 0){ 
      sleep(0); 
    } 
  }else{ 
    
    // Cada proceso hijo se encarga de procesar 'tasks_per_nodes' ciudades
    
    tuple nearest[cnt]; 
    for(int from=start[id]; from < end[id]; from += 1){ 

      // Log del progreso de cada nodo 
      if ((from % 100) == 0){ 
        fprintf(stderr,"nodo: %ld => %lf %\n",id,((double)(from - start[id])/(end[id] - start[id]))); 
        fflush(stderr); 
      } 

      // Reiniciar el vector de distancias
      for(int i=0; i < cnt; i+= 1){
        nearest[i].value = 1/0.0; 
      } 

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
        fprintf(file,"%d\n",nearest[to].id);
      } 
      fclose(file);
    } 
  }

  // Housekeeping 
  free_inst(inst); 
  free(start); 
  free(end); 
  return 0; 
} 
