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
 *May 15, 2013	Shane Honanie       Initially created
 *Aug 02, 2013  Nicholas Barczak    Added operator overloads and more consistent matrix definition of forces.
 *                                  Also added methods for derivative access.
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
#ifndef MATREACTFORCE_H
#define MATREACTFORCE_H
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

typedef std::complex<double> complexDouble;

//######################################### Class Separator ###########################################################
/**
 * This class holds data for reactive force matrix whch includes force coefficients.
 */

class matForceReact : public osea::ofreq::oFreqCore
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    matForceReact(); /**< The default constructor. */

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
     * @brief The constructor.  Takes a vector of complex matrices and stores them as derivatives.
     *
     * The constructor.  Takes a vector of complex matrices and stores them as derivatives.  Assumes that the matrices
     * in the vector are order in sequence of increasing order of derivative. (index 0 = derivative order 0.)
     * @param forceIn The list of forces.
	 */
    matForceReact(std::vector<arma::cx_mat> forceIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    virtual ~matForceReact(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Operator overload to add two matForceReact objects together.
     *
     * This overloads the + operator to add two matForceReact objects together.  Functions are added on a per-derivative
     * basis.  The function recognizes the derivative matrices contained within each object.  Only derivatives of the
     * same order are added together.
     * @param forceOther The other objects of type matForceReact that will be added.
     * @return Returns an object of type matForceReact.  The new object will contain the same order of derivatives as
     * the highest derivative of the two added functions.
     */
    virtual matForceReact operator+(matForceReact& forceOther);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Operator overload to subtract two matForceReact objects together.
     *
     * This overloads the - operator to subtract two matForceReact objects together.  Functions are subtracted on a
     * per-derivative basis.  The function recognizes the derivative matrices contained within each object.
     * Only derivatives of the same order are subtracted together.  Order of operations does matter.
     * @param forceOther The other objects of type matForceReact that will be subtracted.  forceOther is always
     * subtracted from the calling object.
     * @return Returns an object of type matForceReact.  The new object will contain the same order of derivatives as
     * the highest derivative of the two subtracted functions.
     */
    virtual matForceReact operator-(matForceReact& forceOther);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Scalar multiplication with the ForceReact array.
     *
     * multiplied to each individual element in the array.
     * @param scalar Double.  Variable passed by value.  The scalar that should be multiplied to each element in the
     * array
     * @return Returns a matForceReact object.  Variable passed by value.
     */
    matForceReact operator*(double scalar);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Scalar division with the ForceReact array.
     *
     * Divided to each individual element in the array.
     * @param scalar Double.  Variable passed by value.  The scalar that should be divided to each element in the
     * array
     * @return Returns a matForceReact object.  Variable passed by value.
     */
    matForceReact operator/(double scalar);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The maximum order of the derivatives.
     *
     * The maximum order of the derivatives (Integer).  Also the total size of the vector containing the derivatives.
     * @return Returns the maximum order of derivatives in the force.
     */
    int getMaxOrder();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Derivative Returns the complex matrix for only the order of derivative specified.
     *
     * Derivative Returns the complex matrix for only the order of derivative specified.
     * @param order Integer input to specify the order of the derivative.
     * @return Returns a complex matrix that contains the force coefficients for the given order of derivative.  Passed
     * as a value.
     */
    arma::cx_mat getDerivative(int order);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Inputs a derivative matrix
     * @param order The order of the derivative matrix.  Also is sequence in the vector that contains the matrices.
     * @param Coeff The matrix of complex numbers that contains the force coefficients for the derivative.  Passed as a
     * value, not a reference.
     */
    void setDerivative(unsigned int order, arma::cx_mat Coeff);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the vector of derivatives.
     *
     * Provides direct access to the vector of derivatives.  Allows for use of vector operations on the derivatives
     * object.
     * @return Returns reference to the vector of complex matrices which contain the derivatives.  Variable passed by
     * reference.
     */
    std::vector<arma::cx_mat> &listDerivative();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the derivative specified by the index.
     *
     * Allows for direct access to edit the derivative or just retrieve information from.  Index is also the order
     * of the derivative.
     * @param index Unsigned integer.  Specifies the index of which derivative to retrieve from the list.
     * @return Complex matrix returned.  Returns the complex matrix for the derivative specified by the index.
     * Returned variable is passed by reference.
     */
    arma::cx_mat &listDerivative(unsigned int index);

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
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Defines the vector of derivatives.
     *
     * Defines the vector of derivatives.  Each entry in vector represents the order of the derivative.
     */
    std::vector<arma::cx_mat> pderiv;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief the number of the object in the outside vector that contains it.
     *
     * This is similar to the name parameter in other force objects.  It is an identifier.  In this case, a numerical
     * identifier.  Normally correlates to the objects index in a vector of other objects of the same class.
     */
    int pId;

//==========================================Section Separator =========================================================
private:


};

}   //Namespace ofreq
}   //Namespace osea

#endif
