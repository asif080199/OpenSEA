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

    //Create output objects by default.
    Initialize();
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

    //Create output objects by default.
    Initialize();
}

//------------------------------------------Function Separator --------------------------------------------------------
OutputsBody::~OutputsBody()
{
    //Default destructor

    //Delete matrix of results
    this->ClearResult();

    //Delete each of the OutputDerived objects.

    //Delete Global Solution objects.
    for (unsigned int i = 0; i < plistGlobalSolution.size(); i++)
        delete plistGlobalSolution.at(i);

    //Delete Global Motion objects.
    for (unsigned int i = 0; i < plistGlobalMotion.size(); i++)
        delete plistGlobalMotion.at(i);

    //Delete Global Velocity objects.
    for (unsigned int i = 0; i < plistGlobalVelocity.size(); i++)
        delete plistGlobalVelocity.at(i);

    //Delete GLobal Acceleration objects.
    for (unsigned int i = 0; i < plistGlobalAcceleration.size(); i++)
        delete plistGlobalAcceleration.at(i);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::Initialize()
{
    //Create the GlobalMotion object.  Only need one.
    if (plistGlobalMotion.size() == 0)
        this->addGlobalMotion();

    //Create the GlobalVelocity object.  Only need one.
    if (plistGlobalVelocity.size() == 0)
        this->addGlobalVelocity();

    //Create the GlobalAcceleration object.  Only need one.
    if (plistGlobalAcceleration.size() == 0)
        this->addGlobalAcceleration();

    //Don't need to add any GlobalSolution objects by default.
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
std::vector<SolutionSet> &OutputsBody::listSolutionSet()
{
    return *plistSolution;
}

//------------------------------------------Function Separator --------------------------------------------------------
osea::ofreq::SolutionSet &OutputsBody::listSolutionSet(int index)
{
    try
    {
        return plistSolution->at(index);
    }
    catch(const std::exception &err)
    {
        //Write error message
        logErr.Write(ID + "Unknown error occurred." + std::string(err.what()));
        logStd.Notify();   
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
osea::ofreq::SolutionSet &OutputsBody::refCurSolution()
{
    return listSolutionSet(pCurBody);
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
std::vector<Body> &OutputsBody::listBody()
{
    return *plistBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
Body &OutputsBody::listBody(int bodIn)
{
    return plistBody->at(bodIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::setCurOutput(OutputDerived *input)
{
    pCurOutput = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
OutputDerived &OutputsBody::refCurOutput()
{
    return *pCurOutput;
}

//------------------------------------------Function Separator --------------------------------------------------------
OutputDerived *OutputsBody::getCurOutput()
{
    return pCurOutput;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<arma::cx_mat*> &OutputsBody::listResult()
{
    return plistResult;
}

//------------------------------------------Function Separator --------------------------------------------------------
arma::cx_mat &OutputsBody::listResult(unsigned int index)
{
    //Check for out of bounds errors.  Resize if necessary.
    if ((index > plistResult.size() - 1) ||
            (plistResult.size() == 0))
    {
        //Resize
        plistResult.resize(index + 1);
    }

    return *(plistResult.at(index));
}

//------------------------------------------Function Separator --------------------------------------------------------
arma::cx_mat* &OutputsBody::refResult(unsigned int index)
{
    //Check for out of bounds errors.  Resize if necessary.
    if ((index > plistResult.size() - 1) ||
            (plistResult.size() == 0))
    {
        //Resize
        plistResult.resize(index + 1);
    }

    return plistResult.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::ClearResult()
{   
    //Clear the array of result data.
    for (unsigned int i = 0; i < plistResult.size(); i++)
    {
        delete plistResult.at(i);
    }
    plistResult.clear();
    plistResult.resize(0);

    //Set the pointer for the current OutputDerived object back to null.
    pCurOutput = NULL;
}

//------------------------------------------Function Separator --------------------------------------------------------
//------------------------------------------Function Separator --------------------------------------------------------
//Derived Outputs Now Listed Below
//------------------------------------------Function Separator --------------------------------------------------------
//------------------------------------------Function Separator --------------------------------------------------------

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<GlobalMotion*> &OutputsBody::listGlobalMotion()
{
    return plistGlobalMotion;
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalMotion &OutputsBody::listGlobalMotion(unsigned int index)
{
    //Returns the GlobalMotion object specified by the index.

    //Check if out of bounds.
    if ((index > plistGlobalMotion.size() - 1) ||
            (plistGlobalMotion.size() == 0))
    {
        //Resize global motion
        plistGlobalMotion.resize(index + 1);
    }

    return *(plistGlobalMotion.at(index));
}

//------------------------------------------Function Separator --------------------------------------------------------
int OutputsBody::calcGlobalMotion(unsigned int index)
{
    int errVal = 0;         //Set error value, initialized with a default value of 0.

    //First clear results matrix
    this->ClearResult();

    //Now calculate requested results object.  Check for out of bounds error.
    if ((index <= plistGlobalMotion.size() - 1) &&
            (plistGlobalMotion.size() != 0))
    {
        //Calc result for each frequency.
        for (unsigned int i = 0; i < plistFreq->size(); i++)
        {
            try
            {
                errVal = listGlobalMotion(index).calcOutput(i);
                if (errVal != 0)
                    throw std::runtime_error("Failed to calculate derived output:  body motion.");
            }
            catch (const std::exception &err)
            {
                //Write error message
                logErr.Write(ID + std::string(err.what()));
                logStd.Notify();

                //Stop program execution
               
            }
        }
    }

    return errVal;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalMotion(GlobalMotion *input)
{
    //Adds a global motion object to the list.
    input->setOutputBody(this);

    plistGlobalMotion.push_back(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalMotion()
{
    //Create new global motion object on the stack.
    GlobalMotion *mot1 = new GlobalMotion(this);

    plistGlobalMotion.push_back(mot1);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<GlobalVelocity*> &OutputsBody::listGlobalVelocity()
{
    return plistGlobalVelocity;
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalVelocity &OutputsBody::listGlobalVelocity(unsigned int index)
{
    //Returns the GlobalVelocity object specified by the index.

    //Check if current index out of bounds.
    if ((index > plistGlobalVelocity.size() - 1) ||
            (plistGlobalVelocity.size() == 0))
    {
        plistGlobalVelocity.resize(index + 1);
    }

    //Write output.
    return *(plistGlobalVelocity.at(index));
}

//------------------------------------------Function Separator --------------------------------------------------------
int OutputsBody::calcGlobalVelocity(unsigned int index)
{
    int errVal = 0;         //Set error value, initialized with a default value of 0.

    //First clear results matrix
    this->ClearResult();

    //Now calculate requested results object.  Check for out of bounds error.
    if ((index <= plistGlobalVelocity.size() - 1) &&
            (plistGlobalVelocity.size() != 0))
    {
        //Calc result for each frequency.
        for (unsigned int i = 0; i < plistFreq->size(); i++)
        {
            try
            {
                errVal = listGlobalVelocity(index).calcOutput(i);
                if (errVal != 0)
                    throw std::runtime_error("Failed to calculate derived output:  body velocity.");
            }
            catch (const std::exception &err)
            {
                //Write error message
                logErr.Write(ID + std::string(err.what()));
                logStd.Notify();

                //Stop program execution
               
            }
        }
    }

    return errVal;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalVelocity(GlobalVelocity *input)
{
    //Adds a global motion object to the list.
    input->setOutputBody(this);
    plistGlobalVelocity.push_back(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalVelocity()
{
    //Create new GlobalVelocity object
    GlobalVelocity *vel1 = new GlobalVelocity(this);

    plistGlobalVelocity.push_back(vel1);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<GlobalAcceleration*> &OutputsBody::listGlobalAcceleration()
{
    return plistGlobalAcceleration;
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalAcceleration &OutputsBody::listGlobalAcceleration(unsigned int index)
{
    //Returns the GlobalAcceleration object specified by the index.

    //Check if requested index is out of bounds.
    if ((index > plistGlobalAcceleration.size() - 1) ||
            (plistGlobalAcceleration.size() == 0))
    {
        //Resize vector
        plistGlobalAcceleration.resize(index + 1);
    }

    //Write outputs
    return *(plistGlobalAcceleration.at(index));
}

//------------------------------------------Function Separator --------------------------------------------------------
int OutputsBody::calcGlobalAcceleration(unsigned int index)
{
    int errVal = 0;         //Set error value, initialized with a default value of 0.

    //First clear results matrix
    this->ClearResult();

    //Now calculate requested results object.  Check for out of bounds error.
    if ((index <= plistGlobalAcceleration.size() - 1) &&
            (plistGlobalAcceleration.size() != 0))
    {
        //Calc result for each frequency.
        for (unsigned int i = 0; i < plistFreq->size(); i++)
        {
            try
            {
                errVal = listGlobalAcceleration(index).calcOutput(i);
                if (errVal != 0)
                    throw std::runtime_error("Failed to calculate derived output:  body acceleration.");
            }
            catch (const std::exception &err)
            {
                //Write error message
                logErr.Write(ID + std::string(err.what()));
                logStd.Notify();

                //Stop program execution
               
            }
        }
    }

    return errVal;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalAcceleration(GlobalAcceleration *input)
{
    //Adds a global motion object to the list.
    input->setOutputBody(this);
    plistGlobalAcceleration.push_back(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalAcceleration()
{
    GlobalAcceleration *acc1 = new GlobalAcceleration(this);

    plistGlobalAcceleration.push_back(acc1);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<GlobalSolution*> &OutputsBody::listGlobalSolution()
{
    return plistGlobalSolution;
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalSolution &OutputsBody::listGlobalSolution(unsigned int index)
{
    //Returns the GlobalSolution object specified by the index.
    //Check if requested index is out of bounds
    if ((index > plistGlobalSolution.size() - 1) ||
            (plistGlobalSolution.size() == 0))
    {
        //Resize
        plistGlobalSolution.resize(index + 1);
    }

    //Write output
    return *(plistGlobalSolution.at(index));
}

//------------------------------------------Function Separator --------------------------------------------------------
int OutputsBody::calcGlobalSolution(unsigned int index)
{
    int errVal = 0;         //Set error value, initialized with a default value of 0.

    //First clear results matrix
    this->ClearResult();

    //Now calculate requested results object.  Check for out of bounds error.
    if ((index <= plistGlobalSolution.size() - 1) &&
            (plistGlobalSolution.size() != 0))
    {
        //Calc result for each frequency.
        for (unsigned int i = 0; i < plistFreq->size(); i++)
        {
            try
            {
                errVal = listGlobalSolution(index).calcOutput(i);
                if (errVal != 0)
                    throw std::runtime_error("Failed to calculate derived output:  body derivative solution.");
            }
            catch (const std::exception &err)
            {
                //Write error message
                logErr.Write(ID + std::string(err.what()));
                logStd.Notify();

                //Stop program execution
               
            }
        }
    }

    return errVal;
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalSolution(GlobalSolution *input)
{
    //Adds a global motion object to the list.
    input->setOutputBody(this);
    plistGlobalSolution.push_back(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void OutputsBody::addGlobalSolution()
{
    GlobalSolution *sol1 = new GlobalSolution(this);
    plistGlobalSolution.push_back(sol1);
}
