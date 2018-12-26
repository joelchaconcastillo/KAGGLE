#ifndef _INCL_2OPT
#define _INCL_2OPT
///LS variables..

void ls2opt(){
    vector<int> Permutation(NCITIES);
	int indexnPathtmp[NCITIES];
    for(int i = 0; i < NCITIES; i++)
    {
	
	nPath[i] = path[i];
	Permutation[i] = i;
	indexnPathtmp[nPath[i]]=i;
    }
    int cont = 100;
  //  while(cont--)
    {
//       random_shuffle(Permutation.begin(), Permutation.end()); 
       for(int ii = 1; ii < NCITIES;ii++)
       {
	   if( !(ii%100)) printf("%d\n", ii);
	
           if( Permutation[ii] == 0 || Permutation[ii] == NCITIES-1) continue;
	   int i = Permutation[ii];
	   int K = 10;
		vector<int> nearestn;
	    Kneares(K, nPath[i], nearestn);
//	      cout << ii << " " << Permutation[ii]<<endl;
  //         for(int jj = ii+1; jj < NCITIES; jj++)

           for(int jj = 0; jj < K; jj++)
           {
//		cout <<jj <<endl;
		int j = nearestn[jj]%NCITIES;
		if(j==0)continue;
		
	   if( !(j%10000)) printf("%d\n", j);

		if( max(i,j) - min(i,j) > 5000) continue;
              double olddistance = evaluate(nPath, i, j);
//	      reversePath(nPath, indexnPath, i, j);
	
//              reverse(nPath+i, nPath+j);
              double newdistance = evaluatereverse(nPath, i, j);
	//	printf("%lf %lf %d %d %d %d\n", olddistance, newdistance,i,j, nPath[i], nPath[j]);
              if( newdistance >= olddistance)
              {	
              //   reverse(nPath+i, nPath+j);
              }
              else
              {
				
	           reversePath(nPath, indexnPath, i, j);
	      printf("pivot %d %d %f\n", i, j, evaluate(nPath));
       	      //random_shuffle(Permutation.begin(), Permutation.end()); 
//                 ii = 0;
              }
              
           }
       }
    }

	      printf("pivot %f\n",evaluate(nPath));
}
#endif
