#include <bits/stdc++.h>
#include <algorithm>
#include <omp.h>
using namespace std;
#define REP(i, n) for (int i = 0; i < int(n); i++)
#define forn(i,n) for (int i = 0; i < int(n); i++)
#define FOR(i, a, b) for (int i = a; i < int(b); i++)
#define FOREACH(i, v) for (__typeof((v).begin()) i = (v).begin(); i != (v).end(); i++)
#define SIZE(v) ((int)(v).size())
#define zero(v) memset(v, 0, sizeof(v))
#define pb push_back
#define mp make_pair
#define st first
#define nd second
#define ll long long
#define pii pair<int, int>
#define NCITIES 197769

const int MAX = 1001000;

struct city{
    int id;
    double x, y;
};

char fileInstance[1010], fileSolution[1010];
int n;
city cities[MAX];
int path[MAX];

void readInstance(int *argc, char ***argv){
    char trash[100];
    strcpy(fileInstance, (*argv)[1]);
    FILE *file = fopen(fileInstance, "r");
    int N = NCITIES;
    double x, y;
    int id;
    fscanf(file, " %s\n", trash);
    while(N--){
        fscanf(file, "%d,%lf,%lf\n", &id, &x, &y);
        cities[id].id = id;
        cities[id].x = x; //KHL only can recieve a matrix-integers only..
        cities[id].y = y;
    }
    fclose(file);
}

void readSolution(int *argc, char ***argv){
    char trash[100];
    strcpy(fileSolution, (*argv)[2]);
    FILE *file = fopen(fileSolution, "r");
    fscanf(file, " %s\n", trash);
    for(int i = 0; i < n; i++) fscanf(file, "%d", path + i);
    fclose(file);
}
double distance(city &A, city &B)
{
   return sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
}

int main(int argc, char **argv){
    readInstance(&argc, &argv);// printf("Read Instance\n");
//omp_set_num_threads(10);    
    #pragma omp parallel for
    for(int i = 0; i < NCITIES; i++)
    {
        priority_queue< pair< double, int > > pq;
	for(int j = 0; j < NCITIES; j++)
	{
	   if(i == j) continue;
	   double dist  = distance(cities[i], cities[j]);
	   pq.push(make_pair(-dist, j));
	}
	char name[100];
	sprintf(name, "NNearest_files/%d.txt", i);
        FILE *file = fopen(name, "w");
	while(!pq.empty())
	{
	   fprintf(file, "%f %d\n", -pq.top().first, pq.top().second); 
	   pq.pop();	
	}
        fclose(file);

	//save in a special file 
	cout << i << endl;
    }

}
