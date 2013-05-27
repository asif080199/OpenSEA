#ifndef OUTPUTDERIVED_H
#define OUTPUTDERIVED_H
#include "body.h"
using namespace std;

class OutputDerived
{
public:
	OutputDerived();
	~OutputDerived();
	Body theBody;
	int curWaveDirection;
	//vector<OutputDerived> outputList;
	//vector<OutputDerived> outputType;
	string name;
	virtual cx_mat calculateOutput(cx_mat,double) = 0;
};
#endif
