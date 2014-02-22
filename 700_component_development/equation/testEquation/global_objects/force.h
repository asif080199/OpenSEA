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
#ifndef FORCE_H
#define FORCE_H
#include "equation.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

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
 * This (base) class holds data for a force object.
 */

class Force
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
	Force(); /**< The default constructor. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Overloaded constructor for Force object.  Allows the System object to pass the index of the newly created
     * Force object in the list of other similar Force objects.  Used during object creation for proper association
     * with Body object.
     * @param indexIn Integer.  The index of the Force object as it exists in the list of other similar Force objects
     * contained under the System object.  Variable passed by value.
     * @sa dictBodies
     * @sa System
     * @sa Body
     */
    Force(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    virtual ~Force(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the name of the force.
	 * @param newName The name of the force.
	 */
	void setForceName(std::string);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Retrieve the name of the force.
	 * @return newName The name of the force.
	 */
	std::string getForceName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the system index.  The index of the Force object as it exists in the list of other similar Force objects
     * contained under the System object.
     * @param indexIn Integer.  The index of the Force object as it exists in the list of other similar Force objects
     * contained under the System object.  Variable passed by value.
     */
    void setSystemIndex(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the system index.  The index of the Force object as it exists in the list of other similar
     * Force objects contained under the System object.
     * @return  Integer.  The index of the Force object as it exists in the list of other similar
     * Force objects contained under the System object.  Variable passed by value.
     */
    int getSystemIndex();

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    std::string forceName; /**< The force name. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The index of the Force object as it exists in the list of other similar Force objects
     * contained under the System object.
     */
    int pSysIndex;

//==========================================Section Separator =========================================================
private:

};

}   //Namespace ofreq
}   //Namespace osea

#endif
