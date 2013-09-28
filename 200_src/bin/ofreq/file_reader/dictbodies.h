/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date		Author				Description
 *---------------------------------------------------------------------------------------------------------------------
 *Mar 09 2013	Nicholas Barczak		Initially Created
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
#ifndef DICTBODIES_H
#define DICTBODIES_H
#include "dictionary.h"

using namespace std;

//######################################### Class Separator ###########################################################
/**
 * The dictBodies class defines the key-word value pairs associated with the Bodies.in input file.  Just as a normal
 * dictionary defines the meaning of words, the dictBodies class works in the same way.  The dictBodies class takes
 * individual pairs of keywords and values.  It has a definition for each of these keywords.  The definition is
 * whatever actions are necessary to process the value of key-pair and apply it to the program.  This may include
 * variable type conversions.  It will also use slots and signals to retrieve pointers to any appropriate objects that
 * the dictBodies object needs to interact with.  It will use the properties of those objects to apply the values
 * it finds in the key-value pair.  Any objects created in the dictBodies class can be safely deleted once all file
 * reading is done.
 *
 * Note:  The code for the dictBodies object always references the last object in the list.  This assumes that no
 * other commands get issued in the input file between the creation of an object and the definition of key-value
 * pairs associated with that object.  Currently, I can not imagine any situation where this assumption would be
 * violated.  But do consider this when planning error recovery methods.
 * @sa Dictionary
 * @sa FileReader
 */
class dictBodies: public Dictionary
{
    Q_OBJECT

    //==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    dictBodies();

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
     * @param keyIn String containing the key name.  Variable passed by value.
     * @param valIn Vector of strings containing the key values.  Variable passed by value.
     * @return Returns status of assigning key.  Returned value is an integer, passed by value.
     * See list of return codes below:
     * 0:  Key definition found.  Success.
     * 1:  No key found. / General error message.
     * 2:  Key is invalid within current active object.
     */
    int defineKey(string keyIn, vector<string> valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Function that defines how to interpret the class name.  Class name implies declaration of
     * a new object of the class named by the class name.  This is a separate set of definitions to handle class
     * declarations.
     * @param nameIn String, variable passed by value.  The name of the class name.
     * @return Returns status of assigning key.  Returned value is an integer, passed by value.
     * See list of return codes below:
     * 0:  Key definition found.  Success.
     * 1:  No key found. / General error message.
     * 2:  Key is invalid within current active object.
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
     * @brief Records the index of the last force within the Body object.  This is because there is the global list of
     * forces recorded in the System object.  And then the local list of forces explicitely used in the Body object.
     * @sa Body
     * @sa System
     */
    int pForceBodyIndex;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Records the index of the current Body object.
     */
    int pBody;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Records if the MassProp object is currently active.  This tracks how many mass properties have been
     * entered.  Toggles each element in the array as true when the key is entered.  Used if any future features
     * need to track if a KEY property has been set.  Array values are as follows:
     * [0]:  KEY_MASS
     * [1]:  KEY_IXX
     * [2]:  KEY_IYY
     * [3]:  KEY_IZZ
     * [4]:  KEY_IXY
     * [5]:  KEY_IXZ
     * [6]:  KEY_IYZ
     */
    bool pMassProp[7];

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Records if the COG properties have been set.  Each element in the array is a different COG property.
     * Used if any future features need to track if a KEY property has been set.  Properties are recorded as follows:
     * [0]:  KEY_COGX
     * [1]:  KEY_COGY
     * [2]:  KEY_COGZ
     */
    bool pCOG[3];

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief This sets the integer variable pForceIndex to specify the current force index that object properties
     * will be assigned under.  If no input is provided, the function finds the index of the last equation + 1.
     * @param forceIn The input for the index of the force.  Default input is -1, which triggers the function to
     * find the index of the last force in the list + 1.  Variable is passed by value.
     */
    void setForceIndex(int forceIn = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    //Class Name constants
    static const string OBJECT_BODY; /**< Object designator for new body object.*/
    static const string OBJECT_MASSPROP; /**< Object designator for new mass property object. */
    static const string OBJECT_CENTROID; /**< Object designator for new centroid object. */
    static const string OBJECT_FORCE_ACTIVE; /**< Object designator for new ForceActive object. */
    static const string OBJECT_FORCE_REACT; /**< Object designator for new ForceReact object. */
    static const string OBJECT_FORCE_CROSS; /**< Object designator for new ForceCross object. */
    static const string OBJECT_MODEL; /**< Object designator for model of ForceCross object. */

    //------------------------------------------Function Separator ----------------------------------------------------
    //Keyword name static constants
    static const string KEY_NAME; /**< Key for body name */
    static const string KEY_HYDROBODY; /**< Key for hydrobody name. */
    static const string KEY_MASS; /**< Key for mass of body */
    static const string KEY_IXX; /**< Key for moment of inertia, X-X axis */
    static const string KEY_IYY; /**< Key for moment of inertia, Y-Y axis */
    static const string KEY_IZZ; /**< Key for moment of inertia, Z-Z axis */
    static const string KEY_IXY; /**< Key for cross-product of inertia, X-Y coupling */
    static const string KEY_IXZ; /**< Key for cross-product of inertia, X-Z coupling */
    static const string KEY_IYZ; /**< Key for cross-product of inertia, Y-Z coupling */
    static const string KEY_COGX; /**< Key for center of gravity, X-axis coordinate */
    static const string KEY_COGY; /**< Key for center of gravity, Y-axis coordinate */
    static const string KEY_COGZ; /**< Key for center of gravity, Z-axis coordinate */
    static const string KEY_HEADING; /**< Key for heading of body, Z-axis rotation */
    static const string KEY_MOTION; /**< Key to specify motion model.*/
    static const string KEY_LINKEDBODY; /**< Key for linked body */
    static const string KEY_MODEL; /**< Key for model to use for force specification. */
};

#endif // DICTBODIES_H
