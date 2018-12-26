#ifndef _INCL_COMMON
#define _INCL_COMMON
const int MAX = 1001000;

struct city{
    int id;
    //long long x, y;
    double x, y;
};
char currentDirectory[1000];
char filename_newSolution[100];
char filename_oldSolution[100];
char filename_instance[100];
double minx=MAX, miny=MAX, maxx=-MAX,maxy=-MAX, minknn=MAX, maxknn=-MAX;
city cities[MAX];
int path[MAX], nPath[MAX], path_segment[MAX], indexnPath[MAX];
vector<int> primeIds;
priority_queue<pair<double, int>> prime_sorted;
bitset<MAX> used;
int K = 1;
int l, r, len;
long long INF = 1e6, currentCost;
int L = 10;
double C = INF;
int seed = 0;
vector<vector<long long> > distances;
bitset<1001000> isPrime;
long long sieve_size;

void Knearesdist(int k, int index, double *nindex)
{
	char filecity[1000];
	sprintf(filecity,"/tmp/NNearest_files/%d.txt",index);
        FILE * file = fopen(filecity, "r");
	double distance;
	int indexc;
	int cont = 0;
	while(cont < k)	
	{
	   fscanf(file, "%lf %d\n", &distance, &indexc);
//	   if( (indexnPath[indexc] +1) %10==0)
	   {
		nindex[cont] = distance;
		cont++;
	   }
	}
        fclose(file);
}
void Kneares(int k, int index, vector<int> &nindex)
{
	char filecity[1000];
	sprintf(filecity,"/tmp/NNearest_files/%d.txt",index);
        FILE * file = fopen(filecity, "r");
	double distance;
	int indexc;
	int cont = 0;
	while(cont < k)	
	{
	   fscanf(file, "%lf %d\n", &distance, &indexc);
//	   if( (indexnPath[indexc] +1) %10==0)
	   {
		nindex.push_back(indexnPath[indexc]);
//		nindex[cont] = indexnPath[indexc];
		cont++;
	   }
	}

        fclose(file);

}
void Kneares(double ratio, int index, vector<int> &nindex)
{
	char filecity[1000];
	sprintf(filecity,"/tmp/NNearest_files/%d.txt",index);
        FILE * file = fopen(filecity, "r");
	double distance;
	int indexc;
	fscanf(file, "%lf %d\n", &distance, &indexc);
	double maxdist = distance*ratio;
	while( distance <= maxdist)	
	{
	   fscanf(file, "%lf %d\n", &distance, &indexc);
		nindex.push_back(indexnPath[indexc]);
	}

        fclose(file);

}

double getKNearestdistance(int k, int index)
{
	char filecity[1000];
	sprintf(filecity,"../Basics/NNearest_files/%d.txt",index);
       FILE * file = fopen(filecity, "r");
	double distance;
	double totaldistance = 0.0;
//	double maxdistance =  sqrt((maxx-minx)*(maxx-minx) + (maxy-miny)*(maxy-miny));
	int indexc;

	fscanf(file, "%lf %d\n", &distance, &indexc);
	while(!isPrime[indexc])	
	{
	   fscanf(file, "%lf %d\n", &distance, &indexc);
	}
	totaldistance = distance;
	
//	for(int i = 0; i < k; i++)
//	{
//	   fscanf(file, "%lf %d\n", &distance, &indexc);
//	   totaldistance += distance;//(distance/maxdistance);
//	   
//	}
//	totaldistance /=((double)k);
        fclose(file);
        return totaldistance;
}
void readInstance(){
    FILE *file = fopen(filename_instance, "r");
    int N = NCITIES;//n = 197769;
    double x, y;
    int id;
    char trash[100];
    fscanf(file, " %s\n", trash);
    while(N--){
        fscanf(file, "%d,%lf,%lf\n", &id, &x, &y);
        cities[id].id = id;
        cities[id].x = x;
        cities[id].y = y;
	minx = min(minx, x);
	maxx = max(maxx, x);
	miny = min(miny, y);
	maxy = min(miny, y);
	double ne[1];
	if( isPrime[id]) 
	{
	        Knearesdist(1,id , ne);
		prime_sorted.push( make_pair(ne[0],id)  );
		primeIds.pb(id);
	}
	

        //cities[id].x = round(x); //KHL only can recieve a matrix-integers only..
        //cities[id].y = round(y);
    }
    fclose(file);
}

void readSolution(){
    FILE *file = fopen(filename_oldSolution, "r");
    char trash[100];
    fscanf(file, " %s\n", trash);
    for(int i = 0; i < NCITIES; i++){
	 fscanf(file, "%d\n", path + i);
	 indexnPath[path[i]] = i;
	}
    fclose(file);
}
void sieve(long long upperbound){
    sieve_size = upperbound + 1;
    isPrime.set();
    isPrime[0] = isPrime[1] = false;
	
    for(long long i = 2; i < sieve_size; i++) if(isPrime[i]){
        for(long long j = i*i; j < sieve_size; j += i) isPrime[j] = false;
    }
}
double distd(int pos, int c1, int c2){
    double d = (sqrt((cities[c1].x - cities[c2].x)*(cities[c1].x - cities[c2].x) + (cities[c1].y - cities[c2].y)*(cities[c1].y - cities[c2].y)));
    pos++; // Esta es la parte que agregue
    if(pos%L == 0){
        if(isPrime[c1]) return d;
        else return 1.1*d;
    }
    else return d;
}
double distdknn(int pos, int c1, int c2, double distance1, double distance2){
	double dx = (cities[c1].x - cities[c2].x);///(maxx-minx);
	double dy = (cities[c1].y - cities[c2].y);///(maxy-miny);
	double dknn = (distance1 - distance2);///(maxknn - minknn);
    double d = sqrt( dx*dx+ dy*dy + dknn*dknn );
    pos++; // Esta es la parte que agregue
    if(pos%L == 0){
        if(isPrime[c1]) return d;
        else return 1.1*d;
    }
    else return d;
}
double distmodified(int pos, int c1, int c2){
    double d = (sqrt((cities[c1].x - cities[c2].x)*(cities[c1].x - cities[c2].x) + (cities[c1].y - cities[c2].y)*(cities[c1].y - cities[c2].y)));
    pos++; // Esta es la parte que agregue
    if(pos%L == 0){
        if(isPrime[c1]) return d;
        else return 10.1*d;
    }
    else return d;
}
long long distl(int pos, int c1, int c2){
    long long d = round(sqrt((cities[c1].x - cities[c2].x)*(cities[c1].x - cities[c2].x) + (cities[c1].y - cities[c2].y)*(cities[c1].y - cities[c2].y)));
    pos++; // Esta es la parte que agregue
    if(pos%L == 0){
        if(isPrime[c1]) return d;
        else return round(1.1*d);
    }
    else return d;
}
double distdtsp(int pos, int c1, int c2){
    return (sqrt((cities[c1].x - cities[c2].x)*(cities[c1].x - cities[c2].x) + (cities[c1].y - cities[c2].y)*(cities[c1].y - cities[c2].y)));
}

void constructMatrix(){
	currentCost = 0; // check the currentCost and check if the KHL is working...
	for (int i = l-1; i <= r; i++){
		currentCost += (  (distl(i, path[(i)%NCITIES], path[(i+1)%NCITIES])) + C);
	}

    //int len = r - l + 1;
    int sizeMatrix = L*len + 2; //plus 2 cuz the edges that owns to start and end nodes are considered..
    //vector<vector<long long> > distances(sizeMatrix, vector<long long>(sizeMatrix, INF));
    distances.resize(sizeMatrix, vector<long long>(sizeMatrix, INF));
    
    int nL = sizeMatrix - 2;
    int nR = sizeMatrix - 1;
    // Nodo final a nodo inicial
    distances[nR][nL] = 0;

    // Nodo inicial a todos los demas nodos
    int curMod = (l - 1)%L;
    for(int i = 0; i < len; i++){
        distances[nL][L*i + (curMod + 1)%L] = (distl(curMod, path[l - 1], path[(l + i)%NCITIES])) + C;
        //distances[nL][L*i + (l)%L] = dist( l , path[l - 1], path[l + i]) + C;
    }

    // Todos los nodos a nodo final
    curMod = (r)%L;
    for(int i = 0; i < len; i++){
        distances[L*i + (curMod+1)%L][nR] = distl(curMod, path[(l + i)%NCITIES], path[(r + 1)%NCITIES]) + C;
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
                distances[L*i + m][L*j + m] = distl(m - 1, path[(l + i)%NCITIES], path[(l + j)%NCITIES]) + C;
            }
}
void saveMatrix(char *fileInstance) //save a temporal file, which will be an input to the KHL tool
{
    FILE *file = fopen(fileInstance, "w");
    int sizeMatrix = distances.size();
    // Imprimir matriz
    //printf("NAME : KAGGLE suma costos desde %d a %d %lld  \nTYPE : ATSP\nDIMENSION : %d\nEDGE_WEIGHT_TYPE: EXPLICIT\nEDGE_WEIGHT_FORMAT: FULL_MATRIX \nEDGE_WEIGHT_SECTION\n", l-1, r+1, currentCost, sizeMatrix);
    fprintf(file, "NAME : KAGGLE suma costos desde %d a %d %lld  \nTYPE : ATSP\nDIMENSION : %d\nEDGE_WEIGHT_TYPE: EXPLICIT\nEDGE_WEIGHT_FORMAT: FULL_MATRIX \nEDGE_WEIGHT_SECTION\n", l-1, r+1, currentCost, sizeMatrix);
    for(int i = 0; i < sizeMatrix; i++)
        for(int j = 0; j < sizeMatrix; j++)
	    fprintf(file, "%lld%c", distances[i][j], j == sizeMatrix - 1 ? '\n' : ' ');
            //printf("%lld%c", distances[i][j], j == sizeMatrix - 1 ? '\n' : ' ');
    fclose(file);
}
void readNewSolution(char *fileSolution){
    //strcpy(fileSolution, (*argv)[1]);
    char trash[100];
    FILE *file = fopen(fileSolution, "r");
    fscanf(file, "%[^\n]\n", trash); //HEADER of six lines...
    fscanf(file, "%[^\n]\n", trash);
    fscanf(file, "%[^\n]\n", trash);
    fscanf(file, "%[^\n]\n", trash);
    fscanf(file, "%[^\n]\n", trash);
    fscanf(file, "%[^\n]\n", trash);
    //int l = atoi((*argv)[3]), r = atoi((*argv)[4]);
    int n = (r-l+1)*L+2;
    for(int i = 0; i < n; i++) 
    {
	fscanf(file, "%d\n", path_segment + i);
    }
    fclose(file);
}
void generateStartPath(char *filename)
{
  //  char fileSolution[1010];
  //  strcpy(fileSolution, (*argv)[5]);
  //  FILE * file = fopen(fileSolution, "w");
    FILE * file = fopen(filename, "w");
	fprintf(file, "NAME : kagglesanta2018.1502730768.tour\nCOMMENT : Length = 1502730768\nCOMMENT : Found by LKH [Keld Helsgaun] Fri Dec 14 10:51:59 2018\nTYPE : TOUR\nDIMENSION : %d\nTOUR_SECTION\n", (len)*L+2);

    for(int i = 0; i < len*L; i++)
 	{
	   fprintf(file, "%d\n", i+1);
	}
    fprintf(file, "%d\n", len*L+1);
    fprintf(file, "%d\n", len*L+2);
    fclose(file);
}
void transformPath(){
    for(int i = 0; i < NCITIES; i++) nPath[i] = path[i]; //copy path 
    int n = (r-l+1)*L+2;
    int len = n/L, start = -1;
    for(int i = 0; i < n; i++){
        if(path_segment[i] == n - 1) start = i;
        if(path_segment[i] < n - 1) path_segment[i] = (path_segment[i] - 1) / L;
    }
   cout << "startt "<< path_segment[start]<<endl;
//    for(int i = 0; i < n; i++) cout << path_segment[i]<<endl;
    int i = start+1; // the first node is not taken into account
    int nlen = 0;
    while(path_segment[i] != n){
        if(!used[path_segment[i]]){
            used[path_segment[i]] = true;
            nPath[(l + nlen++)%NCITIES] = path[(path_segment[i]+l)%NCITIES];
        }
        i = (i + 1)%n;
    }
//    for(int i = 0; i < nlen; i++) cout << nPath[i]<<endl;
}
void modifySolution(){
//    vector<int> p;
//    for(int i = l; i <= r; i++) p.pb(path[i]);
    //for(int i = l,k = 0; i <= r; i++, k++) nPath[i] = path[k];// p[k];//nPath[k];// p[nPath[k]];
}
void saveNewSolution()
{
  //  char fileSolution[1010];
  //  strcpy(fileSolution, (*argv)[5]);
  //  FILE * file = fopen(fileSolution, "w");
    FILE * file = fopen(filename_newSolution, "w");
	fprintf(file, "Path\n");
    for(int i = 0; i < NCITIES; i++) fprintf(file, "%d\n", nPath[i]);
    fclose(file);
}
void reversePath(int *pathc, int *invpath, int l, int r){
    reverse(pathc + l, pathc + r + 1);
    for(int i = l; i <= r; i++) invpath[pathc[i]] = i;
}
double evaluate(int *pathc){
    double len = 0.0;
    for(int i = 0; i < NCITIES; i++){
        int j = (i + 1)%NCITIES;
        len += distd(i, pathc[i], pathc[j]);
    }
    return len;
}
double evaluate(int * pathc, int l, int r){
    double len = 0.0;
    int i = (l - 1 + NCITIES)%NCITIES;
    while(true){
        int j = (i + 1)%NCITIES;
        len += distd(i, pathc[i], pathc[j]);
        if(i == r) break;
        i = j;
    }
    return len;
}
double evaluatereverse(int * pathc, int l, int r){
    double len = 0.0;
	int a = min(l,r);
	int b = max(l,r);
	len = distd(a-1, pathc[a-1], pathc[b]);
	for(int i = b, step=a; i >a; i--, step++)
	   len += distd(step, pathc[i], pathc[i-1]);
	len += distd(b, pathc[a], pathc[b+1]);

    return len;
}
double evaluate(int * pathc, vector<int> &indexes){
    double len = 0.0;
    for(int k = 0; k < indexes.size()-1; k++)
    {
       int i = (indexes[k] - 1 + NCITIES)%NCITIES;
       while(true){
        int j = (i + 1)%NCITIES;
        len += distd(i, pathc[i], pathc[j]);
        if(i == indexes[k+1]) break;
        i = j;
       }
    }
    return len;
}
double evaluate2(int * pathc, vector<int> &indexes){
    double len = 0.0;
    for(int k = 0; k < indexes.size()-1; k++)
    {
       int i = (indexes[k] - 1 + NCITIES)%NCITIES;
       while(true){
        int j = (i + 1)%NCITIES;
        len += distd(i, pathc[i], pathc[j]);
        if(i == indexes[k+1]) break;
        i = j;
       }
    }
    return len;
}
void swap(int *lpath, int a, int b){ int tmp = lpath[a]; lpath[a] = lpath[b]; lpath[b] = tmp;  }

#endif
