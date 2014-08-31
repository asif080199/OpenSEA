/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date          Author				Description
 *---------------------------------------------------------------------------------------------------------------------
 *Aug 30, 2013  Nicholas Barczak    Initially created
 *May 7, 2014       Nicholas Barczak    Debugged and updated to Rev 1.0
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
#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QObject>
#include "objectgroup.h"
#include "../system_objects/system.h"
#include "../system_objects/ofreqcore.h"
#include <complex>
#include <vector>
#include <string>

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
//class System;       /** < Prototype class declaration of System object.  Allows reference to system object.*/
class FileReader;   /**< Prototype class declaration of FileReader.  Allows reference to parent object.*/


//######################################### Class Separator ###########################################################
/**
 * This is a virtual class definition, inheritted by each fileDictionary object.  Contains the basic functions for
 * how to recursively progress through the definitions for an ObjectGroup object that is fed in.
 * @sa ObjectGroup
 */

class Dictionary : public QObject, public osea::ofreq::oFreqCore
{
    Q_OBJECT

//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    explicit Dictionary(QObject *parent);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.
     */
    Dictionary();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Constructor that also passes pointer to the owning file reader.
     * @param parent Pointer to FileReader object, pointer passed by value.
     */
    Dictionary(osea::FileReader *parent);

//==========================================Section Separator =========================================================
signals:


//==========================================Section Separator =========================================================
public slots:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Public signal for the ObjectGroup object that is sent to the Dictionary object for procesing.
     * @param input The ObjectGroup object that contains the class definitions.  Variable passed by value.
     */
    virtual void setObject(ObjectGroup input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the system object for the dictionary to reference.
     * @param ptSystem Pointer to the System object.  Variable passed by value.
     */
    virtual void setSystem(ofreq::System* ptInput);

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pure virtual function that defines how to interpret the key values.  Contains a list of key names and
     * corresponding actions to take for interpretting each key value.
     * @param keyIn std::string containing the key name.  Variable passed by value.
     * @param valIn Vector of strings containing the key values.  Variable passed by value.
     * @return Returns status of assigning key.  Returned value is an integer, passed by value.
     * See list of return codes below:
     * 0:  Key definition found.  Success.
     * 1:  No key found. / General error message.
     * 2:  Key is invalid within current active object.
     * 99: Function virtual definition only.  Not currently defined.
     */
    virtual int defineKey(std::string keyIn, std::vector<std::string> valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pure virtual function that defines how to interpret the class name.  Class name implies declaration of
     * a new object of the class named by the class name.  This is a separate set of definitions to handle class
     * declarations.
     * @param nameIn std::string, variable passed by value.  The name of the class name.
     * @return Returns status of assigning key.  Returned value is an integer, passed by value.
     * See list of return codes below:
     * 0:  Key definition found.  Success.
     * 1:  No key found. / General error message.
     * 2:  Key is invalid within current active object.
     * 99: Function virtual definition only.  Not currently defined.
     */
    virtual int defineClass(std::string nameIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Converts a std::string of a complex number into a complex object (double base type) i.e. std::complex<double>.
     * @param input The std::string which holds the complex number.  Valid input formats are:
     * 1.00+1.00i
     * 1.00-1.00i
     * 1.00+i1.00
     * 1.00-i1.00
     * 1.414<0.785398 (angle must be in radians)
     * 1.414<-0.785398 (angle must be in radians)
     * @return Returns a std::complex<double> object.  Variable passed by value.
     */
    std::complex<double> convertComplex(std::string input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the System object.  Used to reference any important variables in the System object.
     */
    ofreq::System* ptSystem;

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the parent FileReader object.  Sometimes useful if you need to use the parent object.
     */
    osea::FileReader *pParent;

};

}   //Namespace osea

#endif
