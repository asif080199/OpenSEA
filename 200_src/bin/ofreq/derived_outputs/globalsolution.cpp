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

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
GlobalSolution::GlobalSolution()
{
    //Set the order of the derivative as a default.
    this->setDerivative(0);
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
cx_mat GlobalSolution::calcOutput(double freqIn)
{
    cx_mat output;
	complexDouble compI(0,1); //comlex number w/ 0 real, 1 imaginary used below for computations
    complexDouble wavefreq(freqIn,0);   //Wave frequency.

    //Get number of rows
    int nrow;
    nrow = getSolution(freqIn).refSolnMat().n_rows;

    //resize matrix
    output.set_size(1, nrow);

    for(int i = 0; i < nrow; i++)
	{
        output(1,i) = pow(wavefreq, orderDerivative) * pow(compI, orderDerivative) * getSolution(freqIn).getSolnMat(i);
	}
    return output;
}

//==========================================Section Separator =========================================================
//Protected Functions
//------------------------------------------Function Separator --------------------------------------------------------
Solution &GlobalSolution::getSolution(double freqIn)
{
    //Gets the Solution object for the specified frequency.
    int freqIndex;      //The index of the specified frequency in the list of frequencies.

    //Search for the index of the frequency specified.
    for (unsigned int i = 0; i < plistFreq->size(); i++)
    {
        //Check if the frequency matches the specified freq in.
        if (plistFreq->at(i) == freqIn)
        {
            freqIndex = i;
            break;
        }
    }

    //Retrieve the solution object.
    return plistSolution->at(pCurBody).refSolution(*pCurWaveDir, freqIndex);
}

//==========================================Section Separator =========================================================
//Private Functions



