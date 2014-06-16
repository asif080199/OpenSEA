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
 *Jun 14, 2014  Nicholas Barczak        Initially created.
 *Jun 15, 2014  Nicholas Barczak        Debugged and updated to Rev 1.0
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
#ifndef SEAMODEL_LONGCREST_H
#define SEAMODEL_LONGCREST_H
#include "seamodel.h"


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
 * @brief The SeaModel_LongCrest class creates a single wave spectra that acts in only one direction.
 *
 * This SeaModel implements long crested waves.  Long crested waves are a wave spectrum that only acts in one
 * direction.  There are no waves in any other direction.  This SeaModel is useful for evaluating bodies in an
 * idealized setting.
 *
 * The SeaModel produces two primary results:
 * 1.  Wave energy at a specified wave direction and wave frequency.
 * 2.  Wave amplitude at a specified wave direction and wave frequency.
 *
 * You can also define a wave scaling parameter for the wave spectrum.  This may be necessary if you want to scale
 * the results somehow.
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
class SeaModel_LongCrest : public SeaModel
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Inherits from SeaModel class.
     */
    SeaModel_LongCrest();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.  Nothing happens here.
     */
    ~SeaModel_LongCrest();

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
    double getWaveEnergy(double dirIn, int freqIndex);

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
    double getWaveEnergy(double dirIn, double freqIn);

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
    double getWaveAmp(double dirIn, int freqIndex);

//==========================================Section Separator =========================================================
protected:


//==========================================Section Separator =========================================================
private:


};

}   //  Namespace osea

#endif // SEAMODEL_LONGCREST_H
