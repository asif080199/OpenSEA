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
 *May 15, 2013  Shane Honanie       Initially created.
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
#ifndef MATACTIVEFORCE_H
#define MATACTIVEFORCE_H
#include <complex>
#include <QtGlobal>
#ifdef Q_OS_WIN
    #include "../../lib/armadillo.h"  //References the armadillo library in lib folder.
#elif defined Q_OS_LINUX
    #include <armadillo>    //Armadillo library included with standard system libraries.
#endif
#include "../system_objects/ofreqcore.h"

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
 * This class holds all data for an active force matrix.
 */

class matForceActive : public osea::ofreq::oFreqCore
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    matForceActive(); /**< The default constructor. */

    //------------------------------------------Function Separator ----------------------------------------------------
    ~matForceActive(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Operator overload to add two matForceActive objects together.
     *
     * Object entries are added on a coefficient basis.  Only coefficients of the same index are added together.
     * @param forceOther The other matForceActive object that you wish to add.  Variable passed by value.
     * @return The result of the addition operation.
     */
    matForceActive operator+(matForceActive forceOther);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Operator overload to subtract two matForceActive objects together.
     *
     * Object entries are subtracted on a coefficient basis.  Only coefficients of the same index are subtracted.
     * @param forceOther The other matForceActive object that you wish to subtract.  Variable passed by value.
     * @return The result of the subtraction operation.
     */
    matForceActive operator-(matForceActive forceOther);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Scalar multiplication with the forceActive array.
     *
     * multiplied to each individual element in the array.
     * @param scalar Double.  Variable passed by value.  The scalar that should be multiplied to each element in the
     * array
     * @return Returns a matForceActive object.  Variable passed by value.
     */
    matForceActive operator *(double scalar);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Scalar division with the forceActive array.
     *
     * Divided to each individual element in the array.
     * @param scalar Double.  Variable passed by value.  The scalar that should be divided to each element in the
     * array
     * @return Returns a matForceActive object.  Variable passed by value.
     */
    matForceActive operator/(double scalar);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the coefficients matrix.
     *
     * Returns the coefficients matrix.
     * @return Returns the coefficients matrix.
     */
    arma::cx_mat &listCoefficient();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets all the coefficients for the matrix in one go.
     *
     * Takes an armadillo cx_mat object as input and records all those values as the matrix of coefficients.
     * @param input Armadillo cx_mat object, variable passed by value.  The matrix of coefficients  for the Active
     * force object.  Matrix should be Nx1.  (Unlimited number of rows, but only one column.)  Do not use an
     * armadillo column matrix.  Follow-on functions expect a full 2D matrix that just happens to have one column.
     */
    void setCoefficient(arma::cx_mat input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the force id number for the object.
     *
     * This is similar to the name parameter in other force objects.  It is an identifier.  In this case, a numerical
     * identifier.  Normally correlates to the objects index in a vector of other objects of the same class.
     * @param num The integer number to input as the objects integer id.
     */
    void setId(int num);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the force id number for the object.
     *
     * This is similar to the name parameter in other force objects.  It is an identifier.  In this case, a numerical
     * identifier.  Normally correlates to the objects index in a vector of other objects of the same class.
     * @return Returns the force id number, integer data type.
     */
    int getId();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the size of the matrix in each order of derivative.
     *
     * Returns the size of the matrix in each order of derivative.  Integer output type.
     * @return Returns the size of the matrix in each order of derivative.
     */
    int getMatSize();

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief the number of the object in the outside vector that contains it.
     *
     * This is similar to the name parameter in other force objects.  It is an identifier.  In this case, a numerical
     * identifier.  Normally correlates to the objects index in a vector of other objects of the same class.
     */
    int pId;

    //------------------------------------------Function Separator ----------------------------------------------------
    arma::cx_mat pCoeff; /**< Matrix of force coefficients. */
};

}   //Namespace ofreq
}   //Namespace osea

#endif

