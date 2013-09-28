/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

//License
/*-------------------------------------------------------------------------------------------------------------------*\
 *Copyright Datawave Marine Solutions, 2013.
 *This file is part of OpenSEA.

 *OpenSEA is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.

 *OpenSEA is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.

 *You should have received a copy of the GNU General Public License
 *along with OpenSEA.  If not, see <http://www.gnu.org/licenses/>.
\*-------------------------------------------------------------------------------------------------------------------*/

#include "forcereact.h"

//------------------------------------------Function Separator --------------------------------------------------------
ForceReact::ForceReact() : Force()
{}

//------------------------------------------Function Separator --------------------------------------------------------
ForceReact::~ForceReact()
{}

//------------------------------------------Function Separator --------------------------------------------------------
void ForceReact::setCurDerivative(int newOrder)
{
	currentDerivative = newOrder;
}

//------------------------------------------Function Separator --------------------------------------------------------
void ForceReact::setCurEquationNum(int newEquationNum)
{
	currentEquation = newEquationNum -1;
}

//------------------------------------------Function Separator --------------------------------------------------------
void ForceReact::setCoeff(vector<string> newList, bool isDirectList)
{
	if(isDirectList) //key value pair
	{
		for(unsigned int i = 0; i <= newList.size()/2; i+=2)
		{
            pDerivative[currentDerivative].equationList[currentEquation].
			coefficients[atoi(newList[i].c_str())-1] = atof(newList[i+1].c_str());
		}
	}
	else //is sequential
	{
		for(unsigned int i = 0; i < MAX_COEFFICIENTS; i++)
            pDerivative[currentDerivative].equationList[currentEquation].
			coefficients[i] = atof(newList[i].c_str());
	}
}

//------------------------------------------Function Separator --------------------------------------------------------
void ForceReact::testPrint()
{
	
	cout << "##Reactive Forces##" << endl;
	cout << "Name: " << forceName << endl;
    for(unsigned int i = 0l; i < pDerivative.size(); i++)
	{
		cout << "Derivative Order#: " << i << endl;
        pDerivative[i].testPrint();
	}
	cout<< endl;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<Derivative> &ForceReact::listDerivatives()
{
    return this->getDerivatives();
}

//------------------------------------------Function Separator --------------------------------------------------------
Derivative ForceReact::getDerivative(int num)
{
    //Return the derivative object specified.

    //Also checks to ensure that the request will not return an out of bounds error.  If requested index
    //is greater than current vector size, the vector size is increased.
    if(num > pDerivative.size() - 1)
    {
        //resize vector
        pDerivative.resize(num + 1);
    }

    return pDerivative[num];
}

//------------------------------------------Function Separator --------------------------------------------------------
Derivative &ForceReact::refDerivative(int num)
{
    //Return the derivative object specified.
    //Value is a pointer to the derivative.

    //Also checks to ensure that the request will not return an out of bounds error.  If requested index
    //is greater than current vector size, the vector size is increased.
    if(num > pDerivative.size() - 1)
    {
        //resize vector
        pDerivative.resize(num + 1);
    }

    return pDerivative[num];
}

//------------------------------------------Function Separator --------------------------------------------------------
void ForceReact::addDerivative()
{
    //Adds a new derivative object.
    pDerivative.push_back(Derivative());
}

//------------------------------------------Function Separator --------------------------------------------------------
void ForceReact::addDerivative(Derivative derivIn, int ordIn)
{
    //Adds a new derivative object

    //Check if the order is high enough.
    if (ordIn > pDerivative.size() - 1)
    {
        //Resize vector
        pDerivative.resize(ordIn + 1);
    }

    //Add in the new derivative.  This will overwrite any pre-existing derivatives.
    pDerivative[ordIn] = derivIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
int ForceReact::getMaxOrd()
{
    return pDerivative.size();
}
