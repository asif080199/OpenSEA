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
 *Jun 7, 2014	Nicholas Barczak		Initially Created
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
#ifndef SPECPM_H
#define SPECPM_H
#include "wavespecbase.h"

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
 * @brief The Pierson-Moskowitz Spectrum
 *
 * This is a wave spectrum based on the Pierson-Moskowitz formula.  It is a one parameter spectrum, and depends on
 * wind speed.  The spectrum uses the characteristic wind speed.  This spectrum assumes a wind speed measured at a
 * height of 10 m above mean sea level.  The original wave spectra calculated by Pierson and Moskowitz used wind
 * measurements at a height of 19.5 m above mean sea level.  The input wind speed was adjusted to account for this
 * difference.
 */
class SpecPM : public WaveSpecBase
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Nothing happens here.
     */
    SpecPM();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.  Nothing happens here.
     */
    ~SpecPM();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the spectral energy for the frequency requested.
     *
     * Returns the spectral energy in units of m^2/(rad/s).  Returns value for the frequency requested.  Calculates
     * spectral energy based on the Pierson-Moskowitz spectrum.
     * @param freq Double, passed by value.  The wave frequency that you want the spectral energy for, specified in
     * units of rad/s.
     * @return Returns a double, passed by value.  Returned variable is the wave spectral energy for the requested
     * wave frequency, in units of m^2/(rad/s).
     */
    double getSpecEnergy(double freq);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the wind speed for the Pierson-Moskowitz spectrum.
     *
     * Wind speed is assumed set at a height of 10.0 m.  The function automatically corrects wind speed to the
     * function value of wind speed at 19.5 m height.
     * @param speedIn Double, passed by value.  The wind speed, in units of m/s.  Set at a height of 10.0 m above
     * mean sea level.
     */
    void setWindSpeed(double speedIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the wind speed.
     *
     * Gets the wind speed at a height of 10.0 m above mean sea level.
     * @return Returns a double, passed by value.  Returned variable is the wind speed, set at a height of 10.0 m
     * above mean sea level.
     */
    double getWindSpeed();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the significant wave height.
     *
     * Bases the Pierson-Moskowitz spectrum on signficant wave height.
     *
     * @param heightIn Double, passed by value.  The significant wave height to set.  In units of m.
     */
    void setSigWaveHeight(double heightIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the significant wave height.
     * @return Double, passed by value.  The signficant wave height, in units of m.
     */
    double getSigWaveHeight();

//==========================================Section Separator =========================================================
protected:


//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    double pWindSpeed;  /**< The wind speed, at 10.0 m above sea level.  Units of m/s */

    //------------------------------------------Function Separator ----------------------------------------------------
    double pHsig;   /**< The significant wave height.  Can be used in place of wind speed. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The correction factor to convert from wind speed at 10.0 m above sea level to 19.5 m above sea level.
     */
    static double CORRECTION;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Constant used in the formula.  The Philip constant.  Independent of wind speed and wind fetch.
     */
    static double ALPHA;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Constant used in calculating the wave spectra.
     */
    static double BETA;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the characteristic wind speed based on significant wave height.
     * @param HsIn Double, variable passed by value.  The significant wave height to use as input, in units of m.
     * The default value for HsIn is 0.  If left at the default value, the function will automatically use the wave
     * height stored in the object.
     * @return Returns a double, passed by value.  Returned variable is the characteristic wind speed, in units of
     * m/s.
     */
    double Uw_HSig(double HsIn = 0);

};

} //Namespace osea

#endif // SPECPM_H
