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
int main() {
        sieve(10000000);
	vector<city> cities;	
        vi li; //size pathfile
	load_cities("cities.csv", cities);
	load_path(li);
	double score = 0.0;
        for(int i = 0; i < li.size()-1; i++) //without the last city
	{
	  //double step = dist(cities[li[i]-1], cities[li[i+1]-1]);
	  double step = dist(cities[li[i]], cities[li[i+1]]);
//	cout << li[i]<<endl;
	  if(((i+1)%10)==0)
	  {
	     if( !isPrime(li[i]) ) score += step*1.1;
	     else score += step;
	  } else score += step;
//	printf("%d %d %d %lf %lf\n",isPrime(li[i]), li[i],li[i+1], score, step);
	}
	printf("%lf", score);
//	cout << score<<endl;
	return 0;
}

