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
#ifndef HYDRODATA_H
#define HYDRODATA_H
#include "../global_objects/mathinterp.h"
#include "../motion_solver/matforceactive.h"
#include "../motion_solver/matforcecross.h"
#include "../motion_solver/matforcereact.h"

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
 * @brief The hydroData class is the basic definition for a set of hydrodynamic data.
 *
 * A single hydroData object contains all Reactive, Active, and Crossbody forces for a single body, at a single wave
 * direction, for a single wave amplitude.  All wave frequencies are included in the hydroData object.  Force data is
 * provided for each wave frequency.  The hydroData object also contains properties to specify the exact environmental
 * conditions for the given data set (wave amplitude, wave direction, and hydrobody name).
 *
 * The hydroData object can report out force data for any specified wave frequency.  It will perform linear
 * interpolation within the given data set to produce a result.  It will not extend outside the data set defined.
 */
class hydroData : public osea::ofreq::mathInterp
{
//==========================================Section Separator =========================================================
public:

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Nothing happens here.
     */
    hydroData();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
      * @brief Default destructor.  Nothing happens here.
      */
    ~hydroData();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the wave amplitude associated with this set of hydrodata.
     *
     * Every set of hydrodata has a specific set of environmental conditions that were used to generate that data.
     * The wave amplitude is one of those environmental conditions.
     * @param ampIn  Double, variable passed by value.  The wave amplitude used for this set of hydrodata.  Amplitude
     * measured in units of meters (m).
     */
    void setWaveAmp(double ampIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the wave amplitude associated with this set of hydrodata.
     *
     * Every set of hydrodata has a specific set of environmental conditions that were used to generate that data.
     * The wave amplitude is one of those environmental conditions.
     * @return Double, variable passed by value.  The wave amplitude used for this set of hydrodata.  Amplitude
     * measured in units of meters (m).
     */
    double getWaveAmp();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the wave direction associated with this set of hydrodata.
     *
     * Every set of hydrodata has a specific set of environmental conditions that were used to generate that data.
     * The wave direction is one of those environmental conditions.
     * @param dirIn Double, variable passed by value.  The wave direction.  This is the direction of wave travle,
     * measured relative to the body forward motion.  Direction measured in units of radians (rad).  Direction
     * measured in body coordinate system.  A direction of zero deg corresponds to travel along the body positive
     * X-axis.  Direction rotates in counter-clockwise direction.
     */
    void setWaveDir(double dirIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the wave direction associated with this set of hydrodata.
     *
     * Every set of hydrodata has a specific set of environmental conditions that were used to generate that data.
     * The wave direction is one of those environmental conditions.
     * @return Double, variable passed by value.  The wave direction.  This is the direction of wave travle,
     * measured relative to the body forward motion.  Direction measured in units of radians (rad).  Direction
     * measured in body coordinate system.  A direction of zero deg corresponds to travel along the body positive
     * X-axis.  Direction rotates in counter-clockwise direction.
     */
    double getWaveDir();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of reactive force data.
     *
     * This returns a reference to the list of reactive data forces.  Each entry in the list corresponds to a specific
     * wave frequency.
     * @return Returns a vector of matForceReact objects, passed by reference.  This is the list of data objects
     * associated with each frequency in the data set.
     */
    std::vector<ofreq::matForceReact> &listDataReact();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A single entry in the list of reactive force data.
     *
     * This returns a reference to one entry in the list of reactive data forces.  Each entry in the list corresponds
     * to a specific wave frequency.  The index specifies which entry you want from the list.
     * @param index Integer, passed by value.  The index for the specific entry that you want in the list.
     * @return Returns a single entry in the vector of matForceReact objects, passed by reference.  This is the list
     * of data objects associated with each frequency in the data set.
     */
    ofreq::matForceReact &listDataReact(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a single entry ot the list of reactive force data objects.
     *
     * Each entry in the list corresponds to a specific wave frequency.
     * @param forceIn matForceReact object, passed by value.  The entry that you wish to add to the list of DataReact
     * objects.
     */
    void addDataReact(ofreq::matForceReact forceIn = ofreq::matForceReact());

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of Active force data.
     *
     * This returns a reference to the list of active data forces.  Each entry in the list corresponds to a specific
     * wave frequency.
     * @return Returns a vector of matForceActive objects, passed by reference.  This is the list of data objects
     * associated with each frequency in the data set.
     */
    std::vector<ofreq::matForceActive> &listDataActive();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A single entry in the list of Active force data.
     *
     * This returns a reference to one entry in the list of Active data forces.  Each entry in the list corresponds
     * to a specific wave frequency.  The index specifies which entry you want from the list.
     * @param index Integer, passed by value.  The index for the specific entry that you want in the list.
     * @return Returns a single entry in the vector of matForceActive objects, passed by reference.  This is the list
     * of data objects associated with each frequency in the data set.
     */
    ofreq::matForceActive &listDataActive(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a single entry ot the list of Active force data objects.
     *
     * Each entry in the list corresponds to a specific wave frequency.
     * @param forceIn matForceActive object, passed by value.  The entry that you wish to add to the list of DataActive
     * objects.
     */
    void addDataActive(ofreq::matForceActive forceIn = ofreq::matForceActive());

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of crossbody force data.
     *
     * This returns a reference to the list of crossbody data forces.  Each entry in the outer list corresponds to a
     * specific wave frequency.  Each entry in the inner list corresponds to a specific hydrobody that the crossbody
     * force depends on.  This is the body whose motions generate the forces for the crossbody force.
     * @return Returns a vector of matForceCross objects, passed by reference.  This is the list of data objects
     * associated with each frequency in the data set and each hydrobody in the data set.
     */
    std::vector< std::vector< ofreq::matForceCross> > &listDataCross();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A single entry in the list of crossbody force data.
     *
     * This returns a reference to one entry in the list of crossbody data forces.  Each entry in the list corresponds
     * to a specific wave frequency.  The index specifies which entry you want from the list.
     * @param freqInd Integer, passed by value.  The index for the specific entry of frequency that you want in the
     * list.
     * @param bodIndex Integer, passed by value.  The index for the specific entry of hydrobody index that you want
     * in the list.
     * @return Returns a single entry in the vector of matForceCross objects, passed by reference.  This is the list
     * of data objects associated with each frequency in the data set and each hydrobody in the data set.
     */
    ofreq::matForceCross &listDataCross(int freqInd, int bodIndex);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A single entry in the list of crossbody force data.
     *
     * This returns a reference to one entry in the list of crossbody data forces.  Each entry in the list corresponds
     * to a specific wave frequency.  The index specifies which entry you want from the list.
     * @param freqInd Integer, passed by value.  The index for the specific entry of frequency that you want in the
     * list.
     * @param bodyName String, passed by value.  The name for the specific entry of hydrobody index that you want
     * in the list.
     * @return Returns a single entry in the vector of matForceCross objects, passed by reference.  This is the list
     * of data objects associated with each frequency in the data set and each hydrobody in the data set.
     */
    ofreq::matForceCross &listDataCross(int freqInd, std::string bodyName);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a single entry to the list of crossbody forces.
     *
     * Each entry in the list corresponds to a single wave frequency and hydrobody name.
     * @param forceIn matForceCross object, passed by value.  The entry you wish to add to the list of crossbody
     * force dat.  If no input supplied, the default is to create an empty matForceCross object.
     * @param freqInd Integer, passed by value.  The index of the frequency entry that you would like to add the
     * entry add.  Remember that there are two indices.  If the frequency index is specified, that frequency index
     * is located, and a new hydrobody entry is added at that frequency index.  If no frequency index is specified,
     * the new entry will be added as a new frequency index.
     */
    void addDataCross(ofreq::matForceCross forceIn = ofreq::matForceCross(), int freqInd = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave frequencies.
     *
     * Each entry in the list of wave frequencies corresponds to the matching entry in the list of dataCross,
     * dataReact, and dataActive objects.  Wave frequencies are measured in units of radians per second (rad/s).
     * @return Returns a vector of doubles, passed by reference.  References the list of wave frequencies.
     */
    std::vector<double> &listWaveFreq();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A single entry in the list of wave frequencies.
     *
     * Each entry in the list of wave frequencies corresponds to the matching entry in the list of dataCross,
     * dataReact, and dataActive objects.  Wave frequencies are measured in units of radians per second (rad/s).
     * @param index Integer, passed by value.  The index of the specific entry that you want from the list.
     * @return Returns a double, passed by reference.  References a single entry in the list of wave frequencies.
     */
    double &listWaveFreq(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds an entry to the list of wave frequencies.
     *
     * Each entry in the list of wave frequencies corresponds to the matching entry in the list of dataCross,
     * dataReact, and dataActive objects.  Wave frequencies are measured in units of radians per second (rad/s).
     * @param freqIn Double, passed by value.  The value of the wave frequency that you want added to the list of
     * wave frequencies.  Wave frequencies are measured in units of radians per second (rad/s).
     */
    void addWaveFreq(double freqIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Interpolates through all entries of DataReact forces and returns the data at the specified frequency.
     *
     * The function searches through all entries in the listDataReact of forces.  It finds the two entries that are
     * closest to the specified frequency.  It then performs linear interpolation to obtain the resulting data set
     * at the requested frequency.  This is interpolation only, if a frequency is requested that extends beyond
     * the data set, it will only return the result for the closest matching frequency.
     * @param freqIn Double, passed by value.  The frequency that you want data interpolated for.  Wave frequency
     * measured in units of radians per second (rad/s).
     * @return Returns a matForceReact object, passed by value.  This is the result of the interpolation at the
     * requested wave frequency.
     */
    ofreq::matForceReact getDataReact(double freqIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Interpolates through all entries of DataActive forces and returns the data at the specified frequency.
     *
     * The function searches through all entries in the listDataActive of forces.  It finds the two entries that are
     * closest to the specified frequency.  It then performs linear interpolation to obtain the resulting data set
     * at the requested frequency.  This is interpolation only, if a frequency is requested that extends beyond
     * the data set, it will only return the result for the closest matching frequency.
     * @param freqIn Double, passed by value.  The frequency that you want data interpolated for.  Wave frequency
     * measured in units of radians per second (rad/s).
     * @return Returns a matForceReact object, passed by value.  This is the result of the interpolation at the
     * requested wave frequency.
     */
    ofreq::matForceActive getDataActive(double freqIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Interpolates through all entries of DataCross forces and returns the data at the specified frequency.
     *
     * The function searches through all entries in the listDataCross of forces.  It finds the two entries that are
     * closest to the specified frequency.  It then performs linear interpolation to obtain the resulting data set
     * at the requested frequency.  This is interpolation only, if a frequency is requested that extends beyond
     * the data set, it will only return the result for the closest matching frequency.
     *
     * Because the DataCross objects also have multiple entries for each hydrobody, you must also specify the index
     * of the hydrobody you desire.
     * @param freqIn Double, passed by value.  The frequency that you want data interpolated for.  Wave frequency
     * measured in units of radians per second (rad/s).
     * @param hydroInd Integer, passed by value.  The index of the hydrobody that you want data interpolated for.
     * The same hydrobody index will be used on both entries that are used in the interpolation.
     * @return Returns a matForceCross object, passed by value.  This is the result of the interpolation at the
     * requested wave frequency and requested hydrobody index.
     */
    ofreq::matForceCross getDataCross(double freqIn, int hydroInd);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Interpolates through all entries of DataCross forces and returns the data at the specified frequency.
     *
     * The function searches through all entries in the listDataCross of forces.  It finds the two entries that are
     * closest to the specified frequency.  It then performs linear interpolation to obtain the resulting data set
     * at the requested frequency.  This is interpolation only, if a frequency is requested that extends beyond
     * the data set, it will only return the result for the closest matching frequency.
     *
     * Because the DataCross objects also have multiple entries for each hydrobody, you must also specify the name
     * of the hydrobody you desire.
     * @param freqIn Double, passed by value.  The frequency that you want data interpolated for.  Wave frequency
     * measured in units of radians per second (rad/s).
     * @param hydroName String, passed by value.  The name of the hydrobody that you want data interpolated for.
     * The same hydrobody name will be used on both entries that are used in the interpolation.
     * @return Returns a matForceCross object, passed by value.  This is the result of the interpolation at the
     * requested wave frequency and requested hydrobody index.
     */
    ofreq::matForceCross getDataCross(double freqIn, std::string hydroName);


//==========================================Section Separator =========================================================
protected:


//==========================================Section Separator =========================================================
private:

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The wave amplitude that was used for the currently defined data set.  Amplitude is measured in meters.
     */
    double pWaveAmp;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The wave direction that was used for the currently defined data set.  Wave direction is measured in
     * radians, with 0 rad corresponding to True North, proceeding counter-clockwise.  Wave direction is defined in
     * the body coordinate system.
     */
    double pWaveDir;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name of the hydrobody object associated with this data set.
     *
     * The hydrobody name is used to associate the data set with a specific body in the motions analysis.
     */
    std::string pHydroBodyName;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of of hydro reactive data associated with this data set.
     *
     * Each entry in the list corresponds to a single wave frequency.  Each entry should contain data for both added
     * mass and added damping properties.  Any added stiffness properties (hydrostatic forces) will also be included.
     */
    std::vector<ofreq::matForceReact> plistDataReact;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of hydro active force data associated with this data set.
     *
     * Each entry in the list corresponds to a single wave frequency.  Each entry should contain all information for
     * excitation forces.  This is the result of incident and diffracted waves.  Normally matrix will contain six
     * entries.
     */
    std::vector<ofreq::matForceActive> plistDataActive;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of hydro crossbody forces associated with this data set.
     *
     * The crossbody data set contains two vectors.  The outer vector is the index associated with wave frequencies.
     * The inner vector is the index associated with each crossbody name.  A separate vector maintains the list of
     * crossbody names.
     */
    std::vector< std::vector< ofreq::matForceCross> > plistDataCross;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave frequencies.  Each entry is in units of rad/s.
     *
     * The entries in this list correspond to the entries in the lists of plistDataReact, plistDataActive,
     * and plistDataCross.
     */
    std::vector<double> plistWaveFreq;
};

}   //Namespace ofreq

}   //Namespace osea

#endif // HYDRODATA_H
