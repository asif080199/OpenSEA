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
#ifndef OUTPUTDERIVED_H
#define OUTPUTDERIVED_H
#include <QtGlobal>
#ifdef Q_OS_WIN
    #include "../../lib/armadillo.h"  //References the armadillo library in lib folder.
#elif defined Q_OS_LINUX
    #include <armadillo>    //Armadillo library included with standard system libraries.
#endif
#include "../global_objects/body.h"
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
//Forward declarations to keep the compiler happy.
class SolutionSet;
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
 *
 * @sa OutputDerived::calcOutput();
 */

class OutputDerived : public osea::ofreq::oFreqCore
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
     * @brief Constructor that also sets the pointer to the OutputsBody object which contains the OutputDerived
     * object.
     * @param input Pointer to the OutputsBody objec that contains this OutputDerived object.  Pointer passed by value.
     *
     * @sa setOutputsBody()
     */
    OutputDerived(OutputsBody *input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * The default destructor.  Nothing happens here.
     */
    ~OutputDerived();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the pointer to the OutputsBody object which contains this OutputDerived object.
     *
     * The OutputsBody object contains critical information that each DerivedOutput object may require.  This
     * information is made avaiable through the pParentBody pointer.  Available information includes:
     * - list of Body objects.
     * - list of SolutionSet objects.
     * - list of wave frequencies.  Wave frequency recorded in units of radians per second.
     * - list of wave directions.  Wave direction recorded in units of radians.  Zero is true North direction.
     * Oriented positive counter-clockwise.
     * - The current wave direction used for calculating the DerivedOutput objects.
     * @param input Pointer to the OutputsBody objec that contains this OutputDerived object.  Pointer passed by value.
     *
     * @sa OutputsBody
     */
    void setOutputBody(OutputsBody *input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the name of the OutputDerived object.  This is a name set by the user to idenfity the object.  If
     * multiple objects of the same type are created, the name can also distinguish between the various objects.
     * @return Returns the name of the OutputDerived object.  std::string variable.  Variable passed by value.
     */
    std::string getName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the name of the class used to create the OutputDerived object.  This is the name set to identify
     * the type of output object.  If multiple objects of the same type are created, the class name will be the same
     * for all objects.
     * @return Returns the class name of the OutputDerived object.  std::string variable.  Variable passed by value.
     */
    std::string getClassName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the name of the OutputDerived object.  This is a name set by the user to idenfity the object.  If
     * multiple objects of the same type are created, the name can also distinguish between the various objects.
     * @param nameIn std::string variable.  Sets the name of the OutputDerived object.  Variable passed by value.
     */
    void setName(std::string nameIn);

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
     *
     * Writes results of calculation to the Results matrix in the OutputsBody object that contains this
     * OutputDerived object.  Calling the calcOutput() function only generates the results.  They must be retrieved
     * from the OutputsBody object in a separate function, using getResult() function.
     *
     * Results written to the Results matrix are always stored in a matrix of complex values.  The exact meaning and
     * organization of the complex matrix changes with each type of OutputDerived object created as a child of this
     * class.  The cx_mat data type is used because that is the most natural data type for the largest number of
     * OutputDerived objects.  It isn't always the best, but it can usually work well for the intended purposes.
     * @param freqIn The wave frequency to use for calculating the OutputDerived object.  Specifies the index of the
     * wave frequency to retrieve from the list of wave frequencies.  Most outputs will depend on the wave frequency.
     * @return Returns an integer for output.  This integer is not the calculation result.  It reports on whether the
     * calculation is successful.  A returned value of zero (0) means a successful calculation.  Other returned
     * values are error codes, each with their own meaning.
     * @sa OutputsBody::getResult()
     */
    virtual int calcOutput(int freqIn = -1) = 0;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the list of body objects.  Only a c++ reference to the list of body objects.
     * @return Returns reference to the list of Body objects.  Variable passed by reference.
     */
    std::vector<Body> &listBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a reference to an individual Body in the list of Body objects.
     * @param bodIn The integer index of the body you wish to retrieve from the list of Body objects.
     * @return Returns the Body object requested.  Returned variable is passed by reference.
     */
    Body &listBody(int bodIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a vector of SolutionSet objects.
     * @return Returns a vector of SolutionSet objects.  Internal storage is just a set of pointers to the object.
     * Variable is passed by reference.
     */
    std::vector<SolutionSet> &listSolutionSet();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a vector of SolutionSet objects.
     * @param index Integer.  Specifies the index for which to retrieve the solution set.  If the requested index is
     * out of bounds, the program will return an error.
     * @return Returns a vector of SolutionSet objects.  Internal storage is just a set of pointers to the object.
     * Variable is passed by reference.
     */
    SolutionSet &listSolutionSet(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the list of frequencies.  Frequencies are in radians per second.
     * @return Returns the list of wave frequencies.  Variable is passed by reference.  Variable is stored internally
     * as a pointer.
     */
    std::vector<double> &listFreq();

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
     * @brief Gets the list of wave directions.  Wave directions are measured in radians.  True North is zero, with
     * positive angles going counter-clockwise.
     * @return Returns the vector of doubles containing the wave directions.  Variable passed by reference.  Variable
     * is stored internally as a pointer.
     */
    std::vector<double> &listWaveDir();

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
     * @brief Gets the current wave direction.  Output is the actual value for the current wave direction, in units
     * of radians.
     * @return Returns a double that is the current wave direction, in units of radians.  Variable is passed by value.
     */
    double getCurWaveDir();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the integer index of the current body.  This represents the Body object that the derived output is
     * associated with.
     * @return Returns the integer index of the current Body object associated with this derived output object.
     * Variable is passed by value.
     */
    int getCurBodyIndex();

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name for the derived output object.  This is a name set by the user to idenfity the object.  If
     * multiple objects of the same type are created, the name can also distinguish between the various objects.
     */
    std::string pName;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The actual name of the class.  This is used in outputs writing to file.  The class name must be hard
     * coded as a variable because functions such as typeid produce unreliable formatting of the output.  The class
     * name is automatically set by the class constructor.  Getter functions can only retrieve the variable.  Not
     * alter it.
     */
    std::string pClassName;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a result to the list of results in the OutputsBody.
     *
     * The list of results contains all the results from calculating each DerivedOutput.  The DerivedOutput objects
     * also have direct access to this list.  But this function handles all the tedious tasks of resizing the list
     * and preventing anything from going out of bounds.
     * @param input The result that you wish to add to the list of results.  Input is a pointer to a matrix of
     * complex numbers.  Please be sure to create all your matrices on the stack so they don't get destroyed once
     * they go out of scope.  Don't worry about memory cleanup.  The OutputsBody object has a Reset() function that
     * automatically deletes all variables from the list of results and clears the memory.
     * @param index [Optional] Integer input.  The index specifies the index in the vector in which you wish to enter
     * the result.  This input is optional.  If no index is specified, the function automatically adds the result as
     * a new entry on the end of the list.
     */
    void addResult(arma::cx_mat* input, int index = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the OutputsBody object.
     *
     * The OutputsBody object contains critical information that each DerivedOutput object may require.  This
     * information is made avaiable through the pParentBody pointer.  Available information includes:
     * - list of Body objects.
     * - list of SolutionSet objects.
     * - list of wave frequencies.  Wave frequency recorded in units of radians per second.
     * - list of wave directions.  Wave direction recorded in units of radians.  Zero is true North direction.
     * Oriented positive counter-clockwise.
     * - THe current wave direction used for calculating the DerivedOutput objects.
     *
     * @sa OutputsBody
     */
    OutputsBody *pParentBody;

//==========================================Section Separator =========================================================
private:


};

}   //namespace ofreq
}   //namespace osea

#endif
