#ifndef _LS2_H_
#define _LS2_H_

//#include "global.h"
//#include "individual.h"
void ls2optprimes(CIndividual &C){

    int *indexnPath = C.inversePath; 
    int *nPath = C.path;
       vector<int> primespermtation = primeIds;
	int repetitions = 50;
    int cont = repetitions;
	vector<int> permutation;
     for(int i = 0; i < NCITIES; i++)  permutation.push_back(i);
    while(cont--)
    {
	//random_shuffle(primespermtation.begin(), primespermtation.end());
	random_shuffle(permutation.begin(), permutation.end());
	int check = 100;//NCITIES;	
       //for(int ii = 0; ii < checkprimes;ii++)
       for(int ii = 0; ii < check;ii++)
       {
//           if( !(ii%100)) printf("%d %d\n", ii, primeIds[ii]);
           //int i = indexnPath[primespermtation[ii]];
           int i = indexnPath[permutation[ii]];

           if( i == 0 || i == NCITIES-1) continue;
           int K = 10;
           int nearestn[K+1];
            Kneares(K, nPath[i], nearestn);

           for(int jj = 0; jj < K; jj++)
           {
//              cout <<jj <<endl;
                int j = indexnPath[nearestn[jj]]%NCITIES;
	      if( j <=i)continue;
 //          if( !(j%10000)) printf("%d\n", j);

                if( max(i,j) - min(i,j) > 5000) continue;
//                if(distdtsp(i,nPath[i],nPath[j]) > 2.0*distdtsp(i, nPath[i], nPath[nearestn[0]]))continue;

              double olddistance = evaluate(nPath, i, j);
              reversePath(nPath, indexnPath, i, j);

//              reverse(nPath+i, nPath+j);
              double newdistance = evaluate(nPath, i, j);
//              printf("%lf %lf %d %d %d %d\n", olddistance, newdistance,i,j, nPath[i], nPath[j]);
              if( newdistance >= olddistance)
              {
              //   reverse(nPath+i, nPath+j);
                   reversePath(nPath, indexnPath, i, j);
              }
		else{
			check +=100;
			 printf("%lf\n", evaluate(nPath));
		         cont = repetitions;
		    }
           }
         }
    }
}
void fast2opt(CIndividual &C){

    bool flag = true;
	vector<int> permutation;
     for(int i = 0; i < NCITIES; i++)  permutation.push_back(i);
    int improvements = 0;
//    while(flag)
//    for(int h = 0; h < 2; h++)
    {
	flag = false;
       for(int ii = 0; ii < permutation.size();ii++)
       {
           int i = C.inversePath[permutation[ii]];
	//	if(!(ii%10000)) cout << ii<<endl;
           if( i == 0 || i == NCITIES-1) continue;
           int K = 50;
           int nearestn[K+1];
            Kneares(K, C.path[i], nearestn);
	   //2opt based in the nearest cities
           for(int jj = 0; jj < K; jj++)
           {
                int j = C.inversePath[nearestn[jj]]%NCITIES;
	      if( j <=i)continue;

                if( max(i,j) - min(i,j) > 5000) continue;

              double olddistance = evaluate( C.path, i, j);
              reversePath( C.path,  C.inversePath, i, j);
              double newdistance = evaluate( C.path, i, j);
              if( newdistance >= olddistance)
              {
                   reversePath( C.path,  C.inversePath, i, j);
              }
		else 	
		   {
		    flag = true;
		    improvements++;
		    }
           }

         }
//	  //2opt based in the edges..
	for(int ii = 0; ii < permutation.size();ii++)
       {
           int i = C.inversePath[permutation[ii]];
 	  for(int jj = max(2,i-10); jj < i; jj++)
           {
              int j = jj;//C.inversePath[C.path[jj]]%NCITIES;
              double olddistance = evaluate( C.path, j, i);
              reversePath( C.path,  C.inversePath, j, i);
              double newdistance = evaluate( C.path, j, i);
              if( newdistance >= olddistance)
              {
                   reversePath( C.path,  C.inversePath, j, i);
              }
		else 
		    {
		      improvements++;
		
		      flag = true;
		    }
           }
	}
	for(int ii = 0; ii < permutation.size();ii++)
       {
           int i = C.inversePath[permutation[ii]];
 	  for(int jj = i+1; jj < min(NCITIES, i+10); jj++)
           {
              int j = jj;//C.inversePath[C.path[jj]]%NCITIES;
              double olddistance = evaluate( C.path, i, j);
              reversePath( C.path,  C.inversePath, i, j);
              double newdistance = evaluate( C.path, i, j);
              if( newdistance >= olddistance)
              {
                   reversePath( C.path,  C.inversePath, i, j);
              }
		else 
		    {
		    improvements++;
		    flag = true;
		   }
           }
	}
	improvements = 0;
	random_shuffle(permutation.begin(), permutation.end());
    }
   C.obj_eval();
}

#endif
