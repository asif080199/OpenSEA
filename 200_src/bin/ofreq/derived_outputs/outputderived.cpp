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

//------------------------------------------Function Separator --------------------------------------------------------
OutputDerived::OutputDerived()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
OutputDerived::~OutputDerived()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
string OutputDerived::getName()
{
    return pName;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::setName(string nameIn)
{
    pName = nameIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<Body> &OutputDerived::listBody()
{
    return *plistBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
Body &OutputDerived::listBody(int bodIn)
{
    return plistBody->at(bodIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::setListBody(vector<Body> &Input)
{
    plistBody = &Input;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<SolutionSet> &OutputDerived::refSolutionSet()
{
    return *plistSolution;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::setSolutionSet(vector<SolutionSet>& Input)
{
    plistSolution = &Input;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &OutputDerived::listFreq()
{
    return *plistFreq;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &OutputDerived::listFreq(int index)
{
    return plistFreq->at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::setListFreq(vector<double>& Input)
{
    plistFreq = &Input;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &OutputDerived::listWaveDir()
{
    return *plistWaveDir;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &OutputDerived::listWaveDir(int index)
{
    return plistWaveDir->at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::setListWaveDir(vector<double>& Input)
{
    plistWaveDir = &Input;
}

//------------------------------------------Function Separator --------------------------------------------------------
int &OutputDerived::getCurWaveDir()
{
    return *pCurWaveDir;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::setCurWaveDir(int &Input)
{
    pCurWaveDir = &Input;
}

//------------------------------------------Function Separator --------------------------------------------------------
int OutputDerived::getCurBody()
{
    return pCurBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::setCurBody(int Input)
{
    pCurBody = Input;
}

//------------------------------------------Function Separator --------------------------------------------------------
int OutputDerived::getCurWaveInd()
{
    return *pCurWaveDir;
}
