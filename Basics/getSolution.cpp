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

int L = 10;
int n;
const int MAX = 1001000;

bitset<MAX> used;
int path[MAX], nPath[MAX];
char fileInstance[1010], fileSolution[1010];


void readNewSolution(int *argc, char ***argv){
    strcpy(fileSolution, (*argv)[1]);
    char trash[100];
    FILE *file = fopen(fileSolution, "r");
    fscanf(file, " %s", trash);
    n = 0;
    while(fscanf(file, "%d", path + n) != EOF){
        n++;
        if(path[n - 1] == -1){
            n--;
            break;
        }
    }
    fclose(file);
}


void transformPath(){
    int len = n/L, start = -1;
    for(int i = 0; i < n; i++){
        if(path[i] == n - 1) start = i;
        if(path[i] < n - 1) path[i] = (path[i] - 1) / L;
    }
    
    int i = start;
    int nlen = 0;
    while(path[i] != n){
        if(!used[path[i]]){
            used[path[i]] = true;
            nPath[nlen++] = path[i];
        }
        i = (i + 1)%n;
    }
}


void readOriginalSolution(int *argc, char ***argv){
    strcpy(fileSolution, (*argv)[2]);
    char trash[100];
    FILE *file = fopen(fileSolution, "r");
    n = 1;
    while(fscanf(file, "%d", path + n) != EOF){
        n++;
    }
    n--;
    fclose(file);
}

int l, r;
void modifySolution(){
    vector<int> p;
    for(int i = l; i <= r; i++) p.pb(path[i]);
    for(int i = l; i <= r; i++) path[l + i] = p[nPath[i]];
}

void printModifiedSolution(int *argc, char ***argv){
    strcpy(fileSolution, (*argv)[2]);
    FILE *file = fopen(fileSolution, "w");
    for(int i = 1; i <= n; i++) fprintf(file, "%d%c", path[i], i == n ? '\n' : ' ');
    fclose(file);
}

int main(int argc, char **argv){
    readNewSolution(&argc, &argv);
    transformPath();
    readOriginalSolution(&argc, &argv);
    l = atoi(argv[3]), r = atoi(argv[4]);
    modifySolution();
    printModifiedSolution(&argc, &argv);
}