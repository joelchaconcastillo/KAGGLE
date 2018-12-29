#ifndef _INCL_2OPT
#define _INCL_2OPT
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
	   if( !(i%10000)) printf("i %d\n", i);
           for(int j = i+1; j < i+10000 && j < NCITIES; j++)
	   {
              double olddistance = evaluate(nPath, i, j);
              double newdistance = evaluatereverse(nPath, i, j);
              if( newdistance < olddistance)
              {
	           reversePath(nPath, indexnPathtmp, i, j);
	           printf("pivot %d %d %f\n", i, j, evaluate(nPath));
              }
           }
       }
	      printf("pivot %f\n",evaluate(nPath));
}
#endif
