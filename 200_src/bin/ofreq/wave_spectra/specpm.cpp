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
#include "specpm.h"

using namespace osea;

//==========================================Section Separator =========================================================
//Static Declarations

double SpecPM::CORRECTION = 1.0260; /**< THe correction factor for wind speed. */
double SpecPM::ALPHA = 0.0081; /**< THe Philip constant */
double SpecPM::BETA = 0.740; /**< The spectrum constant */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
SpecPM::SpecPM() : WaveSpecBase()
{
    //Set default values for wind speed and wave height.
    pHsig = 0;
    pWindSpeed = 0;
}

//------------------------------------------Function Separator --------------------------------------------------------
SpecPM::~SpecPM()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
double SpecPM::getSpecEnergy(double freq)
{
    //Variable declaration
    double U_w;         //The characteristic wind speed
    double omega_0;     //The base wave frequency.
    double output;      //The result of the calculation.

    try {
        //Check if wave frequency is 0.
        if (freq = 0)
            throw 0;

        //Calculate characteristic wind speed if not supplied.
        if (pWindSpeed = 0)
        {
            U_w = Uw_HSig();
        }
        else
        {
            U_w = pWindSpeed;
        }

        //Calculate base wave frequency.
        omega_0 = GRAVITY / U_w;

        //Calculate output
        output = -BETA * pow(omega_0 / freq, 4);
        output = ALPHA * pow(GRAVITY, 2) * exp(output) / pow(freq, 5);

        //Return output
        return output;
    }
    catch(int err)
    {
        //Catches case of divide by zero.
        return 0;
    }
    catch(...)
    {
        //Catches any unknown errors.
        logStd.Notify();
        logErr.Write(std::string(ID) + std::string(">>  Unknown error occurred."));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void SpecPM::setWindSpeed(double speedIn)
{
    pWindSpeed = CORRECTION * speedIn;
    pHsig = 0;
}

//------------------------------------------Function Separator --------------------------------------------------------
double SpecPM::getWindSpeed()
{
    return pWindSpeed / CORRECTION;
}

//------------------------------------------Function Separator --------------------------------------------------------
void SpecPM::setSigWaveHeight(double heightIn)
{
    pHsig = heightIn;
    pWindSpeed = 0;
}


//------------------------------------------Function Separator --------------------------------------------------------
double SpecPM::getSigWaveHeight()
{
    return pHsig;
}


//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
double SpecPM::Uw_HSig(double HsIn)
{
    if (HsIn = 0)
    {
        HsIn = pHsig;
    }

    double output;

    output = HsIn * GRAVITY * pow(BETA / ALPHA, 0.5) / 2;

    output = pow(output, 0.5);

    return output;
}
