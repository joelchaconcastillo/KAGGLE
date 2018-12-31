#ifndef _LS2_H_
#define _LS2_H_

//#include "global.h"
//#include "individual.h"

void fast2opt(CIndividual &C){

	vector<int> permutation;
     for(int i = 0; i < NCITIES; i++)  permutation.push_back(i);
   bool improvement = true;
//    while(improvement)
    for(int h = 0; h < 2; h++)
    {
	improvement = false;
       for(int ii = 0; ii < permutation.size();ii++)
       {
           int i = C.inversePath[permutation[ii]];
	//   if( !(i%3000)) cout << i << endl;
           if( i == 0 || i == NCITIES-1) continue;
           int K =50; //just take five cities..
	    vector<int> Nearestlist = NearestCities[i];
//	   random_shuffle(Nearestlist.begin()+5, Nearestlist.end());
	   //2opt based in the nearest cities
           for(int jj = 0; jj < K; jj++)
           {
                int j = C.inversePath[Nearestlist[jj]]%NCITIES;
	      if( j <=i)continue;

                if( max(i,j) - min(i,j) > 5000) continue; //this is not convienent at all.. !!

              double olddistance = evaluate( C.path, i, j);
              reversePath( C.path,  C.inversePath, i, j);
              double newdistance = evaluate( C.path, i, j);
              if( newdistance >= olddistance)
              {
                   reversePath( C.path,  C.inversePath, i, j);
              }
		else 	
	      {
		   improvement=true;
	      }
           }

         }
//	  //2opt based in the edges..
//	for(int ii = 0; ii < permutation.size();ii++)
//       {
//		if(!(ii%10000)) cout <<"2" <<ii<<endl;
//           int i = C.inversePath[permutation[ii]];
//
//           if( i == 0 || i == NCITIES-1) continue;
// 	  for(int jj = max(2,i-10); jj < i; jj++)
//           {
//              int j = jj;//C.inversePath[C.path[jj]]%NCITIES;
//              double olddistance = evaluate( C.path, j, i);
//              reversePath( C.path,  C.inversePath, j, i);
//              double newdistance = evaluate( C.path, j, i);
//              if( newdistance >= olddistance)
//              {
//                   reversePath( C.path,  C.inversePath, j, i);
//              }
//		else 
//		    {
//		      improvements++;
//		
//		      flag = true;
//		    }
//           }
//	}
//	for(int ii = 0; ii < permutation.size();ii++)
//       {
//		if(!(ii%10000)) cout <<"3" <<ii<<endl;
//           int i = C.inversePath[permutation[ii]];
//
//           if( i == 0 || i == NCITIES-1) continue;
// 	  for(int jj = i+1; jj < min(NCITIES, i+10); jj++)
//           {
//              int j = jj;//C.inversePath[C.path[jj]]%NCITIES;
//              double olddistance = evaluate( C.path, i, j);
//              reversePath( C.path,  C.inversePath, i, j);
//              double newdistance = evaluate( C.path, i, j);
//              if( newdistance >= olddistance)
//              {
//                   reversePath( C.path,  C.inversePath, i, j);
//              }
//		else 
//		    {
//		    improvements++;
//		    flag = true;
//		   }
//           }
//	}
//	random_shuffle(permutation.begin(), permutation.end());
    }
   C.obj_eval();
}

#endif
