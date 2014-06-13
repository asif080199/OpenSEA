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
#ifndef SEAMODEL_H
#define SEAMODEL_H
#include "../system_objects/ofreqcore.h"
#include <vector>
#include <stdlib.h>
#include "../wave_spectra/wavespec.h"
#include <ostream>

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
/**
 * @brief The SeaModel class creates the model for a full sea environment.
 *
 * At the most basic definition, the SeaModel is a collection of WaveSpec objects, each object associated with a
 * specific wave direction.  The SeaModel produces two primary results:
 * 1.  Wave energy at a specified wave direction and wave frequency.
 * 2.  Wave amplitude at a specified wave direction and wave frequency.
 *
 * For each requested result, the SeaModel interpolates between the defined spectra for each wave direction to get a
 * resulting wave energy at the specified wave direction.  The wave direction does not need to match the list of
 * directions supplied with the WaveSpec objects.  That is the flexibility of the SeaModel.  You provide it with a
 * data set, and it provides results for any given point within that data set.
 *
 * The SeaModel does have one important limitation:  You must supply it a full list of wave frequencies before
 * requesting wave amplitude.  This is because the calculations for wave amplitude depend on the spacing of the
 * wave frequencies, and those frequencies may not be regularly spaced.  Individual WaveSpec objects have their own
 * frequency data sets.  And the SeaModel has no way to correlate between those frequency lists.  That is why the
 * SeaModel must have its own governing set of wave frequencies.  When you specify a wave frequency and direction
 * to get wave amplitude, you can only select from one of the governing frequencies defined in the list.  This is
 * necessary to ensure the programming is mathematically correct.
 *
 * The list of wave frequencies does not need to be sorted.  The SeaModel will automatically sort after the list
 * gets defined.  Same for the wave directions.
 */
class SeaModel : public osea::ofreq::oFreqCore
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.
     *
     * Sets sorting lists to false.  (Triggers the class to resort the lists before they get used.)
     */
    SeaModel();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.  Nothing happens here.
     */
    virtual ~SeaModel();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave directions.
     *
     * Wave directions are defined in units of radians, using the global coordinate system.  A direction of 0 rad
     * points to True North.  Wave directions proceed counter-clockwise (from North to West).
     * @return Returns a vector of doubles.  Returned variable passed by reference.  Returns a list of wave directions
     * Each wave direction corresponds to a specific wave spectrum.
     */
    std::vector<double> &listWaveDir();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A specific entry in the list of wave directions.
     *
     * Wave directions are defined in units of radians, using the global coordinate system.  A direction of 0 rad
     * points to True North.  Wave directions proceed counter-clockwise (from North to West).
     * @param index Integer, variable passed by value.  The index of the specific frequency requested in the list
     * of wave directions.
     * @return Returns a double, variable passed by reference.  THis is a specific wave direction in the list of
     * wave directions.
     */
    double &listWaveDir(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave spectra.
     *
     * Each WaveSpec object represents a wave spectra defined for a specific direction.  The WaveSpec objects are
     * stored external to the SeaModel.  The list of wave spectra is only a list of pointers locating the wave spectra.
     * @return Returns a vector of pointers to WaveSpec objects.  Returned vector is passed by reference.
     */
    std::vector<osea::WaveSpecBase*> &listWaveSpec();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A specific entry in the list of wave spectra.
     *
     * Each WaveSpec object represents a wave spectra defined for a specific direction.  The WaveSpec objects are
     * stored external to the SeaModel.  The list of wave spectra is only a list of pointers locating the wave spectra.
     * @param index Integer, variable passed by value.  Specifies the index of the specific WaveSpec object you want.
     * @return Returns a pointer to a WaveSpec object.  Returned pointer is passed by reference.
     */
    osea::WaveSpecBase* &listWaveSpec(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave frequencies.
     *
     * The list of wave frequencies that are used by the sea model to request wave energy or wave amplitude at a
     * specific wave direction and frequency.  This list of frequencies is independent of the frequency list contained
     * in the system object.  For everything conceivable situation, the two lists will be identical.  However, if,
     * for some strange reason, you need to have a separate list of wave frequencies for the SeaModel, that is
     * possible.
     * @return Returns a vector of doubles, variable passed by reference.  Vector is the list of wave frequencies used
     * by the SeaModel.  Frequencies are measured in radians per second (rad/s).
     */
    std::vector<double> &listWaveFreq();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A single entry from the list of wave frequencies.
     *
     * The list of wave frequencies that are used by the sea model to request wave energy or wave amplitude at a
     * specific wave direction and frequency.  This list of frequencies is independent of the frequency list contained
     * in the system object.  For everything conceivable situation, the two lists will be identical.  However, if,
     * for some strange reason, you need to have a separate list of wave frequencies for the SeaModel, that is
     * possible.
     * @param index Integer.  Variable passed by value.  The index of the frequency you want to retrieve from the
     * list of wave frequencies.
     * @return Returns a double, variable passed by reference.  Variable is the wave frequency at the requested index.
     * Frequency measured in units of radians per second (rad/s).
     */
    double &listWaveFreq(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave scaling multipliers.
     *
     * Each WaveSpec object has a wave scaling multiplier associated with it.  Those multipliers are stored in this
     * list.  The multipliers are a quick way to take a single direction wave spectrum and distribute across multiple
     * wave directions.
     * @return Returns a vector of doubles, variable passed by reference.  Returned variable is the list of wave
     * scaling multipliers.
     */
    std::vector<double> &listWaveScale();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A single entry from the list of wave scaling multipliers.
     *
     * Each WaveSpec object has a wave scaling multiplier associated with it.  Those multipliers are stored in this
     * list.  The multipliers are a quick way to take a single direction wave spectrum and distribute across multiple
     * wave directions.
     * @param index Integer, variable passed by value.  The index of the wave scaling multiplier that you want to
     * retrieve from the vector of multipliers.
     * @return Returns a double, variable passed by reference.  Returned variable is a value for a wave scaling
     * multiplier at the index specified.
     */
    double &listWaveScale(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a wave direction to the list of wave directions.
     *
     * Wave directions are defined in units of radians, using the global coordinate system.  A direction of 0 rad
     * points to True North.  Wave directions proceed counter-clockwise (from North to West).
     *
     * The wave directions do not need to be added sequentially.  THe class automatically sorts the list of wave
     * directions before it uses the data.
     * @param dirIn Double, variable passed by value.  The wave direction to add to the list of wave directions.
     */
    void addWaveDir(double dirIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Add a new WaveSpec object to the list of wave spectrum objects.
     *
     * Each WaveSpec object represents a wave spectra defined for a specific direction.  The WaveSpec objects are
     * stored external to the SeaModel.  The list of wave spectra is only a list of pointers locating the wave spectra.
     * The WaveSpec object added corresponds to the matching wave direction and wave scale terms at the same index
     * in their lists.  You do not need to add entries in sequential order.  The class automatically sorts the entries
     * into sequential order before using them for calculations.
     * @param specIn Pointer to a WaveSpec object, variable passed by value.
     */
    void addWaveSpec(osea::WaveSpecBase* specIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a new wave frequency to the list of wave frequencies.
     *
     * The list of wave frequencies that are used by the sea model to request wave energy or wave amplitude at a
     * specific wave direction and frequency.  This list of frequencies is independent of the frequency list contained
     * in the system object.  For everything conceivable situation, the two lists will be identical.  However, if,
     * for some strange reason, you need to have a separate list of wave frequencies for the SeaModel, that is
     * possible.
     *
     *  You do not need to add entries in sequential order.  The class automatically sorts the entries into sequential
     * order before using them for calculations.
     * @param freqIn Double, variable passed by value.  THe frequency that you want to add to the list of wave
     * frequencies.  Units of radians per second (rad/s).
     */
    void addWaveFreq(double freqIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a new wave scaling multiplier to the list of scale multipliers.
     *
     * Each WaveSpec object has a wave scaling multiplier associated with it.  Those multipliers are stored in this
     * list.  The multipliers are a quick way to take a single direction wave spectrum and distribute across multiple
     * wave directions.
     *
     *  You do not need to add entries in sequential order.  The class automatically sorts the entries into sequential
     * order before using them for calculations.
     * @param scaleIn Double, variable passed by value.  The wave scale mutliplier that you want to add to the list
     * of multipliers.
     */
    void addWaveScale(double scaleIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name used to identify this specific Sea Model.
     *
     * Purely for user benefit.  Can be any string.
     * @return Returns a string, passed by value.  String is the name of the Sea Model used to designate this
     * specific object.
     */
    std::string getName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name used to identify this specific Sea Model.
     *
     * Purely for user benefit.  Can be any string.
     * @param nameIn String variable, passed by value.  String is the name of the Sea Model used to designate this
     * specific object.
     */
    void setName(std::string nameIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name used to identify this specific Sea Model.
     *
     * Purely for user benefit.  Can be any string.
     * @return Returns a string, passed by reference.  String is the name of the Sea Model used to designate this
     * specific object.
     */
    std::string &refName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the wave energy from the sea model for the wave direction and wave frequency specified.
     *
     * For each requested result, the SeaModel interpolates between the defined spectra for each wave direction to
     * get a resulting wave energy at the specified wave direction.  The wave direction does not need to match the
     * list of directions supplied with the WaveSpec objects.  That is the flexibility of the SeaModel.  You provide
     * it with a data set, and it provides results for any given point within that data set.
     * @param dirIn Double, variable passed by value.  The wave direction for the requested result.  Direction
     * measured in units of radians, in the global coordinate system.  True North represents a direction of zero.
     * Positive angles are measured counter-clockwise (North to West).
     * @param freqIndex The index of the frequency you want to use in the list of wave frequencies.  THe list of wave
     * frequencies must be defined before calling this function.  You use the index to specify the requested frequency.
     * @return Returns a double, variable passed by value.  Variable is the wave energy (in units of m^2/(rad/s)) at
     * the frequency and wave direction specified.
     */
    virtual double getWaveEnergy(double dirIn, int freqIndex);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the wave energy from the sea model for the wave direction and wave frequency specified.
     *
     * For each requested result, the SeaModel interpolates between the defined spectra for each wave direction to
     * get a resulting wave energy at the specified wave direction.  The wave direction does not need to match the
     * list of directions supplied with the WaveSpec objects.  That is the flexibility of the SeaModel.  You provide
     * it with a data set, and it provides results for any given point within that data set.
     * @param dirIn Double, variable passed by value.  The wave direction for the requested result.  Direction
     * measured in units of radians, in the global coordinate system.  True North represents a direction of zero.
     * Positive angles are measured counter-clockwise (North to West).
     * @param freqIn Double, variable passed by value.  The wave frequency desired.  Can be any wave frequency.
     * This stipulation only works for requesting wave energy.  If you want a wave amplitude, you must use the method
     * of specifying frequency by the index.
     * @return Returns a double, variable passed by value.  Variable is the wave energy (in units of m^2/(rad/s)) at
     * the frequency and wave direction specified.
     */
    virtual double getWaveEnergy(double dirIn, double freqIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the wave amplitude from the sea model for the wave direction and wave frequency specified.
     *
     * For each requested result, the SeaModel interpolates between the defined spectra for each wave direction to
     * get a resulting wave energy at the specified wave direction.  The wave direction does not need to match the
     * list of directions supplied with the WaveSpec objects.  That is the flexibility of the SeaModel.  You provide
     * it with a data set, and it provides results for any given point within that data set.
     * @param dirIn Double, variable passed by value.  The wave direction for the requested result.  Direction
     * measured in units of radians, in the global coordinate system.  True North represents a direction of zero.
     * Positive angles are measured counter-clockwise (North to West).
     * @param freqIndex The index of the frequency you want to use in the list of wave frequencies.  THe list of wave
     * frequencies must be defined before calling this function.  You use the index to specify the requested frequency.
     * @return Returns a double, variable passed by value.  Variable is the wave amplitude (in units of m) at the
     * frqeuency and wave direction specified.
     */
    virtual double getWaveAmp(double dirIn, int freqIndex);

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    std::vector<double> plistWaveFreq; /**< The list of wave frequencies. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave directions.
     *
     * Wave directions are defined in units of radians, using the global coordinate system.  A direction of 0 rad
     * points to True North.  Wave directions proceed counter-clockwise (from North to West).
     */
    std::vector<double> plistWaveDir;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave spectra.
     *
     * Each WaveSpec object in the corresponds to the wave direction for the same index in the pWaveDir list.  Each
     * entyr in the WaveSpec list is a pointer to a WaveSpec object.  Be warned that multiple SeaModel objects may
     * reference the same WaveSpec object, so don't delete a WaveSpec object just because the SeaModel is done using
     * it.
     */
    std::vector<osea::WaveSpecBase*> plistWaveSpec;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of scalar multiples to apply to the wave spectra.
     *
     * Each wave spectra is multipled by a scalar multiple.  This can be an easy way of adjusting wave spectra for
     * different wave directions.  Each wave spectra must have a corresponding wave scale entry.  The default
     * value is to have a wave scale value of 1.0, which does not affect the wave spectra.
     */
    std::vector<double> plistWaveScale;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Tracks if the list of wave frequencies was correctly sorted.
     */
    bool SortedFreq;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Tracks if the list of wave directions was correctly sorted.
     */
    bool SortedDir;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name used to identify this specific sea model.
     *
     * Purely for user benefit.  Can be any string.
     */
    std::string pName;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sorts the list of wave directions and puts them in sequential, ascending order.
     *
     * This also sorts the
     * list of wave spectra to ensure the correct wave directions still correspond to the same wave spectra.
     */
    void SortWaveDir();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sorts the list of wave frequencies and puts them in sequential, ascending order.
     *
     * This sorting must be done before you request a wave amplitude.
     */
    void SortWaveFreq();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the frequency step in the list of wave frequencies.
     *
     * The frequency step is the spacing between wave frequencies.  This calculation is critical for calculating
     * wave amplitude.
     * @param index Integer.  Variable passed by value.  The index is used to specify the wave frequency that you
     * want.  This is selected from the list of wave frequencies.
     * @return Returns double, variable passed by value.  Returned variable is the frequency step for the requested
     * wave frequency.
     */
    double getFreqStep(int index);

//==========================================Section Separator =========================================================
private:


};

} //Namespace osea

#endif // SEAMODEL_H
