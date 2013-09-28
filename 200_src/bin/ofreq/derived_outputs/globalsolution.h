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
 *Aug 13, 2013      Nicholas Barczak    Updated to reflect new motion model and motion solver paradigm.
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
#ifndef GLOBALSOLUTION_H
#define GLOBALSOLUTION_H
#include "outputderived.h"
#include <QtGlobal>
#ifdef Q_OS_WIN
    #include "armadillo.h"  //References the armadillo library in lib folder.
#elif defined Q_OS_LINUX
    #include <armadillo>    //Armadillo library included with standard system libraries.
#endif
#include "../global_objects/solutionset.h"
#include "../global_objects/solution.h"

using namespace std;
using namespace arma;

typedef complex<double> complexDouble; /**< Simple typedef for complexDouble represents complex<double> */

//######################################### Class Separator ###########################################################
//Prototype class declarations

//######################################### Class Separator ###########################################################
/**
 * This class represents the Global Solution.  The Global Solution is the direct output of the solved values for each
 * body.  It provides the motions calculated for each body, translated back to body coordinate system.  The
 * GlobalSolution can output any desired derivative of the solved motions.  Several other child classes are derived
 * from this class.  The only difference is that for those other classes, the order of derivative is predefined.
 * @sa GlobalMotion
 * @sa GlobalVelocity
 * @sa GlobalAcceleration
 */

class GlobalSolution : public OutputDerived
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
	GlobalSolution(); /**< This default constructor creates a Global Solution object. */

    //------------------------------------------Function Separator ----------------------------------------------------
	~GlobalSolution(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the order of the derivative for the object.
     * @param ord Integer input that specifies the order of the derivative.  Value can be anything from 0 or larger.
     * Variable is passed by value.
     */
    virtual void setDerivative(int ord);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the order of derivative set for the object.
     * @return Integer that specifies the order of the derivative.  Value can be anything from 0 or larger.
     * Variable is passed by value.
     */
    int getDerivative();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the GlobalSolution output.  GlobalSolution is the direct output of the solution for each
     * body, translated into body coordinates.  The output is modified to provide the specified order of derivative
     * for the solution.
     * @param freqIn The wave frequency to use for calculating the OutputDerived object.  Most outputs will depend
     * on the wave frequency.
     * @return Returns a complex matrix that is the GlobalSolution object.  The complex matrix is a single row matrix.
     * Each column in the row matrix represents a new degree of freedom variable.
     */
    cx_mat calcOutput(double freqIn);

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The order of derivative to use for calculating output.  Set by child classes for specifying specific
     * outputs.
     */
    int orderDerivative;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the solution matrix to perform operations on.  Accesses the solution matrix from the parent body.
     * @param freqIn The wave frequency.  Used to identify which variable to access in the solution matrix.
     * @return Returns the Solution object to use for calculations.  Variable is passed by reference.
     */
    Solution &getSolution(double freqIn);
//==========================================Section Separator =========================================================
private:

};
#endif
