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
 *Aug 02 2014       Nicholas Barczak    Initially created
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
#ifndef SYSHYDRO_H
#define SYSHYDRO_H
#include "../global_objects/mathinterp.h"
#include "hydrodata.h"
#include "../global_objects/forceactive.h"
#include "../global_objects/forcecross.h"
#include "../global_objects/forcereact.h"

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
 * @brief The sysHydro class contains the complete set of hydrodynamic data associated with a single body.
 *
 * The most important aspect are all the variations possible for hydrodynamic data.  The data can vary with wave
 * frequency, wave direction, and wave amplitude.  The sysHydro has methods for interpolating amongst all that data
 * to create a single output for a requested set of conditions (some combination of wave frequency, wave direction,
 * and wave amplitude.)
 *
 * Note that interpolation for wave direction does have a limitation:  it will only permit interpolation with wave
 * directions that are within 0.785 rad (45 deg) of the available data.  This is due to the physics limitations.  Any
 * data more than 45 deg away from the desired direction really is not relevant.
 */
class sysHydro : public mathInterp
{
//==========================================Section Separator =========================================================
public:

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Nothing happens here.
     */
    sysHydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
      * @brief Default destructor.  Nothing happens here.
      */
    ~sysHydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a full list of the hydroData available for this body.
     *
     * List is organized by two indices.
     * Outer index (rows) = Wave Amplitude
     * Inner index (columns) = Wave Direction
     *
     * Each individual hydroData object also contains data, organized by wave frequency.
     * @return Returns a vector of vectors or hydroData objects, all passed by reference.
     */
    std::vector< std::vector< ofreq::hydroData > > &listHydroData();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a single item in the list of hydroData available.
     * @param ampInd Integer, passed by value.  The index of the wave amplitude that you want to retrieve.
     * @param dirInd Integer, passed by value.  The index of the wave direction you want to retrieve.
     * @return Returns a hydroData object, passed by reference
     */
    ofreq::hydroData &listHydroData(int ampInd, int dirInd);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a hydroData object to the set of available data.
     *
     * If an amplitude index is specified, the hydroData object will be added as a new wave direction for the
     * given amplitude index.  If no amplitude index is specified, a whole new entry for amplitude index will be
     * created.
     * @param ampInd Integer, passed by value.  The index of the outer loop where you want to store the hydroData.
     * @param dataIn hydroData object, passed by value.  The hydroData object that you want to add to the list of
     * available data for this system.
     */
    void addHydroData(int ampInd = -1, ofreq::hydroData dataIn= hydroData());

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the name of the hydrobody associated with this data set.
     * @return Returns a string, passed by value.  The name of the hydrobody associated with this data set.
     */
    std::string getHydroBodyName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the name of the hydrobody associated with this data set.
     * @param nameIn String, passed by value.  The name of the hydrobody associated with this data set.
     */
    void setHydroBodyName(std::string nameIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the forceActive object associated the requested environmental data.
     *
     * Retrieves a single forceActive object based on specification of the following parameters:  wave direction
     * (wave direction set seperately), wave amplitude, and wave frequency.  All interpolation and wave scaling
     * is triggered through this function.
     * @param waveAmp Double, passed by value.  The specific wave amplitude that you want the data for.  Data will
     * be scaled up to match the given wave amplitude.
     * @param waveFreq Double, passed by value.  The specific wave frequency that you want the data for.  Data will
     * be interpolated for the given wave frequency.
     * @return Returns a forceActive object, specific to the environmental conditions you specified.
     * @sa sysHydro::setWaveDirection()
     */
    ofreq::ForceActive getForceActive(double waveAmp, double waveFreq);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the forceReact object associated the requested environmental data.
     *
     * Retrieves a single forceReact object based on specification of the following parameters:  wave direction
     * (wave direction set seperately), wave amplitude, and wave frequency.  All interpolation and wave scaling
     * is triggered through this function.
     * @param waveAmp Double, passed by value.  The specific wave amplitude that you want the data for.  Data will
     * be scaled up to match the given wave amplitude.
     * @param waveFreq Double, passed by value.  The specific wave frequency that you want the data for.  Data will
     * be interpolated for the given wave frequency.
     * @return Returns a forceReact object, specific to the environmental conditions you specified.
     * @sa sysHydro::setWaveDirection()
     */
    ofreq::ForceReact getForceReact(double waveAmp, double waveFreq);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the forceCross object associated the requested environmental data.
     *
     * Retrieves a single forceCross object based on specification of the following parameters:  wave direction
     * (wave direction set seperately), wave amplitude, and wave frequency.  All interpolation and wave scaling
     * is triggered through this function.
     * @param waveAmp Double, passed by value.  The specific wave amplitude that you want the data for.  Data will
     * be scaled up to match the given wave amplitude.
     * @param waveFreq Double, passed by value.  The specific wave frequency that you want the data for.  Data will
     * be interpolated for the given wave frequency.
     * @param linkedBodName String, passed by value.  The name of the linked body that you want to retrieve the
     * forceCross object for.
     * @return Returns a forceCross object, specific to the environmental conditions you specified.
     * @sa sysHydro::setWaveDirection()
     */
    ofreq::ForceCross getForceCross(double waveAmp, double waveFreq, std::string linkedBodName);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the wave direction for the current set of analysis.
     *
     * hydroData gets retrieved for a specific set of environmental conditions.  One of those conditions is the
     * wave direction.  This gets set separately from any other conditions because for each wave direction, there
     * are usually several variations of wave amplitude and wave frequency.
     * @param dirIn Double, passed by value.  The wave direction for the current analysis.
     * The wave direction is defined in the body coordinate system, in units of radians (rad).  Zero rad corresponds
     * to a direction pointing along the X-axis of the body.  The angle rotates counter-clockwise.
     */
    void setWaveDir(double dirIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the wave direction for the current set of analysis.
     *
     * hydroData gets retrieved for a specific set of environmental conditions.  One of those conditions is the
     * wave direction.  This gets set separately from any other conditions because for each wave direction, there
     * are usually several variations of wave amplitude and wave frequency.
     * @param dirIn Double, passed by value.  The wave direction for the current analysis.
     * The wave direction is defined in the body coordinate system, in units of radians (rad).  Zero rad corresponds
     * to a direction pointing along the X-axis of the body.  The angle rotates counter-clockwise.
     */
    double getWaveDir();

//==========================================Section Separator =========================================================
protected:


//==========================================Section Separator =========================================================
private:

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of all hydro data associated with this body.
     *
     * List is organized by two indices.
     * Outer index (rows) = Wave Amplitude
     * Inner index (columns) = Wave Direction
     *
     * Each individual hydroData object also contains data, organized by wave frequency.
     */
    std::vector< std::vector< ofreq::hydroData > > plistHydroData;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name of the hydrobody associated with this data.
     *
     * The hydrobody name is used to match the hydrodata to the correct body in the analysis.
     */
    std::string pHydroBodyName;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The subset of hydroData specific to the given wave direction.
     *
     * This subset is formed after a new wave direction gets set.  The function buildWaveSubset() generates this
     * subset.  It is a smaller subset of data only specific to the selected wave direction.
     * The subset contains pointers to each of the two wave directions closest to the selected wave direction.
     * Each row / outer index will only contain two entries:  the two wave directions.  This will also eliminate any
     * given wave amplitudes that do not include data suitable to the the specified wave directions.
     */
    std::vector< std::vector< ofreq::hydroData* > > plistDirSubset;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The wave diretion set for the current set of hydro data.
     *
     * hydroData gets retrieved for a specific set of environmental conditions.  One of those conditions is the
     * wave direction.  This gets set separately from any other conditions because for each wave direction, there
     * are usually several variations of wave amplitude and wave frequency.
     *
     * The wave direction is defined in the body coordinate system, in units of radians (rad).  Zero rad corresponds
     * to a direction pointing along the X-axis of the body.  The angle rotates counter-clockwise.
     */
    double pWaveDir;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Builds the subset of hydroData specific to the given wave direction.
     *
     * This subset is formed after a new wave direction gets set.  The function buildWaveSubset() generates this
     * subset.  It is a smaller subset of data only specific to the selected wave direction.
     * The subset contains pointers to each of the two wave directions closest to the selected wave direction.
     * Each row / outer index will only contain two entries:  the two wave directions.  This will also eliminate any
     * given wave amplitudes that do not include data suitable to the the specified wave directions.
     */
    void buildWaveSubset();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Scales the wave data based on specified amplitude.
     *
     * The waveScale function can take one of two approaches.  If it only has one set of hydroData to use, it will
     * perform linear scaling.  This is the classic assumption for wave scaling, but rather crude.  If it has two
     * sets of hydroData to use, it will perform non-linear scaling based on fitting of a power function to the
     * two sets of hydroData.
     */
    template<class T>
    T waveScale(double ampIn, T* Data1, T* Data2 = NULL);

};

}   //Namespace ofreq

}   //Namespace osea

#endif // SYSHYDRO_H
