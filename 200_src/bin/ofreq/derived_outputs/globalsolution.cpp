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

#include "globalsolution.h"
#include "outputsbody.h"

using namespace std;
using namespace arma;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
GlobalSolution::GlobalSolution()
{
    //Set the order of the derivative as a default.
    this->setDerivative(0);
    pClassName = "GlobalSolution";
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalSolution::GlobalSolution(OutputsBody *input)
{
    //Set the order of the derivative as a default.
    this->setDerivative(0);

    //Set the outputsbody pointers.
    this->setOutputBody(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalSolution::~GlobalSolution()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
void GlobalSolution::setDerivative(int ord)
{
    //Set the order of derivative
    orderDerivative = ord;
}

//------------------------------------------Function Separator --------------------------------------------------------
int GlobalSolution::getDerivative()
{
    //Get the order of derivative
    return orderDerivative;
}

//------------------------------------------Function Separator --------------------------------------------------------
int GlobalSolution::calcOutput(int freqIn)
{    
    cx_mat* output = new cx_mat();
    complexDouble compI(0,1); //comlex number w/ 0 real, 1 imaginary used below for computations
    complexDouble wavefreq(0,0);   //Wave frequency.
    int errVal = 1;                     //Returned error code.  Set to default starting value of unknown err.

    //Get number of rows
    int nrow;
    nrow = pParentBody->refCurSolution().n_freqs();

    //Get current frequency
    wavefreq.real(pParentBody->listFreq(freqIn));

    //resize matrix
    output->set_size(nrow, 1);

    for(int i = 0; i < nrow; i++)
    {
        output->at(i,1) = pow(wavefreq, orderDerivative) *
                          pow(compI, orderDerivative) *
                          pParentBody->refCurSolution().
                            refSolution(pParentBody->getCurWaveInd(),freqIn).
                            refSolnMat()(i,0);


    }

    //Write result to results list
    addResult(output);

    //Change error to success
    errVal = 0;         //No errors.

    return errVal;
}

//==========================================Section Separator =========================================================
//Protected Functions
//------------------------------------------Function Separator --------------------------------------------------------
Solution &GlobalSolution::getSolution(double freqIn)
{
    //Gets the Solution object for the specified frequency.
    int freqIndex;      //The index of the specified frequency in the list of frequencies.
    bool matchFind;     //Boolean to track if a match was found to the requested frequency.
    matchFind = false;

    //Search for the index of the frequency specified.
    for (unsigned int i = 0; i < pParentBody->refCurSolution().n_freqs(); i++)
    {
        //Check if the frequency matches the specified freq in.
        if (pParentBody->listFreq(i) == freqIn)
        {
            freqIndex = i;
            break;
            matchFind = true;
        }
    }

    //Retrieve the solution object.
    try
    {
        if (!matchFind) //Throw an error if the search never found a matching frequency.
            throw 2;

        return pParentBody->refCurSolution().refSolution(getCurWaveInd(), freqIndex);
    }
    catch (int err)
    {
        //error handler later
    }
}

//==========================================Section Separator =========================================================
//Private Functions



