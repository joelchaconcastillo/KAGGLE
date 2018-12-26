#ifndef _INCL_DP_LS3
#define _INCL_DP_LS3
///LS variables..


bool solve3(int start, int len){
    //double ss = evaluate();
    int n = NCITIES;
	
    double beforescore = evaluate(nPath);

    int curCities[len], position[len];
    for(int i = 0; i < len; i++) curCities[i] = nPath[(start + i)%n], position[i] = (start + i)%n;
    //double dp[(1 << len)][len];
    //int bestIdx[(1 << len)][len];
    long long btmMax = (1 << len); //set all cities to on
    
    for(int u = 0; u < len; u++) dp[0][u] = INF;
    dp[0][0] = 0.0;
    bestIdx[0][0] = -1;

    for(long long btm = 1; btm < btmMax; btm++){ //check each city 
        long long btmSize = __builtin_popcount(btm);
        for(int u = 0; u < len; u++) if((btm & (1 << u)) == 0) { // check if the city has been visitied
            dp[btm][u] = INF;
            for(int v = 0; v < len; v++) if((btm & (1 << v)) != 0){
                double curValue = dp[btm - (1 << v)][v] + distd(start + btmSize - 1, curCities[v], curCities[u]);
                //double curValue = dp[btm - (1 << v)][v] + distdknn(start + btmSize - 1, curCities[v], curCities[u], getKNearestdistance(K, curCities[v]),getKNearestdistance(K, curCities[u]) );
                //double curValue = dp[btm - (1 << v)][v] + distmodified(start + btmSize - 1, curCities[v], curCities[u]);
                if(curValue < dp[btm][u]){
                    dp[btm][u] = curValue;
                    bestIdx[btm][u] = v;
                }
            }
        }
    }

    double curValue = 0.0;

    for(int i = 0; i < len - 1; i++) curValue += distd(start + i, nPath[(start + i)%n], nPath[(start + i + 1)%n]);
    //for(int i = 0; i < len - 1; i++) curValue +=    distdknn(start + i, nPath[(start + i)%n], nPath[(start + i + 1)%n], getKNearestdistance(K,nPath[(start + i)%n]), getKNearestdistance(K,nPath[(start + i + 1)%n]));
    //for(int i = 0; i < len - 1; i++) curValue += distmodified(start + i, nPath[(start + i)%n], nPath[(start + i + 1)%n]);

    int pos = len - 1;
    int u = len - 1;
    int btm = (1 << u) - 1;
    while(true){
        nPath[position[pos]] = curCities[u];
        pos--;
        int v = bestIdx[btm][u];
        if(v < 0) break;
        if((btm & (1 << v)) == 0) printf("NO\n");
        btm -= (1 << v);
        u = v;
    }
	


    double tt = evaluate(nPath);
    mn = min(mn, tt);
    //if(beforescore > dp[((1 << (len - 1))) - 1][len - 1]) return true;
    if(beforescore >  tt) return true;
    else
    {
       for(int i = 0; i < len; i++) nPath[(start+i)%n] = curCities[i];
return false;
    }
 //  fprintf(stderr, "pivot %d\n", start);	
    //printf("%lf\n", mn);
    //printf("*%lf\n", ss - tt);

    //printf("%lf\n", curValue - dp[((1 << (len - 1))) - 1][len - 1]);
}

void improvement_primes(int prime)
{
//   for(int i = 0; i < primeIds.size();i++)
	{
////		if( !(i%1000))
		//fprintf(stdout, "%d\n", primeIds[i]);
//		fprintf(stdout, "%d\n", prime);
		int kk = 2;
		vector<int> indexneares;

	         //Kneares(kk, nPath[indexnPath[primeIds[i]]], indexneares);
	         Kneares(kk, nPath[indexnPath[prime]], indexneares);
	        for(int o = 0; o < kk; o++)
		 {
		
		  double currentevaluation = evaluate(nPath);
		  //swap(nPath, indexneares[o], indexnPath[primeIds[i]]);
		  swap(nPath, indexneares[o], indexnPath[prime]);
//		  printf("%f\n", evaluate(nPath));
		  if( currentevaluation <  evaluate(nPath))
		  //swap(nPath, indexneares[o], indexnPath[primeIds[i]]);
		  swap(nPath, indexneares[o], indexnPath[prime]);
		  else
		  {
			
		  //swap(indexnPath, nPath[indexneares[o]], primeIds[i]);
		  swap(indexnPath, nPath[indexneares[o]], prime);
		  printf("%d %f %f\n",prime, currentevaluation,evaluate(nPath));
		  //printf("%d %f %f\n",primeIds[i], currentevaluation,evaluate(nPath));
		  }
		 }
	}
}

void localSearch3(){
    int n = NCITIES;
    int permutation[n];
    for(int i = 0; i < n; i++){
	nPath[i] = path[i];
	permutation[i] = i;
	}
//    improvement_primes();


    while(true){
        bool p = false;

        //random_shuffle(permutation, permutation + n);
        for(int i = 0; i < n; i++){
			if( !(i%1000))
		fprintf(stdout, "%d\n", i);
        
            if(solve3(permutation[i], len)){

                p = true;
	        i = abs((i-len)%n);
    	        saveNewSolution();
	        
            }
	    	if(isPrime[nPath[permutation[i]]]) improvement_primes(nPath[permutation[i]]);
////	      {
////
////	        
////		  double currentevaluation = evaluate(nPath);
////		  int indexneares[100];
////		  Kneares(10, nPath[i], indexneares);
////		bool flag=false;
////		  for(int o = 0; o < 10; o++)
////		 {
////		  swap(nPath, indexneares[o], i);
////		  if( currentevaluation <  evaluate(nPath))
////		  swap(nPath, indexneares[o], i);
////		  else
////		  {
////			
////		  swap(indexnPath, nPath[indexneares[o]], nPath[i]);
////		  printf("%f %f\n",currentevaluation,evaluate(nPath));
////		  flag=true;
//////		cout << getchar()<<endl;
////		  }
////		 }
		//if(flag) i= 0;


	//	int k = i;
	//	double currentevaluation = evaluate(nPath);
	//	for(int l = 0; l<10; l++)
	//	{
	// 	  while((k+1)%10!=0){k--;}
	//	  swap(nPath, k, i);
	//	  printf("%f %f\n",currentevaluation,evaluate(nPath));
	//	  if( currentevaluation <  evaluate(nPath))
	//	  swap(nPath, k, i);
	//	}
	//	k = i;
	//	for(int l = 0; l<10; l++)
	//	{
	// 	  while((k+1)%10!=0){k++;}
	//	  swap(nPath, k, i);
	//	  if( currentevaluation <  evaluate(nPath))
	//	  swap(nPath, k, i);
	//	}
///	      }
//		printf("pivot %d %f\n", i, evaluate(nPath));
        }
        //if(!p) break;
	break;
    }
}
#endif
