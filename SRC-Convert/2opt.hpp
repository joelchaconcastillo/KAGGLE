#ifndef _INCL_2OPT
#define _INCL_2OPT
///LS variables..

void ls2opt(){
	cout << "aa"<<endl;
    vector<int> Permutation(NCITIES);
    for(int i = 0; i < NCITIES; i++)
    {
	
	nPath[i] = path[i];
	Permutation[i] = i;
    }
    int cont = 100;
    while(cont--)
    {
       random_shuffle(Permutation.begin(), Permutation.end()); 
       for(int ii = 0; ii < 10;ii++)
       {
	
           if( Permutation[ii] == 0 || Permutation[ii] == NCITIES-1) continue;
	   int i = Permutation[ii];
	   int K = 100;
		int nearestn[K+1];
	    Kneares(K, i, nearestn);
	      cout << ii << " " << Permutation[ii]<<endl;
           for(int jj = 0; jj < K; jj++)
           {
//		cout <<jj <<endl;
		int j = nearestn[ii]%NCITIES;

		if( max(i,j) - min(i,j) > 3000) continue;
              double olddistance = evaluate(nPath, i, j);
              reverse(nPath+i, nPath+j);
              double newdistance = evaluate(nPath, i, j);
              if( newdistance >= olddistance)
              {	
                 reverse(nPath+i, nPath+j);
              }
              else
              {
		
	      printf("pivot %d %f\n", Permutation[ii], evaluate(nPath));
       	      //random_shuffle(Permutation.begin(), Permutation.end()); 
//                 ii = 0;
              }
              
           }
       }
    }
}
#endif
