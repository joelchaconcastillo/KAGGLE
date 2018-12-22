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
    double x, y;
};

char fileInstance[1010], fileSolution[1010];
int n;
city cities[MAX];
int path[MAX];
int inversePath[MAX];

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
        cities[id].x = x; //KHL only can recieve a matrix-integers only..
        cities[id].y = y;
    }
    fclose(file);
}

void readSolution(int *argc, char ***argv){
    strcpy(fileSolution, (*argv)[2]);
    FILE *file = fopen(fileSolution, "r");
    char trash[100];
    fscanf(file, " %s", trash);
    for(int i = 0; i < n; i++){
        fscanf(file, "%d", path + i), path[i]--;
        inversePath[path[i]] = i;
    }
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

double evaluate(){
    double len = 0.0;
    for(int i = 0; i < n; i++){
        int j = (i + 1)%n;
        len += dist(i, path[i], path[j]);
    }
    return len;
}

void reversePath(int l, int r){
    reverse(path + l, path + r + 1);
    for(int i = l; i <= r; i++) inversePath[path[i]] = i;
}

vector<int> near[MAX];

void loadNear(int *argc, char ***argv){
    strcpy(fileSolution, (*argv)[3]);
    FILE *file = fopen(fileSolution, "r");
    for(int i = 0; i < n; i++){
        near[i].clear();
        int nr;
        for(int j = 0; j < 50; j++){
            fscanf(file, "%d", &nr);
            near[i].push_back(nr);
        }
    }
    fclose(file);
}

double evaluate(int l, int r){
    double len = 0.0;
    int i = (l - 1 + n)%n;
    while(true){
        int j = (i + 1)%n;
        len += dist(i, path[i], path[j]);
        if(i == r) break;
        i = j;
    }
    return len;
}

int q[MAX];

void check(){
    for(int i = 0; i < n; i++) q[i] = i;
    printf("Begin check\n");
    double curVal = evaluate();
    int pot = 1;
    int cntBest = 0;
    while(true){
        random_shuffle(q, q + n);
        pot = min(2*pot, 3000);

        if(cntBest > 2){
            printf("Holi\n");
            int a = rand()%(n - 3000);
            int b = near[a][rand()%10];
            reversePath(a, b);
            curVal = evaluate();
            cntBest = 0;
        }
        bool noUp = true;
        for(int ii = 1; ii < n; ii++){
            int i = q[ii];
            //reverse(near[i].begin(), near[i].end());
            for(int j = 0; j < 50; j++){
                int a = min(inversePath[i], inversePath[near[i][j]]);
                int b = max(inversePath[i], inversePath[near[i][j]]);

                if(b - a > 5000 && rand()%20 != 0) continue;
                //double cur = evaluate();
                double curAB = evaluate(a, b);
                reversePath(a, b);
                //double tmp = evaluate();
                double tmpAB = evaluate(a, b);

                //if(abs(cur - tmp - curAB + tmpAB) > 1e-5) printf("%lf %lf\n", cur - tmp, curAB - tmpAB);
                //printf("%lf\n", tmpVal - curVal);
                if(tmpAB < curAB){
                    curVal = curVal - curAB + tmpAB;
                    printf("i = %d, curVal = %lf, real = %lf, dist = %d\n", i, curVal, curVal, b - a);
                    noUp = false;
                    continue;
                }
                reversePath(a, b);
            }
        }
        printf("%lf\n", evaluate());
        if(noUp) cntBest++;
    }
    printf("End check\n");
}

bitset<50*MAX> isTabu;
double curVs[50*MAX];
double tmpVs[50*MAX];


void startTabuSearch(set<pair<double, int> > &S){
    for(int i = 0; i < n; i++){
        printf("<i> = %d\n", i);
        for(int j = 0; j < 50; j++){
            int a = min(inversePath[i], inversePath[near[i][j]]);
            int b = max(inversePath[i], inversePath[near[i][j]]);
            if(b - a > 3000){
                curVs[50*i + j] = 1e13;
            }
            else{
                double curAB = evaluate(a, b);
                reversePath(a, b);
                double tmpAB = evaluate(a, b);
                curVs[50*i + j] = -curAB + tmpAB;
                reversePath(a, b);
            }
            S.insert(make_pair(curVs[50*i + j], 50*i + j));
        }
    }
}


void updateTabuSearch(set<pair<double, int> > &S, int l, int r){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < 50; j++){
            int a = min(inversePath[i], inversePath[near[i][j]]);
            int b = max(inversePath[i], inversePath[near[i][j]]);
            if(a > r || l > b) continue;
            if(!isTabu[50*i + j]) S.erase(make_pair(curVs[50*i + j], 50*i + j));
            if(b - a > 3000){
                curVs[50*i + j] = 1e13;
            }
            else{
                double curAB = evaluate(a, b);
                reversePath(a, b);
                double tmpAB = evaluate(a, b);
                curVs[50*i + j] = -curAB + tmpAB;
                reversePath(a, b);
            }
            if(!isTabu[50*i + j]) S.insert(make_pair(curVs[50*i + j], 50*i + j));
        }
    }
}

void checkTrueValues(){
    for(int i = 0; i < n; i++){
        printf("<i> = %d\n", i);
        for(int j = 0; j < 50; j++){
            int a = min(inversePath[i], inversePath[near[i][j]]);
            int b = max(inversePath[i], inversePath[near[i][j]]);
            double tmp;
            if(b - a > 3000){
                tmp = 1e13;
            }
            else{
                double curAB = evaluate(a, b);
                reversePath(a, b);
                double tmpAB = evaluate(a, b);
                tmp = -curAB + tmpAB;
                reversePath(a, b);
            }
            if(tmp != curVs[50*i + j]) printf("%lf %lf %d %d %d %d\n", tmp, curVs[50*i + j], i, j, a, b), getchar();
        }
    }
}


int tabuTime(int t){
    return 100;
}

void tabuSearch(){
    int t = 0;
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > tabuList;
    set<pair<double, int> > S;

    double curFitness = evaluate();
    double bestFitness = curFitness;

    printf("before start\n");
    startTabuSearch(S);
    printf("after start\n");

    int maxT = 200;
    int b[] = {1,2,1,4,1,2,1,8,1,2,1,4,1,2,1};
    int a[15];
    for(int i = 0; i < 15; i++) a[i] = maxT*b[i]/8;
    int j = 0, x = 4*maxT*b[j]/8;

    while(true){

        x--;
        if(x == 0){
            j = (j + 1)%15;
            x = 4*maxT*b[j]/8;
        }
        int tenure = a[j];

        while(!tabuList.empty() && tabuList.top().first <= t){
            int hsh = tabuList.top().second;
            isTabu[hsh] = false;
            S.insert(make_pair(curVs[hsh], hsh));
            tabuList.pop();
        }

        int curCity = S.begin()->second; isTabu[curCity] = true;
        double delta = S.begin()->first;
        S.erase(S.begin());

        int c = curCity/50, idx = curCity%50;
        int a = min(inversePath[c], inversePath[near[c][idx]]);
        int b = max(inversePath[c], inversePath[near[c][idx]]);

        if(b - a <= 3000){
            curFitness += delta;
            reversePath(a, b);
            if(curFitness < bestFitness){
                bestFitness = curFitness;
                printf("best = %lf\n", bestFitness);
                printf("%lf\n", evaluate());
            }
            updateTabuSearch(S, a - 1, b + 1);
            //printf("a = %d, b = %d\n", a, b);
            //checkTrueValues();
        }
        tabuList.push(make_pair(t + tenure, curCity));  
    }
}


int main(int argc, char **argv){
    sieve(400000);// printf("End Sieve\n");
    readInstance(&argc, &argv);// printf("Read Instance\n");
    readSolution(&argc, &argv);// printf("Read Solution\n");

    loadNear(&argc, &argv);
    tabuSearch();
    //check();
}