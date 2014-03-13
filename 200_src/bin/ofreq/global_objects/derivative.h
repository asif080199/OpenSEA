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

//######################################### Class Separator ###########################################################
//Namespace declarations

//==========================================Section Separator =========================================================
/**
 * The namespace for all code created under the OpenSEA project.  There are also several sub-namespaces, one
 * associated with each primary program under osea.
 * 1.)  ohydro:  Code associated with the program ohydro.
 * 2.)  ofreq:   Code associated with the program ofreq.
 * 3.)  otime:   Code associated with the program otime.
 * 4.)  ofourier:  Code associated with the program ofourier.
 * 5.)  obatch:    Code associated with the program obatch.
 * 6.)  guisea:    Code assocaited with the GUI that interacts with all OpenSEA programs.
 * Any code that may have common utility amongst all programs, such as file reading objects, goes under the generic
 * osea namespace.  Any code that is only useful within the specific program it serves, goes under the specific
 * namespace.  When in doubt, default to just the osea namespace.
 *
 * The namespaces are not intended to create an organizational structure.  They are only intended to prevent
 * name conflicts.
 */
namespace osea
{

//==========================================Section Separator =========================================================
/**
 * The namespace of all code specifically associated with ofreq.
 */
namespace ofreq
{

//######################################### Class Separator ###########################################################
/**
 * This class holds data for a derivative.
 */

class Derivative : public osea::ofreq::oFreqCore
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
     * @return Returns a vector of Equation objects.  Returned value passed by reference.
     */
    std::vector<Equation> &listEquation();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Retrieve the equation specified by the number.
     *
     * Retrieves the equation specified by the number.  Value returned is a reference to the equation object.  Allows
     * editting of the equation object, or just data access.
     * @param number Integer representing which equation number should be returned.
     * @return Value returned is a reference to the equation object.  Allows
     * editting of the equation object, or just data access.
     */
    Equation &listEquation(unsigned int number);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the equation requested.  Only specified by the data index property of the equation object.
     *
     * Returns the equation requested.  Only specified by the data index property of the equation object.
     * @param indexIn The integer describing the data index for the equation requested.
     * @return Equation object specified by the DataIndex of indexIn.  Value returned is by value.
     */
    Equation &listDataEquation(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the list of equation objects.
     *
     * This is the same as the listEquation() function, just under a different name.
     * @return Returns a vector of Equation objects.  Returned value passed by reference.
     */
    std::vector<Equation> &listDataEquation();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Retrieve the size of the equation list.
	 * @return The size of the equation list.
	 */
	int getEquationListSize();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Creates a new equation object and adds it to the list of equation objects contained in this derivative.
     *
     * New equation object is created automatically within this function.  Function merely takes the list of input
     * coefficients and creates all equation objects necessary from that.
     * @param EqnDataIn Integer.  The data index of the equation object.  If no input is provided, the function
     * assumes the data index to be the index of the equation's current place in the vector.
     * @param listCoeffsIn Vector of doubles.  The list of coefficients.  Each coefficient corresponds to a single
     * variable.  List of coefficients is organized by _data index_.  The coefficient's position in the list
     * is it's data index.
     */
    void addModelEquation(std::vector<double> listCoeffsIn, int EqnDataIn = -1);

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    std::vector<Equation> pEquationList; /**< The list of equations. */

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

}   //Namespace ofreq
}   //Namespace osea

#endif

