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
vector<Body> &OutputDerived::getlistBody()
{
    return *plistBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::setlistBody(vector<Body> &Input)
{
    plistBody = &Input;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<listSolution> &OutputDerived::getlistSolution()
{
    return *plistSolution;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::setlistSolution(vector<listSolution>& Input)
{
    plistSolution = &Input;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &OutputDerived::getlistFreq()
{
    return *plistFreq;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::setlistFreq(vector<double>& Input)
{
    plistFreq = &Input;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &OutputDerived::getlistWaveDir()
{
    return *plistWaveDir;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputDerived::setlistWaveDir(vector<double>& Input)
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
