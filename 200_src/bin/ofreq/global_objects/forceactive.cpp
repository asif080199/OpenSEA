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

#include "forceactive.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
ForceActive::ForceActive(): Force()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceActive::~ForceActive()
{
    //Clear vector of coefficients.
    pCoefficients.clear();
    pDataIndex.clear();
}

//------------------------------------------Function Separator --------------------------------------------------------
void ForceActive::setCoeff(complex<double> coeffIn, unsigned int index)
{
    //Check if list is large enough for specified index.
    if ((index > pCoefficients.size() - 1) || (pCoefficients.size() == 0))
    {
        //Resize list
        pCoefficients.resize(index + 1);
    }

    //Assign coefficient
    pCoefficients[index] = coeffIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void ForceActive::addEquation(std::complex<double> eqIn, int dataIndex)
{
    //Add coefficient to the list of coefficients.
    pCoefficients.push_back(eqIn);

    //Check if no data index was specified.
    if (dataIndex < 0)
    {
        //No data index specified.  Use default.  Default is just the object occurrance index.
        dataIndex = pCoefficients.size() - 1;
    }

    //Add data index to the list of data indices.
    pDataIndex.push_back(dataIndex);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<complexDouble> &ForceActive::listCoefficient()
{
    return pCoefficients;
}

//------------------------------------------Function Separator --------------------------------------------------------
complexDouble &ForceActive::listCoefficient(unsigned int index)
{
    return pCoefficients[index];
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<complexDouble> &ForceActive::listDataEquation()
{
    return this->listCoefficient();
}

//------------------------------------------Function Separator --------------------------------------------------------
complexDouble &ForceActive::listDataEquation(int index)
{
    //Get the vector occurrence index of the data index specified.
    int i;
    try
    {
        i = findIndex(index);
    }
    catch(int err)
    {
        //If an error, assume that there was no match.  Create a new place in the vector.
        pCoefficients.push_back(complex<double>(0,0));
        pDataIndex.push_back(index);
        i = pCoefficients.size() - 1;
    }
    catch(...)
    {
        //If an error, assume that there was no match.  Create a new place in the vector.
        pCoefficients.push_back(complex<double>(0,0));
        pDataIndex.push_back(index);
        i = pCoefficients.size() - 1;
    }

    //Return coefficient.
    return pCoefficients[i];
}

//------------------------------------------Function Separator --------------------------------------------------------
complexDouble ForceActive::getEquation(int number)
{
    //Return the input coefficient for the equation specified.
    return pCoefficients[number];
}

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
int ForceActive::findIndex(int indexIn)
{
    int output;         //integer to write as output.
    int check;          //The integer to check against.
    bool test = false;  //Boolean to check if a match was found.

    //Finds the integer of the equation object by data index.
    for (unsigned int i = 0 ; i < pDataIndex.size() ; i++)
    {
        //Check the data index of the object.
        if (pDataIndex[i] < 0)
        {
            //No data index set.  Use the position in the list.
            check = i;
        }
        else
        {
            //Data index is used.  Use the position in the list.
            check = pDataIndex[i];
        }

        //Check for match
        if (check == indexIn)
        {
            output = i;
            test = true;    //Note that a match was found.
            break;
        }
    }

    //Check for a match
    if (!test)
    {
        //No match.  Throw an exception
        throw 1;
    }
    return output;
}
