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

//==========================================Section Separator =========================================================
//Inclusions and namespaces
#include "specbretschneider.h"

using namespace osea;

//==========================================Section Separator =========================================================
//Static Declarations

double SpecBretschneider::constA = 172.75; /**< The constant associated with the A variable.*/
double SpecBretschneider::constB = 691; /**< The constant associated with the B variable. */


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
SpecBretschneider::SpecBretschneider() : WaveSpecBase()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
SpecBretschneider::~SpecBretschneider()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
double SpecBretschneider::getSpecEnergy(double freq)
{
    //Variable declaration
    double A;       //The A expression in the equation.
    double B;       //The B expression in the equation.

    //Calculate A constant.
    A = constA * pow(pHsig, 2) / pow(pTMean, 4);

    //Calculate B constant.
    B = constB / pow(pTMean, 4);

    //Calculate output
    return (A / pow(freq,5)) * exp(-B / pow(freq,4));
}

//------------------------------------------Function Separator --------------------------------------------------------
void SpecBretschneider::setSigWaveHeight(double heightIn)
{
    pHsig = heightIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
double SpecBretschneider::getSigWaveHeight()
{
    return pHsig;
}

//------------------------------------------Function Separator --------------------------------------------------------
void SpecBretschneider::setTMean(double valIn)
{
    pTMean = valIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
double SpecBretschneider::getTMean()
{
    return pTMean;
}

//------------------------------------------Function Separator --------------------------------------------------------
void SpecBretschneider::setTPeak(double valIn)
{
    //convert from peak period to meanperiod.
    pTMean = valIn / 1.296;
}

//------------------------------------------Function Separator --------------------------------------------------------
void SpecBretschneider::setTZero(double valIn)
{
    //Convert from zero period to mean period.
    pTMean = 1.0879629 * valIn;
}


//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions
