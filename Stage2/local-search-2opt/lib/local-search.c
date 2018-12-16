#include "local-search.h" 

void free_idsmatrix(IdsMatrix mat){ 
  if(mat.ids != NULL){ 
    for(int r=0; r < mat.rows; r += 1){ 
      if(mat.ids[r] != NULL){ 
        free(mat.ids[r]); 
      }
    }
    free(mat.ids); 
  } 
} 

Neighborhood create_neighborhood(Inst inst){ 
  Neighborhood neigh;

  // Obtener el conjunto de ciudades cercanas a cada ciudad  
  neigh.nearest = load_nearest_cities(inst); 

  reset_neighborhood(&neigh); 

  neigh.indexes = (int*) malloc(sizeof(int)*inst.size); 
  for(int i=0; i < inst.size; i += 1){
    neigh.indexes[i] = i; 
  }
  return neigh; 
} 

void free_neighborhood(Neighborhood ng){ 
  if(ng.indexes != NULL){ 
    free(ng.indexes); 
  }
  free_idsmatrix(ng.nearest); 
} 


Path two_opt_local_search(Inst inst,Path init_sol){ 

  // Medición del tiempo 
  time_t tic = time(0);
  time_t toc = tic; 

  Neighborhood ng = create_neighborhood(inst);

  Path best_sol = clone_path(init_sol); 
  double best_fit = eval_path(best_sol,inst); 

  Path path = clone_path(init_sol); 
  
  long long it = 0; 
  fprintf(stderr,"t: %ld :: it: %ld :: dif: %lf :: fit: %lf \n",
    (toc-tic),it,best_fit - fit,fit);
  while(next_two_opt_neighbor(path,&ng)){ 
    it += 1; 

    two_opt_move(path,ng); 

    double fit = eval_path(path,inst); 

    if(fit <  best_fit) { 
      toc = time(0); 
      fprintf(stderr,"t: %ld :: it: %ld :: dif: %lf :: fit: %lf \n",
        (toc-tic),it,best_fit - fit,fit);

      it = 0; 
      best_fit = fit; 
      path_copy(path,best_sol); 

      reset_neighborhood(&ng); 
    }else{ 
      // Reconstruir la solución original
      two_opt_move(path,ng); 
    }
  } 

  toc = time(0); 
  fprintf(stderr,"t: %ld :: it: %ld :: dif: %lf :: fit: %lf \n",
    (toc-tic),it,best_fit - fit,fit);

  free_path(path); 
  free_neighborhood(ng); 

  return best_sol; 
} 


void two_opt_move(Path path, Neighborhood ng){ 

    int mid =  ng.a_pos + (ng.b_pos - ng.a_pos) / 2 ; 
    int end = ng.b_pos; 

    // Invertir el segmento 
    for(int p = ng.a_pos ; p < mid ; p += 1){ 
      int aux = path.tour[p]; 
      path.tour[p] = path.tour[end]; 
      path.tour[end] = aux;  
      end -= 1; 
    } 
} 

bool next_two_opt_neighbor(const Path path, Neighborhood * ng){ 
  if (ng[0].a_index + 1 >= ng[0].nearest.rows){
      return false;  
  }

  // Elegir una posición del recorrido de forma aleatoria 
  if (ng[0].b_index >= ng[0].nearest.cols){
    // Reinicio el contador de las ciudades cercanas 
    ng[0].b_index = 0; 

    // Se incrementa el contador de la posición en el recorrido 
    ng[0].a_index += 1; 

    int index = random_int(ng[0].a_index,ng[0].nearest.rows-1); 
    
    int aux = ng[0].indexes[ng[0].a_index]; 
    ng[0].indexes[ng[0].a_index] = ng[0].indexes[index];
    ng[0].indexes[index] = aux; 
  } 


  ng[0].a_pos = ng[0].indexes[ng[0].a_index];
  int a_id  = path.tour[ng[0].a_pos]; 
  
   
  // Elegir de forma aleatoria una ciudad de su vecindad

  int index = random_int(ng[0].b_index,ng[0].nearest.cols-1); 
  int aux = ng[0].nearest.ids[a_id][index]; 
  ng[0].nearest.ids[a_id][index] = ng[0].nearest.ids[a_id][ng[0].b_index]; 
  ng[0].nearest.ids[a_id][ng[0].b_index] = aux; 

  int b_id = ng[0].nearest.ids[a_id][ng[0].b_index]; 
  ng[0].b_index += 1;

  // Obtener la posición en el recorrido 
  for(int pos = 0; pos < path.size; pos += 1){ 
    if (b_id == path.tour[pos]){ 
      ng[0].b_pos = pos; 
      break; 
    } 
  }

  // las posiciones deben ser menor a mayor  
  if(ng[0].a_pos > ng[0].b_pos) { 
    int aux = ng[0].a_pos; 
    ng[0].a_pos = ng[0].b_pos; 
    ng[0].b_pos = aux; 
  } 

  return true; 
} 

void reset_neighborhood(Neighborhood * ng) { 
  ng[0].a_index = 1 ; 
  ng[0].b_index = 0; 
} 

IdsMatrix load_nearest_cities(Inst inst){ 

  // Obtenemos la cantidad de ciudades que hay en cada archivo 
  FILE *file = fopen("near-cities/nearest-to-0","r"); 
  char trash[100]; 
  int near_cities_cnt = get_lines_num("near-cities/nearest-to-0"); 
  
  IdsMatrix nearest_cities; 
  nearest_cities.rows = inst.size; 
  nearest_cities.cols = near_cities_cnt; 
  
   // Leer el conjunto de ciudades mas cercanas para cada ciudad 
  char filename[100];
  nearest_cities.ids = (int **) malloc(sizeof(int*) * nearest_cities.rows); 

  for(int id=0; id < inst.size; id+=1){ 
    nearest_cities.ids[id] = (int*) malloc(sizeof(int*)*nearest_cities.cols); 
    sprintf(filename,"near-cities/nearest-to-%d",id); 
    file = fopen(filename,"r"); 
    for(int nc=0; nc < near_cities_cnt; nc += 1){ 
      fscanf(file,"%d\n",&(nearest_cities.ids[id][nc])); 
    }
    fclose(file); 
  } 

  return nearest_cities;
} 


