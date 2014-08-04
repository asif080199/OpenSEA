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

#include "mathinterp.h"

using namespace osea::ofreq;
using namespace std;

//==========================================Section Separator =========================================================
//Static initialization


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
mathInterp::mathInterp()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
mathInterp::~mathInterp()
{

}


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
T mathInterp::iePolate(double x, double x1, double x2, T y1, T y2)
{
    //Calculate ratio
    try {
        //Check for division by zero
        if (x2 == x1)
            throw std::overflow_error("Divide by zero");

        double p = (x - x1) / (x2 - x1);

        //Calculate output and write out
        return p * (y2 - y1) + y1;
    }
    catch(std::overflow_error err)
    {
        logStd.Notify();
        logErr.Write(string("function:  iePolate") + err.what());
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
T mathInterp::iPolate(double x, double x1, double x2, T y1, T y2)
{
    //Calculate ratio
    try {
        //Check for division by zero
        if (x2 == x1)
            throw std::overflow_error("Divide by zero");

        double p = (x - x1) / (x2 - x1);

        //Check for a few cases
        if (p < 0.0)
        {
            //Under lower bound.  Return y1
            return y1;
        }
        else if(p > 1.0)
        {
            //Over upper bound.  Return y2
            return y2;
        }
        else
        {
            //Somewhere in the middle, as it should be.  Perform interpolation.

            //Calculate output and write out
            return p * (y2 - y1) + y1;
        }
    }
    catch(std::overflow_error err)
    {
        logStd.Notify();
        logErr.Write(string("function:  iePolate") + err.what());
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<int> mathInterp::FindMatch(double valSearch, vector<double> &listVal)
{
    double dist[2];         //The distance between the search term and terms considered.
    vector<int> output(2);      //The vector of output terms.

    //Set initial values
    dist[0] = 1E9;
    dist[1] = 1E9;

    //Iterate through data set and find the closest match
    for (int i = 0; i < listVal.size(); i++)
    {
        //Calculate distance
        dist[0] = abs(valSearch - listVal[i]);

        //Check if distance is smaller
        if (dist[0] < dist[1])
        {
            dist[1] = dist[0];
            output[0] = i;
        }
    }

    //Reset distance values
    dist[0] = 1E9;
    dist[1] = 1E9;

    //Iterate through and find the 2nd closest match
    for (int i = 0; i < listVal.size(); i++)
    {
        //Calculate distance
        dist[0] = abs(valSearch - listVal[i]);

        //Check if distance is smaller and not the other index.
        if ((dist[0] < dist[1]) &&
                (i != output[0]))
        {
            dist[1] = dist[0];
            output[1] = i;
        }
    }

    //Write out values
    return output;
}


//==========================================Section Separator =========================================================
//Private Functions

