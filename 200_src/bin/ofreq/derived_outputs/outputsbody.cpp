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

#include "outputsbody.h"

using namespace std;
using namespace arma;
using namespace osea::ofreq;

//------------------------------------------Function Separator --------------------------------------------------------
OutputsBody::OutputsBody()
{
    //Default constructor
}

//------------------------------------------Function Separator --------------------------------------------------------
OutputsBody::OutputsBody(vector<Body> &listBod,
            vector<SolutionSet> &listSoln,
            vector<double> &listFreq,
            vector<double> &listWaveDir)
{
    //Constructor with all the necessary inputs
    plistBody = & listBod;
    plistSolution = & listSoln;
    plistFreq = & listFreq;
    plistWaveDir = & listWaveDir;
}

//------------------------------------------Function Separator --------------------------------------------------------
OutputsBody::~OutputsBody()
{
    //Default destructor
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::setListBody(vector<Body> &listIn)
{
    plistBody = & listIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::setSolutionSet(vector<SolutionSet> &listIn)
{
    plistSolution = & listIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::setListFreq(vector<double> &listIn)
{
    plistFreq = & listIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &OutputsBody::listFreq()
{
    return * plistFreq;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &OutputsBody::listFreq(int index)
{
    return plistFreq->at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::setListWaveDir(vector<double> &listIn)
{
    plistWaveDir = & listIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &OutputsBody::listWaveDir()
{
    return * plistWaveDir;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &OutputsBody::listWaveDir(int index)
{
    return plistWaveDir->at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::setCurWaveDir(int index)
{
    pCurWaveDir = index;
}

//------------------------------------------Function Separator --------------------------------------------------------
int OutputsBody::getCurWaveInd()
{
    return pCurWaveDir;
}

//------------------------------------------Function Separator --------------------------------------------------------
double OutputsBody::getCurWaveDir()
{
    return plistWaveDir->at(pCurWaveDir);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::setCurBody(int index)
{
    pCurBody = index;
}

//------------------------------------------Function Separator --------------------------------------------------------
int OutputsBody::getCurBodyIndex()
{
    return pCurBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
Body &OutputsBody::refCurBody()
{
    return plistBody->at(pCurBody);
}

//------------------------------------------Function Separator --------------------------------------------------------
//------------------------------------------Function Separator --------------------------------------------------------
//Derived Outputs Now Listed Below
//------------------------------------------Function Separator --------------------------------------------------------
//------------------------------------------Function Separator --------------------------------------------------------

//------------------------------------------Function Separator --------------------------------------------------------
int OutputsBody::sizeGlobalMotion()
{
    return pGlobalMotion.size();
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalMotion &OutputsBody::refGlobalMotion(int index)
{
    //Returns the GlobalMotion object specified by the index.
    return pGlobalMotion.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<cx_vector> OutputsBody::getGlobalMotion(int index)
{
    vector<cx_vector> output;      //Output object
    cx_mat temp;  //Direct output returned from derived output object.
    int num;            //Number of equations.  Used to correctly sizing output matrix.

    //Get number of equations.
    num = refCurBody().getEquationCount();

    //Resize output matrix.
    output.resize(plistFreq->size());

    //Iterate through each frequency and calculate the derived output for that frequency.
    for (unsigned int i = 0; i < plistFreq->size(); i++)
    {
        temp = pGlobalMotion[index].calcOutput(plistFreq->at(i));

        //Assign result to output
        output[i].resize(num);
        for (int j = 0; j <= num; j++)
        {
            output[i][j] = temp(1,j);
        }
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalMotion(GlobalMotion input)
{
    //Adds a global motion object to the list.
    pGlobalMotion.push_back(input);    

    //Add necessary information
    pGlobalMotion.back().setListBody(*plistBody);
    pGlobalMotion.back().setSolutionSet(*plistSolution);
    pGlobalMotion.back().setListFreq(*plistFreq);
    pGlobalMotion.back().setListWaveDir(*plistWaveDir);
    pGlobalMotion.back().setCurWaveDir(pCurWaveDir);
    pGlobalMotion.back().setCurBody(pCurBody);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalMotion()
{
    pGlobalMotion.push_back(GlobalMotion());

    //Add necessary information
    pGlobalMotion.back().setListBody(*plistBody);
    pGlobalMotion.back().setSolutionSet(*plistSolution);
    pGlobalMotion.back().setListFreq(*plistFreq);
    pGlobalMotion.back().setListWaveDir(*plistWaveDir);
    pGlobalMotion.back().setCurWaveDir(pCurWaveDir);
    pGlobalMotion.back().setCurBody(pCurBody);
}

//------------------------------------------Function Separator --------------------------------------------------------
int OutputsBody::sizeGlobalVelocity()
{
    return pGlobalVelocity.size();
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalVelocity &OutputsBody::refGlobalVelocity(int index)
{
    //Returns the GlobalVelocity object specified by the index.
    return pGlobalVelocity.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<cx_vector> OutputsBody::getGlobalVelocity(int index)
{
    vector<cx_vector> output;      //Output object
    cx_mat temp;  //Direct output returned from derived output object.
    int num;            //Number of equations.  Used to correctly sizing output matrix.

    //Get number of equations.
    num = refCurBody().getEquationCount();

    //Resize output matrix.
    output.resize(plistFreq->size());

    //Iterate through each frequency and calculate the derived output for that frequency.
    for (unsigned int i = 0; i < plistFreq->size(); i++)
    {
        temp = pGlobalVelocity.at(index).calcOutput(plistFreq->at(i));

        //Assign result to output
        output[i].resize(num);
        for (int j = 0; j <= num; j++)
        {
            output[i][j] = temp(1,j);
        }
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalVelocity(GlobalVelocity input)
{
    //Adds a global motion object to the list.
    pGlobalVelocity.push_back(input);

    //Add necessary information
    pGlobalVelocity.back().setListBody(*plistBody);
    pGlobalVelocity.back().setSolutionSet(*plistSolution);
    pGlobalVelocity.back().setListFreq(*plistFreq);
    pGlobalVelocity.back().setListWaveDir(*plistWaveDir);
    pGlobalVelocity.back().setCurWaveDir(pCurWaveDir);
    pGlobalVelocity.back().setCurBody(pCurBody);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalVelocity()
{
    pGlobalVelocity.push_back(GlobalVelocity());

    //Add necessary information
    pGlobalVelocity.back().setListBody(*plistBody);
    pGlobalVelocity.back().setSolutionSet(*plistSolution);
    pGlobalVelocity.back().setListFreq(*plistFreq);
    pGlobalVelocity.back().setListWaveDir(*plistWaveDir);
    pGlobalVelocity.back().setCurWaveDir(pCurWaveDir);
    pGlobalVelocity.back().setCurBody(pCurBody);
}

//------------------------------------------Function Separator --------------------------------------------------------
int OutputsBody::sizeGlobalAcceleration()
{
    return pGlobalAcceleration.size();
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalAcceleration &OutputsBody::refGlobalAcceleration(int index)
{
    //Returns the GlobalAcceleration object specified by the index.
    return pGlobalAcceleration.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<cx_vector> OutputsBody::getGlobalAcceleration(int index)
{
    vector<cx_vector> output;      //Output object
    cx_mat temp;  //Direct output returned from derived output object.
    int num;            //Number of equations.  Used to correctly sizing output matrix.

    //Get number of equations.
    num = refCurBody().getEquationCount();

    //Resize output matrix.
    output.resize(plistFreq->size());

    //Iterate through each frequency and calculate the derived output for that frequency.
    for (unsigned int i = 0; i < plistFreq->size(); i++)
    {
        temp = pGlobalAcceleration[index].calcOutput(plistFreq->at(i));

        //Assign result to output
        output[i].resize(num);
        for (int j = 0; j <= num; j++)
        {
            output[i][j] = temp(1,j);
        }
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalAcceleration(GlobalAcceleration input)
{
    //Adds a global motion object to the list.
    pGlobalAcceleration.push_back(input);

    //Add necessary information
    pGlobalAcceleration.back().setListBody(*plistBody);
    pGlobalAcceleration.back().setSolutionSet(*plistSolution);
    pGlobalAcceleration.back().setListFreq(*plistFreq);
    pGlobalAcceleration.back().setListWaveDir(*plistWaveDir);
    pGlobalAcceleration.back().setCurWaveDir(pCurWaveDir);
    pGlobalAcceleration.back().setCurBody(pCurBody);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalAcceleration()
{
    pGlobalAcceleration.push_back(GlobalAcceleration());

    //Add necessary information
    pGlobalAcceleration.back().setListBody(*plistBody);
    pGlobalAcceleration.back().setSolutionSet(*plistSolution);
    pGlobalAcceleration.back().setListFreq(*plistFreq);
    pGlobalAcceleration.back().setListWaveDir(*plistWaveDir);
    pGlobalAcceleration.back().setCurWaveDir(pCurWaveDir);
    pGlobalAcceleration.back().setCurBody(pCurBody);
}

//------------------------------------------Function Separator --------------------------------------------------------
int OutputsBody::sizeGlobalSolution()
{
    return pGlobalSolution.size();
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalSolution &OutputsBody::refGlobalSolution(int index)
{
    //Returns the GlobalSolution object specified by the index.
    return pGlobalSolution.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<cx_vector> OutputsBody::getGlobalSolution(int index)
{
    vector<cx_vector> output;      //Output object
    cx_mat temp;  //Direct output returned from derived output object.
    int num;            //Number of equations.  Used to correctly sizing output matrix.

    //Get number of equations.
    num = refCurBody().getEquationCount();

    //Resize output matrix.
    output.resize(plistFreq->size());

    //Iterate through each frequency and calculate the derived output for that frequency.
    for (unsigned int i = 0; i < plistFreq->size(); i++)
    {
        temp = pGlobalSolution[index].calcOutput(plistFreq->at(i));

        //Assign result to output
        output[i].resize(num);
        for (int j = 0; j <= num; j++)
        {
            output[i][j] = temp(1,j);
        }
    }

    //Write output
    return output;

    //Cleanup
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalSolution(GlobalSolution input)
{
    //Adds a global motion object to the list.
    pGlobalSolution.push_back(input);

    //Add necessary information
    pGlobalSolution.back().setListBody(*plistBody);
    pGlobalSolution.back().setSolutionSet(*plistSolution);
    pGlobalSolution.back().setListFreq(*plistFreq);
    pGlobalSolution.back().setListWaveDir(*plistWaveDir);
    pGlobalSolution.back().setCurWaveDir(pCurWaveDir);
    pGlobalSolution.back().setCurBody(pCurBody);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalSolution()
{
    pGlobalSolution.push_back(GlobalSolution());

    //Add necessary information
    pGlobalSolution.back().setListBody(*plistBody);
    pGlobalSolution.back().setSolutionSet(*plistSolution);
    pGlobalSolution.back().setListFreq(*plistFreq);
    pGlobalSolution.back().setListWaveDir(*plistWaveDir);
    pGlobalSolution.back().setCurWaveDir(pCurWaveDir);
    pGlobalSolution.back().setCurBody(pCurBody);
}
