/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date          Author				Description
 *---------------------------------------------------------------------------------------------------------------------
 *May 15, 2013  Shane Honanie       Initially created.
 *Aug 04, 2013  Nicholas Barczak    Removed limits on number of equations in class.
 *
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



//######################################### Class Separator ###########################################################
#ifndef FORCEDERIVATIVE_H
#define FORCEDERIVATIVE_H
#include "equation.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "../global_objects/ofreqcore.h"
using namespace std;

//######################################### Class Separator ###########################################################
/**
 * This class holds data for a derivative.
 */

class Derivative : public oFreqCore
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
	Derivative(); /**< This default constructor creates a Body object. */

    //------------------------------------------Function Separator ----------------------------------------------------
	~Derivative(); /**< The default destructor, nothing happens here. */

   //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of equations.
     *
     * Pointer to the list vector list of equations.  Value returned by reference.
     */
    vector<Equation> &listEquations();
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the equation requested.  Only specified by the data index property of the equation object.
     *
     * Returns the equation requested.  Only specified by the data index property of the equation object.
     * @param indexIn The integer describing the data index for the equation requested.
     * @return Pointer to the Equation object specified by the DataIndex of indexIn.  Value returned is by reference.
     */
    Equation &refIndexEquation(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the equation requested.  Only specified by the data index property of the equation object.
     *
     * Returns the equation requested.  Only specified by the data index property of the equation object.
     * @param indexIn The integer describing the data index for the equation requested.
     * @return Equation object specified by the DataIndex of indexIn.  Value returned is by value.
     */
    Equation getIndexEquation(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Retrieve the equation specified by the number.
     *
     * Retrieves the equation specified by the number.  Value returned is a pointer to the equation object.  Allows
     * editting of the equation object, or just data access.
     * @param number Integer representing which equation number should be returned.
     * @return Value returned is a pointer to the equation object.  Allows
     * editting of the equation object, or just data access.
     */
    Equation &refEquation(unsigned int number);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Retrieve the size of the equation list.
	 * @return The size of the equation list.
	 */
	int getEquationListSize();

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    vector<Equation> pEquationList; /**< The list of equations. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Searches through the list of equation objects to find the one specified by the indexIn parameter.
     *
     * Searches through the list of equation objects to find the one specified by the indexIn parameter.  Returns the
     * integer specifying the position of the object in the vector of equation objects.
     * @param indexIn The data index requested.
     * @return Returns the integer specifying the position of the object in the vector of equation objects.
     */
    int findIndex(int indexIn);

};
#endif

