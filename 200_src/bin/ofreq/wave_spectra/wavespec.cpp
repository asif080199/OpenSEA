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
#include "wavespec.h"

using namespace osea;
using namespace alglib;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
WaveSpec::WaveSpec() : WaveSpecBase()
{
    SplineReady = false;
}

//------------------------------------------Function Separator --------------------------------------------------------
WaveSpec::~WaveSpec()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<double> &WaveSpec::listWaveFreq()
{
    return pWaveFreq;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &WaveSpec::listWaveFreq(int index)
{
    return pWaveFreq.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
void WaveSpec::setWaveFreq(std::vector<double> freqIn)
{
    pWaveFreq = freqIn;

    SplineReady = false;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<double> &WaveSpec::listWaveEnergy()
{
    return pWaveEnergy;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &WaveSpec::listWaveEnergy(int index)
{
    return pWaveEnergy.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
void WaveSpec::setWaveEnergy(std::vector<double> energyIn)
{
    pWaveEnergy = energyIn;

    SplineReady = false;
}

//------------------------------------------Function Separator --------------------------------------------------------
double WaveSpec::getSpecEnergy(double freq)
{
    //First determine if the data is only a single point.
    if (pWaveFreq.size() == 1)
    {
        //Only one data point.  Just return that.
        return pWaveEnergy.at(0);
    }
    else
    {
        //More than one data point.  Setup interpolation.

        //Check if need to build the interpolation spline.
        if (!SplineReady)
        {
            //Build interpolation spline.
            ConstructSpline(freq);
        }

        //Interpolate
        return alglib::spline1dcalc(pSpecCurve, freq);
    }
}


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
void WaveSpec::ConstructSpline(double freqIn)
{
    //First find the index of the point closest to the requested frequency.
    //Assumes the vectors are sequentially ordered.
    double dist[2];     //Distances from frequency in vector to freqIn.
    int index;          //The index of the frequency with the closest match.
    real_1d_array x;    //x values for spline interpolation.
    real_1d_array y;    //y values for spline interpolation.

    //Put data into alglib required data type.
    x.setcontent(pWaveFreq.size(), pWaveFreq.data());
    y.setcontent(pWaveEnergy.size(), pWaveEnergy.data());

    dist[0] = 1E6;
    dist[1] = 1E6;

    for (int i = 0; i < pWaveFreq.size(); i++)
    {
        //Iterate through and find the index of the frequency with the closest match.
        dist[0] = abs(freqIn - pWaveFreq.at(i));

        if (dist[0] < dist[1])
        {
            index = i;
            dist[1] = dist[0];
        }
    }

    //Select the correct function to build the spline.  Assumes the data has at least two data points.
    if (pWaveFreq.size() == 2)
    {
        //Build linear spline.
        alglib::spline1dbuildlinear(x, y, pSpecCurve);
    }

    else if ((pWaveFreq.size() >= 5) &&
             (index >= 2) &&
             ((pWaveFreq.size() - 1 - index) >= 3))
    {
        //Check for Akima spline.
        /* Requires 5 or more data points, and the points must be arranged in the correct order with at
         * least two data points below the requested frequency, and three data points above the requested frequency.
         */

        //Build Akima spline.
        alglib::spline1dbuildakima(x, y, pSpecCurve);
    }

    else if (pWaveFreq.size() >= 3)
    {
        //Check for cubic spline.

        //Build Hermite spline.
        alglib::spline1dbuildcubic(x, y, pSpecCurve);
    }

    //Notify that the curve is now built.
    SplineReady = true;
}


//==========================================Section Separator =========================================================
//Private Functions

