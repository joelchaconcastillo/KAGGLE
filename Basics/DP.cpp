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
    double x, y;
    int id;
    fscanf(file, " %s\n", trash);
    while(fscanf(file, "%d,%lf,%lf\n", &id, &x, &y) != EOF){
        cities[id].id = id;
        //cities[id].x = round(1000.0*x);
        //cities[id].y = round(1000.0*y);
        cities[id].x = round(x); //KHL only can recieve a matrix-integers only..
        cities[id].y = round(y);

        n++;
    }
    fclose(file);
}

void readSolution(int *argc, char ***argv){
    strcpy(fileSolution, (*argv)[2]);
    FILE *file = fopen(fileSolution, "r");
    char trash[100];
    fscanf(file, " %s", trash);
    for(int i = 0; i < n; i++) fscanf(file, "%d", path + i + 1);
    fclose(file);
}

double INF = 1e10;
int L = 10;


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
    pos++;
    if(pos%L == 0){
        if(isPrime[c1]) return d;
        else return 1.1*d;
    }
    else return d;
}

bool solve(int start, int len){
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
                double curValue = dp[btm - (1 << v)][v] + dist(start + btmSize - 1, curCities[v], curCities[u]);
                if(curValue < dp[btm][u]){
                    dp[btm][u] = curValue;
                    bestIdx[btm][u] = v;
                }
            }
        }
    }

    double curValue = 0.0;

    for(int i = 0; i < len - 1; i++) curValue += dist(start + i, path[(start + i)%n], path[(start + i + 1)%n]);

    int pos = len - 1;
    int u = len - 1;
    int btm = (1 << u) - 1;
    while(true){
        path[position[pos]] = curCities[u];
        pos--;
        int v = bestIdx[btm][u];
        if(v < 0) break;
        btm -= (1 << v);
        u = v;
    }

    printf("%lf\n", curValue - dp[((1 << (len - 1))) - 1][len - 1]);
    return curValue > dp[((1 << (len - 1))) - 1][len - 1];
}


void localSearch(int len){
    int permutation[n];
    for(int i = 0; i < n; i++) permutation[i] = i + 1;
    while(true){
        bool p = false;

        random_shuffle(permutation, permutation + n);

        for(int i = 0; i < n; i++){
            if(solve(permutation[i], len)){
                p = true;
            }
        }

        if(!p) break;
    }
}


int main(int argc, char **argv){
    sieve(400000);// printf("End Sieve\n");
    readInstance(&argc, &argv);// printf("Read Instance\n");
    readSolution(&argc, &argv);// printf("Read Solution\n");
    int len = atoi(argv[3]);
    localSearch(len);
}