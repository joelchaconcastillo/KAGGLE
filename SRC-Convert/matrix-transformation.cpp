//============================================================================
// Author       : Joel Chacón Castillo
// Problem Name : 
// Description  : 
//============================================================================
#include <algorithm>
#include <bitset>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

#define uint64 unsigned long long
#define int64 long long
#define all(ar) ar.begin(), ar.end()
#define pb push_back
#define mp make_pair
#define ff first
#define ss second

#define BIT(n) (1<<(n))
#define AND(a,b) ((a) & (b))
#define OR(a,b) ((a) | (b))
#define XOR(a,b) ((a) ^ (b))
#define sqr(x) ((x) * (x))

typedef pair<int, int> ii;
typedef pair<int, ii> iii;
typedef vector<ii> vii;
typedef vector<int> vi;
typedef vector<double> vd;

#define PI 3.1415926535897932385
#define INF 1000111222
#define eps 1e-7
#define maxN 11
#define NCITIES 197769
///////////////////////////////////
struct city
{
  double x;
  double y;
  int id;
};

int64 _sieve_size; 
bitset<10000010> bs; 
vector<int64> primes;
void sieve(int64 upperbound) { 
   _sieve_size = upperbound + 1; 
   bs.set();
   bs[0] = bs[1] = 0;
   for (int64 i = 2; i <= _sieve_size; i++) 
	if (bs[i]) 
	{
	   for (int64 j = i * i; j <= _sieve_size; j += i) 
		bs[j] = 0;
	   primes.push_back(i);
	} 
} 
bool isPrime(int64 N) { 
   if (N <= _sieve_size) return bs[N];
   for (int i = 0; i < (int)primes.size(); i++)
      if (N % primes[i] == 0) 
	return false;
   return true;
}
void load_cities(const char filename [], vector<city> &cities)
{
   //header....
   char trash[100]; 
   FILE *ptrfile = fopen (filename, "r");
   fscanf(ptrfile, "%s\n", trash);
   cities.resize(NCITIES);
   for(int i = 0; i < cities.size(); i++)
   {
      fscanf(ptrfile,"%d,%lf,%lf\n", &(cities[i].id), &(cities[i].x),&(cities[i].y));
   //   printf("%d %lf %lf\n", (cities[i].id), (cities[i].x),(cities[i].y));
   }
   fclose(ptrfile);
}
void load_path(vi &li) //tail -n +7 File to avoid the header...
{
       int64 id;

    scanf("Path\n");
    for(int i = 0; i < NCITIES+1; i++)//go back to the source
    {
       scanf("%Ld\n", &id);
       li.pb(id);
    }
}
double dist(city &A, city &B)
{
   return sqrt( (A.x-B.x)*(A.x-B.x) + (A.y-B.y)*(A.y-B.y));
}
double composed_distance(city &A, city &B, int step)
{
   double distv = dist(A, B);
   return (isPrime( (step+1) ))? distv: distv*1.1; 
}
void tsp_matrix(int start, int end, vi &li, vector<city> &cities)
{
  int N = end-start+1;  //is +1?... to define...
  int L = 10; //is the number of levels...
   vector < vector< vector <  vd  > > > D (L, vector < vector< vd >  >(L, vector< vd > ( N, vd (N, INF) ) ) );
//  vector < vector< vector < vector < double > > > > D (L, vector< vector <( L, vector < vector < double > > (N, vector < double > (N, INF) )); //Distance matrix .. cuboid LXNXN .. crazy, isnt it?.. 
  // D[Level][Origin][Destine]
  //check the start-node and the end-node...
  for(int i = 0; i < N; i++)
  {
     for(int l = 0; l < L; l++)	//in each level..
     {
	D[l][l][0][i]= composed_distance(cities[li[start]], cities[li[start+i]], l );
	D[l][l][i][N-1]= composed_distance(cities[li[start+i]], cities[li[end]], l );
     }
  } 
  //link nodes between levels... 
  for(int l1 = 0; l1 < L; l1++)
  {
     for(int i = 0; i < N; i++)
     {
	//link each node with its next level with zero distance, 
        //its important to check that the last node is linked with the first in one direction..
	D[l1][(l1+1)%L][i][(i+1)%L] = 0;
	for(int j = 0; j < N; j++)
	   D[l1][(l1+1)%L][i][j] = composed_distance(cities[li[start+i]], cities[li[start+j]], (l1+1)%L ); //
     }
  }
  //print it...|
  int cont = 1;
   for(int i = 0; i < N; i++)
  {
    for(int l1 = 0; l1 < L; l1++)
     {
        for(int l2 = 0; l2 < L; l2++)
	{
          for(int j = 0; j < N; j++)
     	  {
     	    printf("%lf ", D[l1][l2][i][j]);
     	    cont++;
     	    if( ((cont)%(N*L))==0 )
     	  	printf("\n");
     	  }
	}
     }
  }
}
int main() {
        sieve(10000000);
	vector<city> cities;	
        vi li; //size pathfile
	load_cities("cities.csv", cities);
	load_path(li);
	tsp_matrix(5, 10, li, cities);
	return 0;
}

