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

// Revisar si las ciudades aparecen solo una vez
bool sol_sanity_check(int * path){ 
  int city_cnt[NCITIES] = {0}; 
  
  for(int i=0; i < NCITIES+1; i += 1){ 
    city_cnt[path[i]] += 1; 
  } 

  bool sane = true; 
  if (city_cnt[0] != 2){ 
    fprintf(stdout,"las ciudad 0 aparece %d veces en el recorrido\n",city_cnt[0]);
    fflush(stdout);
    sane = false; 
  }
  for(int i=1; i < NCITIES;  i+=1){ 
    if (city_cnt[i] != 1){
      fprintf(stdout,"las ciudad %d aparece %d veces en el recorrido\n", i,city_cnt[i]);
      fflush(stdout);

      sane = false; 
      break; 
    } 
  } 

  return sane; 
}




// Operador de cruza por recombinación de aristas
//
// 
//
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



  // Obtener la lista de ciudades vecinas
  int cities_with_edge_to[NCITIES*4]; 
  for(int i=0; i < NCITIES; i += 1){ 
    int * ptr = cities_with_edge_to + (i*4); 
    for(int j=0; j < 4; j+= 1){ 
      ptr[j] = -1; 
    }
  } 
  for(int i=0; i < neighbor_list->size; i+=1){ 
    edge_list * list = neighbor_list->candidate + i; 
    for(int j=0; j < list->size; j+= 1){ 
      int id = list->edge[j]; 
      int * ptr = cities_with_edge_to + (id*4); 
      for(int k=0; k < 4; k += 1){ 
        if(ptr[k] == -1){ 
          ptr[k] = i; 
          break; 
        } 
      } 
    }
  } 


  // Loop para crear el nuevo recorrido empleando las aristas candidatas de 
  while(true){ 
    
    // Agrega la ciudad al recorrido 
    new_tour[t_ptr] = x; 
    t_ptr += 1;  

    // Criterio de paro (hemos terminado el recorrido
    if(t_ptr == NCITIES){ 
      break; 
    }
    
    // Borrar la ciudad que se ha agregado al recorrido 
    indexes[cities[c_end]] = indexes[x]; 
    swp(cities,indexes[x],c_end);
    indexes[x] = c_end; 
    c_end -= 1; 

    // Borrar la ciudad de las ciudades candidatas
    int * ptr = cities_with_edge_to + (x*4); 
    for(int i=0; i < 4; i+=1){ 
      if(ptr[i] == -1){ 
        break; 
      } 
      edge_list * list = neighbor_list->candidate + ptr[i]; 
      for(int j=0; j <= list->end; j+=1){ 
        if(list->edge[j] == x){ 
          swp(list->edge,list->end,j); 
          list->end -= 1; 
          break; 
        }
      } 
    } 

    // Seleccionar de forma aleatoria si no hay mas aristas en la ciudad en
    // la que estamos. 
    if (neighbor_list->candidate[x].end < 0){ 
      x = cities[random_int(0,c_end)]; 
    }else{ 
      int rem_cities = neighbor_list->candidate[x].end;
      erx_tuple min = {0,1.0/0.0};
      for(int i=0; i <= rem_cities; i+=1){ 
        int    id    = neighbor_list->candidate[x].edge[i];
        double value = neighbor_list->candidate[id].end;
        if (min.val > value){ 
          min.val = value; 
          min.id  = id; 
        } 
      }
      x = min.id; 
    } 
  } 
  new_tour[NCITIES] = 0; 

//  fprintf(stdout,"is_a_valid_tour : %s\n", ((sol_sanity_check(new_tour))? "yes" : "no"));
//  fflush(stdout);

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



  //fprintf(stdout,"parent 1 is_a_valid_tour : %s\n", ((sol_sanity_check(P1.path))? "yes" : "no"));
  //fflush(stdout);
  //fprintf(stdout,"parent 2 is_a_valid_tour : %s\n", ((sol_sanity_check(P2.path))? "yes" : "no"));
  //fflush(stdout);




  //fprintf(stdout,"iniciando la cruza\n"); 
  //fflush(stdout); 
  time_t tic = time(0); 

 
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
  ////fprintf(stdout,"recorrido antes: ["); 
  ////for(int i=0; i < 10; i += 1){ 
  ////  fprintf(stdout,"%d,",C1.path[i]); 
  ////} 
  ////fprintf(stdout,"], obj_val: %lf \n",C1.cost); 


  // Realizar la cruza 
  erx(&neighbor_list, C1);  
  erx(&neighbor_list, C2);  
 
  C1.obj_eval(); 
  //fprintf(stdout,"recorrido después: ["); 
  //for(int i=0; i < 10; i += 1){ 
  //  fprintf(stdout,"%d,",C1.path[i]); 
  //} 
  //fprintf(stdout,"], obj_val: %lf \n",C1.cost); 

  time_t toc =  (time(0) - tic);
 // fprintf(stdout,"tiempo de la cruza: %ld\n",toc);
 // fflush(stdout); 

  free_city_edges(&p1_list); 
  free_city_edges(&p2_list); 
  free_city_edges(&neighbor_list); 
} 

void erx2(vector<set<int>> &edges, CIndividual &Ci){ 
  
   vecto<bool> citySelected(NCITIES, false);
   int indexpath = 0;
   int i_city = random_int(0,NCITIES);
   Ci.path[indexpath] = i_city; //first conection...
   citySelected[i_city] = true; 

   for(int i = 0; i < NCITIES-1; i++)
   {
	bool unselected = true;
	//select the best city
	int minv = INFINITY;
	int j_selected = -1;
	for(int j = 0; j < edges[i_city].size(); i++)
        {
	  int j_city =edges[i_city][j];
	  if(citySelected[j_city]) continue;	
	  //select the lowest number of edges, otherwise at randomly
	  if( countcity[j_city] <  minv  )  
	  {
	     minv = countcity[j_city];
	     j_selected = j_city;
	     unselected = false;
	  }
        }

	if(j_selected == -1)
	{
	     
	}

	indexpath++;
	Ci.path[indexpath] = i_city; //first conecction...
        citySelected[i_city] = true; 	
   }
}

void xover_kaggle2(
  CIndividual &P1, 
  CIndividual &P2, 
  CIndividual &C1, 
  CIndividual &C2) { 



  //fprintf(stdout,"parent 1 is_a_valid_tour : %s\n", ((sol_sanity_check(P1.path))? "yes" : "no"));
  //fflush(stdout);
  //fprintf(stdout,"parent 2 is_a_valid_tour : %s\n", ((sol_sanity_check(P2.path))? "yes" : "no"));
  //fflush(stdout);




  //fprintf(stdout,"iniciando la cruza\n"); 
  //fflush(stdout); 
  time_t tic = time(0); 

 
  //////INITIALIZATION...
  vector< set<int> > edges_C1(NCITIES), edges_C2(NCITIES);
// 
  for(int i = 0; i < NCITIES; i++)
  {
      edges_C1[P1.path[i]].push_back(P1.path[(i+1)%NCITIES]);
      edges_C1[P2.path[i]].push_back(P2.path[(i+1)%NCITIES]);
      edges_C1[P1.path[(i+1)%NCITIES]].push_back(P1.path[i]);
      edges_C1[P2.path[(i+1)%NCITIES]].push_back(P2.path[i]);

      edges_C2[P1.path[i]].push_back(P1.path[(i+1)%NCITIES]);
      edges_C2[P2.path[i]].push_back(P2.path[(i+1)%NCITIES]);
      edges_C2[P1.path[(i+1)%NCITIES]].push_back(P1.path[i]);
      edges_C2[P2.path[(i+1)%NCITIES]].push_back(P2.path[i]);
  }
 //operator.... 


    erx2(edges_C1, C1);
    erx2(edges_C2, C2);
//    city_edges p1_list; 
//  init_city_edges(&p1_list);  
//  city_edges p2_list; 
//  init_city_edges(&p2_list);  

//  // Extraer las aristas de los recorridos 
//  get_edge_list(P1.path,&p1_list); 
//  get_edge_list(P2.path,&p2_list); 

  // Combinar las aristas
  city_edges neighbor_list = merge_city_edges_lists(p1_list,p2_list); 

  C1.obj_eval(); 
  ////fprintf(stdout,"recorrido antes: ["); 
  ////for(int i=0; i < 10; i += 1){ 
  ////  fprintf(stdout,"%d,",C1.path[i]); 
  ////} 
  ////fprintf(stdout,"], obj_val: %lf \n",C1.cost); 


  // Realizar la cruza 
  erx(&neighbor_list, C1);  
  erx(&neighbor_list, C2);  
 
  C1.obj_eval(); 
  //fprintf(stdout,"recorrido después: ["); 
  //for(int i=0; i < 10; i += 1){ 
  //  fprintf(stdout,"%d,",C1.path[i]); 
  //} 
  //fprintf(stdout,"], obj_val: %lf \n",C1.cost); 

  time_t toc =  (time(0) - tic);
 // fprintf(stdout,"tiempo de la cruza: %ld\n",toc);
 // fflush(stdout); 

  free_city_edges(&p1_list); 
  free_city_edges(&p2_list); 
  free_city_edges(&neighbor_list); 
}



#endif
