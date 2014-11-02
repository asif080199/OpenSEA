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

#include "derivative.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Public functions:

//------------------------------------------Function Separator --------------------------------------------------------
Derivative::Derivative()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
Derivative::~Derivative()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
vector<Equation> &Derivative::listEquation()
{
    //Return equation list pointer.
    return pEquationList;
}

//------------------------------------------Function Separator --------------------------------------------------------
Equation &Derivative::listEquation(unsigned int number)
{
    //Check to see if the equation is out of bounds.  If so, resize vector.
    if ((number > pEquationList.size() - 1) || (pEquationList.size() == 0))
    {
        pEquationList.resize(number + 1);
    }

    //Return pointer to the specified equation.
    return pEquationList.at(number);
}

//------------------------------------------Function Separator --------------------------------------------------------
Equation &Derivative::listDataEquation(int indexIn)
{
    //Get equation by data index, returns value.
    int i;

    //Find the index integer.
    try
    {
        i = findIndex(indexIn);
    }
    catch (int err)
    {
        //Assume no data index found.  Create a new place in the vector for the data index.
        pEquationList.push_back(Equation());
        i = pEquationList.size() - 1;
        pEquationList.at(i).setDataIndex(indexIn);
    }
    catch(...)
    {
        //Assume no data index found.  Create a new place in the vector for the data index.
        pEquationList.push_back(Equation());
        i = pEquationList.size() - 1;
        pEquationList.at(i).setDataIndex(indexIn);
    }

    //Write output
    return pEquationList.at(i);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<Equation> &Derivative::listDataEquation()
{
    //return the equation list
    return pEquationList;
}

//------------------------------------------Function Separator --------------------------------------------------------
int Derivative::getEquationListSize()
{
    return pEquationList.size();
}

//------------------------------------------Function Separator --------------------------------------------------------
void Derivative::addModelEquation(std::vector<double> listCoeffsIn, int EqnDataIn)
{
    //Adds a new equation object to the list for the current derivative.
    Equation newEqn;

    //Set data index for new equation.
    if (EqnDataIn < 0)
    {
        //No data index assigned.  Create a default.  Default is the last item in the list.
        //List will be expanded by 1 when I add the new equation object in.  So I take the size of the list.
        EqnDataIn = pEquationList.size();
    }
    newEqn.setDataIndex(EqnDataIn);

    //Iterate through and for each object in the list of coefficients, add to the equation model.
    //Only add the object if it is non zero
    for (unsigned int i = 0; i < listCoeffsIn.size(); i++)
    {
        if (listCoeffsIn.at(i) != 0.0)
        {
            //Add each coefficient to the list of coefficients for the equation object.
            newEqn.addVariable(listCoeffsIn.at(i), i);
        }
    }

    //Finally add the new equation object to the list of equation objects.
    pEquationList.push_back(newEqn);
}

//==========================================Section Separator =========================================================
//Protected functions:

//==========================================Section Separator =========================================================
//Private functions:

//------------------------------------------Function Separator --------------------------------------------------------
int Derivative::findIndex(int indexIn)
{
    int output;         //integer to write as output.
    int check;          //The integer to check against.
    bool test = false;  //Boolean to check if a match was found.

    //Finds the integer of the equation object by data index.
    for (unsigned int i = 0; i < pEquationList.size(); i++)
    {
        //Check the data index of the object.
        if (pEquationList.at(i).getDataIndex() < 0)
        {
            //No data index set.  Use the position in the list.
            check = i;
        }
        else
        {
            //Data index is used.  Use the position in the list.
            check = pEquationList.at(i).getDataIndex();
        }

        //Check for match
        if (check == indexIn)
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
