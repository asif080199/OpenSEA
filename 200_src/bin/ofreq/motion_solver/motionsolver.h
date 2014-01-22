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
 *May 15, 2013	Shane Honanie		Initially Created
 *Aug 01, 2013  Nicholas Barczak    Added support for unlimited number of bodies in assembled global motion matrix.
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
#ifndef MOTIONSOLVER_H
#define MOTIONSOLVER_H
#include <complex>
#include <QtGlobal>
#ifdef Q_OS_WIN
    #include "armadillo.h"  //References the armadillo library in lib folder.
#elif defined Q_OS_LINUX
    #include <armadillo>    //Armadillo library included with standard system libraries.
#endif
#include "matbody.h"
#include "matforcereact.h"
#include "matforcecross.h"
#include "../global_objects/ofreqcore.h"

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
 * This class holds data for the motion solver and performs calculations on all of the data to get the solution matrix.
 * The motion solver performs a series of operations.
 * 1.)  Sum each force object for all forces within a set.
 * 2.)  Sum each force object for all the derivatives defined within each force.  Applied frequency dependence.
 * 3.)  Sum user reactive forces, hydro reactive forces, and body mass into a single object for each body.
 * 4.)  Sum user cross-body forces, hydro cross-body forces into a single object for each body.
 * 5.)  Sum user active forces and hydro active forces into a single object for each body.
 * 6.)  Assemble reactive forces and cross-body forces into a single global response matrix. ([A])
 * 7.)  Assemble active forces into a single global active force matrix. ([F])
 * 8.)  Solve the linear system system of equations formed by the equation [A] * [x] = [F]
 * 9.)  Redistribute the solution back to each body object.
 */

class MotionSolver : public osea::ofreq::oFreqCore
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The default constructor.
     */
    MotionSolver();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Constructor.  Initialize with objects already added.
     *
     * This constructor combines creation of the class with adding the body objects to the class at the same time.
     * The list of matBody objects is added to the set of bodies that the motion solver solves.  Bodies inputs are
     * passed by value, and not by reference.
     * @param listBodIn The vector list of bodies to add to the motion solver object.
     */
    MotionSolver(std::vector<matBody> listBodIn);

    //------------------------------------------Function Separator ----------------------------------------------------
	~MotionSolver(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Add a body to the motion solver set.
     *
     * Add a body to the motion solver set.  After initialization, this is how the motion solver gets the correct data
     * to perform math operations on.
     * @param The matBody object to add the motion solver set.  Body inputs are added passed by value, and not by
     * reference.
     */
    void addBody(matBody bodIn);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
     * @brief Sum Reactive forces for each set.
     *
     * Sum Reactive forces for each set.  This iterates through each reactive force in a set and adds the forces
     * together.  It respects derivatives in the summation.  If the input list of forces is empty (listForces), the
     * function returns a NULL pointer.
     * @param listForces The list of reactive forces associated with each body.  This list may be anything from
     * zero to infinite number of entries.
     * @return The Sum of reactive force matrices.  Returned variable is a pointer.
	 */
    matForceReact *sumReactSet(std::vector<matForceReact> listForces);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sum cross-body forces for each set.
     *
     * This gets handled a little differently from reactive forces, as the linked
     * body for the force depends on whether two objects are summed together.  Output from this function is a
     * vector of cross-body forces.  Each entry in the vector contains a cross-body force object.  If the input list
     * of forces (listForces) is empty, the function returns a NULL pointer.
     * @param CrossBodMat The vector of cross-body force matrices.
     * @return A vector of complex matrices, with each entry in the vectors representing a cross-body force linked to
     * a specific body.  Returned variable is a pointer.
     */
    std::vector<matForceCross *> sumCrossSet(std::vector<matForceCross> listForces);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
     * @brief Sum active forces for each set.
     *
     * Takes the input vector and sums all force objects together to create an aggregate force that is the total of
     * all force objects supplied in the input vector (listForces).  If the input vector is empty, the function
     * returns a NULL pointer.
     * @param listForces Vector of matForceActive objects.  Vector can be unlimited size.  Each entry in the vector
     * is one of the active forces to be added into the total aggregate active force.
     * @return The Sum of active force matrix.  Variable is returned as pointer.
	 */
    arma::cx_mat *sumActiveSet(std::vector<matForceActive> listForces);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
     * @brief Sums all derivatives for the reactive force object entered.
     *
     * Matrix force objects normally store a separate matrix of coefficients for each derivative.  However, to solve
     * for object motions at a given frequency, it is necessary to combine the various derivatives into a single
     * matrix.  The function uses a formula to combine the various derivative formulas into a single output matrix.
     * The output matrix is only valid for the specific frequency set at the time of calling this function.  If the
     * input object is a NULL pointer, the function also returns a NULL pointer.
     * @param forceIn The reactive force matrix.  Variable is a pointer to a matForceReact object.
     * Variable passed by value.
     * @return Single matrix that is the derivative sum of each matrix for each derivative contained within the input
     * object.  Returned variable is a pointer.  Returned pointer is set to NULL if input pointer is NULL.
	 */
    arma::cx_mat *sumDerivative(matForceReact *forceIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sums all derivatives for the cross-body force objects entered.
     *
     * Matrix force objects normally store a separate matrix of coefficients for each derivative.  However, to solve
     * for object motions at a given frequency, it is necessary to combine the various derivatives into a single
     * matrix.  The function uses a formula to combine the various derivative formulas into a single output matrix.
     * The output matrix is only valid for the specific frequency set at the time of calling this function.  If the
     * input object is a NULL pointer, the function also returns a NULL pointer.  For the cross-body forces, the
     * function expects a vector of cross-body forces.  These are all for a single Body object.  Each matForceCross
     * object in the vector represents a cross-body force that depends on the motions of another body.  So for N
     * bodies, it is possible for the vector to contain up to N - 1 matForceCross objects.
     * @param forceIn The cross-body force matrix.  Variable is a vector of pointers to matForceCross objects.
     * Variable passed by value.
     * @return Returns vector of single matrices.  Each matrix in the vector is the derivative sum of each matrix
     * for each derivative contained within the input matForceCross object.  Returned variable is a pointer to a
     * vector of single matrices.  Returned pointer is set to NULL if input pointer is NULL.
     */
    std::vector<arma::cx_mat *> sumDerivative(std::vector<matForceCross *> forceIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the current wave frequency.
     *
     * Sets the current wave frequency.  This is used in calculating the summations and must be set before calling
     * the solve method.
     * @param freqIn The input wave frequency.  A double precision floating point value.  Used when summing derivatives.
     */
    void setWaveFreq(double freqIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculate the Solution
     */
    void calculateOutputs();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Get the solution for the solved equation.
     *
     * Get the solution for the solved equation.  Returns the full vector of complex matrices.  Each element in the
     * vector is a solution matrix specific to the body.  The vector is ordered in the same sequence that the bodies
     * were added to the motionsolver object.  This is the vector of solutions for each Body object.  It applies to
     * a single wave frequency and single wave direction.
     * @return Returns the full vector of complex matrices.  Each element in the
     * vector is a solution matrix specific to the body.  The vector is ordered in the same sequence that the bodies
     * were added to the motionsolver object.  Returned variable passed by reference.
     */
    std::vector<arma::cx_mat> &listSolution();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Get the solution for the solved equation, for a single body.
     *
     * Returns the solution for solved equations for a single Body object.  This is a matrix of solutions for equations
     * of motion.  It applies to a single Body, single wave frequency, single wave direction.  The Bodies are ordered
     * in the same sequence in which they were added to the motionsolver object.
     * @param bod Integer.  Specifies which Body object the solution should be retrieved for.  The Bodies are ordered
     * in the same sequence in which they were added to the motionsolver object.
     * @return  Returns a matrix of complex doubles.  This matrix contains the solution of motions for the Body
     * specified.  Returned variable passed by reference.
     * @sa Body
     */
    arma::cx_mat &listSolution(unsigned int bod);



//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    double curWaveFrequency; /**< The current wave frequency */

    //Global Matrices
    arma::cx_mat globReactiveMat; /**< A Matrix (Reactive Force Matrix Global)*/
    arma::cx_mat globActiveMat;   /**< F Matrix (Active Force Matirx Global)*/
    arma::cx_mat globSolnMat;	  /**< X Matrix (Solution Column Matrix)*/

    //Output solution values
    /**
     * @brief The vector list of solutions.
     *
     * The vector list of solutions.  Each entry in the vector contains a matrix of a size specific to each body.
     */
    std::vector<arma::cx_mat> plistSolution;

    //------------------------------------------Function Separator ----------------------------------------------------
    std::vector<matBody> plistBody; /**< Body with Force Coefficients */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The current body that all force summations are associated.
     *
     * At some points in the summation, it is necessary for the various summation functions to know which body from
     * the list of bodies the forces are currently summing for.  This variable provides a common reference for all
     * the functions to reference.
     */
    int curSumBody;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds the two matrices together, anticipating possibilities for various combinations of pointers.
     *
     * This function should be applied to matForceActive and matForceReact objects that were already summed into
     * complex matrix objects.  The inputs are two pointers to the objects.  Depending on the state of the pointers
     * changes how the function adds the two together.  If a pointer is NULL, that pointer is replaced with an
     * appropriately sized matrix of zeros.  If both pointers are NULL, the function returns a matrix of zeros.  If
     * only one pointer is NULL, just the other matrix is returned.  If neither pointer is NULL, the two matrices
     * are added together.
     * @param Input1 Pointer to a complex matrix.  The pointer can have a NULL value.  If NULL, the input is replaced
     * with a matrix of zeros.
     * @param Input2 Pointer to a complex matrix.  The pointer can have a NULL value.  If NULL, the input is replaced
     * with a matrix of zeros.
     * @param ForceType Specifies which type of force the input matrices represent.  If both pointers are NULL values
     * the function will have no way to know what size to make the output matrix of zeros.  By specifying the force
     * type, the function knows which shape to make the output matrix of zeros.  The following values are used to
     * specify the force types.
     * -2:  Active force types.  Column matrix of zeros should be created.
     * -1:  Reactive force type.  Square matrix of zeros should be created.
     * >=0:   Cross-body force type.  Non-square matrix of zeros should be created.  The currently active body
     * is already known.  In this case, the integer of ForceType specifies the index of the other body object, the
     * one that the cross-body forces pick their body motions from.
     * @return Returns a complex matrix that is the summation of the two inputs.  Returned variable passed by value.
     * Even if both inputs are NULL pointers, the function will return a matrix of zeros.
     */
    arma::cx_mat SumSingle(arma::cx_mat *Input1, arma::cx_mat *Input2, int ForceType);
};

}   //Namespace ofreq
}   //Namespace osea

#endif
