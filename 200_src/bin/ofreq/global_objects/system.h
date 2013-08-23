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
 *May 15 2013       Shane Honanie       Initially created
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
#ifndef SYSTEM_H
#define SYSTEM_H
#include "wavedirections.h"
#include "wavefrequencies.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

//######################################### Class Separator ###########################################################
/**
 * This class holds data for the system object.  The system object controls the overall behavior of the program.
 * It also decides which analysis type to run:  motion or resonant frequency.  The system object controls the current
 * wave environment settings.
 */

class System
{

//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
	System(); /**< The default constructor. */

    //------------------------------------------Function Separator ----------------------------------------------------
	~System(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
	void testPrint(); /**< Test print to console the values of all data members. */

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the analysis ype.
	 * @param analysisTypeIn The analysis type.
	 */
	void setAnalysisType(string);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the wave frequencies.
	 * @param vecIn The list of wave frequencies.
	 */
	void setWaveFrequencies(vector<double>);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the wave directions.
	 * @param vecIn The list of wave directions.
	 */
	void setWaveDirections(vector<double>);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the spread model.
	 * @param spreadIn The spread model.
	 */
	void setSpreadModel(string);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Retrieve the list of wave frequencies.
	 * @return The list of wave frequencies.
	 */
	vector<double> getWaveFrequencies();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of wave frequencies.
     * @return Pointer to the list of wave frequencies.  Variable passed by reference.
     */
    vector<double> &refWaveFrequencies();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Retrieve the list of wave directions.
	 * @return The list of wave directions.
	 */
	vector<double> getWaveDirections();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of wave directions.
     * @return Pointer to the list of wave directions  Variable passed by reference.
     */
    vector<double> &refWaveDirections();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the current wave index.
     * @param input Integer specifying the index of the current wave direction in the list of wave directions.
     */
    void setCurWaveDirInd(int input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the current frequency index.
     * @param input Integer specifying the index of the current wave frequency in the list of wave frequencies.
     */
    void setCurFreqInd(int input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the current wave direction index.
     * @return Integer.  Returns the current wave direction index.  Variable passed by value.
     */
    int getCurWaveDirInd();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the current wave frequency index.
     * @return Integer.  Returns the current wave frequency index.  Variable passed by value.
     */
    int getCurFreqInd();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the current wave direction.  Actual value of wave direction angle.  Angle specified as radians
     * with zero as True North, positive counter-clockwise.
     * @return Double.  Returns the current wave direction.  Actual value of wave direction angle.  Angle specified as
     * radians with zero as True North, positive counter-clockwise.  Variable passed by value.
     */
    double getCurWaveDir();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the current wave frequency.  Actual value of the wave frequency.  Value specified with units of
     * radians per second.
     * @return Double.  Returns the current wave frequency.  Actual value of the wave frequency.  Value specified with
     * units of radians per second.
     */
    double getCurFreq();

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
	string analysisType; /**< The analysis type. */

    //------------------------------------------Function Separator ----------------------------------------------------
	WaveDirections waveDirections; /**< The wave directions object. */

    //------------------------------------------Function Separator ----------------------------------------------------
	WaveFrequencies waveFrequencies; /**< The wave frequencies object. */

    //------------------------------------------Function Separator ----------------------------------------------------
    int pCurWaveDir; /**< The index of the current wave direction. */

    //------------------------------------------Function Separator ----------------------------------------------------
    int pCurWaveFreq; /**< The index of the current wave frequency. */
};
#endif

