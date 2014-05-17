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
 *May 14, 2014	Nicholas Barczak		Initially Created
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
#ifndef WAVESPEC_H
#define WAVESPEC_H
#include "../system_objects/ofreqcore.h"
#include <vector>
#include "interpolation.h"
#include <stdlib.h>
#include "stdafx.h"

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
 * @brief The WaveSpec class defines wave spectrums.
 *
 * Wave spectrums are essentially mathematical functions that define a relationship between wave frequency, and
 * wave energy.  The main use of a WaveSpec object is to ask for the wave spetral energy at a given wave frequency.
 * The WaveSpec class is the most basic definition.  In this class, the user provides tables of wave frequency and
 * wave energy.  The class then interpolates to return the wave energy at the requested frequency.
 *
 * Other classes inherit from the WaveSpec class.  This is just the most basic definition of the wave spectrum.
 */
class WaveSpec : public osea::ofreq::oFreqCore
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Nothing happens here.
     */
    WaveSpec();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.  Nothing happens here.
     */
    virtual ~WaveSpec();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the list of wave frequencies.
     *
     * Returned variable is passed by reference.  The wave frequencies are the global wave frequencies. (rad/s)
     * @return Returns a vector of doubles.  These are the wave frequencies that the spectrum is based on.  Returned
     * variable is passed by reference.
     */
    std::vector<double> &listWaveFreq();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a single wave frequency at the requested index.
     *
     * Returned variable is passed by reference.  Returns the wave frequency from the vector.  Returns variable at
     * specified index.
     * @param index Integer.  Variable passed by value.  The index from the vector of wave frequencies that should be
     * returned.
     * @return Returns the wave frequency from the vector, the single entry requested by index.  Returned variable is
     * a double, passed by reference.
     */
    double &listWaveFreq(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the list of wave frequencies. (rad/s)
     *
     * Sets the list of wave frequencies (rad/s).  Wave frequencies are based on global wave frequency.  Frequencies
     * are set by passing the entire vector at once.
     * @param freqIn Vector of doubles.  Variable passed by value.  The vector of wave frequencies.
     */
    void setWaveFreq(std::vector<double> freqIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the list of wave energy.
     *
     * Returned variable is passed by reference.  The wave energy are the global wave energy. (rad/s)
     * @return Returns a vector of doubles.  These are the wave energy that the spectrum is based on.  Returned
     * variable is passed by reference.
     */
    std::vector<double> &listWaveEnergy();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a single wave frequency at the requested index.
     *
     * Returned variable is passed by reference.  Returns the wave frequency from the vector.  Returns variable at
     * specified index.
     * @param index Integer.  Variable passed by value.  The index from the vector of wave energy that should be
     * returned.
     * @return Returns the wave frequency from the vector, the single entry requested by index.  Returned variable is
     * a double, passed by reference.
     */
    double &listWaveEnergy(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the list of wave energy. (rad/s)
     *
     * Sets the list of wave energy (rad/s).  wave energy are based on global wave frequency.  Frequencies
     * are set by passing the entire vector at once.
     * @param energyIn Vector of doubles.  Variable passed by value.  The vector of wave energy.
     */
    void setWaveEnergy(std::vector<double> energyIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the spectral energy for the frequency requested.
     *
     * Returns the spectral energy in units of m^2/(rad/s).  Returns value for the frequency requested.  The requested
     * frequency does not need to match any of the frequencies included in the input data.  The function automatically
     * interpolates for the frequency requested.  The function will chose from a variety of interpolation methods,
     * depending on how many data points are available.  The following lists the decision matrix for interpolation
     * methods.
     * 1 data point:        Always return that data point.
     * 2 data points:       Linear interpolation.
     * 3 data points:       Cubic spline interpolation.
     * 5+ data points:      Akima spline.
     * @param freq Double.  Variable passed by value.  The wave frequency that you want to retrieve the spectral
     * energy for.  Frequency should be global wave frequency, in units of rad/s.
     * @return Returns a double, passed by value.  Returns the spectral wave energy (m^2/(rad/s)) at the requested
     * wave frequency.
     */
    virtual double getSpecEnergy(double freq);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name used to identify this specific wave spectra.
     *
     * Purely for user benefit.  Can be any string.
     * @return Returns a string, passed by value.  String is the name of the wave spectra used to designate this
     * specific object.
     */
    std::string getName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name used to identify this specific wave spectra.
     *
     * Purely for user benefit.  Can be any string.
     * @param nameIn String variable, passed by value.  String is the name of the wave spectra used to designate this
     * specific object.
     */
    void setName(std::string nameIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name used to identify this specific wave spectra.
     *
     * Purely for user benefit.  Can be any string.
     * @return Returns a string, passed by reference.  String is the name of the wave spectra used to designate this
     * specific object.
     */
    std::string &refName();


//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    std::vector<double> pWaveFreq; /**< The list of wave frequencies. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave spectral energy.
     *
     * Corresponds to the list of wave frequencies.
     */
    std::vector<double> pWaveEnergy;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The mathematical spline constructed for interpolation of the wave spectrum data.
     */
    alglib::spline1dinterpolant pSpecCurve;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Constructs the interpolation spline, based on how many data points are available.
     *
     * The function uses the index to determine if there are enough data points on either side of the requested
     * frequency.  This does assume that the wave frequency list is sequentially ordered.  Technically, the spline
     * interpolation functions do not need this to construct the spline, but oFreq needs this to ensure enough
     * data points exist.
     * @param freqIn double.  Specifies the wave frequency requested.  The class uses this
     * to determine which spline interpolation function it can use.
     */
    void ConstructSpline(double freqIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Boolean to track if the spline function was constructed.  Initially set to false.
     */
    bool SplineReady;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name used to identify this specific wave spectra.
     *
     * Purely for user benefit.  Can be any string.
     */
    std::string pName;

//==========================================Section Separator =========================================================
private:

};

} //Namespace osea

#endif // WAVESPEC_H
