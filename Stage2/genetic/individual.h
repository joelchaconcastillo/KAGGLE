#ifndef __INDIVIDUAL_H_
#define __INDIVIDUAL_H_

#include "global.h"
#include "common.h"
class CIndividual{
public:
	CIndividual();
	virtual ~CIndividual();

	vector <int> path, inversePath;
//	int *path;
//        int *inversePath;
	double cost;
	void   init(char *fileSolution);
	void   obj_eval();
	void   show_objective();
	void   show_variable();

    bool   operator<(const CIndividual &ind2);
    bool   operator<<(const CIndividual &ind2);
    bool   operator==(const CIndividual &ind2);
    void   operator=(const CIndividual &ind2);
};

CIndividual::CIndividual()
{
}
CIndividual::~CIndividual()
{
}
void CIndividual::init(char *fileSolution)
{
   readNewSolution(fileSolution, this->path, this->inversePath);
}

void CIndividual::obj_eval()
{
	this->cost = evaluate(this->path);
}
void CIndividual::show_objective()
{
	printf("%f ",cost);
	printf("\n");
}
void CIndividual::show_variable()
{
    for(int n=0; n< nvar; n++)
		printf("%d\n",path[n]);
	printf("\n");
}
void CIndividual::operator=(const CIndividual &ind2)
{
	for(int i =  0; i  < nvar; i++) path[i] = ind2.path[i];
	for(int i =  0; i  < nvar; i++) inversePath[i] = ind2.inversePath[i];
	cost = ind2.cost;
}

bool CIndividual::operator<(const CIndividual &ind2)
{
	return this->cost < ind2.cost;
}
bool CIndividual::operator<<(const CIndividual &ind2)
{
	return this->cost <= ind2.cost;
}

bool CIndividual::operator==(const CIndividual &ind2)
{
	if(ind2.cost==cost) return true;
	else return false;
}
#endif

