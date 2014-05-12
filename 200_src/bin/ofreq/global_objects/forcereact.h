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
 *May 7, 2014       Nicholas Barczak    Debugged and updated to Rev 1.0
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
#ifndef FORCEREACTIVE_H
#define FORCEREACTIVE_H
#include "force.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "derivative.h"

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
 * This class holds all of the data for a reactive force.
 */

class ForceReact: public Force
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    ForceReact(); /**< This default constructor. */

    //------------------------------------------Function Separator ----------------------------------------------------
    ~ForceReact(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the current derivative.
	 * @param neworder The order of derivative.
	 */
	void setCurDerivative(int);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the current number of the equation.
	 * @param newEquationNum The number of the equation.
	 */
	void setCurEquationNum(int);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Another implementation of getDerivatives, under a different name.
     * @return Returns the vector of derviative objects.  Returned object is by reference.
     */
    std::vector<Derivative> &listDerivative();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Retrieve the derivative object specified by the index number.
     *
     * Retrieve the derivative object specified by the index number.
     * @param num The index number of the derivative object.
     * @return Returns the derivative object specified by integer num.  Returned value is by value.
     */
    Derivative getDerivative(unsigned int num);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Retrieve the derivative object specified by the index number.
     *
     * Retrieve the derivative object specified by the index number.  Retrieves a pointer to the derivative object.
     * @param num The index number of the derivative object.
     * @return Returns a pointer to the derivative object specified by integer num.  Returned value is by reference.
     */
    Derivative &listDerivative(unsigned int num);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a Derivative object to the list of derivatives.  Creates a blank derivative object.  Assumed to be
     * the latest order of derivative in the list.
     */
    void addDerivative();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a Derivative object in the list of derivatives.  Sets the new objects as the input for the new
     * derivative object.  Uses the properties of the Derivative object to set the correct index.
     * @param derivIn The derivative object to add to the list of derivatives.
     * @param ordIn The order of the derivative.
     *
     */
    void addDerivative(Derivative derivIn, unsigned int ordIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the maximum order of the derivatives included in the force object.
     *
     * Returns the maximum order of the derivatives included in the force object.
     * @return Returns integer.  Returns the maximum order of the derivatives included in the force object.  Returned
     * result passed by value.
     */
    int getMaxOrd();

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    std::vector<Derivative> pDerivative; /**< This list of derivatives. */
	int currentDerivative; /**< The current order derivative. */
	int currentEquation; /**< This current equation number. */

//==========================================Section Separator =========================================================
private:

};

}   //Namespace ofreq
}   //Namespace osea

#endif

