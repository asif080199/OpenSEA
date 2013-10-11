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
#include "../global_objects/body.h"
#include "../global_objects/ofreqcore.h"

using namespace std;
using namespace arma;

//######################################### Class Separator ###########################################################
//Forward declarations to keep the compiler happy.
class SolutionSet;

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
 *
 * Developers note:  The original scheme had the OutputDerived class include a pointer to the contain parent class,
 * OutputsBody.  But this creates a cyclic dependency of header files, and will not compile.  The only resolution I
 * found to this was to not include the parent class and pass all the necessary information to each individual
 * OutputDerived object.  This is tedious, but within the reasons of the methods defined by the OutputsBody class.
 * And it allow compilation.  All data items are passed by reference to avoid excess memory duplication.
 *
 * @sa OutputDerived::calcOutput();
 */

class OutputDerived : public oFreqCore
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
     * The default destructor.  Nothing happens here.
     */
    ~OutputDerived();

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

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the list of body objects.  Only a c++ reference to the list of body objects.
     * @return Returns reference to the list of Body objects.  Variable passed by reference.
     */
    vector<Body> &listBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a reference to an individual Body in the list of Body objects.
     * @param bodIn The integer index of the body you wish to retrieve from the list of Body objects.
     * @return Returns the Body object requested.  Returned variable is passed by reference.
     */
    Body &listBody(int bodIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the list of Body objects.
     * @param Input THe list of Body objects.  Variable passed by reference.
     */
    void setListBody(vector<Body> &Input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a vector of SolutionSet objects.
     * @return Returns a vector of SolutionSet objects.  Internal storage is just a set of pointers to the object.
     * Variable is passed by reference.
     */
    vector<SolutionSet> &refSolutionSet();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the list of solutions.
     * @param Input The vector list of SolutionSet objects.  Variable is passed by reference.  Variable is stored
     * internally as a pointer.
     */
    void setSolutionSet(vector<SolutionSet> &Input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the list of frequencies.  Frequencies are in radians per second.
     * @return Returns the list of wave frequencies.  Variable is passed by reference.  Variable is stored internally
     * as a pointer.
     */
    vector<double> &listFreq();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns individual frequency from the list of wave frequencies.
     *
     * Returns the frequency specified by the index.
     * @param index Integer.  The index which specifies which wave frequency to return.
     * @return Double.  Returns individual frequency from the list of wave frequencies.  Returned variabled is passed
     * by reference.
     */
    double &listFreq(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the list of wave frequencies.  Frequencies are in radians per second.
     * @param Input The list of wave frequencies.  Variable is passed by reference.  Variable is stored internally as
     * a pointer.
     */
    void setListFreq(vector<double>& Input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the list of wave directions.  Wave directions are measured in radians.  True North is zero, with
     * positive angles going counter-clockwise.
     * @return Returns the vector of doubles containing the wave directions.  Variable passed by reference.  Variable
     * is stored internally as a pointer.
     */
    vector<double> &listWaveDir();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns individual wave direction from the list of wave directions.
     *
     * Returns the wave direction specified by the index.  Wave directions are measured in radians.  True North is
     * zero, with positive angles going counter-clockwise.
     * @param index Integer.  The index which specifies which wave direction to return.
     * @return Double.  Returns individual wave direction from the list of wave directions.  Returned variable is
     * passed by reference.
     */
    double &listWaveDir(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the list of wave directions.  Wave directions are measured in radians.  True North is zero, with
     * positive angles going counter-clockwise.
     * @param Input The vector of doubles containing the wave directions.  Variable is passed by reference.  Variable
     * is stored internally as a pointer.
     */
    void setListWaveDir(vector<double>& Input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the integer index of the current wave direction.
     * @return Gets the integer index of the current wave direction.  Variable is passed by reference.  Variable
     * is stored internally as a pointer.
     */
    int &getCurWaveDir();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the integer index of the current wave direction.
     * @param Input The integer index of the current wave direction in the vector list of wave directions.  Variable
     * is passed by reference.  Variable is stored internally as a pointer.
     */
    void setCurWaveDir(int &Input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the integer index of the current body.  This represents the Body object that the derived output is
     * associated with.
     * @return Returns the integer index of the current Body object associated with this derived output object.
     * Variable is passed by value.
     */
    int getCurBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the integer index of the current body.  This represents the Body object that the derived output is
     * associated with.
     * @param Input The integer index of the current Body object associated with this derived output object.
     * Variable is passed by value
     */
    void setCurBody(int Input);

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name for the derived output object.  This is a name set by the user to idenfity the object.  If
     * multiple objects of the same type are created, the name can also distinguish between the various objects.
     */
    string pName;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The vector list of Body objects.
     */
    vector<Body>* plistBody;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The vector list of SolutionSet objects.
     */
    vector<SolutionSet>* plistSolution;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave frequencies.  Wave frequency recorded in units of radians per second.
     */
    vector<double>* plistFreq;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave directions.  Wave direction recorded in units of radians.  Zero is true North direction.
     * Oriented positive counter-clockwise.
     */
    vector<double>* plistWaveDir;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The current wave direction used for calculating the DerivedOutput objects.
     */
    int* pCurWaveDir;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The index of the current body used for calculating derived outputs.  This index is the position of
     * the current body in the list of Body objects.
     */
    int pCurBody;

//==========================================Section Separator =========================================================
private:

};
#endif
