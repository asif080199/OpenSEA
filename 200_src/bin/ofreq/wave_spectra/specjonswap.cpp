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
#include "specjonswap.h"

using namespace osea;

//==========================================Section Separator =========================================================
//Static Declarations

double SpecJONSWAP::constA = 172.75; /**< The constant associated with the A variable.*/
double SpecJONSWAP::constB = 691; /**< The constant associated with the B variable. */
double SpecJONSWAP::constParam = 0.658;   /**< The constant in the front of the JONSWAP wave spectrum. */
double SpecJONSWAP::gamITTC = 3.3;  /**< The ITTC Specified value for the wave bandwidth parameter, gamma. */
double SpecJONSWAP::GAM1 = 0.07; /**< The first value for gamma exponent in the JONSWAP spectrum.*/
double SpecJONSWAP::GAM2 = 0.09; /**< The second value for gamma exponent in the JONSWAP spectrum. */


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
SpecJONSWAP::SpecJONSWAP() : WaveSpecBase()
{
    //Set default value for spectrum bandwidth parameter.
    pGamma = gamITTC;
}

//------------------------------------------Function Separator --------------------------------------------------------
SpecJONSWAP::~SpecJONSWAP()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
double SpecJONSWAP::getSpecEnergy(double freq)
{
    //Variable declaration
    double A;       //The A expression in the equation.
    double B;       //The B expression in the equation.
    double C;       //The C expression in the equation.  THis creates the peakedness of the spectrum.
    double J;       //The J expression.  This is the exponent applied in the C parameter.
    double output;  //The calculated output.
    double gamexp;  //The value for the gamma exponent expression.

    //Calculate A constant.
    A = constA * pow(pHsig, 2) / pow(pTMean, 4);

    //Calculate B constant.
    B = constB / pow(pTMean, 4);

    //Calculate output, for Bretschneider spectrum
    output =  (A / pow(freq,5)) * exp(-B / pow(freq,4));

    //Calculate gamma exponent
    if (freq < 2 * PI / pTMean)
        gamexp = GAM1;
    else
        gamexp = GAM2;

    //Calculate J value
    J = exp( (-1 / (2 * pow(gamexp, 2))) * pow((freq * pTMean / (2 * PI)) - 1, 2) );

    //Calculate C value
    C = pow(pGamma, J);

    //Calculate output
    return constParam * C * output;
}

//------------------------------------------Function Separator --------------------------------------------------------
void SpecJONSWAP::setSigWaveHeight(double heightIn)
{
    pHsig = heightIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
double SpecJONSWAP::getSigWaveHeight()
{
    return pHsig;
}

//------------------------------------------Function Separator --------------------------------------------------------
void SpecJONSWAP::setTMean(double valIn)
{
    pTMean = valIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
double SpecJONSWAP::getTMean()
{
    return pTMean;
}

//------------------------------------------Function Separator --------------------------------------------------------
void SpecJONSWAP::setTPeak(double valIn)
{
    //convert from peak period to meanperiod.
    pTMean = valIn / 1.296;
}

//------------------------------------------Function Separator --------------------------------------------------------
void SpecJONSWAP::setTZero(double valIn)
{
    //Convert from zero period to mean period.
    pTMean = 1.0879629 * valIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void SpecJONSWAP::setSpecBandParam(double valIn)
{
    pGamma = valIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
double SpecJONSWAP::getSpecBandParam()
{
    return pGamma;
}

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions
