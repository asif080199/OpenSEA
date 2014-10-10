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
 *Aug 02 2014       Nicholas Barczak    Initially created
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
#ifndef HYDROMANAGER_H
#define HYDROMANAGER_H
#include <math.h>
#include "../global_objects/mathinterp.h"
#include "hydrodata.h"
#ifndef FORCEACTIVE_H
#include "../global_objects/forceactive.h"
#endif
#ifndef FORCECROSS_H
#include "../global_objects/forcecross.h"
#endif
#ifndef FORCEREACT_H
#include "../global_objects/forcereact.h"
#endif
#ifdef Q_OS_WIN
    #include "../../lib/armadillo.h"  //References the armadillo library in lib folder.
#elif defined Q_OS_LINUX
    #include <armadillo>    //Armadillo library included with standard system libraries.
#endif

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
 * @brief The HydroManager class contains the complete set of hydrodynamic data associated with a single body.
 *
 * The most important aspect are all the variations possible for hydrodynamic data.  The data can vary with wave
 * frequency, wave direction, and wave amplitude.  The HydroManager has methods for interpolating amongst all that data
 * to create a single output for a requested set of conditions (some combination of wave frequency, wave direction,
 * and wave amplitude.)
 *
 * Note that interpolation for wave direction does have a limitation:  it will only permit interpolation with wave
 * directions that are within 0.785 rad (45 deg) of the available data.  This is due to the physics limitations.  Any
 * data more than 45 deg away from the desired direction really is not relevant.
 */
class HydroManager : public mathInterp
{
//==========================================Section Separator =========================================================
public:

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Nothing happens here.
     */
    HydroManager();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
      * @brief Default destructor.  Nothing happens here.
      */
    ~HydroManager();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a full list of the hydroData available for this body.
     *
     * List is organized by two indices.
     * Outer index (rows) = Wave Amplitude
     * Inner index (columns) = Wave Direction
     *
     * Each individual hydroData object also contains data, organized by wave frequency.
     * @return Returns a vector of vectors or hydroData objects, all passed by reference.
     */
    std::vector< std::vector< ofreq::hydroData > > &listHydroData();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a single item in the list of hydroData available.
     * @param ampInd Integer, passed by value.  The index of the wave amplitude that you want to retrieve.
     * @param dirInd Integer, passed by value.  The index of the wave direction you want to retrieve.
     * @return Returns a hydroData object, passed by reference
     */
    ofreq::hydroData &listHydroData(int ampInd, int dirInd);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a hydroData object to the set of available data.
     *
     * If an amplitude index is specified, the hydroData object will be added as a new wave direction for the
     * given amplitude index.  If no amplitude index is specified, a whole new entry for amplitude index will be
     * created.
     * @param ampInd Integer, passed by value.  The index of the outer loop where you want to store the hydroData.
     * @param dataIn hydroData object, passed by value.  The hydroData object that you want to add to the list of
     * available data for this system.
     */
    void addHydroData(int ampInd = -1, ofreq::hydroData dataIn= hydroData());

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a hydroData object to the set of available data.
     *
     * @param dataIn hydroData object, passed by value.  The hydroData object that you want to add to the list of
     * available data for this system.
     */
    void addHydroData(ofreq::hydroData dataIn= hydroData());

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the name of the hydrobody associated with this data set.
     * @return Returns a string, passed by value.  The name of the hydrobody associated with this data set.
     */
    std::string getHydroBodyName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the name of the hydrobody associated with this data set.
     * @param nameIn String, passed by value.  The name of the hydrobody associated with this data set.
     */
    void setHydroBodyName(std::string nameIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the final set of hydrodata.
     *
     * Calculates the final set of hydrodata, after all interpolations for wave frequency, wave direction, and
     * scaling for wave amplitude.  The hydroData is stored as a private variable.  Individual data objects can be
     * retrieved with the functions getForceActive(), getForceReact(), and getForceCross().
     * @param waveAmp Double, passed by value.  The specific wave amplitude that you want the data for.  Data will
     * be scaled up to match the given wave amplitude.
     * @param waveFreq Double, passed by value.  The specific wave frequency that you want the data for.  Data will
     * be interpolated for the given wave frequency.
     * @sa HydroManager::setWaveDirection()
     * @sa HydroManager::getForceActive()
     * @sa HydroManager::getForceReact()
     * @sa HydroManager::getForceCross()
     */
    void calcHydroData(double waveAmp, double waveFreq);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the forceActive object associated the requested environmental data.
     *
     * Retrieves a single forceActive object based on specification of the following parameters:  wave direction
     * (wave direction set seperately), wave amplitude, and wave frequency.  All interpolation and wave scaling
     * is triggered through the function calcHydroData.
     * @return Returns a pointer to the forceActive object, specific to the environmental conditions you specified.
     * Pointer returned by value.
     * @sa HydroManager::setWaveDirection()
     * @sa HydroManager::calcHydroData()
     */
    ofreq::ForceActive *getForceActive();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the forceReact object associated the requested environmental data.
     *
     * Retrieves a single forceReact object based on specification of the following parameters:  wave direction
     * (wave direction set seperately), wave amplitude, and wave frequency.  All interpolation and wave scaling
     * is triggered through the function calcHydroData.
     * @return Returns a pointer to the forceReact object, specific to the environmental conditions you specified.
     * Pointer returned by value.
     * @sa HydroManager::setWaveDirection()
     * @sa HydroManager::calcHydroData()
     */
    ofreq::ForceReact *getForceReact();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the forceCross object associated the requested environmental data.
     *
     * Retrieves a single forceCross object based on specification of the following parameters:  wave direction
     * (wave direction set seperately), wave amplitude, and wave frequency.  All interpolation and wave scaling
     * is triggered through the function calcHydroData.
     * @param linkedBodName String, passed by value.  The name of the linked body that you want to retrieve the
     * forceCross object for.
     * @return Returns a pointer to the forceCross object, specific to the environmental conditions you specified.
     * Pointer returned by value.
     * @sa HydroManager::setWaveDirection()
     * @sa HydroManager::calcHydroData()
     */
    ofreq::ForceCross *getForceCross(std::string linkedBodName);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the wave direction for the current set of analysis.
     *
     * hydroData gets retrieved for a specific set of environmental conditions.  One of those conditions is the
     * wave direction.  This gets set separately from any other conditions because for each wave direction, there
     * are usually several variations of wave amplitude and wave frequency.
     * @param dirIn Double, passed by value.  The wave direction for the current analysis.
     * The wave direction is defined in the body coordinate system, in units of radians (rad).  Zero rad corresponds
     * to a direction pointing along the X-axis of the body.  The angle rotates counter-clockwise.
     */
    void setWaveDir(double dirIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the wave direction for the current set of analysis.
     *
     * hydroData gets retrieved for a specific set of environmental conditions.  One of those conditions is the
     * wave direction.  This gets set separately from any other conditions because for each wave direction, there
     * are usually several variations of wave amplitude and wave frequency.
     * @param dirIn Double, passed by value.  The wave direction for the current analysis.
     * The wave direction is defined in the body coordinate system, in units of radians (rad).  Zero rad corresponds
     * to a direction pointing along the X-axis of the body.  The angle rotates counter-clockwise.
     */
    double getWaveDir();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Defines the gravity defined for this hydromanager set.
     *
     * The gravity definition is a vector quantity to allow the possibility that gravity might change, depending on the
     * definition of the coordinate system.  It may seem odd, but ofreq is about the concept of flexiblity.  And you
     * never know.  Some scientistic might want to research wave dynamics on Jupiter.  So gravity was made flexible.
     * @param gravIn Vector, passed by value.  The definition for gravity associated with this hydromanager.
     */
    void setGravity(std::vector<double> gravIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the gravity defined for this hydromanager set.
     *
     * The gravity definition is a vector quantity to allow the possibility that gravity might change, depending on the
     * definition of the coordinate system.  It may seem odd, but ofreq is about the concept of flexiblity.  And you
     * never know.  Some scientistic might want to research wave dynamics on Jupiter.  So gravity was made flexible.
     * @return Vector, passed by value.  The definition for gravity associated with this hydromanager.
     */
    std::vector<double> getGravity();

//==========================================Section Separator =========================================================
protected:


//==========================================Section Separator =========================================================
private:

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of all hydro data associated with this body.
     *
     * List is organized by two indices.
     * Outer index (rows) = Wave Amplitude
     * Inner index (columns) = Wave Direction
     *
     * Each individual hydroData object also contains data, organized by wave frequency.
     */
    std::vector< std::vector< ofreq::hydroData > > plistHydroData;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name of the hydrobody associated with this data.
     *
     * The hydrobody name is used to match the hydrodata to the correct body in the analysis.
     */
    std::string pHydroBodyName;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The subset of hydroData specific to the given wave direction.
     *
     * This subset is formed after a new wave direction gets set.  The function buildWaveSubset() generates this
     * subset.  It is a smaller subset of data only specific to the selected wave direction.
     * The subset contains pointers to each of the two wave directions closest to the selected wave direction.
     * Each row / outer index will only contain two entries:  the two wave directions.  This will also eliminate any
     * given wave amplitudes that do not include data suitable to the the specified wave directions.
     */
    std::vector< std::vector< ofreq::hydroData* > > plistDirSubset;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The wave diretion set for the current set of hydro data.
     *
     * hydroData gets retrieved for a specific set of environmental conditions.  One of those conditions is the
     * wave direction.  This gets set separately from any other conditions because for each wave direction, there
     * are usually several variations of wave amplitude and wave frequency.
     *
     * The wave direction is defined in the body coordinate system, in units of radians (rad).  Zero rad corresponds
     * to a direction pointing along the X-axis of the body.  The angle rotates counter-clockwise.
     */
    double pWaveDir;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Builds the subset of hydroData specific to the given wave direction.
     *
     * This subset is formed after a new wave direction gets set.  The function buildWaveSubset() generates this
     * subset.  It is a smaller subset of data only specific to the selected wave direction.
     * The subset contains pointers to each of the two wave directions closest to the selected wave direction.
     * Each row / outer index will only contain two entries:  the two wave directions.  This will also eliminate any
     * given wave amplitudes that do not include data suitable to the the specified wave directions.
     */
    void buildWaveSubset();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Scales the wave data based on specified amplitude.
     *
     * The waveScale function can take one of two approaches.  If it only has one set of hydroData to use, it will
     * perform linear scaling.  This is the classic assumption for wave scaling, but rather crude.  If it has two
     * sets of hydroData to use, it will perform non-linear scaling based on fitting of a power function to the
     * two sets of hydroData.
     * @param ampIn Double, variable passed by value.  The wave amplitude that you desire.  Everything will be scaled
     * to match this wave amplitude.
     * @param amp1 Double, variable passed by value.  The wave amplitude of the first data set.  Corresponds to the value
     * supplied in Data 1.
     * @param Data1 Double, variable passed by value.  The data magnitude of the first data set.
     * @param amp2 Double, variable passed by value.  The wave amplitude of the second data set.  Corresponds to the value
     * supplied in Data 2.
     * @param Data2 Double, variable passed by value.  The data magnitude of the second data set.
     * @return Returns a double, variable passed by value.  The output from wave scaling of the data sets for the
     * desired wave amplitude output.
     */
    double waveScale(double ampIn, double amp1, double Data1, double amp2 = 0, double Data2 = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Scales the wave data based on specified amplitude.
     *
     * The waveScale function can take one of two approaches.  If it only has one set of hydroData to use, it will
     * perform linear scaling.  This is the classic assumption for wave scaling, but rather crude.  If it has two
     * sets of hydroData to use, it will perform non-linear scaling based on fitting of a power function to the
     * two sets of hydroData.
     *
     * This is just an overload of the waveScale function to handle complex variables.
     * @param ampIn Double, variable passed by value.  The wave amplitude that you desire.  Everything will be scaled
     * to match this wave amplitude.
     * @param amp1 Double, variable passed by value.  The wave amplitude of the first data set.  Corresponds to the value
     * supplied in Data 1.
     * @param Data1 Complex<double>, variable passed by value.  The data magnitude of the first data set.
     * @param amp2 Double, variable passed by value.  The wave amplitude of the second data set.  Corresponds to the value
     * supplied in Data 2.
     * @param Data2 Complex<double>, variable passed by value.  The data magnitude of the second data set.
     * @return Returns a Complex<double>, variable passed by value.  The output from wave scaling of the data sets
     * for the desired wave amplitude output.
     */
    std::complex<double> waveScale(double ampIn, double amp1, std::complex<double> Data1, double amp2 = 0,
                                std::complex<double> Data2 = std::complex<double>(0,0));

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Scales an entire matrix of wave data based on specified amplitude.
     *
     * This invokes the wave scale function for each entry in the matrix.
     *
     * The waveScale function can take one of two approaches.  If it only has one set of hydroData to use, it will
     * perform linear scaling.  This is the classic assumption for wave scaling, but rather crude.  If it has two
     * sets of hydroData to use, it will perform non-linear scaling based on fitting of a power function to the
     * two sets of hydroData.
     * @param ampIn Double, variable passed by value.  The wave amplitude that you desire.  Everything will be scaled
     * to match this wave amplitude.
     * @param amp1 Double, variable passed by value.  The wave amplitude of the first data set.  Corresponds to the value
     * supplied in Data 1.
     * @param mat1 MatForceActive, passed by reference.  The first data set.  The matrix associated with the wave amp1.
     * @return Returns matForceActive, passed by value.  The result of wave scaling the input matrices.
     */
    ofreq::matForceActive waveScale(double ampIn, double amp1, ofreq::matForceActive &mat1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Scales an entire matrix of wave data based on specified amplitude.
     *
     * This invokes the wave scale function for each entry in the matrix.
     *
     * The waveScale function can take one of two approaches.  If it only has one set of hydroData to use, it will
     * perform linear scaling.  This is the classic assumption for wave scaling, but rather crude.  If it has two
     * sets of hydroData to use, it will perform non-linear scaling based on fitting of a power function to the
     * two sets of hydroData.
     * @param ampIn Double, variable passed by value.  The wave amplitude that you desire.  Everything will be scaled
     * to match this wave amplitude.
     * @param amp1 Double, variable passed by value.  The wave amplitude of the first data set.  Corresponds to the value
     * supplied in Data 1.
     * @param mat1 MatForceActive, passed by reference.  The first data set.  The matrix associated with the wave amp1.
     * @param amp2 Double, variable passed by value.  The wave amplitude of the second data set.  Corresponds to the value
     * supplied in Data 2.
     * @param mat2 MatForceActive, passed by reference.  The second data set.  The matrix associated with the wave amp2.
     * @return Returns matForceActive, passed by value.  The result of wave scaling the input matrices.
     */
    ofreq::matForceActive waveScale(double ampIn, double amp1, ofreq::matForceActive &mat1, double amp2,
                                    ofreq::matForceActive &mat2);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Scales an entire matrix of wave data based on specified amplitude.
     *
     * This invokes the wave scale function for each entry in the matrix.
     *
     * The waveScale function can take one of two approaches.  If it only has one set of hydroData to use, it will
     * perform linear scaling.  This is the classic assumption for wave scaling, but rather crude.  If it has two
     * sets of hydroData to use, it will perform non-linear scaling based on fitting of a power function to the
     * two sets of hydroData.
     * @param ampIn Double, variable passed by value.  The wave amplitude that you desire.  Everything will be scaled
     * to match this wave amplitude.
     * @param amp1 Double, variable passed by value.  The wave amplitude of the first data set.  Corresponds to the value
     * supplied in Data 1.
     * @param mat1 matForceReact, passed by reference.  The first data set.  The matrix associated with the wave amp1.
     * @return Returns matForceReact, passed by value.  The result of wave scaling the input matrices.
     */
    ofreq::matForceReact waveScale(double ampIn, double amp1, ofreq::matForceReact &mat1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Scales an entire matrix of wave data based on specified amplitude.
     *
     * This invokes the wave scale function for each entry in the matrix.
     *
     * The waveScale function can take one of two approaches.  If it only has one set of hydroData to use, it will
     * perform linear scaling.  This is the classic assumption for wave scaling, but rather crude.  If it has two
     * sets of hydroData to use, it will perform non-linear scaling based on fitting of a power function to the
     * two sets of hydroData.
     * @param ampIn Double, variable passed by value.  The wave amplitude that you desire.  Everything will be scaled
     * to match this wave amplitude.
     * @param amp1 Double, variable passed by value.  The wave amplitude of the first data set.  Corresponds to the value
     * supplied in Data 1.
     * @param mat1 matForceReact, passed by reference.  The first data set.  The matrix associated with the wave amp1.
     * @param amp2 Double, variable passed by value.  The wave amplitude of the second data set.  Corresponds to the value
     * supplied in Data 2.
     * @param mat2 matForceReact, passed by reference.  The second data set.  The matrix associated with the wave amp2.
     * @return Returns matForceReact, passed by value.  The result of wave scaling the input matrices.
     */
    ofreq::matForceReact waveScale(double ampIn, double amp1, ofreq::matForceReact &mat1, double amp2,
                                    ofreq::matForceReact &mat2);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Scales an entire matrix of wave data based on specified amplitude.
     *
     * This invokes the wave scale function for each entry in the matrix.  The wave scaling only scales a single
     * matForceCross object.  Any vectors of objects must be handled one at a time.
     *
     * The waveScale function can take one of two approaches.  If it only has one set of hydroData to use, it will
     * perform linear scaling.  This is the classic assumption for wave scaling, but rather crude.  If it has two
     * sets of hydroData to use, it will perform non-linear scaling based on fitting of a power function to the
     * two sets of hydroData.
     * @param ampIn Double, variable passed by value.  The wave amplitude that you desire.  Everything will be scaled
     * to match this wave amplitude.
     * @param amp1 Double, variable passed by value.  The wave amplitude of the first data set.  Corresponds to the value
     * supplied in Data 1.
     * @param mat1 matForceCross, passed by reference.  The first data set.  The matrix associated with the wave amp1.
     * @return Returns matForceCross, passed by value.  The result of wave scaling the input matrices.
     */
    ofreq::matForceCross waveScale(double ampIn, double amp1, ofreq::matForceCross &mat1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Scales an entire matrix of wave data based on specified amplitude.
     *
     * This invokes the wave scale function for each entry in the matrix.  The wave scaling only scales a single
     * matForceCross object.  Any vectors of objects must be handled one at a time.
     *
     * The waveScale function can take one of two approaches.  If it only has one set of hydroData to use, it will
     * perform linear scaling.  This is the classic assumption for wave scaling, but rather crude.  If it has two
     * sets of hydroData to use, it will perform non-linear scaling based on fitting of a power function to the
     * two sets of hydroData.
     * @param ampIn Double, variable passed by value.  The wave amplitude that you desire.  Everything will be scaled
     * to match this wave amplitude.
     * @param amp1 Double, variable passed by value.  The wave amplitude of the first data set.  Corresponds to the value
     * supplied in Data 1.
     * @param mat1 matForceCross, passed by reference.  The first data set.  The matrix associated with the wave amp1.
     * @param amp2 Double, variable passed by value.  The wave amplitude of the second data set.  Corresponds to the value
     * supplied in Data 2.
     * @param mat2 matForceCross, passed by reference.  The second data set.  The matrix associated with the wave amp2.
     * @return Returns matForceCross, passed by value.  The result of wave scaling the input matrices.
     */
    ofreq::matForceCross waveScale(double ampIn, double amp1, ofreq::matForceCross &mat1, double amp2,
                                    ofreq::matForceCross &mat2);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Variable for the maximum distance allowed between wave directions for the data to still be valid.
     */
    static double DISTMAX;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Finds the index of the two sets of hydrodata objects that most closely match the given amplitude.
     *
     * Searches through the wave subset.  Checks the wave amplitude for each item in the list.  And returns the
     * two amplitudes that are closest to the given amplitude.
     * @param ampIn Double, passed by value.  The amplitude that we are trying to match.
     * @return Returns a vector of integers, passed by value.  Integers are the index of the two closest wave amplitude
     * data sets.  Index pertains to the list of subset hydrodata.  If only one object is present, the returned
     * vector will have a size of one.  If more than one object is present, the returned vector will have a size
     * of two.
     */
    std::vector<int> findMatchAmplitude(double ampIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The final set of hydrodata, after all interpolation for wave frequency, wave direction, and scaling
     * for wave amplitude.
     */
    hydroData pHydroFinal;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Interpolates between two hydroData objects for wave direction.
     *
     * Uses the wave direction set by setWaveDir() function.  Two sets of hydrodata are supplied, by reference.
     * The data are then interpolated.
     * When interpolating between crossbody objects, both hydroData sets must have the exact same crossbody objects.
     * They both must be referenced by the same hydrobody names.
     * @param Data1 Hydrodata object, passed by reference.  The first hydrodata set that you want to use for
     * interpolation.
     * @param Data2 Hydrodata object, passed by reference.  The second hydrodata set that you want to use for
     * interpolation.
     * @return Returns a hydrodata object, interpolated for wave direction.  All forces are interpolated as well.
     */
    ofreq::hydroData interpWaveDir(ofreq::hydroData &Data1, ofreq::hydroData &Data2);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Performs the wave scaling for an entire hydrodata set.
     *
     * The scaling can take one of two approaches.  If it only has one set of hydroData to use, it will
     * perform linear scaling.  This is the classic assumption for wave scaling, but rather crude.  If it has two
     * sets of hydroData to use, it will perform non-linear scaling based on fitting of a power function to the
     * two sets of hydroData.
     * @param ampIn Double, passed by value.  The amplitude that everything should get scaled to.
     * @param Data1 Hydrodata object, passed by reference.  The first hydrodata set that you want to use for wave
     * scaling.
     * @return
     */
    ofreq::hydroData ScaleHydroData(double ampIn, ofreq::hydroData &Data1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Performs the wave scaling for an entire hydrodata set.
     *
     * The scaling can take one of two approaches.  If it only has one set of hydroData to use, it will
     * perform linear scaling.  This is the classic assumption for wave scaling, but rather crude.  If it has two
     * sets of hydroData to use, it will perform non-linear scaling based on fitting of a power function to the
     * two sets of hydroData.
     * @param ampIn Double, passed by value.  The amplitude that everything should get scaled to.
     * @param Data1 Hydrodata object, passed by reference.  The first hydrodata set that you want to use for wave
     * scaling.
     * @param Data2 Hydrodata object, passed by reference.  The second hydrodata set that you want to use
     * for wave scaling.
     * @return
     */
    ofreq::hydroData ScaleHydroData(double ampIn, ofreq::hydroData &Data1, ofreq::hydroData &Data2);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief vector tracking what was used for gravity in the hydrodynamic analysis.
     *
     * The gravity definition is a vector quantity to allow the possibility that gravity might change, depending on the
     * definition of the coordinate system.  It may seem odd, but ofreq is about the concept of flexiblity.  And you
     * never know.  Some scientistic might want to research wave dynamics on Jupiter.  So gravity was made flexible.
     */
    std::vector<double> pGravity;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Checks the list of wave directions to see if any 0 rad entries should be copied to 2*pi entries.
     *
     * First, check through the list of hydrodata, each amplitude.  We want to know if there is a wave direction
     * that is set at 0 rad or 2*pi rad, without its matching counter-part.  This is important because the
     * interpolation functions are based on absolute distance.  They don't understand the math concepts that the
     * directions reset after 2*pi rad.
     *
     * The methods of testing in this process assume that anything within 4% of 0 rad should be copied. (effectively
     * within 2 deg of 0).
     */
    void checkDirList();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the output forceActive object generated by the HydroManager.
     */
    ofreq::ForceActive *ptForceActive = NULL;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the output forceReact object generated by the HydroManager.
     */
    ofreq::ForceReact *ptForceReact = NULL;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the output forceCross object generated by the HydroManager.
     */
    ofreq::ForceCross *ptForceCross = NULL;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Resets the pointers to the output force objects.
     *
     * The reset happens every time one of the input definitions changes.  This is a change to a new wave direction,
     * or a new call for calculating hydrodynamic forces.
     */
    void ResetOutput();


};

}   //Namespace ofreq

}   //Namespace osea

#endif // HydroManager_H
