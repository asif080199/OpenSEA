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
#ifndef WAVESPECBASE_H
#define WAVESPECBASE_H
#include "../system_objects/ofreqcore.h"
#include <stdlib.h>

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
 * @brief The base class for wave spectra.  Contains only the most basic function definitions and properties.
 */
class WaveSpecBase : public osea::ofreq::oFreqCore
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The default constructor.  Nothing happens here.
     */
    WaveSpecBase();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The default destructor.  Nothing happens here.
     */
    virtual ~WaveSpecBase();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the spectral energy for the frequency requested.
     *
     * Returns the spectral energy in units of m^2/(rad/s).  Returns value for the frequency requested.  This is a
     * pure virtual function.  The exact method of calculation for wave spectral energy depends on the child class.
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

#endif // WAVESPECBASE_H
