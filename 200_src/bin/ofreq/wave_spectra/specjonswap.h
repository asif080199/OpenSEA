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
#ifndef SPECJONSWAP_H
#define SPECJONSWAP_H
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
 * @brief The SpecJONSWAP class.  This defines the JONSWAP wave spectrum.  This spectrum offers two options:
 * You can define the bandwidth parameter (gamma) yourself.  Or, if left undefined, the bandwidth parameter will
 * default to the value specified by the ITTC.
 */
class SpecJONSWAP : public WaveSpecBase
{
//==========================================Section Separator =========================================================
public:

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The default constructor.  Sets the default value for the bandwidth parameter.
     */
    SpecJONSWAP();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The default destructor.  Nothing happens here.
     */
    ~SpecJONSWAP();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the spectral energy for the frequency requested.
     *
     * Returns the spectral energy in units of m^2/(rad/s).  Returns value for the frequency requested.  Calculates
     * spectral energy based on the Bretschneider spectrum.
     * @param freq Double, passed by value.  The wave frequency that you want the spectral energy for, specified in
     * units of rad/s.
     * @return Returns a double, passed by value.  Returned variable is the wave spectral energy for the requested
     * wave frequency, in units of m^2/(rad/s).
     */
    double getSpecEnergy(double freq);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the significant wave height.
     * @param heightIn Double, passed by value.  The significant wave height to set.  In units of m.
     */
    void setSigWaveHeight(double heightIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the significant wave height.
     * @return Double, passed by value.  The signficant wave height, in units of m.
     */
    double getSigWaveHeight();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the mean period for the wave spectrum, based on an input mean wave period.
     * @param valIn Double, variable passed by value.  The mean period for the wave spectrum, in units of s.
     */
    void setTMean(double valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the mean period for the wave spectrum.
     * @return Double, variable passed by value.  The mean period for the wave spectrum, in units of s.
     */
    double getTMean();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the mean period of the wave spectrum, based on an input peak period.
     *
     * The function converts peak wave period to mean wave period.  It then stores the converted mean wave
     * period.
     * @param valIn The peak wave period that you want to use as input.  Double, variable passed by value.
     */
    void setTPeak(double valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the mean period of the wave spectrum, based on an input zero crossing period.
     *
     * The function converts zero crossing wave period to mean wave period.  It then stores the converted mean wave
     * period.
     * @param valIn The zero crossing wave period that you want to use as input.  Double, variable passed by value.
     */
    void setTZero(double valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the spectrum bandwidth parameter.
     *
     * The spectrum bandwidth parameter determines how focused the energy will be in the JONSWAP spectrum.  A value
     * of 1.0 is a widely distributed energy spectrum.  Values of 3.3 - 6.0 are relatively concentrated, producing
     * a very peaky spectrum.  Value is also typically called gamma.
     *
     * @param valIn Double, variable passed by value.  The spectrum bandwidth parameter.
     */
    void setSpecBandParam(double valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the spectrum bandwidth parameter.
     *
     * The spectrum bandwidth parameter determines how focused the energy will be in the JONSWAP spectrum.  A value
     * of 1.0 is a widely distributed energy spectrum.  Values of 3.3 - 6.0 are relatively concentrated, producing
     * a very peaky spectrum.  Value is also typically called gamma.
     *
     * @return Double, variable passed by value.  The spectrum bandwidth parameter.
     */
    double getSpecBandParam();

//==========================================Section Separator =========================================================
protected:


//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    //Constants
    static double constA;   /**< The constant associated with the A variable in the JONSWAP spectrum.*/
    static double constB;   /**< The constant associated with the B variable in the JONSWAP spectrum.*/
    static double constParam;   /**< The constant in the front of the JONSWAP wave spectrum. */
    static double gamITTC;  /**< The ITTC Specified value for the wave bandwidth parameter, gamma. */
    static double GAM1; /**< The first value for gamma exponent in the JONSWAP spectrum.*/
    static double GAM2; /**< The second value for gamma exponent in the JONSWAP spectrum. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The mean wave period used in calculating the Bretschneider spectrum.
     */
    double pTMean;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The significant wave height used in calculating the spectrum.
     */
    double pHsig;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The spectrum bandwidth parameter.  Default value is gamITTC.
     */
    double pGamma;

};

}   //osea namespace

#endif // SPECJONSWAP_H
