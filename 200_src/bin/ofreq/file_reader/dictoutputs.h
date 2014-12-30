/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date              Author				Description
 *---------------------------------------------------------------------------------------------------------------------
 *Mar 09 2013       Nicholas Barczak	Initially Created
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
#ifndef DICTOUTPUTS_H
#define DICTOUTPUTS_H
#include "dictionary.h"

//Include Report objects.
#include "../reports/report.h"
#include "../reports/repbodacceleration.h"
#include "../reports/repbodmotion.h"
#include "../reports/repbodsolution.h"
#include "../reports/repbodvelocity.h"
#include "../reports/replocalsolution.h"
#include "../reports/replocalacceleration.h"
#include "../reports/repwavespectra.h"
#include "../reports/repstaticbodyaccel.h"
#include "../reports/repdesignaccel.h"
#include "../reports/repdynfreeboard.h"
#include "../reports/repbodforces.h"
#include "../reports/repbodpower.h"
#include "../reports/repcapturewidth.h"
#include "../reports/repreleff.h"
#include "../reports/repabseff.h"

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
//Prototype class declarations

//######################################### Class Separator ###########################################################
/**
 * @brief The dictOutputs class defines the keyword-value pairs associated with the Outputs.in input file.
 *
 * Just as a normal
 * dictionary defines the meaning of words, the dictOutputs class works in the same way.  The dictOutputs class takes
 * individual pairs of keywords and values.  It has a definition for each of these keywords.  The definition is
 * whatever actions are necessary to process the value of key-pair and apply it to the program.  This may include
 * variable type conversions.  Any objects created in the dictOutputs class can be safely deleted once all file
 * reading is done.
 *
 * Note:  The code for the dictOutputs object always references the last object in the list.  This assumes that no
 * other commands get issued in the input file between the creation of an object and the definition of key-value
 * pairs associated with that object.  Currently, I can not imagine any situation where this assumption would be
 * violated.  But do consider this when planning error recovery methods.
 * @sa Dictionary
 * @sa FileReader
 */
class dictOutputs : public Dictionary
{
    Q_OBJECT
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.
     */
    dictOutputs();

//==========================================Section Separator =========================================================
signals:

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
    ofreq::Report *ptRep = NULL;

    //------------------------------------------Function Separator ----------------------------------------------------
    //Class Name constants
    static std::string OBJECT_REPBODACCELERATION;  /**< Key to declare global acceleration object. */
    static std::string OBJECT_REPBODVELOCITY;  /**< Key to declare global velocity object. */
    static std::string OBJECT_REPBODMOTION;    /**< Key to declare global motion object. */
    static std::string OBJECT_REPBODSOLUTION;  /**< Key to declare global solution object. */
    static std::string OBJECT_REPLOCALACCELERATION; /**< Key to declare local acceleration object. */
    static std::string OBJECT_REPLOCALSOLUTION; /**< Key to declare local solution object. */
    static std::string OBJECT_REPWAVESPECTRA; /**< Key to declare wave spectra object. */
    static std::string OBJECT_REPSTATICBODACCEL; /**< Key to declare static body acceleration report. */
    static std::string OBJECT_REPDESIGNACCEL;   /**< Key to declare design acceleration report. */
    static std::string OBJECT_REPDYNFREEBOARD;  /**< Key to declare dynamic freeboard report. */
    static std::string OBJECT_REPBODFORCES; /**< Key to declare body forces report. */
    static std::string OBJECT_REPBODPOWER;  /**< Key to declare body power report. */
    static std::string OBJECT_REPCAPWIDTH;  /**< Key to declare capture width report. */
    static std::string OBJECT_REPRELEFF;  /**< Key to declare report of relative efficiency. */
    static std::string OBJECT_REPABSEFF;  /**< Key to declare report of absolute efficiency. */

    //------------------------------------------Function Separator ----------------------------------------------------
    //Keyword name constants
    static std::string KEY_BODY;    /**< Key to declare which body report is associated with. */
    static std::string KEY_NAME;    /**< Key to declare name of report object. */
    static std::string KEY_RAO;     /**< Key to calculate RAO or not. */
};

}   //Namespace ofreq

}   //Namespace osea

#endif // DICTOUTPUTS_H
