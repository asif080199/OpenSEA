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
#ifndef OUTPUTDERIVED_H
#define OUTPUTDERIVED_H
#include <QtGlobal>
#ifdef Q_OS_WIN
    #include "armadillo.h"  //References the armadillo library in lib folder.
#elif defined Q_OS_LINUX
    #include <armadillo>    //Armadillo library included with standard system libraries.
#endif
#include "../motion_solver/body.h"
#include "outputsbody.h"

using namespace std;
using namespace arma;

//######################################### Class Separator ###########################################################
//Forward declarations to keep the compiler happy.
class OutputsBody;

//######################################### Class Separator ###########################################################
/**
 * This abstract class represents the Derived Outputs.  Derived outputs are any additional information that needs to
 * be calculated from basic information within a body.  That can be anything from taking derivatives to calculating
 * an empirical equation for motion sickness incidence.  The constructor for this class requires a pointer to the
 * parent class to access any further information required for calculation.
 *
 * The Derived Output class can not be used directly.  Individual types of Derived Outputs must be developed and
 * inherit this Derived Output class.  This abstract class provides a common framework that all Derived Outputs must
 * share.  The most important part is the calcOutput() method.  Everything that uses a Derived Output object
 * expects to have this calcOutput() method, and will call it by that name.
 * @sa OutputDerived::calcOutput();
 */

class OutputDerived
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Nothing happens here.
     */
    OutputDerived();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Overloaded constructor to feed the pointer to the parent class containing the OutputDerived object.
     * @param Input Pointer to the OutputsBodyobject.  This should be the OutputsBody object which contains the
     * constructed OutputDerived object.  Variable passed by reference.
     */
    OutputDerived(OutputsBody &Input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * The default destructor.  Nothing happens here.
     */
    ~OutputDerived();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the pointer to the parent class containing the OutputDerived object.
     * @param Input Pointer to the OutputsBodyobject.  This should be the OutputsBody object which contains the
     * constructed OutputDerived object.  Variable passed by reference.
     */
    void setParent(OutputsBody &Input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the name of the OutputDerived object.  This is a name set by the user to idenfity the object.  If
     * multiple objects of the same type are created, the name can also distinguish between the various objects.
     * @return Returns the name of the OutputDerived object.  String variable.  Variable passed by value.
     */
    string getName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the name of the OutputDerived object.  This is a name set by the user to idenfity the object.  If
     * multiple objects of the same type are created, the name can also distinguish between the various objects.
     * @param nameIn String variable.  Sets the name of the OutputDerived object.  Variable passed by value.
     */
    void setName(string nameIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the current wave direction used by the OutputDerived object.  Some child classes will need this
     * to determine proper calculation of the output.
     * @return Returns a double specifying the angle of the current wave direction.  Units of radians.  True north
     * is zero radians, positive counter-clockwise.
     */
    double getCurWaveDir();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the index of the current wave direction used by the OutputDerived object.  Some child classes will
     * need this to determine proper calculation of output.  This differs from the getCurWaveDir() method in that
     * the other method returns the actual value of the angle.  This returns the index of the current angle in the
     * list of wave directions.
     * @return Returns an integer that represents the index of the current angle in the list of wave directions.
     * Variable passed by value.
     */
    int getCurWaveInd();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pure virtual member.  Calculates the output from the OutputDerived object.
     * @param freqIn The wave frequency to use for calculating the OutputDerived object.  Most outputs will depend
     * on the wave frequency.
     * @return Returns a complex matrix that is the OutputDerived result.  The exact meaning and organization of the
     * complex matrix changes with each type of OutputDerived object created as a child of this class.  The cx_mat
     * data type is used because that is the most natural data type for the larges number of OutputDerived objects.
     * It isn't always the best, but it can usually work well for the intended purposes.
     */
    virtual cx_mat calcOutput(double freqIn) = 0;

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Contains a pointer to the Body object used for calculating all these derived outputs.
     */
    Body* pBody;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Contains a pointer to the OutputsBody object which contains all these derived outputs.  Used to retrieve
     * information.
     */
    OutputsBody* pParent;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name for the derived output object.  This is a name set by the user to idenfity the object.  If
     * multiple objects of the same type are created, the name can also distinguish between the various objects.
     */
    string pName;

    //------------------------------------------Function Separator ----------------------------------------------------


//==========================================Section Separator =========================================================
private:

};
#endif
