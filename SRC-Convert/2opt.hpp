#ifndef _INCL_2OPT
#define _INCL_2OPT
///LS variables..

void ls2opt2(){
	int indexnPathtmp[NCITIES];
    for(int i = 0; i < NCITIES; i++)
    {
	nPath[i] = path[i];
	indexnPathtmp[nPath[i]]=i;
    }
    bool improved = true;
    while(improved)
    {
 	improved = false;
       for(int i = 1; i < NCITIES-1;i++)
       {
	   if( !(i%10000)) printf("i %d\n", i);
           for(int j = i+1; j < NCITIES; j++)
	   {
              double olddistance = evaluate(nPath, i, j);
              double newdistance = evaluatereverse(nPath, i, j);
              if( newdistance < olddistance)
              {
	           reversePath(nPath, indexnPathtmp, i, j);
	           printf("pivot %d %d %f\n", i, j, evaluate(nPath));
		   improved = true; 
              }
           }
       }
     }
	      printf("pivot %f\n",evaluate(nPath));
}
double walk1(int *cpath, int &Step, int src, int dest)
{
	return distd(Step++, cpath[src], cpath[dest]);
}
double walk2(int *cpath, int &Step, int src, int dest)
{
   double d = 0.0;
   if( dest >= src)
   {
	for(int i = src; i < dest; i++, Step++) d+= distd(Step, cpath[i], cpath[i+1]);
   }
   else
	for(int i = src; i > dest; i--, Step++) d+= distd(Step, cpath[i], cpath[i-1]);
  return d;
}
void makewalk1(int *cpath, vector<int> &newp, int src, int dest)
{
	newp.push_back(cpath[src]);
	newp.push_back(cpath[dest]);
}
void makewalk2(int *cpath, vector<int> &newp, int src, int dest)
{
   if( dest >= src)
	for(int i = src+1; i < dest; i++) newp.push_back(cpath[i]);
   else
	for(int i = src-1; i > dest; i--) newp.push_back(cpath[i]);
}
void ls2opt(){
	int indexnPathtmp[NCITIES];
    for(int i = 0; i < NCITIES; i++)
    {
	nPath[i] = path[i];
	indexnPathtmp[nPath[i]]=i;
    }
    bool improved = true;
    while(improved)
    {
 	improved = false;
       for(int i = 1; i < NCITIES-1;i++)
       {
	   if( !(i%10000)) printf("i %d\n", i);
           for(int j = i+2; j < NCITIES; j++)
	   {
	      int a = i-1, b = i, c = j-1, d = j;
	      int Stepd0, Stepd1; Stepd0 = Stepd1 = a;
	      double d0 = walk1(nPath, Stepd0, a, b) + walk2(nPath, Stepd0, b, c) + walk1(nPath,Stepd0 , c, d); //since the problem is asymmetric, a->b is different to b->a, for this reason is needed to split up the cases.
	      double d1 = walk1(nPath, Stepd1, a, c) + walk2(nPath, Stepd1, c, b) + walk1(nPath, Stepd1, b, d);
              double olddistance = evaluate(nPath, i, j-1);
              double newdistance = evaluatereverse(nPath, i, j-1);
	           printf("pivot -1- %f %f \n", olddistance, newdistance);
	           printf("pivot -2- %f %f \n", d0, d1);
	      if(d1-d0 > 0)
              {
	           reversePath(nPath, indexnPathtmp, i, j);
	           printf("pivot %d %d %f\n", i, j, evaluate(nPath));
		   improved = true; 
              }
           }
       }
     }
	      printf("pivot %f\n",evaluate(nPath));
}
void ls3opt(){
	int indexnPathtmp[NCITIES];
    for(int i = 0; i < NCITIES; i++)
    {
	nPath[i] = path[i];
	indexnPathtmp[nPath[i]]=i;
    }
       for(int i = 1; i < NCITIES-4;i++)
       {
	   if( !(i%10000)) printf("i %d\n", i);
           //for(int j = i+2; j < NCITIES-1; j++)
           for(int j = i+2; j < i+10 && j < NCITIES-2; j++)
	   {
//	     for(int k = j+2; j < NCITIES; j++)
	     for(int k = j+2; k < j+101 && k<NCITIES ; k++)
   	     {
		 double a = i, b=i+1, c = j, d = j+1, e = k, f = k+1;
		 vector<int> Steps(8, a);
		 double mincost = DBL_MAX; 
		 int index = 0;
		 priority_queue< pair < double, int> > D; //complexity insertion O(log(n)) and get minimum O(1)
                 D.push(make_pair(-(walk1(nPath,Steps[0], a, b)+ walk2(nPath,Steps[0], b, c) + walk1(nPath, Steps[0], c, d) + walk2(nPath, Steps[0], d, e) + walk1(nPath, Steps[0], e, f)), 0)); //identity
	
                 D.push(make_pair(-(walk1(nPath,Steps[1], a, b)+ walk2(nPath,Steps[1],  b, c) + walk1(nPath,Steps[1], c, e) + walk2(nPath, Steps[1], e,d)+ walk1(nPath, Steps[1], d,f)),1)); //2-opt
                 D.push(make_pair(-(walk1(nPath,Steps[2], a, c)+ walk2(nPath,Steps[2],  c, b) + walk1(nPath,Steps[2], b, d) + walk2(nPath, Steps[2], d,e)+ walk1(nPath, Steps[2], e,f)),2)); //2-opt
                 D.push(make_pair(-(walk1(nPath,Steps[3], a, e)+ walk2(nPath,Steps[3],  e, d) + walk1(nPath,Steps[3], d, c) + walk2(nPath, Steps[3], c,b)+ walk1(nPath, Steps[3], b,f)),3)); //2-opt
                 D.push(make_pair(-(walk1(nPath,Steps[4], a, d)+ walk2(nPath,Steps[4],  d, e) + walk1(nPath,Steps[4], e, b) + walk2(nPath, Steps[4], b,c)+ walk1(nPath, Steps[4], c,f)),4)); //3-opt
                 D.push(make_pair(-(walk1(nPath,Steps[5], a, e)+ walk2(nPath,Steps[5],  e, d) + walk1(nPath,Steps[5], d, b) + walk2(nPath, Steps[5], b,c)+ walk1(nPath, Steps[5], c,f)),5)); //3-opt
                 D.push(make_pair(-(walk1(nPath,Steps[6], a, d)+ walk2(nPath,Steps[6],  d, e) + walk1(nPath,Steps[6], e, c) + walk2(nPath, Steps[6], c,b)+ walk1(nPath, Steps[6], b,f)),6)); //3-opt
                 D.push(make_pair(-(walk1(nPath,Steps[7], a, c)+ walk2(nPath,Steps[7],  c, b) + walk1(nPath,Steps[7], b, e) + walk2(nPath, Steps[7], e,d)+ walk1(nPath, Steps[7], d,f)),7)); //3-opt
		   vector<int> new_walk;
		if(D.top().second != 0)	
	              printf("pivot d1 %d %d %d %d %f\n", D.top().second, i, j, k, evaluate(nPath));
		 switch(-D.top().second)
		 {
		   case 0: continue;
		 case 1:
		      reversePath(nPath, indexnPathtmp, e, d);
		 break;
		 case 2:
		   reversePath(nPath, indexnPathtmp, c, b);
		  break;
		 case 3:
		   reversePath(nPath, indexnPathtmp, e, d);
		   reversePath(nPath, indexnPathtmp, c, b);
		  break;
		 case 4:
		   makewalk1(nPath, new_walk, a, d);
		   makewalk2(nPath, new_walk, d, e); 
		   makewalk1(nPath, new_walk, e, b);
		   makewalk2(nPath, new_walk, b, c);
		   makewalk1(nPath, new_walk, c, f); 
		  break;
		 case 5:
		   makewalk1(nPath, new_walk, a, e);
		   makewalk2(nPath, new_walk, e, d); 
		   makewalk1(nPath, new_walk, d, b);
		   makewalk2(nPath, new_walk, b, c);
		   makewalk1(nPath, new_walk, c, f); 
		   for(int l = a, o = 0; l <=f; l++, o++) nPath[l] = new_walk[o];
		  break;
		 case 6:
		   makewalk1(nPath, new_walk, a, d);
		   makewalk2(nPath, new_walk, d, e); 
		   makewalk1(nPath, new_walk, e, c);
		   makewalk2(nPath, new_walk, c, b);
		   makewalk1(nPath, new_walk, b, f); 
		   for(int l = a, o = 0; l <=f; l++, o++) nPath[l] = new_walk[o];
		  break;
		 case 7:
		   makewalk1(nPath, new_walk, a, c);
		   makewalk2(nPath, new_walk, c, b); 
		   makewalk1(nPath, new_walk, b, e);
		   makewalk2(nPath, new_walk, e, d);
		   makewalk1(nPath, new_walk, d, f); 
		   for(int l = a, o = 0; l <=f; l++, o++) nPath[l] = new_walk[o];
		  break;
		 }
	     }
           }
       }
	//check solution...
        vector<int> cc(NCITIES,0);
	for(int i = 0; i < NCITIES; i++)
	{
		cc[nPath[i]]++;
		if(cc[nPath[i]] > 1) {cout << "nou" << endl; exit(0);}
	}
	      printf("pivot %f\n",evaluate(nPath));
}
void lsfast2opt(){
	int indexnPathtmp[NCITIES];
    for(int i = 0; i < NCITIES; i++)
    {
	nPath[i] = path[i];
	indexnPathtmp[nPath[i]]=i;
    }
    bool improved = true;
    while(improved)
    {
 	improved = false;
       for(int i = 1; i < NCITIES-1;i++)
       {
	   if( !(i%10000)) printf("i %d\n", i);
	    int K = 50;	
	    vector<int> nearestn;
	    Kneares(K, nPath[i], nearestn);
           for(int jn = 0; jn < K; jn++)
	   {
		
	      int j =  nearestn[jn];
		if(j <= i+1 || j == 0 || j == NCITIES-1 ) continue;//improvement #1

	      int a = i-1, b = i, c = j-1, d = j;
	      int Stepd0, Stepd1; Stepd0 = Stepd1 = a;
	      double d0 = walk1(nPath, Stepd0, a, b) + walk2(nPath, Stepd0, b, c) + walk1(nPath,Stepd0 , c, d); //since the problem is asymmetric, a->b is different to b->a, for this reason is needed to split up the cases.
	      double d1 = walk1(nPath, Stepd1, a, c) + walk2(nPath, Stepd1, c, b) + walk1(nPath, Stepd1, b, d);
              double olddistance = evaluate(nPath, i, j-1);
              double newdistance = evaluatereverse(nPath, i, j-1);
	           printf("pivot -1- %f %f \n", olddistance, newdistance);
	           printf("pivot -2- %f %f \n", d0, d1);
	      if(d1-d0 > 0)
              {
	           reversePath(nPath, indexnPathtmp, i, j);
	           printf("pivot %d %d %f\n", i, j, evaluate(nPath));
		   improved = true; 
              }
           }
       }
     }
	      printf("pivot %f\n",evaluate(nPath));
}
void lsfast3opt(){
	int indexnPathtmp[NCITIES];
    for(int i = 0; i < NCITIES; i++)
    {
	nPath[i] = path[i];
	indexnPathtmp[nPath[i]]=i;
    }
       for(int i = 1; i < NCITIES-4;i++)
       {
	   if( !(i%10000)) printf("i %d\n", i);
	    int K1 = 50;	
	    vector<int> nearestn1;
	    Kneares(K1, nPath[i], nearestn1);
           for(int jn = 0; jn < K1 ; jn++)
	   {
	     int j = nearestn1[jn];

	     if(j <= i+1 || j == 0 || j == NCITIES-1 ) continue;

	    if( !(i%10000)) printf("i %d\n", i);
	    int K2 = 50;	
	    vector<int> nearestn2;
	    Kneares(K2, j, nearestn2);
	     for(int kn = 0; kn < K2 ; kn++)
   	     {
		 int k = nearestn2[kn];
	         if(k <= j+1 || k == 0 || k == NCITIES-1 ) continue;

		 double a = i, b=i+1, c = j, d = j+1, e = k, f = k+1;
		 vector<int> Steps(8, a);
		 double mincost = DBL_MAX; 
		 int index = 0;
		 priority_queue< pair < double, int> > D; //complexity insertion O(log(n)) and get minimum O(1)
                 D.push(make_pair(-(walk1(nPath,Steps[0], a, b)+ walk2(nPath,Steps[0], b, c) + walk1(nPath, Steps[0], c, d) + walk2(nPath, Steps[0], d, e) + walk1(nPath, Steps[0], e, f)), 0)); //identity
	
                 D.push(make_pair(-(walk1(nPath,Steps[1], a, b)+ walk2(nPath,Steps[1],  b, c) + walk1(nPath,Steps[1], c, e) + walk2(nPath, Steps[1], e,d)+ walk1(nPath, Steps[1], d,f)),1)); //2-opt
                 D.push(make_pair(-(walk1(nPath,Steps[2], a, c)+ walk2(nPath,Steps[2],  c, b) + walk1(nPath,Steps[2], b, d) + walk2(nPath, Steps[2], d,e)+ walk1(nPath, Steps[2], e,f)),2)); //2-opt
                 D.push(make_pair(-(walk1(nPath,Steps[3], a, e)+ walk2(nPath,Steps[3],  e, d) + walk1(nPath,Steps[3], d, c) + walk2(nPath, Steps[3], c,b)+ walk1(nPath, Steps[3], b,f)),3)); //2-opt
                 D.push(make_pair(-(walk1(nPath,Steps[4], a, d)+ walk2(nPath,Steps[4],  d, e) + walk1(nPath,Steps[4], e, b) + walk2(nPath, Steps[4], b,c)+ walk1(nPath, Steps[4], c,f)),4)); //3-opt
                 D.push(make_pair(-(walk1(nPath,Steps[5], a, e)+ walk2(nPath,Steps[5],  e, d) + walk1(nPath,Steps[5], d, b) + walk2(nPath, Steps[5], b,c)+ walk1(nPath, Steps[5], c,f)),5)); //3-opt
                 D.push(make_pair(-(walk1(nPath,Steps[6], a, d)+ walk2(nPath,Steps[6],  d, e) + walk1(nPath,Steps[6], e, c) + walk2(nPath, Steps[6], c,b)+ walk1(nPath, Steps[6], b,f)),6)); //3-opt
                 D.push(make_pair(-(walk1(nPath,Steps[7], a, c)+ walk2(nPath,Steps[7],  c, b) + walk1(nPath,Steps[7], b, e) + walk2(nPath, Steps[7], e,d)+ walk1(nPath, Steps[7], d,f)),7)); //3-opt
		   vector<int> new_walk;
		if(D.top().second != 0)	
	              printf("pivot d1 %d %d %d %d %f\n", D.top().second, i, j, k, evaluate(nPath));
		 switch(-D.top().second)
		 {
		   case 0: continue;
		 case 1:
		      reversePath(nPath, indexnPathtmp, e, d);
		 break;
		 case 2:
		   reversePath(nPath, indexnPathtmp, c, b);
		  break;
		 case 3:
		   reversePath(nPath, indexnPathtmp, e, d);
		   reversePath(nPath, indexnPathtmp, c, b);
		  break;
		 case 4:
		   makewalk1(nPath, new_walk, a, d);
		   makewalk2(nPath, new_walk, d, e); 
		   makewalk1(nPath, new_walk, e, b);
		   makewalk2(nPath, new_walk, b, c);
		   makewalk1(nPath, new_walk, c, f); 
		  break;
		 case 5:
		   makewalk1(nPath, new_walk, a, e);
		   makewalk2(nPath, new_walk, e, d); 
		   makewalk1(nPath, new_walk, d, b);
		   makewalk2(nPath, new_walk, b, c);
		   makewalk1(nPath, new_walk, c, f); 
		   for(int l = a, o = 0; l <=f; l++, o++) nPath[l] = new_walk[o];
		  break;
		 case 6:
		   makewalk1(nPath, new_walk, a, d);
		   makewalk2(nPath, new_walk, d, e); 
		   makewalk1(nPath, new_walk, e, c);
		   makewalk2(nPath, new_walk, c, b);
		   makewalk1(nPath, new_walk, b, f); 
		   for(int l = a, o = 0; l <=f; l++, o++) nPath[l] = new_walk[o];
		  break;
		 case 7:
		   makewalk1(nPath, new_walk, a, c);
		   makewalk2(nPath, new_walk, c, b); 
		   makewalk1(nPath, new_walk, b, e);
		   makewalk2(nPath, new_walk, e, d);
		   makewalk1(nPath, new_walk, d, f); 
		   for(int l = a, o = 0; l <=f; l++, o++) nPath[l] = new_walk[o];
		  break;
		 }
	     }
           }
       }
	//check solution...
        vector<int> cc(NCITIES,0);
	for(int i = 0; i < NCITIES; i++)
	{
		cc[nPath[i]]++;
		if(cc[nPath[i]] > 1) {cout << "nou" << endl; exit(0);}
	}
	      printf("pivot %f\n",evaluate(nPath));
}
#
#endif
