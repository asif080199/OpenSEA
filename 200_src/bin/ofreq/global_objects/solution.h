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
 *Aug 10, 2013	Nicholas Barczak		Initially Created
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
#ifndef SOLUTION_H
#define SOLUTION_H
#include <QtGlobal>
#ifdef Q_OS_WIN
    #include "armadillo.h"  //References the armadillo library in lib folder.
#elif defined Q_OS_LINUX
    #include <armadillo>    //Armadillo library included with standard system libraries.
#endif

using namespace std;
using namespace arma;

//######################################### Class Separator ###########################################################
//Prototype class declarations.
class Body;

//######################################### Class Separator ###########################################################
/**
 * This class defines a solution object.  The solution object records the basic value of motion solution.  The motion
 * solution is translated back into body coordinate system.
 */

class Solution
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.
     *
     * Default constructor.  Nothing done here.
     */
    Solution();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Creates the object and sets the reference body for the object.
     *
     * Creates the object and sets the reference body for the object.
     * @param bodIn The reference body.  Variabled passed by reference.
     */
    Solution(Body &bodIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.
     *
     * Default destructor.  Nothing done here.
     */
    ~Solution();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns direct access to the solution matrix.
     *
     * Returns direct access to the solution matrix.  Provides a pointer to the solution matrix.
     * It gets filled with the output from the motion solver.  Output is a column
     * matrix (n by 1) of complex numbers.  Output is in units of meters.
     * @return Returns direct access to the solution matrix.  Provides a pointer to the solution matrix.  Returned
     * variable is passed by reference.
     */
    cx_mat &refSolnMat();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the solution matrix.
     *
     * It gets filled with the output from the motion solver.  Output is a column
     * matrix (n by 1) of complex numbers.  Output is in units of meters.
     *
     * Returns the solution matrix as a whole.
     * @return Returned value is a complex number matrix.  Returned variable is passed by value.
     */
    cx_mat getSolnMat();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a single entry in the solution matrix.  Entry specified by index.
     * @param index The index of the entry to retrieve from the solution matrix.
     * @return Returns a complex number data type.  Single value from the solution matrix.  Value specified by the
     * index input.  Variable passed by value.
     */
    complex<double> getSolnMat(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the solution matrix as a whole.
     * @param matIn The input matrix to set as the solution matrix.
     */
    void setSolnMat(cx_mat matIn);

//    //------------------------------------------Function Separator ----------------------------------------------------
//    /**
//     * @brief Gets the body object used as the reference body.  Variable passed by value.
//     * @return Gets the body object used as the reference body.  Variable passed by value.
//     */
//    Body getBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns reference pointer to the reference body.  Variable passed by reference.
     *
     * Returns reference pointer to the reference body.  Variable passed by reference.  Used for direct access to the
     * reference body and all its member functions.
     * @return Returns reference pointer to the reference body.  Variable passed by reference.
     */
    Body &refBody();

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Private variable for the solution matrix.
     *
     * Private variable for the solution matrix.  Used to store the solution from the motion solver.  This variable is
     * initially empty on body creation.  It gets filled with the output from the motion solver.  Output is a column
     * matrix (n by 1) of complex numbers.  Output is in units of meters.
     */
    cx_mat pSoln;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the body object associated with the solution object.
     *
     * Pointer to the body object associated with the solution object.  Useful if you need to find information about
     * the body associated with the solution, such as coordinate transforms.
     */
    Body* pBod;
};

#endif // SOLUTION_H
