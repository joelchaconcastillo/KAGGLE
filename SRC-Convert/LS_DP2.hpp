#ifndef _INCL_DP_LS2
#define _INCL_DP_LS2
///LS variables..


bool solve2(int start, int len){
    //double ss = evaluate();
    int n = NCITIES;
    int curCities[len], position[len];
    for(int i = 0; i < len; i++) curCities[i] = nPath[(start + i)%n], position[i] = (start + i)%n;
    //double dp[(1 << len)][len];
    //int bestIdx[(1 << len)][len];
    int btmMax = (1 << len); //set all cities to on
    
    for(int u = 0; u < len; u++) dp[0][u] = INF;
    dp[0][0] = 0.0;
    bestIdx[0][0] = -1;

    for(int btm = 1; btm < btmMax; btm++){ //check each city 
        int btmSize = __builtin_popcount(btm);
        for(int u = 0; u < len; u++) if((btm & (1 << u)) == 0) { // check if the city has been visitied
            dp[btm][u] = INF;
            for(int v = 0; v < len; v++) if((btm & (1 << v)) != 0){
                //double curValue = dp[btm - (1 << v)][v] + distdtsp(start + btmSize - 1, curCities[v], curCities[u]);
                double curValue = dp[btm - (1 << v)][v] + distd(start + btmSize - 1, curCities[v], curCities[u]);
                //double curValue = dp[btm - (1 << v)][v] + distmodified(start + btmSize - 1, curCities[v], curCities[u]);
                if(curValue < dp[btm][u]){
                    dp[btm][u] = curValue;
                    bestIdx[btm][u] = v;
                }
            }
        }
    }

    double curValue = 0.0;

    //for(int i = 0; i < len - 1; i++) curValue += distdtsp(start + i, nPath[(start + i)%n], nPath[(start + i + 1)%n]);
    for(int i = 0; i < len - 1; i++) curValue += distd(start + i, nPath[(start + i)%n], nPath[(start + i + 1)%n]);
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
    if(curValue > dp[((1 << (len - 1))) - 1][len - 1]) return true;
    else
    {
       for(int i = 0; i < len; i++) nPath[i] = path[(start + i)%n];
return false;
    }
 //  fprintf(stderr, "pivot %d\n", start);	
    //printf("%lf\n", mn);
    //printf("*%lf\n", ss - tt);

    //printf("%lf\n", curValue - dp[((1 << (len - 1))) - 1][len - 1]);
}


void localSearch2(){
    int n = NCITIES;
    int permutation[n];
    for(int i = 0; i < n; i++){
	nPath[i] = path[i];
	permutation[i] = i;
	}
    while(true){
        bool p = false;

        //random_shuffle(permutation, permutation + n);
        for(int i = 0; i < n; i++){
            if(solve(permutation[i], len)){
		
                p = true;
		i = abs((i-len)%n);
    		saveNewSolution();
		
            }
		printf("pivot %d %f\n", i, evaluate(nPath));
        }
        //if(!p) break;
	break;
    }
}
#endif
