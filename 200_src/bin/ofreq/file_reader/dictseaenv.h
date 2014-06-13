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
 *Jun 10, 2014      Nicholas Barczak	Initially Created
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
#ifndef DICTSEAENV_H
#define DICTSEAENV_H
#include "dictionary.h"

//Include all the wave spectra
#include "../wave_spectra/specbretschneider.h"
#include "../wave_spectra/specjonswap.h"
#include "../wave_spectra/specpm.h"
#include "../wave_spectra/wavespec.h"
#include "../wave_spectra/wavespecbase.h"

//Include all sea models
#include "../sea_models/seamodel.h"
#include "../sea_models/seamodel_dualdirection.h"
#include "../sea_models/seamodel_singledirection.h"

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
 * @brief The dictSeaEnv class defines keyword value pairs associated with the SeaEnv.in input file.
 *
 * Just as a normal dictionary defines the meaning of words, the dictSeaEnv class works in the same way.  The
 * dictSeaEnv class takes individual pairs of keywords and values.  It has a definition for each of these keywords.
 * The definition is whatever actions are necessary to process the value of key-pair and apply it to the program.
 * This may include variable type conversions.  It will also use slots and signals to retrieve pointers to any
 * appropriate objects that the dictBodies object needs to interact with.  It will use the properties of those
 * objects to apply the values it finds in the key-value pair.
 *
 * Any objects created in the dictSeaEnv class can be safely deleted once all file reading is done.
 */
class dictSeaEnv : public Dictionary
{
    Q_OBJECT

//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Nothing happens here.
     */
    dictSeaEnv();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.  Nothing happens here.
     */
    ~dictSeaEnv();

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
     * @brief A subset of class definition.  Used to define specific class types.
     *
     * The user input file is structured so that the exact type is not known initially.  This function handles the
     * exact class specification as a subset of the keyword definitions.  It was segregated into its own function
     * to define that these keywords are special.
     * @param nameIn The keyword that specifies the exact class type to create.
     * @return Returns status of assigning key.  Returned value is an integer, passed by value.
     * See list of return codes below:
     * 0:  Key definition found.  Success.
     * 1:  No key found. / General error message.
     * 2:  Key is invalid within current active object.
     * 99: Function virtual definition only.  Not currently defined.
     */
    int buildClass(std::string nameIn);


    //------------------------------------------Function Separator ----------------------------------------------------
    std::string pClassType; /**< Records which type of class is created, before specifying exact class type*/
    std::string pSubClassType; /**< Records which specify type of class is created. */

    //------------------------------------------Function Separator ----------------------------------------------------
    int pObjIndex;  /**< The index of the object that properties are currently being added to.*/

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The index of the last entry of the sea model objects.
     *
     * This variable is used to track the last index of two important lists in the sea model object:  listSpectrum
     * and listWaveScale.  Whenever an object gets added to the listSpectrum, a new listScale object is automatically
     * created.  Hence, the pListIndex is used to compare between the two lists and determine which action is
     * appropriate.
     */
    int pListIndex;

    //------------------------------------------Function Separator ----------------------------------------------------
    bool pClassSet;  /**< Records if the exact class type was set before any properties assigned.*/

    //------------------------------------------Function Separator ----------------------------------------------------
    bool pMakeNewWaveDef;    /**< Records if a new wave definition needs to be added to the sea model. */

    //------------------------------------------Function Separator ----------------------------------------------------
    //Class Name constants
    static std::string OBJECT_WAVE_SPEC; /**< Class designation for a wave spectra. */
    static std::string OBJECT_SEA_MODEL; /**< Class designation for a sea model. */
    static std::string OBJECT_WAVE_DEF; /**< Class designation for a wave direction definition. */

    //------------------------------------------Function Separator ----------------------------------------------------
    //Keyword name constants
    static std::string KEY_CUSTOM;   /**< Class designation for either custom wave spectra, or custom sea model.*/
    static std::string KEY_BRETSCHNEIDER; /**< Class designation for Bretschneider type of wave spectrum. */
    static std::string KEY_ITTC; /**< Class designation for Bretschneider type of wave spectrum, just different name.*/
    static std::string KEY_JONSWAP; /**< Class designation for JONSWAP type of wave spectrum.*/
    static std::string KEY_PM; /**< Class designation for Pierson Moskowitz type of wave spectrum. */
    static std::string KEY_1D; /**< Class designation for a single direction sea model. */
    static std::string KEY_2D; /**< Class designation for a dual direction sea model. */
    static std::string KEY_LONG; /**< Class designation for a long crested wave sea model. */

    static std::string KEY_CLASS;   /**< Keyword to specify class type for sea model or wave spectrum. */
    static std::string KEY_NAME;    /**< Keyword to specify the user name designation for sea model or wave spectrum. */
    static std::string KEY_FREQUENCY; /**< Keyword to specify the frequencies of a custom wave spectrum. */
    static std::string KEY_WAVEENERGY; /**< Keyword to specity wave energy of a custom wave spectrum. */
    static std::string KEY_HSIG;    /**< Keyword to specify the significant wave height for definition of a wave spectrum. */
    static std::string KEY_TMEAN;   /**< Keyword to specify the mean period for definition of a wave spectrum. */
    static std::string KEY_TPEAK;   /**< Keyword to specify the peak period for definition of a wave spectrum. */
    static std::string KEY_TZERO;   /**< Keyword to specify the zero crossing period for definition of a wave spectrum. */
    static std::string KEY_BANDPARAM;   /**< Keyword to specify gamma value of JONSWAP spectrum. */
    static std::string KEY_GAMMA;   /**< Keyword to specify gamma value of JONSWAP spectrum, under a different name. */
    static std::string KEY_WIND;    /**< Keyword to specify the wind speed of the Pierson Moskowitz spectrum. */
    static std::string KEY_DIRECTION; /**< Keyword to specify the wave direction for a sea model component. */
    static std::string KEY_SPECTRUM;  /**< Keyword to specify the wave spectrum for a sea model component. */
    static std::string KEY_SCALE;   /**< Keyword to specify the scale factor for a sea model component. */


};

} //Namespace ofreq

}   //Namespace osea

#endif // DICTSEAENV_H
