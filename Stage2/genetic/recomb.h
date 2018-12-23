#ifndef _RECOMBINATION_H_
#define _RECOMBINATION_H_

#include "global.h"
#include "individual.h"


// Estructura para ordenar los ids por algún valor
//
typedef struct { 
  int id; 
  double val; 
}erx_tuple; 

// Función de comparación para usar en qsort 
//
int cmp_fun(const void *at, const void *bt){ 
  erx_tuple a = ((erx_tuple*) at)[0]; 
  erx_tuple b = ((erx_tuple*) bt)[0]; 

  if(a.val == b.val){ return 0; }
  if(a.val < b.val) { return -1;} 
  if(a.val > b.val) { return 1; } 
} 


// Lista de ciudades que son parte de una arista con una tercera 
// ciudad. 
typedef struct{ 
  int * edge; 
  int size; 
  int end; 
}edge_list;

void free_edge_list(edge_list * list){ 
  if( list->edge != NULL) {free(list->edge); }
  list->size = 0; 
  list->end = 0; 
} 

// Lista de las aristas de cada ciudad 
//
typedef struct{ 
  edge_list * candidate;
  int size; 
  int end; 
}city_edges; 

void free_city_edges(city_edges * list){ 
  if( list->candidate != NULL){ free_edge_list(list->candidate) ; } 
  list->size = 0; 
  list->end = 0; 
} 

// -------------------------------------------------------------------
// IO
// -------------------------------------------------------------------
void print_city_edges(city_edges  list){ 
  fprintf(stdout,"p_list = ["); 
  for(int i=0; i < 5; i+=1){ 
    for(int j=0; j < 2; j+=1){ 
      fprintf(stdout," %d, ",list.candidate[i].edge[j]); 
    } 
    fprintf(stdout,",\n"); 
  } 
  fprintf(stdout,"]\n"); 
}

// --------------------------------------------------------------------
// Funciones
// --------------------------------------------------------------------


// Inicializar la lista de aristas para cada ciudad 
//
void init_city_edges(city_edges * list){ 
  list->candidate = (edge_list *) malloc(sizeof(edge_list)*NCITIES); 

  for(int i=0; i < NCITIES; i += 1){ 
    list->candidate[i].edge = (int *) malloc(sizeof(int)*2); 
    for(int j=0; j < 2; j += 1){ 
      list->candidate[i].edge[j] = 0; 
    } 
    list->candidate[i].size = 0; 
    list->candidate[i].end = 0; 
  }

  list->size = NCITIES-1; 
  list->end = NCITIES-1;
} 

// Obtener las aristas presentes en los recorridos 
// en la recombinación de aristas se considera 0 ... a - x - b ... 0 las
// aristas en ambas direcciones, pero cuando no es simétrico consideré es 
// demasiado destructivo y solo se considera 0 .. x - b .. 0. 
//
void get_edge_list(int * path, city_edges * list){ 
  for(int i=0; i < NCITIES; i+=1){ 
    int pos = list->candidate[path[i]].size; 
    list->candidate[path[i]].edge[pos] = path[i+1]; 
    list->candidate[path[i]].size += 1; 
  }
}


// Revisa si la arista 'edge' se encuentra en la lista 'list
//
// Regresa true, si se encuentra  y false en caso contrario.
//
bool includes(edge_list list, int edge){ 
  for(int i=0; i < list.size; i += 1){ 
    if (list.edge[i] == edge){ return true;} 
  } 
  return false; 
} 


// Se agrega la arista 'edge' en la lista de aristas 'list' solo si no es  
// un duplicado 
// 
// Regresa 'true' si se ha agregado y 'false' en caso contrario. 
//
bool push_edge(edge_list * list, int edge){ 
  bool pushed = true; 

  for(int i=0; i < list->size; i+= 1){ 
    if(includes(*list,edge)){ 
      pushed = false; 
      break; 
    } 
  } 

  if(pushed){ 
    list->edge[list->size] = edge; 
    list->size += 1; 
  } 
  return pushed; 
} 


// Combinar las dos listas de aristas de cada ciudad 'p1' y 'p2' en 
// una tercera lista, que no tendrá soluciones repetidas. Es decir 
// si p1 contiene a-b y p2 a-c la lista 'neighbor' tendrá a-b,a-c
// si p1 contiene a-b y p2 a-b la lista 'neighbor' tendrá a-b
//
city_edges merge_city_edges_lists(city_edges p1_list, city_edges p2_list){ 
  city_edges neighbor_list; 
  init_city_edges(&neighbor_list);  

  for(int i=0; i < NCITIES; i += 1){ 
    edge_list * a_cand = p1_list.candidate + i; 
    edge_list * b_cand = p2_list.candidate + i; 
    edge_list * merge  = neighbor_list.candidate + i; 

    for(int j=0; j < a_cand->size; j+= 1){ 
      int edge = a_cand->edge[j]; 
      push_edge(merge,edge); 
    } 
    
    for(int j=0; j < b_cand->size; j+= 1){ 
      int edge = b_cand->edge[j]; 
      push_edge(merge,edge); 
    }
  } 

  return neighbor_list; 
} 


// Intercambiar los valores en las posiciones 'i' y 'j' del arreglo 'list'
//
void swp(int * list, int i, int j){ 
  int aux = list[i]; 
  list[i] = list[j];
  list[j] = aux;
} 

// Obtener un número entero aleatorio en el rango [lowe,upper] 
//
int random_int(int lower, int upper) { 
  return ( rand() % (upper - lower + 1)) + lower;
} 


void erx(city_edges * neighbor_list, CIndividual &C){ 
  
  // Reiniciar los punteros de búsqueda de la lista 
  for(int i=0; i < neighbor_list->size; i+= 1){ 
    int end = neighbor_list->candidate[i].size - 1; 
    neighbor_list->candidate[i].end = end; 
  } 

  // Crear la lista de ciudades candidatas
  int cities[NCITIES]; 
  int indexes[NCITIES]; 
  for(int i=0; i < NCITIES; i+=1){ 
    cities[i] = i; 
    indexes[i] = i; 
  }
  int c_end = NCITIES-1; 

  int new_tour[NCITIES+1];
  int t_ptr = 0; 
  int x = 0; 

  /* fprintf(stdout,"start loop\n"); */
  /* fflush(stdout); */

  // Loop para crear el nuevo recorrido empleando las aristas candidatas de 
  while(true){ 
    
    /* fprintf(stdout,"t_ptr %d\n",t_ptr); */
   // Agrega la ciudad al recorrido 
    new_tour[t_ptr] = x; 
    t_ptr += 1;  

    /* fprintf(stdout,"stop criteria %d\n",t_ptr); */
    /* fflush(stdout); */
    // Criterio de paro (hemos terminado el recorrido
    if(t_ptr == NCITIES-1){ 
      break; 
    }
    
    /* fprintf(stdout,"new_tour = ["); */ 
    /* for(int i=0; i < 10; i += 1){ */ 
    /*   fprintf(stdout,"%d,",new_tour[i]); */ 
    /* } */ 
    /* fprintf(stdout,"]\n"); */ 

    /* fprintf(stdout,"x %d\n",x); */
    /* fprintf(stdout,"c_end: %d, posx: %d, c_end:%d city_end: %d\n",x,indexes[x],c_end,cities[c_end]); */
    /* fprintf(stdout,"NCITIES %d\n",NCITIES); */

    // Borrar la ciudad que se ha agregado al recorrido 
    swp(cities,indexes[x],c_end);
    indexes[x] = c_end; 
    indexes[cities[c_end]] = x; 
    c_end -= 1; 

    /* if (t_ptr == 4){ */ 
    /*   exit(0); */ 
    /* } */ 
    /* fprintf(stdout,"here\n"); */

    // Borrar la ciudad de las ciudades candidatas
    for(int i=0; i < neighbor_list->size; i+= 1){ 
      edge_list * list = neighbor_list->candidate + i ; 
      for(int j=0; j < list->end; j += 1){ 
        if (list->edge[j] == x){ 
          swp(list->edge,list->end-1,j); 
          list->end -= 1; 
          break; 
        } 
      } 
    } 

    // Seleccionar de forma aleatoria si no hay mas aristas en la ciudad en
    // la que estamos. 
    if (neighbor_list->candidate[x].size == 0){ 
      x = cities[random_int(0,c_end)]; 
    }else{ 
      int rem_cities = neighbor_list->candidate[x].end;
      erx_tuple sorted[rem_cities]; 
      /* fprintf(stdout,"rem_cities %d\n",rem_cities); */
      for(int i=0; i <= rem_cities; i+=1){ 
        /* fprintf(stdout,"i%d\n",i); */
        /* fprintf(stdout,"sorted[i].id %d\n",sorted[i]); */
        sorted[i].id = neighbor_list->candidate[x].edge[i]; 
        /* fprintf(stdout,"sorted[i].id %d\n",sorted[i]); */
        sorted[i].val = neighbor_list->candidate[sorted[i].id].end;  
      }
      qsort(sorted,rem_cities,sizeof(erx_tuple),cmp_fun); 
      x = sorted[0].id; 
    } 
  } 
  new_tour[NCITIES-1] = 0; 


  // Copiar el recorrido en el hijo 
  for(int i=0; i < NCITIES+1; i+=1){ 
    C.path[i] = new_tour[i];  
  } 
} 


void xover_kaggle(
  CIndividual &P1, 
  CIndividual &P2, 
  CIndividual &C1, 
  CIndividual &C2) { 

  fprintf(stdout,"iniciando la cruza\n"); 
  fflush(stdout); 
  time_t tic = time(0); 

  /* fprintf(stdout,"Xover\n"); */ 
 
  // Inicializar las listas de aristas
    city_edges p1_list; 
  init_city_edges(&p1_list);  
  city_edges p2_list; 
  init_city_edges(&p2_list);  

  // Extraer las aristas de los recorridos 
  get_edge_list(P1.path,&p1_list); 
  get_edge_list(P2.path,&p2_list); 

  // Combinar las aristas
  city_edges neighbor_list = merge_city_edges_lists(p1_list,p2_list); 

  C1.obj_eval(); 
  fprintf(stdout,"recorrido antes: ["); 
  for(int i=0; i < 10; i += 1){ 
    fprintf(stdout,"%d,",C1.path[i]); 
  } 
  fprintf(stdout,"], obj_val: %lf \n",C1.cost); 

/*   print_city_edges(p1_list); */
/*   print_city_edges(p2_list); */
/*   print_city_edges(neighbor_list); */

  

  // Realizar la cruza 
  erx(&neighbor_list, C1);  
  erx(&neighbor_list, C2);  
 
  C1.obj_eval(); 
  fprintf(stdout,"recorrido después: ["); 
  for(int i=0; i < 10; i += 1){ 
    fprintf(stdout,"%d,",C1.path[i]); 
  } 
  fprintf(stdout,"], obj_val: %lf \n",C1.cost); 

  /* print_city_edges(p1_list); */
  /* print_city_edges(p2_list); */
  /* print_city_edges(neighbor_list); */

  /* fprintf(stdout,"out xover\n"); */  
  time_t toc =  (time(0) - tic);
  fprintf(stdout,"tiempo de la cruza: %ld\n",toc);
  fflush(stdout); 
} 



 



#endif
