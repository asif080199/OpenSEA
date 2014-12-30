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
 *Dec 05, 2014	Nicholas Barczak		Initially Created
 *Dec 14, 2014  Nicholas Barczak        Debugged and updated to Rev 1.0
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
//Include statements and headers
#ifndef REPORT_H
#define REPORT_H
#include "../system_objects/ofreqcore.h"
#include "../global_objects/solutionset.h"
#include "../global_objects/body.h"
#include "data.h"
#include <cxxabi.h>

#include <QObject>
#ifdef Q_OS_WIN
    //Any windows specific inclusions go in here.
#elif defined Q_OS_LINUX
    //Any linux specific inclusions go in here.
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
//Abstract Class Declarations
class ReportManager;    /**< Report Manager object declaration */
class System;   /**< System object declaration */

//######################################### Class Separator ###########################################################
/**
 * @brief Abstract class for any body summary outputs from the oFreq program.
 *
 * The reports class is a subset of outputs from oFreq.  Each output is related to a summary of body performance.
 * The outputs can take on many forms and each may depend on different information.  Some outputs required additional
 * user input.  The one common thread is that each output is keyed to only a single Body.  The output represents some
 * characteristics for the performance of that entire Body object.  Outputs are calculated, and then provide a vector
 * of data points, organized by wave frequency.  All outputs are eventually written out as text data files, organized
 * by wave frequency.
 */
class Report : public ofreq::oFreqCore
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Sets a pointer to the system object.
     * @param ptIn Pointer to ReportManager object that owns this Report object, pointer passed by value.
     */
    Report(ofreq::ReportManager *ptIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    Report();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
      * @brief Default destructor.  Erases all dynamic memory associated with object.
      */
    ~Report();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the pointer to the System object.
     * @param ptIn The pointer to the System object.  Pointer passed by value.
     */
    void setSystem(ofreq::System *ptIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the pointer to the System object.
     * @return Pointer to the System object, pointer passed by value.
     */
    ofreq::System* getSystem();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Set the pointer to the Body object associated with this Report.
     * @param ptIn The pointer to the Body object associated with this report, Pointer passed by value.
     */
    void setBody(ofreq::Body *ptIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Get the pointer to the Body object associated with this Report.
     * @return The pointer to the Body object associated with this Report, pointer passed by value.
     */
    ofreq::Body* getBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the index of the Body object, in the list of Body objects.
     *
     * This is useful because Reports will be organized into folders by Body objects.  Each folder is numbered by the
     * index of the Body object.  So the Report must have a body index associated with this, to ensure it gets placed
     * in the correct folder.
     * @return Returns integer, passed by value.  The index of the Body object assocated with this Report.  Index
     * based on a zero-based numbering system.
     */
    int getBodIndex();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets a pointer to the ReportManager object which contains this Report.
     * @param ptIn Pointer to the ReportManager object which contains this Report, pointer passed by value.
     */
    void setManager(ofreq::ReportManager *ptIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets a pointer to the ReportManger object which contains this Report.
     * @return Pointer to the ReportManager object which contains this Report, pointer passed by value.
     */
    ofreq::ReportManager* getManager();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the name associated with this Report object.
     *
     * This is the name that the user will recognize the Report by.  Each class of report gets written to its own
     * file.  But there may be multiple instances of each class of report generated.  The name is how the user will
     * distinguish one Report from another.
     * @param nameIn String variable, passed by value.  The name of the Report assigned by the user.
     */
    void setName(std::string nameIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the name associated with this Report object.
     *
     * This is the name that the user will recognize the Report by.  Each class of report gets written to its own
     * file.  But there may be multiple instances of each class of report generated.  The name is how the user will
     * distinguish one Report from another.
     * @return String variable, passed by value.  The name of the Report assigned by the user.
     */
    std::string getName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the name of the class associated with this Report object.
     *
     * The Report class is a pure virtual class.  There are multiple different types of reports, each derived from
     * this abstract class.  oFreq needs to know the class of the report, to know which file to write the report out
     * to.  This is done with the getClass() function.  This uses macros in the code.
     * @return Returns the name of the Report class.  Returned variable is a string, variable passed by value.
     */
    virtual std::string getClass();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides the filename associated with this Report object.
     * @return Returns a string object, passed by value.  String is the filename associated with this Report.
     */
    virtual std::string getFileName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pure virtual member.  Generates the Report Data.
     *
     * Writes the results of the calculation to the vector of Data values.  Calling the calcReport function only
     * generates the values.  They must be retrieved from the object after calculation, using the listData() function.
     *
     * @param freqInd Integer, variable passed by value.  The wave frequency index to use for calculating the Report
     * object's data.  Specifies the index of the wave frequency to retrieve from the list of wave frequencies.
     * Most Report values will depend on the wave frequency specified.
     */
    virtual void calcReport(int freqInd = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pure virtual member.  Calculates the RAO report data.
     *
     * Write the results of the calculation of the vector of RAO Data values.  Calling calcRAO function only
     * generates the value.  THey must be retrieved from the object after calculations, using the listRAO() function.
     *
     * @param freqInd Integer, variable passed by value.  The wave frequency index to use for calculating the Report
     * object's data.  Specifies the index of the wave frequency to retrieve from the list of wave frequencies.
     * Most Report values will depend on the wave frequency specified.
     */
    virtual void calcRAO(int freqInd = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Triggers whether the report should calculate the RAO or not.
     *
     * Sets whether the Report should calculate the RAO value.  If set to no, the calcRAO function will do nothing.
     * if set to yes (true), the calcRAO function works.  By default, this is set to true.
     * @param calcYes Boolean input, passed by value.  Determines whether the calcRAO function produces output.
     * if set to true, the calcRAO function will produce output.
     */
    void calcRAO(bool calcYes);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Clears the list of Data objects.  Erases any existing data objects.
     */
    void clearData();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Lists the Data calculated by the report.
     *
     * Returns the list of Data objects calculated by the Report object.  Until the calcReport() function is executed,
     * this list will be blank.
     * @return Returns vector of Data objects, returned vector passed by reference.
     */
    std::vector< ofreq::Data > &listData();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Lists a single entry in vector of Data calculated by the report.
     *
     * Returns a single entry from the list of Data objects calculated by the Report object.  Until the calcReport()
     * function is executed, this list will be blank.  Single entry is specified by the input, indexIn.
     * @param indexIn Integer, variable passed by value.  The index of the Data object you wish to retrieve.
     * @return Returns single entry from the vector of Data objects.  Single entry is specified by the indexIn
     * variable.  Returned value is a Data object, variable passed by reference.
     */
    ofreq::Data &listData(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Lists the RAO's calculated by the report.
     *
     * Returns the list of RAO objects calculated by the Report object.  Until the calcRAO() function is executed,
     * this list will be blank.  The calcRAO() function requires the calcReport() function as a pre-requisite.
     * @return Returns a vector of Data objects, returned vector passed by reference.
     */
    std::vector< ofreq::Data > &listRAO();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Lists a single entry in the vector of RAO's calculated by the report.
     *
     * Returns a single entry from the list of RAO objects calculated by the Report object.  Until the calcRAO()
     * function is executed, this list will be blank.  The calcRAO() function requires the calcReport() function as
     * a pre-requisite.
     * @param indexIn Integer, variable passed by value.  The index of the Data object you wish to retrieve.
     * @return Returns single entry from the vector of RAO objects.  Single entry is specified by the indexIn
     * parameter.  Returned object is a Data object, variabled passed by reference.
     */
    ofreq::Data &listRAO(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the value of the data type expected for values from the Data objects.
     *
     * All data is internally stored as complex<double> data in the report object.  However, it can be extracted as
     * different data types (complex, double, or int).  This variable tells outside functions how to expect the data
     * to be retrieved.  There are different retrieval functions, based on expected data type.  This variable just
     * tells which data type to expect.  Possible values:
     * 0:   Complex<double>
     * 1:   Double
     * 2:   Int
     * 3:   String
     * @return Returns integer, passed by value.  Integer specifies what type of data is expected.
     */
    int getDataType();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the current wave direction index for the Report.
     *
     * Sets the current wave direction index for the Report.  The report retrieves data from several other objects.
     * It must know the current wave direction index to retrieve the correct set of data.
     * @param indIn Integer, variable passed by value.  The index of the current wave direction.
     */
    void setCurWaveInd(int indIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Add a constant to the Report for calculation.
     *
     * Adds a constant to the Report.  Constants are user inputs that the report uses for completing the calculation.
     * Not all Reports have constants.  Constants can be any of the following data types:
     * complex<double>
     * int
     * double
     * string
     * Data (object)
     * @param keyIn The name associated with this constant.  Used to identify the constant.  String, variable passed
     * by value.
     * @param valIn The value of the constant.  String, variable passed by value.
     */
    void addConst(std::string keyIn, std::string valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Add a constant to the Report for calculation.
     *
     * Adds a constant to the Report.  Constants are user inputs that the report uses for completing the calculation.
     * Not all Reports have constants.  Constants can be any of the following data types:
     * complex<double>
     * int
     * double
     * string
     * Data (object)
     * @param keyIn The name associated with this constant.  Used to identify the constant.  String, variable passed
     * by value.
     * @param valIn The value of the constant.  Integer, variable passed by value.
     */
    void addConst(std::string keyIn, int valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Add a constant to the Report for calculation.
     *
     * Adds a constant to the Report.  Constants are user inputs that the report uses for completing the calculation.
     * Not all Reports have constants.  Constants can be any of the following data types:
     * complex<double>
     * int
     * double
     * string
     * Data (object)
     * @param keyIn The name associated with this constant.  Used to identify the constant.  String, variable passed
     * by value.
     * @param valIn The value of the constant.  Double, variable passed by value.
     */
    void addConst(std::string keyIn, double valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Add a constant to the Report for calculation.
     *
     * Adds a constant to the Report.  Constants are user inputs that the report uses for completing the calculation.
     * Not all Reports have constants.  Constants can be any of the following data types:
     * complex<double>
     * int
     * double
     * string
     * Data (object)
     * @param keyIn The name associated with this constant.  Used to identify the constant.  String, variable passed
     * by value.
     * @param valIn The value of the constant.  Complex<double>, variable passed by value.
     */
    void addConst(std::string keyIn, std::complex<double> valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Add a constant to the Report for calculation.
     *
     * Adds a constant to the Report.  Constants are user inputs that the report uses for completing the calculation.
     * Not all Reports have constants.  Constants can be any of the following data types:
     * complex<double>
     * int
     * double
     * string
     * Data (object)
     * @param keyIn The name associated with this constant.  Used to identify the constant.  String, variable passed
     * by value.
     * @param valIn The value of the constant.  Vector of doubles, variable passed by value.
     */
    void addConst(std::string keyIn, std::vector<double> valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Add a constant to the Report for calculation.
     *
     * Adds a constant to the Report.  Constants are user inputs that the report uses for completing the calculation.
     * Not all Reports have constants.  Constants can be any of the following data types:
     * complex<double>
     * int
     * double
     * string
     * Data (object)
     * @param keyIn The name associated with this constant.  Used to identify the constant.  String, variable passed
     * by value.
     * @param valIn The value of the constant.  Vector of integers, variable passed by value.
     */
    void addConst(std::string keyIn, std::vector<int> valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Add a constant to the Report for calculation.
     *
     * Adds a constant to the Report.  Constants are user inputs that the report uses for completing the calculation.
     * Not all Reports have constants.  Constants can be any of the following data types:
     * complex<double>
     * int
     * double
     * string
     * Data (object)
     * @param keyIn The name associated with this constant.  Used to identify the constant.  String, variable passed
     * by value.
     * @param valIn The value of the constant.  Vector of complex<double> variables, variable passed by value.
     */
    void addConst(std::string keyIn, std::vector< std::complex<double> > valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Add a constant to the Report for calculation.
     *
     * Adds a constant to the Report.  Constants are user inputs that the report uses for completing the calculation.
     * Not all Reports have constants.  Constants can be any of the following data types:
     * complex<double>
     * int
     * double
     * string
     * Data (object)
     * @param keyIn The name associated with this constant.  Used to identify the constant.  String, variable passed
     * by value.
     * @param datIn The value of the constant.  Data object.  Sends the data in directly.  Variable passed by
     * value.
     */
    void addConst(std::string keyIn, ofreq::Data datIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a list of the constant keys included in this Report.
     * @return Returns a vector of strings, variable passed by reference.  Returned variables are the key names used
     * in the key-value pairs for constants.
     */
    std::vector<std::string> &listConstKey();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a single entry from the list of constant keys included in the Report.
     * @param indexIn Integer, variable passed by value.  The index of the key that you want to retrieve the name
     * for.
     * @return Returns a single string, variable passed by reference.  Returned variable is the key name, requested
     * by the indexIn variable.
     */
    std::string &listConstKey(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a list of the constant values associated with the constants included in this Report.
     * @return Returns a vector of Data objects, passed by reference.  Returned variables are the values to the
     * key-value pairs.
     */
    std::vector<ofreq::Data> &listConstVal();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a single entry from the list of constant values associated with the constants included in this
     * Report.
     * @param indexIn Integer, variable passed by value.  The index of the key that you want to retrieve the name for.
     * @return Returns a single entry from the Vector of Data objects, single entry passed by reference.  Returned
     * variables are the values for the key-value pairs.
     */
    ofreq::Data &listConstVal(int indexIn);

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The index of the current wave direction.
     *
     * Most of the report writing functions need to know the current wave index so that they know which directory to
     * put results in.  This private variable just tracks that result, so that it will be easier to reference constantly.
     */
    int curWaveInd;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the System object.
     */
    ofreq::System *ptSystem = NULL;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the Body object.
     */
    ofreq::Body *ptBody = NULL;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Index of the Body object that this Report references.
     */
    int pBodIndex = -1;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief List of data objects.  The results of the calculation of the report.  Vector of Data is only generated
     * after report gets calculated.
     */
    std::vector<ofreq::Data> plistData;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The results of the calculation of RAO for report.
     *
     * List of Data objects.  Vector of Data is only generated after report gets calculated.
     */
    std::vector<ofreq::Data> plistRAO;

    //------------------------------------------Function Separator ----------------------------------------------------
    bool pCalcRAO = true;   /**< Boolean to track if should calculate RAO values.  Set to true by default. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name assigned to this report.
     */
    std::string pName = "";

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the Reports Manager object.
     */
    ofreq::ReportManager *ptManager = NULL;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Records the format of the Report data type.
     *
     * All data is internally stored as complex<double> data in the report object.  However, it can be extracted as
     * different data types (complex, double, or int).  This variable tells outside functions how to expect the data
     * to be retrieved.  There are different retrieval functions, based on expected data type.  This variable just
     * tells which data type to expect.  Possible values:
     * 0:   Complex<double>
     * 1:   Double
     * 2:   Int
     */
    int pDataType = 0;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of key names for constants used to calculate this Report.
     *
     * Some repors require user input.  These inputs get stored in the Report constants vector.  The vector allows
     * for an unlimited number of constants.  Each constant is a single piece of information.  If that information
     * requires multiple data entries, the Report allows storage of multiple values for a single constant.  An
     * example would be 3D coordinates.  Those would all be stored under a single constant value.
     *
     * This vector is the list of key names associated with each constant.
     */
    std::vector<std::string> plistConst_Key;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of key values for constants used to calculate this Report.
     *
     * Some repors require user input.  These inputs get stored in the Report constants vector.  The vector allows
     * for an unlimited number of constants.  Each constant is a single piece of information.  If that information
     * requires multiple data entries, the Report allows storage of multiple values for a single constant.  An
     * example would be 3D coordinates.  Those would all be stored under a single constant value.
     *
     * This vector is the list of key names associated with each constant.
     */
    std::vector<ofreq::Data> plistConst_Val;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Boolean to track if Report constants need to be calculated.
     */
    bool pConstCalc = true;

//==========================================Section Separator =========================================================
private:



};

}   //Namespace ofreq

}   //Namespace osea

#endif // REPORT_H
