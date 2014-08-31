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
 *Aug 30 2014       Nicholas Barczak	Initially Created
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
#ifndef DICTCROSSREACT_H
#define DICTCROSSREACT_H
#include "../file_reader/dictionary.h"
#include "../motion_solver/matforcecross.h"
#include <cmath>
#include <complex>
#ifdef Q_OS_WIN
    #include "../../lib/armadillo.h"  //References the armadillo library in lib folder.
#elif defined Q_OS_LINUX
    #include <armadillo>    //Armadillo library included with standard system libraries.
#endif

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

//######################################### Class Separator ###########################################################
//Prototype class declarations
class HydroReader;  /**< Prototype class declaration for constructor. */
class ofreq::hydroData; /**< Prototype class declaration for pointer to hydroData object. */


//######################################### Class Separator ###########################################################
/**
 * The dictCrossReact class is a template class for the dictionaries that read crossbody reactive forces.
 *
 * All of these input files have exactly the same format.  They are just associated with a different order of
 * derivative.  A single static variable changes for each of the children classes.  Everything else remains the same.
 *
 * The dictCrossReact class is itself derived from the Dictionary class.  The Dictionary class defines most of the
 * behavior.  The only thing added to this class is the behavior for how to handle the individual keywords specific
 * to this input file.  In the case of the dictCrossReact class, the same keywords are used in all related input
 * files, hence why a generic class can be used with inheritence.  The following input files are processed with
 * this class.
 * 1.)  crossdamp.out
 * 2.)  crossmass.out
 * 3.)  crossstiffness.out
 * @sa Dictionary
 * @sa FileReader
 * @sa HydroReader
 */
class dictCrossReact : public Dictionary
{
    Q_OBJECT
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    dictCrossReact();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Constructor that also passes pointer to the owning file reader.
     * @param parent Pointer to HydroReader object, pointer passed by value.
     */
    dictCrossReact(osea::HydroReader *parent);

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

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the parent HydroReader object.  Sometimes useful if you need to use the parent object.
     */
    osea::HydroReader *pParent;

    //------------------------------------------Function Separator ----------------------------------------------------
    ofreq::hydroData *pHydroBod;

    //------------------------------------------Function Separator ----------------------------------------------------
    bool pBodOn;  /**< Tracks whether a body object was declared for filling in data associated with that body. */

    //------------------------------------------Function Separator ----------------------------------------------------
    bool pLinkOn; /**< Tracks whether a linked body object was declared for filling in data from that linked body. */

    //------------------------------------------Function Separator ----------------------------------------------------
    std::string pLinkName; /**< The name of the linked body associated with the current data set. */

    //------------------------------------------Function Separator ----------------------------------------------------
    static int ORD; /**< The order of derivative that is associated with all data from this file. */

    //------------------------------------------Function Separator ----------------------------------------------------
    int pFreqInd; /**< The index of the frequency associated with the current set of data. */

    //------------------------------------------Function Separator ----------------------------------------------------
    //Class Name constants
    static std::string OBJECT_BODY; /**< Body object designator. */
    static std::string OBJECT_DATA; /**< Data object designator. */
    static std::string OBJECT_LINKBODY; /**< Object designator for a linked body. */

    //------------------------------------------Function Separator ----------------------------------------------------
    //Keyword name constants
    static std::string KEY_NAME; /**< Keyword for body name */
    static std::string KEY_FREQUENCY; /**< Keyword for frequency. */
    static std::string KEY_VALUE; /**< Keyword for matrix of coefficients. */

//==========================================Section Separator =========================================================
private:

};

}   //Namespace osea

#endif // DICTCROSSREACT_H
