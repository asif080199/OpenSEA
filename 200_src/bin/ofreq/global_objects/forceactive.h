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
#ifndef FORCEACIVE_H
#define FORCEACTIVE_H
#include "force.h"
#include "equation.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
using namespace std;

typedef complex<double> complexDouble;

//######################################### Class Separator ###########################################################
/**
 * This class holds data for an active force.
 */

class ForceActive: public Force
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    ForceActive(); /**< The default constructor. */

    //------------------------------------------Function Separator ----------------------------------------------------
    virtual ~ForceActive(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the coefficient for the specified index.
     * @param coeffIn The value of the coefficient to specify.  Added as a complex number.  Variable passed by value.
     * @param index The equation index of the coefficient to specify.
     */
    void setCoeff(complex<double> coeffIn, unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Retrieve the list of coefficients.
	 * @return The list of coefficients.
	 */
    vector<complexDouble> &listCoefficients();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Another implementation of function listCoefficients.
     * @return Vector containing the list of coefficients.  Argument passed by reference.
     */
    vector<complexDouble> &listEquations();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Get a specific number from the list of coefficients.
     *
     * Get a specific number from the list of coefficients.  Similar to getCoefficients(), only instead of returning
     * the entire vector of coefficients, this only returns a single value in the list.
     * @param number Integer specifying which number should be retrieved from the list.
     * @return Complex Double which is the input coefficient for the active force on the equation specified by number.
     * Returns by value, not by reference.
     */
    complexDouble getEquation(int number);

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    vector<complexDouble> pCoefficients; /**< The list of force coeffients. */

//==========================================Section Separator =========================================================
private:

};
#endif
