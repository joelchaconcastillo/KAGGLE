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
#define NCITIES 197769
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
    fscanf(file, "%[^\n]\n", trash); //HEADER of six lines...
    fscanf(file, "%[^\n]\n", trash);
    fscanf(file, "%[^\n]\n", trash);
    fscanf(file, "%[^\n]\n", trash);
    fscanf(file, "%[^\n]\n", trash);
    fscanf(file, "%[^\n]\n", trash);
    int l = atoi((*argv)[3]), r = atoi((*argv)[4]);
    n = (r-l+2)*L;
    cout << n <<endl;
    for(int i = 0; i < n; i++) 
    {
	fscanf(file, "%d\n", path + i);
    }
   // n = 0;
   // while(fscanf(file, "%d\n", path + n) != EOF){
   //     n++;
   //     if(path[n - 1] == -1){
   //         n--;
   //         break;
   //     }
   // }
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
    fscanf(file,"%s\n", trash);

    for(int i = 0; i < NCITIES; i++) 
    {
	fscanf(file, "%d\n", path + i);
    }
   // n = 1;
   // while(fscanf(file, "%d\n", path + n) != EOF){
   //     n++;
   //     if(path[n]==-1) break;
   // }
   // n--;
    fclose(file);
}

int l, r;
void modifySolution(){
    vector<int> p;
    for(int i = l; i <= r; i++) p.pb(path[i]);
    for(int i = l; i <= r; i++) path[i] =  p[nPath[i-l+1]];
}

void printModifiedSolution(int *argc, char ***argv){
    strcpy(fileSolution, (*argv)[2]);
    //FILE *file = fopen(fileSolution, "w");
    //for(int i = 1; i <= n; i++) fprintf(file, "%d%c", path[i], i == n ? '\n' : ' ');
    for(int i = 1; i <= n; i++) printf("%d\n", path[i]);
    //fclose(file);
}

int main(int argc, char **argv){
    readNewSolution(&argc, &argv);
    transformPath();
    readOriginalSolution(&argc, &argv);
    l = atoi(argv[3]), r = atoi(argv[4]);
    modifySolution();
    printModifiedSolution(&argc, &argv);
}
