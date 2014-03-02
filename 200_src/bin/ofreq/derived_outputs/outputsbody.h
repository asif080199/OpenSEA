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
 *Aug 13, 2013      Nicholas Barczak    Updated to reflect new motion model and motion solver paradigm.  Supplied more
 *                                      information to outputsBody object.  And separated each derived output as a
 *                                      vector of objects, each called through its own property.
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
#ifndef OUTPUTSBODY_H
#define OUTPUTSBODY_H
#include <vector>
#include <complex>
#include <QtGlobal>
#ifdef Q_OS_WIN
    #include "armadillo.h"  //References the armadillo library in lib folder.
#elif defined Q_OS_LINUX
    #include <armadillo>    //Armadillo library included with standard system libraries.
#endif
#include "globalsolution.h"
#include "globalacceleration.h"
#include "globalmotion.h"
#include "globalvelocity.h"
#include "../global_objects/solutionset.h"
#include "../global_objects/body.h"
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

/**
 * @brief Type definition for a vector of complex numbers.  Used to return the calculated output.
 */
typedef std::vector< std::complex<double> > cx_vector;

//######################################### Class Separator ###########################################################
//Forward declarations of classes
class OutputsBody;

//######################################### Class Separator ###########################################################
/**
 * This class holds all types of derived outputs for a single body object.  Each derived output type is contained
 * in a vector.  This allows multiple instances of each output type.  To save on memory, an output type is not
 * calculated until it is requested.  And then the derived output immediately returns the calculations.  Outputs are
 * typically returned as a vector of objects.  Each row in the vector represents a different wave frequency.  The object
 * contained in the returned vector The meaning may change with each derived output type.
 * You should check the documentation for each derived output.
 *
 * Each derived output is listed as its own item in the class.  Each derived output class has the following methods
 * associated with it.  (For this generic example, the term Output refers to any derived output object.)
 * 1.  refOutput:  Pointer to the output object itself.
 * 2.  getOutput:  Calculates the output and writes the result out as a cx_matrix object.
 * 3.  addOutput:  Adds a new output object to the list of objects.  Optionally takes the supplied output.  Otherwise
 *                 creates a new output object in the vector list.
 *
 * To use the outputsBody class, you must supply several properties to it.  These properties give the outputs class
 * full knowledge of the current state of affairs.  This allows the different derived outputs to have the most
 * flexibility for how to calculate results.  Set the following properties:
 *
 * Data Sets:
 * 1.  Bodies list
 * 2.  Solutions list
 * 3.  Frequencies list
 * 4.  Wave directions list
 *
 * Individual properties
 * 1.  Current body for outputsBody object.
 * 2.  Current wave direction.
 *
 * Once these properties are set, you may calculate derived outputs.  Inputs for derived outputs may be set
 * before all these properties are set.
 *
 * The following derived outputs are available:
 * @sa GlobalSolution
 * @sa GlobalMotion
 * @sa GlobalVelocity
 * @sa GlobalAcceleration
 *
 * Developers note:  The original scheme had the OutputDerived class include a pointer to the contain parent class,
 * OutputsBody.  But this creates a cyclic dependency of header files, and will not compile.  The only resolution I
 * found to this was to not include the parent class and pass all the necessary information to each individual
 * OutputDerived object.  This is tedious, but within the reasons of the methods defined by the OutputsBody class.
 * And it allow compilation.  All data items are passed by reference to avoid excess memory duplication.
  */

class OutputsBody : public osea::ofreq::oFreqCore
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor for the OutputsBody object.  Nothing happens here.
     */
    OutputsBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Overloaded constructor for the OutputsBody object.  Includes inputs for Body list, Solution list,
     * frequencies list, and wave directions list.  All inputs are passed by reference and held as constant variables
     * to avoid changing the referenced variables.
     * @param listBod The vector of Body objects to use for the object.  Contains all the information about Body forces.
     * @param listSoln The vector of SolutionSet objects to use for the object.  Contains all the information about
     * solutions for each body.  Each SolutionSet object in the vector represents the solutions for all frequencies
     * for a single Body object.
     * @param listFreq The vector of wave frequencies to use for the object.  Each wave frequency corresponds to a
     * Solution object in the SolutionSet object.
     * @param listWaveDir The vector of wave directions to use for the object.  This is provided mostly for reference.
     */
    OutputsBody(std::vector<Body> &listBod,
                std::vector<SolutionSet> &listSoln,
                std::vector<double> &listFreq,
                std::vector<double> &listWaveDir);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.  Nothing happens here.
     */
    ~OutputsBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Creates each of the OutputDerived objects in their respective lists.
     *
     * Each list of OutputDerived object can contain any number of objects.  The initialize function is called to
     * generate each of these OutputDerived objects.
     */
    void Initialize();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the list of Body objects to calculate derived outputs for.  Derived outputs are calculated for only
     * one Body object in the list.  The rest are included as reference for cross-body forces.
     * @param listIn The vector of Body objects to assign to this OutputsBody.  Input is passed by reference.  Input
     * is held as a constant value, so that it can not be modified by the class.
     */
    void setListBody(std::vector<Body> &listIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the list of SolutionSet objects to calculate derived outputs for.  Derived outputs are calculated
     * for only one SolutionSet object in the list.  The rest are included as reference for cross-body forces.
     * @param listIn The vector of SolutionSet objects to assign to this OutputsBody.  Input is passed by reference.
     * Inputs is held as a constant value, so that it can not be modified by the class.
     */
    void setSolutionSet(std::vector<SolutionSet> &listIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides the list of the SolutionSet objects.
     *
     * Derived outputs are calculated for only one SolutionSet object in the list.  The rest are included as reference
     * for cross-body forces.
     * @return Returns a vector containing the SolutionSet objects.  Variable passed by reference.
     */
    std::vector<SolutionSet> &listSolutionSet();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides a single entry from the list of the SolutionSet objects.
     *
     * Derived outputs are calculated for only one SolutionSet object in the list.  The rest are included as reference
     * for cross-body forces.  This implementation of the function only returns a single entry from the list.
     * @param index Integer.  Specifies the index for which to retrieve the solution set.  If the requested index is
     * out of bounds, the program will return an error.
     * @return Returns a single SolutionSet object requested from the list of SolutionSet objects.  Requested variable
     * is passed by reference.
     */
    SolutionSet &listSolutionSet(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides access to the solution object for the current body.  Saves the trouble of trying to remember
     * which is the current body.
     * @return Returns the solution object for the current body.  Returned variable passed by reference.
     */
    osea::ofreq::SolutionSet &refCurSolution();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the list of wave frequencies.
     * @param listIn The vector of doubles representing the wave frequencies.  Frequencies entered in units of radians
     * per second.  Input is passed by reference and held as a constant so that the class can not change the frequencies.
     */
    void setListFreq(std::vector<double> &listIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the list of wave frequencies.  Provides direct access to the list.
     * @return Returns the vector of doubles representing the wave frequencies.  Frequencies entered in units of radians
     * per second.  Variable is passed by reference.
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
     * @brief Sets the list of wave directions.
     * @param listIn The vector of doubles representing the wave directions.  Directions entered in units of radians.
     * Input is passed by reference and held as a constant so that the class can not change the directions.
     */
    void setListWaveDir(std::vector<double> &listIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the list of wave directions.  Provides direct access to the list.
     * @return Returns the vector of doubles representing the wave directions.  Directions entered in units of radians.
     * Variable is passed by reference.
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
     * @brief Sets the current wave direction.
     * @param index Integer input specifying the index of the current wave direction from the list set by
     * setListWaveDir.
     */
    void setCurWaveDir(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the index of the current wave directio entry.
     * @return Integer.  Returns the index of the current wave directio entry.  Variable passed by value.
     */
    int getCurWaveInd();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the current wave direction.  Output is the actual value for the current wave direction, in units
     * of radians.
     * @return Returns a double that is the current wave direction, in units of radians.  Variable is passed by value.
     */
    double getCurWaveDir();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the current Body to use.  Input selects from the list of supplied Body objects.
     * @param index Integer input that selects from the list of supplied Body objects.  Variable is passed by value.
     */
    void setCurBody(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the index of the current Body in the list of Body objects.
     * @return Returns the index of the current Body assigned to this OutputsBody object.  This differs from other
     * function refCurBody() because refCurBody() returns a pointer to the body directly.  But this function,
     * getCurBodyIndex() returns the integer index of the Body object in the vector list of Body objects.
     * Returned variable is passed by value.
     */
    int getCurBodyIndex();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the current Body objects used by the class for calculating Derived Outputs.  The returned value
     * depends on the last input of setCurBody
     * @return Returns a pointer the Body object used by the class for calculating Derived Outputs.  The returned
     * variable is passed by reference.
     */
    Body &refCurBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of Bodies.
     * @return Reference to vector of Body objects.  Variable passed by reference.
     * @sa Body
     */
    std::vector<Body> &listBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Direct access to an individual Body from the list of Bodies.
     * @param bodIn Integer specifying which Body object to access in the list of Bodies.
     * @return Returns reference to the Body object specified by input bodIn.
     * @sa listBody()
     */
    Body &listBody(int bodIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the pointer to the last OutputDerived object that calculated the results and wrote them for access.
     *
     * When a call is made to calculate the outputs of a DerivedOutput object, that object writes its results to the
     * Results storage in the OutputsBody.  The pointer is then set to that OutputDerived object.  This is in case
     * you need to access the OutputDerived object for any reason.  You don't need to remember which object did the
     * calculation.  You can just access the object.
     * @param input Pointer to the OutputDerived object that performed the last results calculation.
     */
    void setCurOutput(OutputDerived *input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the last OutputDerived object that calculated the results.
     *
     * When a call is made to calculate the outputs of a DerivedOutput object, that object writes its results to the
     * Results storage in the OutputsBody.  The pointer is then set to that OutputDerived object.  This is in case
     * you need to access the OutputDerived object for any reason.  You don't need to remember which object did the
     * calculation.  You can just access the object.
     * @return Returns the OutputDerived object that performed the last calculation.  Variable passed by reference.
     */
    OutputDerived &refCurOutput();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the last OutputDerived object that calculated the results.
     *
     * When a call is made to calculate the outputs of a DerivedOutput object, that object writes its results to the
     * Results storage in the OutputsBody.  The pointer is then set to that OutputDerived object.  This is in case
     * you need to access the OutputDerived object for any reason.  You don't need to remember which object did the
     * calculation.  You can just access the object.
     * @return Returns a pointer to the OutputDerived object that performed the last calculation.  Pointer passed
     * by value.
     */
    OutputDerived *getCurOutput();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The results from calculation of a DerivedOutput.
     *
     * When a call is made to calculate a DerivedOutput object, the object stores the results of its calculation in
     * the results matrix.  Those results can be accessed through this method.  This method is also used by the
     * DerivedOutput object to write the results.  DerivedOutput object automatically resizes the output buffer as
     * needed.  The output buffer is a vector storing pointers to matrices of complex numbers.
     * @return Returns direct access to the stored results matrix.  Returned variable is a vector storing pointers
     * to matrices of complex numbers.  matrix of undetermined size.  Returned variable is passed by reference.
     */
    std::vector<arma::cx_mat*> &listResult();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The results from calculation of a DerivedOutput.
     *
     * When a call is made to calculate a DerivedOutput object, the object stores the results of its calculation in
     * the results matrix.  Those results can be accessed through this method.  This method is also used by the
     * DerivedOutput object to write the results.  DerivedOutput object automatically resizes the output buffer as
     * needed.  The output buffer is a vector storing pointers to matrices of complex numbers.
     * @param index Integer input that specifies which matrix to retrieve from the list of results.  Most commonly,
     * the index represents the index of a wave frequency from the list of wave frequencies.  (i.e.  The list is
     * organized by wave frequencies.)
     * @return Returns direct access to the stored results matrix.  Returned variable is a matrix of complex numbers.
     * Matrix of undetermined size.  Returned variable is passed by reference.
     */
    arma::cx_mat &listResult(unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The results from calculation of a DerivedOutput.
     *
     * When a call is made to calculate a DerivedOutput object, the object stores the results of its calculation in
     * the results matrix.  Those results can be accessed through this method.  This method is also used by the
     * DerivedOutput object to write the results.  DerivedOutput object automatically resizes the output buffer as
     * needed.  The output buffer is a vector storing pointers to matrices of complex numbers.
     * @param index Integer input that specifies which matrix to retrieve from the list of results.
     * @return Returns a pointer to the stored results matrix.  Returned variable is a pointer to a matrix of
     * complex numbers.  Matrix of undetermined size.  Returned variable is passed by reference.
     */
    arma::cx_mat* &getResult(unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Clears all calculated results.
     *
     * Clears the calculated results from all internal storage.  And removes any pointers to the DerivedOutput object
     * that calculated the result.  The DerivedOutput object itself is not deleted.
     */
    void ClearResult();

    //------------------------------------------Function Separator ----------------------------------------------------
    //------------------------------------------Function Separator ----------------------------------------------------
    //Derived Outputs Now Listed Below
    //------------------------------------------Function Separator ----------------------------------------------------
    //------------------------------------------Function Separator ----------------------------------------------------

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the list of GlobalMotion objects.
     *
     * @return Returns a vector of pointers to each of the global motion objects.  Returned variable passed by
     * reference.
     */
    std::vector<GlobalMotion*> &listGlobalMotion();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the GlobalMotion object from the list of GlobalMotion objects.
     * @sa GlobalMotion
     * @param index The index of which GlobalMotion object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalMotion object per OutputsBody.  The default value selects this object.
     * @return Returns the GlobalMotion object from the list of GlobalMotion objects. Returned variable is passed
     * by reference.  Returns only the object specified by the input index.
     */
    GlobalMotion &listGlobalMotion(unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the GlobalMotion object from the list of GlobalMotion objects.
     *
     * Outputs from calculation is written to the results matrix.  You can retrieve the results from the calculation
     * by use of the getResult() function.  Calculating any other DerivedOutput will erase your results from the
     * Results matrix and you will need to recalculate them.
     * @param index The index of which GlobalMotion object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalMotion object per OutputsBody.  The default value selects this object.
     * @return Returns an integer for output.  This integer is not the calculation result.  It reports on whether the
     * calculation is successful.  A returned value of zero (0) means a successful calculation.  Other returned
     * values are error codes, each with their own meaning.
     * @sa GlobalMotion
     */
    int calcGlobalMotion(unsigned int index = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a new GlobalMotion object to the list of GlobalMotion objects.  This version sets the new
     * GlobalMotion object equal to the information passed in.
     * @param input Pointer to the new GlobalMotion object to add to the list of GlobalMotion objects.  Pointer is
     * passed by value.
     */
    void addGlobalMotion(GlobalMotion *input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Overloaded function for adding a GlobalMotion object to the list of GlobalMotion objects.  This version
     * has no input GlobalMotion object and automatically creates a blank GlobalMotion object.
     */
    void addGlobalMotion();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the GlobalVelocity object from the list of GlobalVelocity objects.
     *
     * Outputs from calculation is written to the results matrix.  You can retrieve the results from the calculation
     * by use of the getResult() function.  Calculating any other DerivedOutput will erase your results from the
     * Results matrix and you will need to recalculate them.
     * @param index The index of which GlobalVelocity object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalVelocity object per OutputsBody.  The default value selects this object.
     * @return Returns an integer for output.  This integer is not the calculation result.  It reports on whether the
     * calculation is successful.  A returned value of zero (0) means a successful calculation.  Other returned
     * values are error codes, each with their own meaning.
     * @sa GlobalVelocity
     */
    int calcGlobalVelocity(unsigned int index = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the list of GlobalVelocity objects.
     *
     * @return Returns a vector of pointers to each of the global motion objects.  Returned variable passed by
     * reference.
     */
    std::vector<GlobalVelocity*> &listGlobalVelocity();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the GlobalVelocity object from the list of GlobalVelocity objects.
     * @sa GlobalVelocity
     * @param index The index of which GlobalVelocity object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalVelocity object per OutputsBody.  The default value selects this object.
     * @return Returns the GlobalVelocity object from the list of GlobalVelocity objects. Returned variable is passed
     * by reference.  Returns only the object specified by the input index.
     */
    GlobalVelocity &listGlobalVelocity(unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a new GlobalVelocity object to the list of GlobalVelocity objects.  This version sets the new
     * GlobalVelocity object equal to the information passed in.
     * @param input Pointer to the new GlobalVelocity object to add to the list of GlobalVelocity objects.
     * Pointer is passed by value.
     */
    void addGlobalVelocity(GlobalVelocity *input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Overloaded function for adding a GlobalVelocity object to the list of GlobalVelocity objects.  This version
     * has no input GlobalVelocity object and automatically creates a blank GlobalVelocity object.
     */
    void addGlobalVelocity();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the GlobalAcceleration object from the list of GlobalAcceleration objects.
     *
     * Outputs from calculation is written to the results matrix.  You can retrieve the results from the calculation
     * by use of the getResult() function.  Calculating any other DerivedOutput will erase your results from the
     * Results matrix and you will need to recalculate them.
     * @param index The index of which GlobalAcceleration object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalAcceleration object per OutputsBody.  The default value selects this object.
     * @return Returns an integer for output.  This integer is not the calculation result.  It reports on whether the
     * calculation is successful.  A returned value of zero (0) means a successful calculation.  Other returned
     * values are error codes, each with their own meaning.
     * @sa GlobalAcceleration
     */
    int calcGlobalAcceleration(unsigned int index = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the list of GlobalAcceleration objects.
     *
     * @return Returns a vector of pointers to each of the global motion objects.  Returned variable passed by
     * reference.
     */
    std::vector<GlobalAcceleration *> &listGlobalAcceleration();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the GlobalAcceleration object from the list of GlobalAcceleration objects.
     * @sa GlobalAcceleration
     * @param index The index of which GlobalAcceleration object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalAcceleration object per OutputsBody.  The default value selects this object.
     * @return Returns the GlobalAcceleration object from the list of GlobalAcceleration objects. Returned variable is passed
     * by reference.  Returns only the object specified by the input index.
     */
    GlobalAcceleration &listGlobalAcceleration(unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a new GlobalAcceleration object to the list of GlobalAcceleration objects.  This version sets the new
     * GlobalAcceleration object equal to the information passed in.
     * @param input Pointer to the new GlobalAcceleration object to add to the list of GlobalAcceleration objects.
     * Pointer is passed by value.
     */
    void addGlobalAcceleration(GlobalAcceleration *input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Overloaded function for adding a GlobalAcceleration object to the list of GlobalAcceleration objects.
     * This version has no input GlobalAcceleration object and automatically creates a blank GlobalAcceleration object.
     */
    void addGlobalAcceleration();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the GlobalSolution object from the list of GlobalSolution objects.
     *
     * Outputs from calculation is written to the results matrix.  You can retrieve the results from the calculation
     * by use of the getResult() function.  Calculating any other DerivedOutput will erase your results from the
     * Results matrix and you will need to recalculate them.
     * @param index The index of which GlobalSolution object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalSolution object per OutputsBody.  The default value selects this object.
     * @return Returns an integer for output.  This integer is not the calculation result.  It reports on whether the
     * calculation is successful.  A returned value of zero (0) means a successful calculation.  Other returned
     * values are error codes, each with their own meaning.
     * @sa GlobalSolution
     */
    int calcGlobalSolution(unsigned int index = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the list of GlobalSolution objects.
     *
     * @return Returns a vector of pointers to each of the global motion objects.  Returned variable passed by
     * reference.
     */
    std::vector<GlobalSolution*> &listGlobalSolution();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the GlobalSolution object from the list of GlobalSolution objects.
     * @sa GlobalSolution
     * @param index The index of which GlobalSolution object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalSolution object per OutputsBody.  The default value selects this object.
     * @return Returns the GlobalSolution object from the list of GlobalSolution objects. Returned variable is passed
     * by reference.  Returns only the object specified by the input index.
     */
    GlobalSolution &listGlobalSolution(unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a new GlobalSolution object to the list of GlobalSolution objects.  This version sets the new
     * GlobalSolution object equal to the information passed in.
     * @param input Pointer to the new GlobalSolution object to add to the list of GlobalSolution objects.
     * Pointer is passed by value.
     */
    void addGlobalSolution(GlobalSolution *input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Overloaded function for adding a GlobalSolution object to the list of GlobalSolution objects.
     * This version has no input GlobalSolution object and automatically creates a blank GlobalSolution object.
     */
    void addGlobalSolution();

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The vector list of Body objects.
     */
    std::vector<Body>* plistBody;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The vector list of SolutionSet objects.
     */
    std::vector<SolutionSet>* plistSolution;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave frequencies.  Wave frequency recorded in units of radians per second.
     */
    std::vector<double>* plistFreq;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave directions.  Wave direction recorded in units of radians.  Zero is true North direction.
     * Oriented positive counter-clockwise.
     */
    std::vector<double>* plistWaveDir;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The current wave direction used for calculating the DerivedOutput objects.
     */
    int pCurWaveDir;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The index of the current body used for calculating derived outputs.  This index is the position of
     * the current body in the list of Body objects.
     */
    int pCurBody;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to whichever was the last output object to generate a result set.
     *
     * This may prove useful to find any information related to which OutputDerived object actually generated the
     * result.
     */
    OutputDerived *pCurOutput;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Matrix of results calculated by one of the DerivedOutput objects.
     *
     * This matrix is used to specifically store results that require complex data storage.
     */
    std::vector<arma::cx_mat*> plistResult;

    //------------------------------------------Function Separator ----------------------------------------------------
    //------------------------------------------Function Separator ----------------------------------------------------
    //Derived Outputs Now Listed Below
    //------------------------------------------Function Separator ----------------------------------------------------
    //------------------------------------------Function Separator ----------------------------------------------------

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of GlobalMotion objects.  Records the amplitude of motions for the Body.
     */
    std::vector<GlobalMotion*> plistGlobalMotion;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of GlobalVelocity objects.  Records the velocity of motions for the Body.
     */
    std::vector<GlobalVelocity*> plistGlobalVelocity;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of GlobalAcceleration objects.  Records the acceleration of motions for the Body.
     */
    std::vector<GlobalAcceleration*> plistGlobalAcceleration;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of GlobalSolution objects.  Records the output of any derivative of solve motion amplitude
     * for the Body.
     */
    std::vector<GlobalSolution*> plistGlobalSolution;

};

}   //namespace ofreq
}   //namespace osea

#endif
