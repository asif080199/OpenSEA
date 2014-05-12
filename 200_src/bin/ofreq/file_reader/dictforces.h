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
//Put header file includes and constant variables in here.
#ifndef DICTFORCES_H
#define DICTFORCES_H
#include <vector>
#include <string>
#include "dictionary.h"

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
//Prototype class defitions

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
     * @brief Function that defines how to interpret the key values.  Contains a list of key names and
     * corresponding actions to take for interpretting each key value.
     * @param keyIn std::string containing the key name.  Variable passed by value.
     * @param valIn Vector of strings containing the key values.  Variable passed by value.
     * @return Returns status of assigning key.  Returned value is an integer, passed by value.
     * See list of return codes below:
     * 0:  Key definition found.  Success.
     * 1:  No key found. / General error message.
     * 2:  Key is invalid within current active object.
     * 99: Function virtual definition only.  Not currently defined.
     */
    int defineKey(std::string keyIn, std::vector<std::string> valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Function that defines how to interpret the class name.  Class name implies declaration of
     * a new object of the class named by the class name.  This is a separate set of definitions to handle class
     * declarations.
     * @param nameIn std::string, variable passed by value.  The name of the class name.
     * @return Returns status of assigning key.  Returned value is an integer, passed by value.
     * See list of return codes below:
     * 0:  Key definition found.  Success.
     * 1:  No key found. / General error message.
     * 2:  Key is invalid within current active object.
     * 99: Function virtual definition only.  Not currently defined.
     */
    int defineClass(std::string nameIn);

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
    /**
     * @brief This sets the integer variable pForceIndex to specify the current force index that object properties
     * will be assigned under.  If no input is provided, the function finds the index of the last equation + 1.
     * @param forceIn The input for the index of the force.  Default input is -1, which triggers the function to
     * find the index of the last force in the list + 1.  Variable is passed by value.
     */
    void setForceIndex(int forceIn = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief This sets the integer variable pEqn to specify the current equation that object properties
     * will be assigned under.  If no input is provided, the function finds the index of the last equation + 1.
     * @param eqIn The input for the index of the equation.  Default input is -1, which triggers the function to
     * find the index of the last equation in the list + 1.  Variable is passed by value.
     */
    void setEquation(int eqIn = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief This sets the integer variable pOrd to specify the order of the current derivative that objects will
     * be created under.  If no input is provided, the function finds the current highest order of derivative + 1.
     * @param ordIn The input for the order of the derivative.  Default input is -1, which triggers the function to
     * find the current highest order of the derivative + 1.  Variable is passed by value.
     */
    void setDerivative(int ordIn = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    //Class Names
    static std::string OBJECT_FORCE_ACTIVE; /**< Keyword for force_active definition.*/
    static std::string OBJECT_FORCE_REACT; /**< Keyword for force_react class definition.*/
    static std::string OBJECT_FORCE_CROSS; /**< Keyword for force_cross class definition.*/
    static std::string OBJECT_DERIVATIVE; /**< Keyword for derivative class definition. */
    static std::string OBJECT_EQUATION; /**< Keyword for equation designation.*/
    static std::string OBJECT_FORCE; /**< Keyword for force designation as part of force_active definition. */

    //------------------------------------------Function Separator ----------------------------------------------------
    //Keyword Names
    static std::string KEY_NAME; /**< Keyword for the name the user assigns for a force.*/
    static std::string KEY_COEFF; /**< Keyword for coefficient designation.*/
    static std::string KEY_NUMBER; /**< Keyword for equation number designation. */
    static std::string KEY_ORDER; /**< Keyword for order of derivative designation. */
    static std::string KEY_FORCE; /**< Keyword for force coefficients designation.*/
    static std::string KEY_EQUATION; /**< Keyword for equation designation.*/
};

}   //Namespace ofreq
}   //Namespace osea

#endif
