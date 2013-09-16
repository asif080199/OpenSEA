/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date          Author                  Description
 *---------------------------------------------------------------------------------------------------------------------
 *Sep 15 2013	Nicholas Barczak		Initially Created
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
//Put header file includes and constant variables in here.
#ifndef DICTFORCES_H
#define DICTFORCES_H
#include <vector>
#include <string>
#include "dictionary.h"
//#include "../motion_solver/forceactive.h"
//#include "../motion_solver/forcecross.h"
//#include "../motion_solver/forcereact.h"
//#include "../motion_solver/derivative.h"
//#include "../motion_solver/equation.h"

//######################################### Class Separator ###########################################################
//Prototype class defitions

//######################################### Class Separator ###########################################################
//Put constant variables in this section.

//==========================================Section Separator =========================================================
//Class Names
const string KEY_FORCE_ACTIVE = "force_active"; /**< Keyword for force_active definition.*/
const string KEY_FORCE_REACT = "force_reactive"; /**< Keyword for force_react class definition.*/
const string KEY_FORCE_CROSS = "force_crossbody"; /**< Keyword for force_cross class definition.*/
const string KEY_DERIVATIVE = "derivative"; /**< Keyword for derivative class definition. */
const string KEY_EQUATION = "equation"; /**< Keyword for equation designation.*/

//==========================================Section Separator =========================================================
//Keyword Names
const string KEY_NAME = "name"; /**< Keyword for the name the user assigns for a force.*/
const string KEY_COEFF = "coeff"; /**< Keyword for coefficient designation.*/
const string KEY_NUMBER = "number"; /**< Keyword for equation number designation. */
const string KEY_ORDER = "order"; /**< Keyword for order of derivative designation. */
const string KEY_FORCE = "force"; /**< Keyword for force coefficients designation.*/

//######################################### Class Separator ###########################################################
//Put this at the beginning of each class definition, right before the Doxygen formatted explanation.
//Start doxygen documentation like this:
/**
 * The dictForces class defines the key-word value pairs associated with the Forces.in input file.  Just as a normal
 * dictionary defines the meaning of words, the dictForces class works in the same way.  The dictForces class takes
 * individual pairs of keywords and values.  It has a definition for each of these keywords.  The definition is
 * whatever actions are necessary to process the value of key-pair and apply it to the program.  This may include
 * variable type conversions.  It will also use slots and signals to retrieve pointers to any appropriate objects that
 * the dictForces object needs to interact with.  It will use the properties of those objects to apply the values
 * it finds in the key-value pair.  Any objects created in the dictForces class can be safely deleted once all file
 * reading is done.
 *
 * Note:  The code for the dictForces object always references the last object in the list.  This assumes that no
 * other commands get issued in the input file between the creation of an object and the definition of key-value
 * pairs associated with that object.  Currently, I can not imagine any situation where this assumption would be
 * violated.  But do consider this when planning error recovery methods.
 * @sa Dictionary
 * @sa FileReader
 */
class dictForces: public Dictionary
{
    Q_OBJECT

//==========================================Section Separator =========================================================
public:
    dictForces();

//==========================================Section Separator =========================================================
signals:

//==========================================Section Separator =========================================================
public slots:

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pure virtual function that defines how to interpret the key values.  Contains a list of key names and
     * corresponding actions to take for interpretting each key value.
     * @param keyIn String containing the key name.  Variable passed by value.
     * @param valIn Vector of strings containing the key values.  Variable passed by value.
     * @return Returns 0 if definition found.  Returns 1 if no definition found.
     */
    int defineKey(string keyIn, vector<string> valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pure virtual function that defines how to interpret the class name.  Class name implies declaration of
     * a new object of the class named by the class name.  This is a separate set of definitions to handle class
     * declarations.
     * @param nameIn String, variable passed by value.  The name of the class name.
     * @return Returns 0 if definition found.  Returns 1 if no definition found.
     */
    int defineClass(string nameIn);

//==========================================Section Separator =========================================================
private:  
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Integer variable.  Can take on an integer from 1 to 3.  This is how the dictionary remembers which type
     * of force object was created.  Valid values are:
     * 0:  Variable not set
     * 1:  forceActive_user
     * 2:  forceReact_user
     * 3:  forceCross_user
     */
    int pForceType;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Index of last force currently being used.
     * Any value less than zero means variable not set.
     */
    int pForceIndex;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Order of the derivative object with properties currently assigned.
     * Any value less than 0 means variable not set.
     */
    int pOrd;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Integer which specified the current equation to add properties for.
     */
    int pEqn;

    //------------------------------------------Function Separator ----------------------------------------------------
    void setEquation();

    //------------------------------------------Function Separator ----------------------------------------------------
    void setDerivative();
};

#endif // DICTFORCES_H
