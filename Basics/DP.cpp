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
    n = 0;
    int x, y, id;
    fscanf(file, " %s", trash);
    while(fscanf(file, "%d %d %d", &id, &x, &y) != EOF){
        cities[n].id = id;
        cities[n].x = x;
        cities[n].y = y;
        n++;
    }
    fclose(file);
}

void readSolution(int *argc, char ***argv){
    strcpy(fileSolution, (*argv)[2]);
    FILE *file = fopen(fileSolution, "r");
    for(int i = 0; i < n; i++) fscanf(file, "%d", path + i + 1);
    fclose(file);
}

double INF = -1.0;
int L = 3;


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

double dist(int pos, int c1, int c2){
    double d = sqrt((cities[c1].x - cities[c2].x)*(cities[c1].x - cities[c2].x) + (cities[c1].y - cities[c2].y)*(cities[c1].y - cities[c2].y));
    if(pos%L == 0){
        if(isPrime[c1]) return d;
        else return 1.1*d;
    }
    else return d;
}

void solve(int start, int len){
    int curCities[len], position[len];
    for(int i = 0; i < len; i++) curCities[i] = path[(start + i)%n], position[i] = (start + i)%n;
    double dp[(1 << len)][len];
    int bestIdx[(1 << len)][len];

    int btmMax = (1 << len);
    
    for(int u = 0; u < len; u++) dp[0][u] = INF;
    dp[0][0] = 0.0;
    bestIdx[0][0] = -1;

    for(int btm = 1; btm < btmMax; btm++){
        int btmSize = __builtin_popcount(btm);
        for(int u = 0; u < len; u++) if((btm & (1 << u)) == 0) {
            dp[btm][u] = INF;
            for(int v = 0; v < len; v++) if((btm & (1 << v)) != 0){
                double curValue = dp[btm - (1 << v)][v] + dist(start + btmSize, curCities[v], curCities[u]);
                if(curValue < dp[btm][u]){
                    dp[btm][u] = curValue;
                    bestIdx[btm][u] = v;
                }
            }
        }
    }


    int pos = len - 1;
    int u = len - 1;
    int btm = (1 << u) - 1;
    while(u >= 0){
        path[position[pos]] = curCities[u];
        pos--;
        int v = bestIdx[btm][u];
        btm -= (1 << v);
        u = v;
    }
}


double C = 0;


void constructMatrix(int l, int r){
    int len = r - l + 1;
    int sizeMatrix = L*len + 2;
    vector<vector<double> > distances(sizeMatrix, vector<double>(sizeMatrix, 0.0));
    for(int i = 0; i < sizeMatrix; i++)
        for(int j = 0; j < sizeMatrix; j++) distances[i][j] = INF;
    
    int nL = sizeMatrix - 2;
    int nR = sizeMatrix - 1;
    // Nodo final a nodo inicial
    distances[nR][nL] = 0;

    // Nodo inicial a todos los demas nodos
    int curMod = (l - 1)%L;
    for(int i = 0; i < len; i++){
        distances[nL][L*i + (curMod + 1)%L] = dist(curMod, path[l - 1], path[l + i]) + C;
    }

    // Todos los nodos a nodo final
    curMod = r%L;
    for(int i = 0; i < len; i++){
        distances[L*i + curMod][nR] = dist(curMod, path[l + i], path[r + 1]) + C;
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
    printf("Matrix:\n");
    for(int i = 0; i < sizeMatrix; i++)
        for(int j = 0; j < sizeMatrix; j++)
            printf("%.1f%c", distances[i][j], j == sizeMatrix - 1 ? '\n' : ' ');
}


int main(int argc, char **argv){
    sieve(400000);// printf("End Sieve\n");
    readInstance(&argc, &argv);// printf("Read Instance\n");
    readSolution(&argc, &argv);// printf("Read Solution\n");

    int l = atoi(argv[3]), r = atoi(argv[4]);
    constructMatrix(l, r);
}