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

using namespace std;
using namespace arma;

/**
 * @brief Type definition for a vector of complex numbers.  Used to return the calculated output.
 */
typedef vector< complex<double> > cx_vector;

//######################################### Class Separator ###########################################################
//Forward declarations of classes

//######################################### Class Separator ###########################################################
/**
 * This class holds all types of derived outputs for a single body object.  Each derived output type is contained
 * in a vector.  This allows multiple instances of each output type.  To save on memory, an output type is not
 * calculated until it is requested.  And then the derived output immediately returns the calculations.  Outputs are
 * typically returned as a vector of objects.  Each row in the vectorrepresents a different wave frequency.  THe object
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
 * * Developers note:  The original scheme had the OutputDerived class include a pointer to the contain parent class,
 * OutputsBody.  But this creates a cyclic dependency of header files, and will not compile.  The only resolution I
 * found to this was to not include the parent class and pass all the necessary information to each individual
 * OutputDerived object.  This is tedious, but within the reasons of the methods defined by the OutputsBody class.
 * And it allow compilation.  All data items are passed by reference to avoid excess memory duplication.
  */

class OutputsBody : public oFreqCore
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
    OutputsBody(vector<Body> &listBod,
                vector<SolutionSet> &listSoln,
                vector<double> &listFreq,
                vector<double> &listWaveDir);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.  Nothing happens here.
     */
    ~OutputsBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the list of Body objects to calculate derived outputs for.  Derived outputs are calculated for only
     * one Body object in the list.  The rest are included as reference for cross-body forces.
     * @param listIn The vector of Body objects to assign to this OutputsBody.  Input is passed by reference.  Input
     * is held as a constant value, so that it can not be modified by the class.
     */
    void setListBody(vector<Body> &listIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the list of SolutionSet objects to calculate derived outputs for.  Derived outputs are calculated
     * for only one SolutionSet object in the list.  The rest are included as reference for cross-body forces.
     * @param listIn The vector of SolutionSet objects to assign to this OutputsBody.  Input is passed by reference.
     * Inputs is held as a constant value, so that it can not be modified by the class.
     */
    void setSolutionSet(vector<SolutionSet> &listIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the list of wave frequencies.
     * @param listIn The vector of doubles representing the wave frequencies.  Frequencies entered in units of radians
     * per second.  Input is passed by reference and held as a constant so that the class can not change the frequencies.
     */
    void setListFreq(vector<double> &listIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the list of wave frequencies.  Provides direct access to the list.
     * @return Returns the vector of doubles representing the wave frequencies.  Frequencies entered in units of radians
     * per second.  Variable is passed by reference.
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
     * @brief Sets the list of wave directions.
     * @param listIn The vector of doubles representing the wave directions.  Directions entered in units of radians.
     * Input is passed by reference and held as a constant so that the class can not change the directions.
     */
    void setListWaveDir(vector<double> &listIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the list of wave directions.  Provides direct access to the list.
     * @return Returns the vector of doubles representing the wave directions.  Directions entered in units of radians.
     * Variable is passed by reference.
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
    //------------------------------------------Function Separator ----------------------------------------------------
    //Derived Outputs Now Listed Below
    //------------------------------------------Function Separator ----------------------------------------------------
    //------------------------------------------Function Separator ----------------------------------------------------

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief returns the number of entries for the vector of GlobalMotion outputs.
     * @return Integer.  Returns the number of entries for the vector of GlobalMotion outputs.  Variable passed by
     * value.
     */
    int sizeGlobalMotion();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the GlobalMotion object from the list of GlobalMotion objects.
     * @sa GlobalMotion
     * @param index The index of which GlobalMotion object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalMotion object per OutputsBody.  The default value selects this object.
     * @return Returns the GlobalMotion object from the list of GlobalMotion objects. Returned variable is passed
     * by reference.  Returns only the object specified by the input index.
     */
    GlobalMotion &refGlobalMotion(int index = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the GlobalMotion object from the list of GlobalMotion objects and returns the output.
     * @sa GlobalMotion
     * @param index The index of which GlobalMotion object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalMotion object per OutputsBody.  The default value selects this object.
     * @return Returns a vector of vectors, which is the GlobalMotion Results.  Each row in the vector represents
     * a wave frequency.  Each entry in the vector for that frequency represents a result for one of the degrees
     * of freedom.  All entries are complex numbers.  Returned variable is passed by value.  Value is not stored in
     * memory and must be calculated new each time method is called.
     */
    vector<cx_vector> getGlobalMotion(int index = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a new GlobalMotion object to the list of GlobalMotion objects.  This version sets the new
     * GlobalMotion object equal to the information passed in.
     * @param input The new GlobalMotion object to add to the list of GlobalMotion objects.  Variable is passed by
     * value.
     */
    void addGlobalMotion(GlobalMotion input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Overloaded function for adding a GlobalMotion object to the list of GlobalMotion objects.  This version
     * has no input GlobalMotion object and automatically creates a blank GlobalMotion object.
     */
    void addGlobalMotion();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief returns the number of entries for the vector of GlobalVelocity outputs.
     * @return Integer.  Returns the number of entries for the vector of GlobalVelocity outputs.  Variable passed by
     * value.
     */
    int sizeGlobalVelocity();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the GlobalVelocity object from the list of GlobalVelocity objects.
     * @sa GlobalVelocity
     * @param index The index of which GlobalVelocity object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalVelocity object per OutputsBody.  The default value selects this object.
     * @return Returns the GlobalVelocity object from the list of GlobalVelocity objects. Returned variable is passed
     * by reference.  Returns only the object specified by the input index.
     */
    GlobalVelocity &refGlobalVelocity(int index = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the GlobalVelocity object from the list of GlobalVelocity objects and returns the output.
     * @sa GlobalVelocity
     * @param index The index of which GlobalVelocity object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalVelocity object per OutputsBody.  The default value selects this object.
     * @return Returns a vector of vectors, which is the GlobalVelocity Results.  Each row in the vector represents
     * a wave frequency.  Each entry in the vector for that frequency represents a result for one of the degrees
     * of freedom.  All entries are complex numbers.  Returned variable is passed by value.  Value is not stored in
     * memory and must be calculated new each time method is called.
     */
    vector<cx_vector> getGlobalVelocity(int index = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a new GlobalVelocity object to the list of GlobalVelocity objects.  This version sets the new
     * GlobalVelocity object equal to the information passed in.
     * @param input The new GlobalVelocity object to add to the list of GlobalVelocity objects.  Variable is passed by
     * value.
     */
    void addGlobalVelocity(GlobalVelocity input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Overloaded function for adding a GlobalVelocity object to the list of GlobalVelocity objects.  This version
     * has no input GlobalVelocity object and automatically creates a blank GlobalVelocity object.
     */
    void addGlobalVelocity();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief returns the number of entries for the vector of GlobalAcceleration outputs.
     * @return Integer.  Returns the number of entries for the vector of GlobalAcceleration outputs.  Variable passed by
     * value.
     */
    int sizeGlobalAcceleration();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the GlobalAcceleration object from the list of GlobalAcceleration objects.
     * @sa GlobalAcceleration
     * @param index The index of which GlobalAcceleration object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalAcceleration object per OutputsBody.  The default value selects this object.
     * @return Returns the GlobalAcceleration object from the list of GlobalAcceleration objects. Returned variable is passed
     * by reference.  Returns only the object specified by the input index.
     */
    GlobalAcceleration &refGlobalAcceleration(int index = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the GlobalAcceleration object from the list of GlobalAcceleration objects and returns the output.
     * @sa GlobalAcceleration
     * @param index The index of which GlobalAcceleration object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalAcceleration object per OutputsBody.  The default value selects this object.
     * @return Returns a vector of vectors, which is the GlobalAcceleration Results.  Each row in the vector represents
     * a wave frequency.  Each entry in the vector for that frequency represents a result for one of the degrees
     * of freedom.  All entries are complex numbers.  Returned variable is passed by value.  Value is not stored in
     * memory and must be calculated new each time method is called.
     */
    vector<cx_vector> getGlobalAcceleration(int index = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a new GlobalAcceleration object to the list of GlobalAcceleration objects.  This version sets the new
     * GlobalAcceleration object equal to the information passed in.
     * @param input The new GlobalAcceleration object to add to the list of GlobalAcceleration objects.  Variable is
     * passed by value.
     */
    void addGlobalAcceleration(GlobalAcceleration input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Overloaded function for adding a GlobalAcceleration object to the list of GlobalAcceleration objects.
     * This version has no input GlobalAcceleration object and automatically creates a blank GlobalAcceleration object.
     */
    void addGlobalAcceleration();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief returns the number of entries for the vector of GlobalSolution outputs.
     * @return Integer.  Returns the number of entries for the vector of GlobalSolution outputs.  Variable passed by
     * value.
     */
    int sizeGlobalSolution();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the GlobalSolution object from the list of GlobalSolution objects.
     * @sa GlobalSolution
     * @param index The index of which GlobalSolution object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalSolution object per OutputsBody.  The default value selects this object.
     * @return Returns the GlobalSolution object from the list of GlobalSolution objects. Returned variable is passed
     * by reference.  Returns only the object specified by the input index.
     */
    GlobalSolution &refGlobalSolution(int index = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the GlobalSolution object from the list of GlobalSolution objects and returns the output.
     * @sa GlobalSolution
     * @param index The index of which GlobalSolution object to retrieve from the list of objects.  For this Derived
     * Output, there is only one GlobalSolution object per OutputsBody.  The default value selects this object.
     * @return Returns a vector of vectors, which is the GlobalSolution Results.  Each row in the vector represents
     * a wave frequency.  Each entry in the vector for that frequency represents a result for one of the degrees
     * of freedom.  All entries are complex numbers.  Returned variable is passed by value.  Value is not stored in
     * memory and must be calculated new each time method is called.
     */
    vector<cx_vector> getGlobalSolution(int index = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a new GlobalSolution object to the list of GlobalSolution objects.  This version sets the new
     * GlobalSolution object equal to the information passed in.
     * @param input The new GlobalSolution object to add to the list of GlobalSolution objects.  Variable is
     * passed by value.
     */
    void addGlobalSolution(GlobalSolution input);

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
    int pCurWaveDir;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The index of the current body used for calculating derived outputs.  This index is the position of
     * the current body in the list of Body objects.
     */
    int pCurBody;

    //------------------------------------------Function Separator ----------------------------------------------------
    //------------------------------------------Function Separator ----------------------------------------------------
    //Derived Outputs Now Listed Below
    //------------------------------------------Function Separator ----------------------------------------------------
    //------------------------------------------Function Separator ----------------------------------------------------

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of GlobalMotion objects.  Records the amplitude of motions for the Body.
     */
    vector<GlobalMotion> pGlobalMotion;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of GlobalVelocity objects.  Records the velocity of motions for the Body.
     */
    vector<GlobalVelocity> pGlobalVelocity;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of GlobalAcceleration objects.  Records the acceleration of motions for the Body.
     */
    vector<GlobalAcceleration> pGlobalAcceleration;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of GlobalSolution objects.  Records the output of any derivative of solve motion amplitude
     * for the Body.
     */
    vector<GlobalSolution> pGlobalSolution;

};
#endif
