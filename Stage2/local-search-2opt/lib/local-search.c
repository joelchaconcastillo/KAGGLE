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

Neighborhood create_neighborhood(Inst inst,Path init_sol){ 
  Neighborhood neigh;

  // Obtener el conjunto de ciudades cercanas a cada ciudad  
  neigh.nearest = load_nearest_cities(inst); 
  neigh.path = clone_path(init_sol); 

  // Obtener la posición de cada ciudad en el recorrido 
  neigh.city_pos = (int*) malloc(sizeof(int)*inst.size); 
  for(int i=0; i < inst.size; i+= 1){ 
    neigh.city_pos[neigh.path.tour[i]] = i; 
  } 

  reset_neighborhood(&neigh); 

  // Vector para elegir los candidatos 
  neigh.indexes = (int*) malloc(sizeof(int)*inst.size); 
  for(int i=0; i < inst.size; i += 1){
    neigh.indexes[i] = i; 
  }

  return neigh; 
} 

void reset_neighborhood(Neighborhood * ng) { 
  ng[0].a_index = 1 ; 
  ng[0].b_index = 0 ;
  ng[0].restarted = true; 
} 

void free_neighborhood(Neighborhood ng){ 
  if(ng.indexes != NULL){ 
    free(ng.indexes); 
  }

  if (ng.city_pos != NULL){ 
    free(ng.city_pos); 
  } 

  free_idsmatrix(ng.nearest); 
} 


Path two_opt_local_search(Inst inst,Path init_sol){ 

  // Medición del tiempo 
  time_t tic = time(0);
  time_t toc = tic; 
  
  Neighborhood ng = create_neighborhood(inst,init_sol);
  
  Path best_sol = clone_path(init_sol); 
  double best_fit = eval_path(best_sol,inst); 

  Path path = clone_path(init_sol); 
  
  long long it = 0; 
  fprintf(stderr,"t: %ld :: it: %ld :: dif: %lf :: fit: %lf \n",
    (toc-tic),it,0.0,best_fit);
  while(next_two_opt_neighbor(&ng)){ 
    it += 1; 
    double fit = eval_path(ng.path,inst); 

    if(fit <  best_fit) { 
      toc = time(0); 
      fprintf(stderr,"t: %ld :: it: %ld :: dif: %lf :: fit: %lf \n",
        (toc-tic),it,best_fit - fit,fit);

      it = 0; 
      best_fit = fit; 
      path_copy(ng.path,best_sol); 

      reset_neighborhood(&ng); 
    }
    sleep(0);
  } 

  toc = time(0); 
  fprintf(stderr,"t: %ld :: it: %ld :: dif: %lf :: fit: %lf \n",
    (toc-tic),it,0.0,best_fit);

  free_path(path); 
  free_neighborhood(ng); 

  return best_sol; 
} 


void two_opt_move(Neighborhood * ng){ 

    int mid =  ng[0].a_pos + (ng[0].b_pos - ng[0].a_pos) / 2 ; 
    int end = ng[0].b_pos; 

    // Invertir el segmento [ a ... b ] del recorrido   
    for(int p = ng[0].a_pos ; p < mid ; p += 1){ 

      // Invertir el recorrido 
      int aux = ng[0].path.tour[p]; 
      ng[0].path.tour[p] = ng[0].path.tour[end]; 
      ng[0].path.tour[end] = aux;  

      // Actualizar los índices de posición 
      ng[0].city_pos[ng[0].path.tour[end]] = end;
      ng[0].city_pos[ng[0].path.tour[p]] = p; 

      // Reducir el intervalo por la derecha
      end -= 1; 
    } 
} 

bool next_two_opt_neighbor(Neighborhood * ng){ 

  // Si se ha reiniciado la vecindad no se deshace el movimiento 2opt 
  // de lo contrario significa que aún se esta explorando y debe regresarse a 
  // la solución anterior. 
  if( ng[0].restarted ) { ng[0].restarted = false; }
  else{ two_opt_move(ng); } 


  // Si se ha terminado de visitar la vecindad se regresa la bandera de paro
  if (ng[0].a_index + 1 >= ng[0].nearest.rows){ return false;  }


  // Si se ha terminado de explorar las ciudades vecinas de la posición 
  // a_index se procede a elegir un nuevo candidato
  if (ng[0].b_index >= ng[0].nearest.cols){

    // Reinicio el contador de las ciudades cercanas 
    ng[0].b_index = 0; 

    // Se incrementa el contador de la posición en el recorrido 
    ng[0].a_index += 1; 

    int index = random_int(ng[0].a_index,ng[0].nearest.rows-1); 
   
    // Intercambiar las posiciones del recorrido candidatas 
    int aux = ng[0].indexes[ng[0].a_index]; 
    ng[0].indexes[ng[0].a_index] = ng[0].indexes[index];
    ng[0].indexes[index] = aux; 
  } 

  // Obtener la posición candidata y la etiqueta de la ciudad 
  ng[0].a_pos = ng[0].indexes[ng[0].a_index];
  int a_id  = ng[0].path.tour[ng[0].a_pos]; 
  
   
  // Elegir de forma aleatoria una ciudad de su vecindad
  int index = random_int(ng[0].b_index,ng[0].nearest.cols-1); 

  // Intercambiar las ciudades más cercanas para eliminarlas de la 
  // vecindad
  int aux = ng[0].nearest.ids[a_id][index]; 
  ng[0].nearest.ids[a_id][index] = ng[0].nearest.ids[a_id][ng[0].b_index]; 
  ng[0].nearest.ids[a_id][ng[0].b_index] = aux; 

  // Obtener la posición en el recorrido de la etiqueta
  int b_id = ng[0].nearest.ids[a_id][ng[0].b_index]; 
  ng[0].b_pos = ng[0].city_pos[b_id]; 
  
  // Incrementar el contador de la posición en las ciudades más cercanas
  ng[0].b_index += 1;

  // las posiciones deben ser menor a mayor  
  if(ng[0].a_pos > ng[0].b_pos) { 
    int aux = ng[0].a_pos; 
    ng[0].a_pos = ng[0].b_pos; 
    ng[0].b_pos = aux; 
  }

  // Se realiza el movimiento 2-opt 
  two_opt_move(ng); 

  return true; 
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


