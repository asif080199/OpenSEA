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
 *Aug 29, 2014      Nicholas Barczak    Initially created
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
//Put header files
#ifndef HYDROREADER_H
#define HYDROREADER_H
#include "../file_reader/filereader.h"
#include "../hydro_data/hydrodata.h"
#include "dictcrossdamp.h"
#include "dictcrossmass.h"
#include "dictcrossstiff.h"
#include "dictforceexcite.h"
#include "dicthydrodamp.h"
#include "dicthydrodirection.h"
#include "dicthydroenv.h"
#include "dicthydrofrequency.h"
#include "dicthydromass.h"
#include "dicthydrostiff.h"

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


//######################################### Class Separator ###########################################################
/**
 * HydroReader is an extension of the class FileReader.  FileReader simply reads the text file and
 * parses it into keword value pairs.  FileReader reads in the input files.  It then passes those input files to the
 * Parser object.  Parser then segments the file in a series of object groupings with a vector list of keyword
 * value pairs.
 * The file reader interprets a limited number of those keywords to recognize new object declarations.  It creates
 * the new objects.  It then parses the information in that object into a series
 * of keyword-value pairs.  Each pair is sent to a Dictionary object that interprets the information.
 *
 * The HydroReader extension contains the subset of dictionaries specific to the interpretation of hydrodynamic data.
 * It also contains additional procedures to look into subfolder.  HydroReader interprets each subfolder as a wave
 * direction, based on keywords and key-value pairs.
 *
 * To use this class, the following sequence must be followed.
 * 1.)  Create HydroReader object.
 * 2.)  Create Dictionary objects for each file type you will read.
 * 3.)  Satisfy any follow on dependencies for Dictionary objects.
 * 4.)  Feed in the target path to the HydroReader object (path of the directory containing all files).
 * 5.)  Trigger a read of the hydro system.  The HydroReader will work through each file in the directory for the
 *      hydro system.  Any folders not part of the standard hydro system will be ignored.
 */
class HydroReader : public FileReader
{
    Q_OBJECT
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.
     */
    HydroReader();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Tells the HydroReader to progress through reading each of the files in the hydrosystem.
     * @return Returns an integer.  Returns 0 for success.  All others are error codes.
     */
    int readHydroSys();

//==========================================Section Separator =========================================================
public slots:


//==========================================Section Separator =========================================================
signals:



//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads the environment data file in the hydro system.
     * @return Returns an integer, variable passed by value.  Returns 0 for success.  All others are error codes.
     */
    int readEnvironment();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads the directions data file in the hydro system.
     * @return Returns an integer, variable passed by value.  Returns 0 for success.  All others are error codes.
     */
    int readDirections();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads the frequences data file in the hydro system.
     * @return Returns an integer, variable passed by value.  Returns 0 for success.  All others are error codes.
     */
    int readFrequencies();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads the excitation force data file in the hydro system.
     * @param index Integer, variable passed by value.  The index of the wave direction that you should pull files from.
     * @return Returns an integer, variable passed by value.  Returns 0 for success.  All others are error codes.
     */
    int readForceExcite(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads the hydrodynamic mass data file in the hydro system.
     * @param index Integer, variable passed by value.  The index of the wave direction that you should pull files from.
     * @return Returns an integer, variable passed by value.  Returns 0 for success.  All others are error codes.
     */
    int readHydroMass(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads the hydrodynamic damping data file in the hydro system.
     * @param index Integer, variable passed by value.  The index of the wave direction that you should pull files from.
     * @return Returns an integer, variable passed by value.  Returns 0 for success.  All others are error codes.
     */
    int readHydroDamp(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads the hydrodynamic stiffness data file in the hydro system.
     * @param index Integer, variable passed by value.  The index of the wave direction that you should pull files from.
     * @return Returns an integer, variable passed by value.  Returns 0 for success.  All others are error codes.
     */
    int readHydroStiff(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads the crossbody mass data file in the hydro system.
     * @param index Integer, variable passed by value.  The index of the wave direction that you should pull files from.
     * @return Returns an integer, variable passed by value.  Returns 0 for success.  All others are error codes.
     */
    int readCrossMass(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads the crossbody damping data file in the hydro system.
     * @param index Integer, variable passed by value.  The index of the wave direction that you should pull files from.
     * @return Returns an integer, variable passed by value.  Returns 0 for success.  All others are error codes.
     */
    int readCrossDamp(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads the crossbody stiffness data file in the hydro system.
     * @param index Integer, variable passed by value.  The index of the wave direction that you should pull files from.
     * @return Returns an integer, variable passed by value.  Returns 0 for success.  All others are error codes.
     */
    int readCrossStiff(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Searches through the list of HydroManager objects and returns index of the manager with matching name.
     *
     * You supply the name of a hydrobody associated with a HydroManager object.  The function then searches through
     * the list of existing HydroManager objects stored in the system.  It finds the index of the object which
     * matches your input.
     *
     * If no HydroManager exists, it returns -1.
     * @param BodyName String, variable passed by value.  The name of the hydrobody associated with the HydroManager
     * you are trying to find.
     * @return Returns an integer, variable passed by value.  The index of the HydroManager object you are trying
     * to find.  If no HydroManager exists, it returns -1.
     */
    int findHydroManager(std::string BodyName);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Searches through the list of HydroData objects and returns index of the object with matching wave amp.
     *
     * You supply the index of the HydroManager that you want to search through, and the wave amplitude.  The
     * function then searches through the listof existing HydroData objects stored in that HydroManager.  It finds the
     * index of the object which matches your input.
     *
     * If no HydroData object exists with matching criteria, it returns -1.
     * @param BodyInd Integer, variable passed by value.  The index of the HydroManager that you want to search through.
     * @param waveAmpIn Double, variable passed by value.  The wave amplitude that you want to find the matching
     * HydroData objects for.
     * @return Returns an integer, variable passed by value.  The index of the HydroData objects you are trying to
     * match.  If no HydroData object exists, it returns -1.
     */
    int findHydroDataAmp(int BodyInd, double waveAmpIn);

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Searches through the list of hydrodata objects defined within this class to find the one with the
     * matching name. (the listTempHydro variable)
     *
     * If no matching hydrodata set is found, the function will create a new data set, intialize all the critical
     * values, and then return the index of this new hydrodata set.
     * @param BodyName String, variable passed by value.  The hydrobodyname of the object that we need to match.
     * @return Returns an integer, variable passed by value.  The index of the hydrodata set in the list of temporary
     * hydrodata objects contained in this class.  (the listTempHydro vector.)
     */
    int findHydroDataTemp(std::string BodyName);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The index of the wave direction currently reading data from.
     */
    unsigned int WaveInd;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief vector tracking what was used for gravity in the hydrodynamic analysis.
     *
     * The gravity definition is a vector quantity to allow the possibility that gravity might change, depending on the
     * definition of the coordinate system.  It may seem odd, but ofreq is about the concept of flexiblity.  And you
     * never know.  Some scientistic might want to research wave dynamics on Jupiter.  So gravity was made flexible.
     */
    std::vector<double> pGravity;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave frequencies.  Each entry is in units of rad/s.
     */
    std::vector<double> plistWaveFreq;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The wave amplitude associated with this hydro system.  Measured in units of m.
     */
    double pWaveAmp;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The depth of water associated with this set of hydrodata.
     *
     * Some results may change depending on deep water or shallow water.  So the water depth is recorded.
     */
    double pDepth;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Density of the water associated with this set of hydrodata.  Some calculated results may change,
     * depending on the water density.  Hence why it was recorded.
     */
    double pDensity;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave directions.  Each entry is in units of rad.  Directions are relative to the body
     * coordinate system.
     */
    std::vector<double> plistWaveDir;

    //------------------------------------------Function Separator ----------------------------------------------------
    std::vector<ofreq::hydroData> plistTempHydro;

    //------------------------------------------Function Separator ----------------------------------------------------
    // Directory Names
    static std::string DIR;  /**< The letter prefix for directory names of wave direction directories. */

    //------------------------------------------Function Separator ----------------------------------------------------
    // Input File Names
    static std::string DIRECTIONS; /**< The filename for the wave directions file. */
    static std::string ENVIRONMENT; /**< The filename for the wave environmental conditions file. */
    static std::string FREQUENCIES; /**< The filename for the wave frequencies file. */
    static std::string FORCEEXCITE; /**< THe filename for the wave excitation forces file. */
    static std::string HYDROMASS; /**< The filename for the added mass forces file. */
    static std::string HYDRODAMP; /**< The filename for the added damping forces file. */
    static std::string HYDROSTIFF; /**< The filename for the added stiffness forces file. */
    static std::string CROSSMASS; /**< The filename for the crossbody mass forces file. */
    static std::string CROSSDAMP; /**< The filename for the crossbody damping forces file. */
    static std::string CROSSSTIFFNESS; /**< The filename for the crossbody stiffness forces file. */

    //------------------------------------------Function Separator ----------------------------------------------------
    // Class Name Designators
    static std::string OBJ_DIRECTIONS; /**< The designation for the wave directions object. */
    static std::string OBJ_ENVIRONMENT; /**< The designation for the wave environmental conditions object. */
    static std::string OBJ_FREQUENCIES; /**< The designation for the wave frequencies object. */
    static std::string OBJ_FORCEEXCITE; /**< THe designation for the wave excitation forces object. */
    static std::string OBJ_HYDROMASS; /**< The designation for the added mass forces object. */
    static std::string OBJ_HYDRODAMP; /**< The designation for the added damping forces object. */
    static std::string OBJ_HYDROSTIFF; /**< The designation for the added stiffness forces object. */
    static std::string OBJ_CROSSMASS; /**< The designation for the crossbody mass forces object. */
    static std::string OBJ_CROSSDAMP; /**< The designation for the crossbody damping forces object. */
    static std::string OBJ_CROSSSTIFFNESS; /**< The designation for the crossbody stiffness forces object. */

    // Key Value Pair Designators
    // ---------------------------------
};

}   //osea namespace

#endif // HYDROREADER_H
