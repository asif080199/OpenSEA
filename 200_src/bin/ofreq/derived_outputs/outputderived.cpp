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

#include "outputderived.h"
#include "outputsbody.h"

using namespace arma;
using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
OutputDerived::OutputDerived()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
OutputDerived::OutputDerived(OutputsBody *input)
{
    //Set the pointer to containing OutputsBody.
    setOutputBody(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
OutputDerived::~OutputDerived()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::setOutputBody(OutputsBody *input)
{
    pParentBody = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string OutputDerived::getName()
{
    return pName;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string OutputDerived::getClassName()
{
    return pClassName;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::setName(std::string nameIn)
{
    pName = nameIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<Body> &OutputDerived::listBody()
{
    return pParentBody->listBody();
}

//------------------------------------------Function Separator --------------------------------------------------------
Body &OutputDerived::listBody(int bodIn)
{
    return pParentBody->listBody(bodIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<SolutionSet> &OutputDerived::listSolutionSet()
{
    return pParentBody->listSolutionSet();
}

//------------------------------------------Function Separator --------------------------------------------------------
SolutionSet &OutputDerived::listSolutionSet(int index)
{
    return pParentBody->listSolutionSet(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &OutputDerived::listFreq()
{
    return pParentBody->listFreq();
}

//------------------------------------------Function Separator --------------------------------------------------------
double &OutputDerived::listFreq(int index)
{
    return pParentBody->listFreq(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &OutputDerived::listWaveDir()
{
    return pParentBody->listWaveDir();
}

//------------------------------------------Function Separator --------------------------------------------------------
double &OutputDerived::listWaveDir(int index)
{
    return pParentBody->listWaveDir(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
double OutputDerived::getCurWaveDir()
{
    return pParentBody->getCurWaveDir();
}

//------------------------------------------Function Separator --------------------------------------------------------
int OutputDerived::getCurBodyIndex()
{
    return pParentBody->getCurBodyIndex();
}

//------------------------------------------Function Separator --------------------------------------------------------
int OutputDerived::getCurWaveInd()
{
    return pParentBody->getCurWaveInd();
}


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::addResult(arma::cx_mat* input, int index)
{
    //First check if index was specified or not.
    if (index == -1)
    {
        //Index not specified.  Set it to one past the end of the list.
        index = pParentBody->listResult().size();
    }

    //Check if requested index is out of bounds.
    if ((index > pParentBody->listResult().size() - 1) ||
            (pParentBody->listResult().size() == 0))
    {
        //Resize the list
        pParentBody->listResult().resize(index + 1);
    }

    //Place result in the list of results
    pParentBody->getResult(index) = input;
}


//==========================================Section Separator =========================================================
//Private Functions
