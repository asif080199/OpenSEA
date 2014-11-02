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

#include "equation.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
Equation::Equation()
{
	initCoeff();

    //Initialize default value of the data index
    pDataIndex = -1;
}

//------------------------------------------Function Separator --------------------------------------------------------
Equation::Equation(int IndexIn)
{
    //Initialize with Data Index.
    Equation();
    pDataIndex = IndexIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
Equation::~Equation()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &Equation::listCoefficient()
{
    //Return pointer to the list of coefficients.
    return pcoeffs;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &Equation::listCoefficient(unsigned int index)
{
    return pcoeffs.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<double> &Equation::listDataVariable()
{
    return pcoeffs;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &Equation::listDataVariable(int DataIndex)
{
    //Find the position of the coefficient with the specified data index
    int i;

    try
    {
        i = findIndex(DataIndex);
    }
    catch (int err)
    {
        //Assume no match found and create a new place for one.
        pcoeffs.push_back(0.0);
        plistDataIndex.push_back(DataIndex);
        i = pcoeffs.size() - 1;
    }
    catch(...)
    {
        //Assume no match found and create a new place for one.
        pcoeffs.push_back(0.0);
        plistDataIndex.push_back(DataIndex);
        i = pcoeffs.size() - 1;
    }

    //Return the value
    return pcoeffs.at(i);
}

//------------------------------------------Function Separator --------------------------------------------------------
double Equation::getCoefficient(int number)
{
    //Return the coefficient at the specified number.
    return pcoeffs.at(number);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Equation::setCoefficient(int number, double coeffIn)
{
    //Check for out of bounds error
    if ((pcoeffs.size() < number) || (pcoeffs.size() == 0))
    {
        pcoeffs.resize(number + 1);
    }

    //Set the coefficient at the specified number
    pcoeffs.at(number) = coeffIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
unsigned int Equation::getCoefficientListSize()
{
    return pcoeffs.size();
}

//------------------------------------------Function Separator --------------------------------------------------------
void Equation::setDataIndex(int index)
{
    pDataIndex = index;
}

//------------------------------------------Function Separator --------------------------------------------------------
int Equation::getDataIndex()
{
    return pDataIndex;
}

//------------------------------------------Function Separator --------------------------------------------------------
int &Equation::refDataIndex()
{
    return pDataIndex;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Equation::addVariable(double CoeffIn, int VarDataIn)
{
    //Create a new place for the coefficient in the vector of coefficients.
    //Add in the new coefficient.
    pcoeffs.push_back(CoeffIn);

    //Check for default value of VarDataIn
    if (VarDataIn == -1)
    {
        //Assign default value.
        VarDataIn = plistDataIndex.size() - 1;
    }

    plistDataIndex.push_back(VarDataIn);
}



//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
void Equation::initCoeff()
{
    for(unsigned int i = 0; i < pcoeffs.size(); i++)
        pcoeffs.at(i) = 0;
}

//------------------------------------------Function Separator --------------------------------------------------------
int Equation::findIndex(int DataIndexIn)
{
    int output;         //integer to write as output.
    int check;          //The integer to check against.
    bool test = false;  //Boolean to tell if a match was found

    //Finds the integer of the equation object by data index.
    for (unsigned int i = 0; i < plistDataIndex.size(); i++)
    {
        //Check the data index of the object.
        if (plistDataIndex.at(i) < 0)
        {
            //No data index set.  Use the position in the list.
            check = i;
        }
        else
        {
            //Data index is used.  Use the position in the list.
            check = plistDataIndex.at(i);
        }

        //Check for match
        if (check == DataIndexIn)
        {
            output = i;
            test = true;
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
