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
        dist[0] = fabs(valSearch - listVal.at(i));

        //Check if distance is smaller
        if (dist[0] < dist[1])
        {
            dist[1] = dist[0];
            output.at(0) = i;
        }
    }

    //Reset distance values
    dist[0] = 1E9;
    dist[1] = 1E9;

    //Iterate through and find the 2nd closest match
    for (int i = 0; i < listVal.size(); i++)
    {
        //Calculate distance
        dist[0] = fabs(valSearch - listVal.at(i));

        //Check if distance is smaller and not the other index.
        if ((dist[0] < dist[1]) &&
                (i != output.at(0)))
        {
            dist[1] = dist[0];
            output.at(1) = i;
        }
    }

    //Write out values
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
double mathInterp::checkAngle(double angIn)
{
    try
    {
        //Check if direction is less than zero.
        if (angIn < 0)
        {
            angIn += 2*PI;
        }

        if (angIn > 2*PI)
        {
            angIn -= 2*PI;
        }

        //Check if that solved the problem.
        if ((angIn > 2*PI)
                || (angIn < 0))
            throw std::runtime_error("Specified wave direction outside of valid range from 0 to 6.2832 rad.  Perhaps you have the wrong units?");
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));  
    }
}


//==========================================Section Separator =========================================================
//Private Functions

