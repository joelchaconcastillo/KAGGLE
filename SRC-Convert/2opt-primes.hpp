#ifndef _INCL_2OPTPRIMES
#define _INCL_2OPTPRIMES
///LS variables..

void ls2optprimes(){
	int indexnPathtmp[NCITIES];
	vector< int > permutation;
	vector<bool> activecity(NCITIES, true);
    for(int i = 0; i < NCITIES; i++)
    {
	
	nPath[i] = path[i];
	indexnPathtmp[nPath[i]]=i;
	permutation.push_back(i);
    }

	double ratioexpand = 4.0;
	
//       for(int ii = 0; ii < primeIds.size();ii++)
//	{ 
//	primeIds[ii] = prime_sorted.top().second; prime_sorted.pop();}

//		   random_shuffle(primeIds.begin(), primeIds.end()); 
         int maxite = 0;
       //for(int ii = 0; ii < NCITIES;ii++)
//	for(int o = 0; o < 20; o++)
//	int r = rand()%NCITIES;
//	reversePath(nPath, indexnPath, r%NCITIES, (r+10)%NCITIES);	   
int  enough = 0;	
int jump=1;
bool stuck=true;
double best = 10000000;
 while(enough< NCITIES)
  {
	vector<bool> checked(NCITIES, false);

         int maxite = 0;
	best = min(evaluate(nPath),best);
	printf("best... %lf\n", best);
//       for(int ii = permutation.size()-1; ii >=0;ii--)
       for(int ii = 0; ii < permutation.size();ii++)
       {
	
	bool flag=false;
	   enough++;
//	   if(!flag) ratioexpand =4.0;else { flag = false; ratioexpand *=2.0;  }
	   //int i = indexnPath[primeIds[ii]+(o%10)];
	   int i = indexnPath[permutation[ii]];
	checked[i]=true;
//		if(!activecity[permutation[ii]])continue;
	   if( !(ii%1000)) printf("%d %d %d %lf\n", ii, i, enough, best);
//      for( int i = indexnPath[primeIds[ii]]-4; i < indexnPath[primeIds[ii]]+4; i++)
	  {
	   i = i%NCITIES; 
	   //solve(i-4, 10);
           if( i == 0 || i == NCITIES-1) continue;
	   int K = 50;
//		int nearestn[K+1];
	    vector<int> nearestn;
//	    Kneares(20.0, nPath[i], nearestn);
	    Kneares(K, nPath[i], nearestn);
//	      cout << ii << " " << Permutation[ii]<<endl;
  //         for(int jj = ii+1; jj < NCITIES; jj++)

           for(int jj = 0; jj < nearestn.size(); jj++)
           {
	
//		cout <<jj <<endl;
		int j =  nearestn[jj]%NCITIES;
		if(j <= i || j == 0 || j == NCITIES-1 ) continue;
//		if(checked[j]) continue;
		
//	   if( !(j%10000)) printf("%d\n", j);

		if( max(i,j) - min(i,j) > 5000) continue;
//		if(distdtsp(i,nPath[i],nPath[j]) > 2.0*distdtsp(i, nPath[i], nPath[nearestn[0]]))continue; 
              double olddistance = evaluate(nPath, i, j);
	      reversePath(nPath, indexnPath, i, j);
	
//              reverse(nPath+i, nPath+j);
              double newdistance = evaluate(nPath, i, j);
//		printf("%lf %lf %d %d %d %d\n", olddistance, newdistance,i,j, nPath[i], nPath[j]);
              if( newdistance >= olddistance)
              {	
	           reversePath(nPath, indexnPath, i, j);	   
              }
              else
              {
	         printf("pivot %d %lf %lf %lf %d -->  %d\n", j, evaluate(nPath), distdtsp(i, nPath[i],nPath[j]), distdtsp(i, nPath[i], nPath[nearestn[0]]),  max(i,j) - min(i,j), enough);
		activecity[nPath[i]]=true;
		stuck=false;
		enough = 0;
		flag = true;	
		//solve(j, 10);
	
	   //   printf("pivot %d %d %lf %lf %lf %d %d\n", primeIds[ii], j, evaluate(nPath), distdtsp(i, nPath[i],nPath[j]), distdtsp(i, nPath[i], nPath[nearestn[0]]), jj, max(i,j) - min(i,j));
              }
		
              
           }
		if(!flag){ activecity[nPath[i]]=false; }
//		if(!flag) maxite++;
//		else {maxite = 0; flag=false;}
//
//        	if(maxite > 500)
//		{
//		   maxite = 0;
//		   random_shuffle(primeIds.begin(), primeIds.end()); 
//		  flag = false;
//			cout << "hey!!"<<endl;
//		   o++;
//		   ii=0;
//		}
         }
       

       }


	printf("enough.... %d\n", enough);
	enough = 0;
    saveNewSolution();
   jump++;

int ai=(rand()%NCITIES)-20, bi=ai+4;
if(stuck)
{
		   random_shuffle(nPath+ai, nPath+bi); 
for(int i = ai-1; i < bi+1; i++)
    {
	
	indexnPath[nPath[i]]=i;
    }
}
	stuck=true;

  //     random_shuffle(permutation.begin(), permutation.end()); 
  }

	      printf("pivot %f\n",evaluate(nPath));
}
#endif
