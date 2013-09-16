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
#ifndef DICTCONTROL_H
#define DICTCONTROL_H
#include <vector>
#include <string>
#include "dictionary.h"

//######################################### Class Separator ###########################################################
//Put constant variables in this section.

//==========================================Section Separator =========================================================
//Class Names
const string KEY_SYSTEM = "system"; /**< Keyword for system object.*/

//==========================================Section Separator =========================================================
//Keyword Value Pairs
const string KEY_ANALYSIS = "analysis"; /**< Keyword for analysis specification */
const string KEY_FREQUENCY = "frequency"; /**< Keyword for frequency specification */
const string KEY_DIRECTION = "direction"; /**< Keyword for direction list specification. */
const string KEY_WAVEMODEL = "wavemodel"; /**< Keyword for wave model specification. */

//######################################### Class Separator ###########################################################
//Put this at the beginning of each class definition, right before the Doxygen formatted explanation.
//Start doxygen documentation like this:
/**
 * The dictControl class defines the key-word value pairs associated with the Control.in input file.  Just as a normal
 * dictionary defines the meaning of words, the dictControl class works in the same way.  The dictControl class takes
 * individual pairs of keywords and values.  It has a definition for each of these keywords.  The definition is
 * whatever actions are necessary to process the value of key-pair and apply it to the program.  This may include
 * variable type conversions.  It will also use slots and signals to retrieve pointers to any appropriate objects that
 * the dictControl object needs to interact with.  It will use the properties of those objects to apply the values
 * it finds in the key-value pair.  Any objects created in the dictControl object can be safely deleted once all file
 * reading is done.
 * @sa Dictionary
 * @sa FileReader
 */

class dictControl: public Dictionary
{
    Q_OBJECT

//==========================================Section Separator =========================================================
public:
    dictControl();
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
};
#endif // DICTCONTROL_H
