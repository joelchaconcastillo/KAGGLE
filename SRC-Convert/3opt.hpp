#ifndef _INCL_3OPT
#define _INCL_3OPT
///LS variables..

void ls2opt(){
    vector<int> Permutation(NCITIES);
	int indexnPathtmp[NCITIES];
    for(int i = 0; i < NCITIES; i++)
    {
	
	nPath[i] = path[i];
	indexnPathtmp[nPath[i]]=i;
    }

       for(int i = 1; i < NCITIES-1;i++)
       {
	   if( !(i%100)) printf("%d\n", i);
	
	   int K = 10;
		int nearestn[K+1];
	    Kneares(K, nPath[i], nearestn);

           for(int jj = 0; jj < K-1; jj++)
           {
		int j = nearestn[jj]%NCITIES;
	      for(int zz = j+1; zz < K-2; zz++)
	      {
		int z = nearestn[zz]%NCITIES;
				
	   	//	if( !(j%10000)) printf("%d\n", j);

		 //      if( max(i,j) - min(i,j) > 5000) continue;
			vector<int> vkopt;	
			vkopt.push_back(i);
			vkopt.push_back(j);
			vkopt.push_back(z);
			do
			{	
                          double olddistance = evaluate(nPath, vkopt);
	        	//  reversePath(nPath, indexnPath, vkopt);
	
              		  double newdistance = evaluate(nPath, vkopt);
			      if( newdistance >= olddistance)
			      {	
			//	   reversePath(nPath, indexnPath, vkopt);
			      }
			      else
			      {
				    printf("pivot %d %d %f\n", i, j, evaluate(nPath));
				   reversePath(nPath, indexnPath, vkopt);
			      }
			}while(next_permutation(vkopt.begin(), vkopt.end()));
           }
	}
    }

	      printf("pivot %f\n",evaluate(nPath));
}
#endif
