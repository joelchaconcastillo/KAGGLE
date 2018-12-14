#include <bits/stdc++.h>
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

const int MAX = 1001000;

struct city{
    int id;
    long long x, y;
};

char fileInstance[1010], fileSolution[1010];
int n;
city cities[MAX];
int path[MAX];

void readInstance(int *argc, char ***argv){
    char trash[100];
    strcpy(fileInstance, (*argv)[1]);
    FILE *file = fopen(fileInstance, "r");
    int N = n = 197769;
    double x, y;
    int id;
    fscanf(file, " %s\n", trash);
    while(N--){
        fscanf(file, "%d,%lf,%lf\n", &id, &x, &y);
        cities[id].id = id;
        //cities[id].x = round(1000.0*x);
        //cities[id].y = round(1000.0*y);
        cities[id].x = round(x); //KHL only can recieve a matrix-integers only..
        cities[id].y = round(y);
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

long long INF = 1e6;
int L = 10;
double C = 100;


bitset<1001000> isPrime;
long long sieve_size;
void sieve(long long upperbound){
    sieve_size = upperbound + 1;
    isPrime.set();
    isPrime[0] = isPrime[1] = false;
    for(long long i = 2; i < sieve_size; i++) if(isPrime[i]){
        for(long long j = i*i; j < sieve_size; j += i) isPrime[j] = false;
    }
}

long long dist(int pos, int c1, int c2){
    long long d = round(sqrt((cities[c1].x - cities[c2].x)*(cities[c1].x - cities[c2].x) + (cities[c1].y - cities[c2].y)*(cities[c1].y - cities[c2].y)));
    pos++; // Esta es la parte que agregue
    if(pos%L == 0){
        if(isPrime[c1]) return d;
        else return round(1.1*d);
    }
    else return d;
}

void constructMatrix(int l, int r){
	long long currentCost = 0; // check the currentCost and check if the KHL is working...
	for (int i = l -1; i <= r; i++){
		currentCost += (dist(i, path[i], path[i+1]) + C);
	}
	
    int len = r - l + 1;
    int sizeMatrix = L*len + 2; //plus 2 cuz the edges that owns to start and end nodes are considered..
    vector<vector<long long> > distances(sizeMatrix, vector<long long>(sizeMatrix, INF));
    
    int nL = sizeMatrix - 2;
    int nR = sizeMatrix - 1;
    // Nodo final a nodo inicial
    distances[nR][nL] = 0;

    // Nodo inicial a todos los demas nodos
    int curMod = (l - 1)%L;
    for(int i = 0; i < len; i++){
        distances[nL][L*i + (curMod + 1)%L] = dist(curMod, path[l - 1], path[l + i]) + C;
        //distances[nL][L*i + (l)%L] = dist( l , path[l - 1], path[l + i]) + C;
    }

    // Todos los nodos a nodo final
    curMod = (r)%L;
    for(int i = 0; i < len; i++){
        distances[L*i + (curMod+1)%L][nR] = dist(curMod, path[l + i], path[r + 1]) + C;
        //distances[L*i + (r)%L][nR] = dist(r+1, path[l + i], path[r + 1]) + C;
    }

    // Nodo del mismo nivel al modulo previo
    for(int i = 0; i < len; i++)
        for(int m = 0; m < L; m++)
            distances[L*i + m][L*i + (m - 1 + L)%L] = 0;

    // Aristas sobre cada nivel del multigrafo
    for(int m = 0; m < L; m++)
        for(int i = 0; i < len; i++)
            for(int j = 0; j < len; j++){
                distances[L*i + m][L*j + m] = dist(m - 1, path[l + i], path[l + j]) + C;
            }
    // Imprimir matriz
    printf("NAME : KAGGLE suma costos desde %d a %d %lld  \nTYPE : ATSP\nDIMENSION : %d\nEDGE_WEIGHT_TYPE: EXPLICIT\nEDGE_WEIGHT_FORMAT: FULL_MATRIX \nEDGE_WEIGHT_SECTION\n", l-1, r+1, currentCost, sizeMatrix);
    for(int i = 0; i < sizeMatrix; i++)
        for(int j = 0; j < sizeMatrix; j++)
            printf("%lld%c", distances[i][j], j == sizeMatrix - 1 ? '\n' : ' ');
}

int main(int argc, char **argv){
    sieve(400000);// printf("End Sieve\n");
    readInstance(&argc, &argv);// printf("Read Instance\n");
    readSolution(&argc, &argv);// printf("Read Solution\n");

    int l = atoi(argv[3]), r = atoi(argv[4]);
    constructMatrix(l, r); // 100 200
}
