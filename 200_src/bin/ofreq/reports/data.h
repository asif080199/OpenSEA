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
 *Nov 05, 2014	Nicholas Barczak		Initially Created
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
//Include statements and headers
#ifndef DATA_H
#define DATA_H
#include "../system_objects/ofreqcore.h"
#include "complex"

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

//==========================================Section Separator =========================================================
/**
 * The namespace of all code specifically associated with ofreq.
 */
namespace ofreq
{

//######################################### Class Separator ###########################################################
/**
 The Data object is a generic storage vector for indexed data.  Each data object has a wave frequency index
 associated with it.  Inside the data object is a vector of values.  This can be only a single value, or multiple
 values.  Internally, the data object stores all data as complex variables.  But the user can add or retried data in
 multiple other data formats.  The object handles all variable type conversions.
 */

class Data : public ofreq::oFreqCore
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default object constructor.
     */
    Data();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Constructor that assigns data index as well.
     * @param IndexIn Integer, variable passed by value.  The index that will be associated with this data object.
     */
    Data(int IndexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Get the data index assigned to this data object.
     * @return Returns integer, passed by value.  The index that is associated with this data object.
     */
    int getIndex();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Set the data index assigned to this data object.
     * @param IndexIn Integer, variable passed by value.  The index that will be associated with this data object.
     */
    void setIndex(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Return the full vector list of values associated with this object.
     * @return Returns a vector of complex variables, variable passed by reference.
     */
    std::vector<std::complex<double> > &listValue();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Return a single entry from the vector list of values associated with this object.
     * @param indexIn Integer, variable passed by value.  The index of the desired value from the list of values.
     * This is the index of the entry in the list of values, not the index of the data object itself.
     * @return Returns complex variable, passed by reference.  This is the entry from the list of values, for the index
     * requested.
     */
    std::complex<double> &listValue(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Return a single entry from the vector list of values associated with this object.
     * @param indexIn Integer, variable passed by reference.  The index of the desired value from the list of values.
     * This is the index of the entry in the list of values, not the index of the data object itself.
     * @return Returns integer variable, passed by value.  This is the entry from the list of values, for the index
     * requested.
     */
    int listValueInt(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Return a single entry from the vector list of values associated with this object.
     * @param indexIn Integer, variable passed by reference.  The index of the desired value from the list of values.
     * This is the index of the entry in the list of values, not the index of the data object itself.
     * @return Returns double variable, passed by value.  This is the entry from the list of values, for the index
     * requested.
     */
    double listValueDouble(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Clears the list of values associated with this data object.
     */
    void clearValue();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a value to the list of values associated with this data object.
     * @param valIn Complex variable, passed by value.  The value that you want to add to the list of values.
     */
    void addValue(std::complex<double> valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a value to the list of values associated with this data object.
     * @param valIn Double variable, passed by value.  The value that you want to add to the list of values.
     */
    void addValue(double valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a value to the list of values associated with this data object.
     * @param valIn Integer variable, passed by value.  The value that you want to add to the list of values.
     */
    void addValue(int valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a string entry to the list of Strings in the Data object.
     * @param valIn String variable, variable passed by value.  The string that you want to add to the list.
     */
    void addString(std::string valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of String data values in the Data object.
     * @return Vector of String data types, variable passed by reference.
     */
    std::vector<std::string> &listString();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct acces to a single entry in the list of String data values in the Data object.
     * @param indexIn Integer, variable passed by value.  The index of the String entry that you want to access.
     * @return Returns single entry from the vector of String data types.  Variable passed by reference.
     */
    std::string &listString(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the format of data for the Data object.
     *
     * This variable tells outside functions how to expect the data
     * to be retrieved.  There are different retrieval functions, based on expected data type.  This variable just
     * tells which data type to expect.  Possible values:
     * 0:   Complex<double>
     * 1:   Double
     * 2:   Int
     * 3:   String
     * @return Returns integer, variable passed by value.  Possible values from 0 to 3, for different data types.  A
     * value of -1 indicates an error in the returned result.
     */
    int getDataType();

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of values associated with this data object.
     */
    std::vector<std::complex<double> > plistValue;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The index associated wiht this data object.
     */
    int pIndex;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The vector of string variable associated with the Data.  Sometimes you want to store a list of names
     * or other string objects.  This exists in the Data object for that purpose.
     */
    std::vector<std::string> plistString;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The data type associated with this Data object.
     *
     * This variable tells outside functions how to expect the data
     * to be retrieved.  There are different retrieval functions, based on expected data type.  This variable just
     * tells which data type to expect.  Possible values:
     * 0:   Complex<double>
     * 1:   Double
     * 2:   Int
     * 3:   String
     */
    int pDataType = -1;
};

}   //Namespace ofreq

}   //Namespace osea

#endif // DATA_H
