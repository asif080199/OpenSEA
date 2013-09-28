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
#include "../global_objects/system.h"
#include <complex>
#include <vector>
#include <string>

using namespace std;

//######################################### Class Separator ###########################################################
//Prototype class declarations
//class System;       /** < Prototype class declaration of System object.  Allows reference to system object.*/


//######################################### Class Separator ###########################################################
/**
 * This is a virtual class definition, inheritted by each fileDictionary object.  Contains the basic functions for
 * how to recursively progress through the definitions for an ObjectGroup object that is fed in.
 * @sa ObjectGroup
 */

class Dictionary : public QObject
{
    Q_OBJECT

//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    explicit Dictionary(QObject *parent = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    Dictionary();

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
    virtual void setSystem(System* ptInput);

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pure virtual function that defines how to interpret the key values.  Contains a list of key names and
     * corresponding actions to take for interpretting each key value.
     * @param keyIn String containing the key name.  Variable passed by value.
     * @param valIn Vector of strings containing the key values.  Variable passed by value.
     * @return Returns status of assigning key.  Returned value is an integer, passed by value.
     * See list of return codes below:
     * 0:  Key definition found.  Success.
     * 1:  No key found. / General error message.
     * 2:  Key is invalid within current active object.
     */
    virtual int defineKey(string keyIn, vector<string> valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pure virtual function that defines how to interpret the class name.  Class name implies declaration of
     * a new object of the class named by the class name.  This is a separate set of definitions to handle class
     * declarations.
     * @param nameIn String, variable passed by value.  The name of the class name.
     * @return Returns status of assigning key.  Returned value is an integer, passed by value.
     * See list of return codes below:
     * 0:  Key definition found.  Success.
     * 1:  No key found. / General error message.
     * 2:  Key is invalid within current active object.
     */
    virtual int defineClass(string nameIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Converts a string of a complex number into a complex object (double base type) i.e. complex<double>.
     * @param input The string which holds the complex number.  Valid input formats are:
     * 1.00+1.00i
     * 1.00-1.00i
     * 1.00+i1.00
     * 1.00-i1.00
     * 1.414<0.785398 (angle must be in radians)
     * 1.414<-0.785398 (angle must be in radians)
     * @return Returns a complex<double> object.  Variable passed by value.
     */
    complex<double> convertComplex(string input);

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the System object.  Used to reference any important variables in the System object.
     */
    System* ptSystem;
    
};

#endif // DICTIONARY_H
