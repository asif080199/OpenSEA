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
 *Aug 30 2014       Nicholas Barczak	Initially Created
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
//Put header file includes and constant variables in here.
#ifndef DICTCROSSDAMP_H
#define DICTCROSSDAMP_H
#include "dictcrossreact.h"

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
 * The dictCrossDamp class interprets the input file crossdamp.out
 *
 * The crossdamp.out input file is a set of crossbody forces, associted with a specific order of derivative.
 * All of these input files have exactly the same format.  They are just associated with a different order of
 * derivative.  A single static variable changes for each of the children classes.  Everything else remains the same.
 * The dictCrossReact class contains all the function definitions for how to interpret the input files.
 *
 * The only thing added in this class is the order of derivative associated with the input file.  This is set through
 * a single static variable.  ORD = 1 for this class.
 *
 * @sa Dictionary
 * @sa FileReader
 * @sa HydroReader
 */
class dictCrossDamp : public dictCrossReact
{
    Q_OBJECT
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.
     */
    dictCrossDamp();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Constructor that also passes pointer to the owning file reader.
     * @param parent Pointer to HydroReader object, pointer passed by value.
     */
    dictCrossDamp(osea::HydroReader *parent);

//==========================================Section Separator =========================================================
signals:


//==========================================Section Separator =========================================================
public slots:


//==========================================Section Separator =========================================================
protected:


//==========================================Section Separator =========================================================
private:

    //------------------------------------------Function Separator ----------------------------------------------------
    //Class Name constants


    //------------------------------------------Function Separator ----------------------------------------------------
    //Keyword name constants


};

}   //Namespace osea

#endif // DICTCROSSDAMP_H
