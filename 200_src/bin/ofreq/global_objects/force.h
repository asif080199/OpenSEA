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
#include "../global_objects/ofreqcore.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

//######################################### Class Separator ###########################################################
/**
 * This (base) class holds data for a force object.
 */

class Force : public oFreqCore
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
	void setForceName(string);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Retrieve the name of the force.
	 * @return newName The name of the force.
	 */
	string getForceName();

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
    string forceName; /**< The force name. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The index of the Force object as it exists in the list of other similar Force objects
     * contained under the System object.
     */
    int pSysIndex;

//==========================================Section Separator =========================================================
private:

};
#endif
