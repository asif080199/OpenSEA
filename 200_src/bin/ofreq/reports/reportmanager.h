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
#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H
#include <string>
#include <typeinfo>
#include <QtGlobal>
#include <QDir>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QIODevice>
#include "../system_objects/ofreqcore.h"
#include "report.h"
#include "../global_objects/ioword.h"

//Report Classes
#include "repdirections.h"
#include "repfrequencies.h"
#include "repglobacceleration.h"
#include "repglobmotion.h"
#include "repglobvelocity.h"
#include "repglobsolution.h"
#include "replocalacceleration.h"
#include "replocalsolution.h"
#include "repwavespectra.h"
#include "repstaticbodyaccel.h"
#include "repdesignaccel.h"
#include "repdynfreeboard.h"
#include "repbodforces.h"
#include "repbodpower.h"
#include "repcapturewidth.h"
#include "repreleff.h"
#include "repabseff.h"
#include "repbodacceleration.h"
#include "repbodmotion.h"
#include "repbodvelocity.h"
#include "repbodsolution.h"

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


//######################################### Class Separator ###########################################################
/**
 * @brief The ReportManager class
 */
class ReportManager : public ofreq::ioword
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor object.
     */
    ReportManager();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor object, including a pointer to the system object.
     * @param ptIn The pointer to the System object.  Pointer passed by value.
     */
    ReportManager(ofreq::System *ptIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.
     */
    ~ReportManager();

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
     * @brief Returns the full list of reports contained in this Report Manager.
     *
     * The list of reports will contain multiple different class types, each inherrited from the Report class type.
     * @return Returns vector of pointers to Report objects.  Vector returned by reference.
     */
    std::vector< ofreq::Report* > &listReport();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a pointer to a single report object contained in the list Reports.
     *
     * The list of reports will contain multiple different class types, each inherrited from the Report class type.
     * @param indexIn Integer, variable passed by value.  Specifies which report to return from the list of Reports.
     * @return Returns a pointer to a single Report object.  The class type can be any of a series of classes which
     * inherit from the Report class.  Returned pointer passed by reference.
     */
    ofreq::Report* &listReportPt(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates all reports and writes results to output files for all wave directions.
     *
     * Perfoms calculations and report writing for all wave directions.  This one command is all that you need to
     * generate all reports contained in the ReportManager.  This command will also generate reports for wave
     * frequencies and wave directions.  Technically, these two reports do not fall under any single wave direction.
     */
    void writeReport();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates all reports and writes results to output files for a single wave direction.
     *
     * Performs calculations and report writing for all reports in a single given wave direction.  This one command
     * will generate and write all reports for a single specified wave direction.
     *
     * This command does not include any options to access the reports for wave directions and wave frequencies.  These
     * two reports do not fit inside any wave direction.
     * @param waveIndexIn Integer, variable passed by value.  The index of the wave direction that you want to
     * calculate outputs for.
     */
    void writeReport(int waveIndexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the given report and writes results to output files for a single wave direction and report.
     *
     * Performs calculations and report writing for a single report, specified by ReportIn.  Function is limited to
     * operating on a single wave direction, specified by waveIndexIn.
     *
     * This command can also access the reports for wave directions and wave frequencies.  These two reports do not
     * depend on wave direction.  Hence the waveIndexIn for these two reports has no consequence on the output.
     * Some integer input is still required, to complete the form of the function, but that input has no meaning.
     * @param waveIndexIn Integer, variable passed by value.  The index of the wave direction that you want to
     * calculate outputs for.
     * @param ReportIn Pointer to the Report that you want to calculate and write out.  Pointer passed by value.
     */
    void writeReport(int waveIndexIn, ofreq::Report *ReportIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates all reports for a single wave direction.
     *
     * Performs all report calculations for a single wave direction.  This one command will iterate through all
     * the reports contained in the ReportManager and perform calculations, using the specified wave direction.  Only
     * performs the calculations of reports.  Results are not written to output files.
     * @param waveIndexIn Integer, variable passed by value.  The index of the wave direction that you want to
     * calculate outputs for.
     */
    void calcReport(int waveIndexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the specified report for a single wave direction.  Only calculates the single report.
     *
     * Performs all calculations on the report for a single wave direction.  Only performs the calculations of
     * Report.  Results are not written to output files.
     * @param waveIndexIn Integer, variable passed by value.  The index of the wave direction that you want to
     * calculate outputs for.
     * @param ReportIn Pointer to the Report that you want to calculate and write out.  Pointer passed by value.
     */
    void calcReport(int waveIndexIn, ofreq::Report *ReportIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Clear all reports of generated data.
     *
     * Clears all reports of generated data.  But the reports are still present and ready to calculate for another
     * set of wave directions.  Useful for keeping memory requirements down.
     */
    void clearReports();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Clear all generated report files.
     *
     * Goes through each wave direction and deletes any files found in the wave direction.  Erases any files
     * previously found.  Only erases known output files and directories.  Does not eliminate any unknown files, or
     * any input files.
     * @return Returns true if all files cleared successfully.  Returns false if some files were not deleted.
     * A lack of success is not necessarily an error in the program.  There could also be some user files in the
     * directory, which the program will not delete.
     */
    bool clearFiles();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a report to the list of reports in the ReportManager.
     * @param ReportIn Pointer to the Report in question.  Pointer passed by value.
     */
    void addReport(ofreq::Report *ReportIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads in from input file the header to be used in all files.  This is a basic header text that should
     * be at the top of all OpenSEA output files.  Simple identification of the program.  Nothing specific for output.
     * @param filePathIn String variable specifying the full location of the folder which has the text for the header
     * file.  Header file must be a simple ASCII text file.
     */
    void setHeader(std::string filePathIn);


//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the directory to write files into.
     *
     * Looks at the provided report object and determines the full path to the directory that the report needs to
     * write into.  This includes the path to the wave direction, and the path to the corect body directory.
     * The function first checks to see if these directories exist.  If so, it simply returns the path to that
     * directory.  If the directories do not exist, the function creates the directories and then returns the
     * path to them.
     * @param ReportIn Pointer to Report object, pointer passed by value.  The Report object that you are trying
     * to write output from.  All written output from Reports is organized into an ordered set of directories.
     * The function reads the properties of the Report to determine which set of directories you need.
     * @param CurWaveInd This is not a formal parameter.  It is a private variable set as part of the ReportManager
     * class.  This variable gets automatically set when you trigger a writeReport() function.
     * @return Returns a QString, variable passed by value.  Returned variable is the path to the directory that you
     * want to add the files to.
     */
    QString setDirectory(ofreq::Report *ReportIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the file to write output to.
     *
     * Looks at the specified Report object and determines the file which should go with the report type.  It then
     * checks the specified directory path to see if the file already exists.  If so, it opens the file and returns
     * the reference to that file.  If no file exists, it first creates the file and adds the appropriate header
     * information.  Then it returns the path to that file.
     * @param ReportIn Pointer to Report object, pointer passed by value.  The Report object that you are trying
     * to write output from.  All written output from Reports is organized into an ordered set of directories and
     * files.  The function reads the properties of the Report to determine which set of files you need.
     * @param dirIn QString object, variable passed by value.  The path to the directory where the file should go.
     */
    void setFile(ofreq::Report *ReportIn, QString dirIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Parses the report to write the data to the ouput file.
     *
     * Iterates through all the Data objects generated by the report.  With each Data object, it writes the output
     * to the report file.
     * @param ReportIn Pointer to Report object, pointer passed by value.
     * @param fileOut QTextStream file, file passed by reference.  The output file stream that the text will be
     * written to.
     * @return Returns boolean variable, variable passed by value.  Returns true if successful.
     */
    bool parseReport(ofreq::Report *ReportIn, QTextStream &fileOut);

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the index of the current wave direction.
     *
     * Also sets the index of the current wave direction on the System object.
     * @param indexIn Integer, variable passed by value.  The index of the current wave direction.
     */
    void setWaveInd(int indexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the System object.
     */
    ofreq::System *ptSystem = NULL;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of all reports to be generated.  List is uncategorized and unordered.  Each report may be a
     * different class type, all inheritted from the Report class.  All reports have a few functions in common.
     * The most important ones are:
     * getName()
     * getClass()
     * calcReport()
     * @sa Report
     */
    std::vector< ofreq::Report* > plistReport;

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
     * @brief Output file that Report is written to.
     *
     * The report gets written to a file for recording the output.  This is the file that the report gets written to.
     * File reference updates and changes depending on the report.  The setFile() function updates the reference to the
     * file.  All other functions just refer to it.
     */
    QFile pFileOutput;

    //------------------------------------------Function Separator ----------------------------------------------------
    std::string header; /**< The filename for the header to be included in all files */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A simple function that writes out multiple tab statements.
     *
     * Based on the TAB variable defined within this class.  Which may actually be a collection of spaces.
     * @param num Integer specifying the number of tab statements to write out.
     * @return Returns string that is the combination of the TAB statements.
     */
    QString TAB(int num = 1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Recursively removes all files in a directory, and the directory itself.
     * @param dirName QString, passed by reference.  The path to the directory that you want removed.
     * @return Returns true if removal successful.  False if error encountered.
     */
    bool removeDir(const QString &dirName);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Creates the directory specified by the std::string path.  Assumes any specified directory is under the root
     * project directory.
     * @param path QString.  The path of the directory to create.
     * @return Returns true if creation sucessful.
     */
    bool createDir(QString path);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Set information about the file to be written after header and above data, included in the seafile block.
     * @param nameIn The name of the object.
     * @return Returns QString.  Qstring contains the file info for the output file.  Everything written into the
     * seafile block.  Variable passed by value.
     */
    QString getInfoBlock(std::string nameIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Converts a string input into a QString output.
     * @param valIn String input, passed by value.
     * @return Returns QString output, with all proper formatting brackets and ready for insert into output file.
     */
    QString Val2String(std::string valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Converts a int input into a QString output.
     * @param valIn int input, passed by value.
     * @return Returns QString output, with all proper formatting brackets and ready for insert into output file.
     */
    QString Val2String(int valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Converts a double input into a QString output.
     * @param valIn Double input, passed by value.
     * @return Returns QString output, with all proper formatting brackets and ready for insert into output file.
     */
    QString Val2String(double valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Converts a complex<double> input into a QString output.
     * @param valIn complex<double> input, passed by value.
     * @return Returns QString output, with all proper formatting brackets and ready for insert into output file.
     */
    QString Val2String(std::complex<double> valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Converts a string to a QString.
     *
     * Accomplishes the conversion with depending on the STL compilation option. QString has a built-in capacity to
     * perform this conversion, but it depends on the STL compilation option in the Qt library.  Since this can change
     * with each compiler, it creates too much of a chance for things to go wrong.  Instead, creating a custom
     * conversion function, with is more generic.
     * @param valIn Standard string, passed by value.
     * @return QString output, passed by value.
     */
    QString a2Qstr(std::string valIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Converts a QString to a standard string.
     *
     * Accomplishes the conversion with depending on the STL compilation option. QString has a built-in capacity to
     * perform this conversion, but it depends on the STL compilation option in the Qt library.  Since this can change
     * with each compiler, it creates too much of a chance for things to go wrong.  Instead, creating a custom
     * conversion function, with is more generic.
     * @param valIn Standard QString, passed by value.
     * @return standard string output, passed by value.
     */
    std::string Qstr2a(QString valIn);

    //=================================== Variable Declarations =======================================================

    //------------------------------------------Function Separator ----------------------------------------------------
    //Filename Specification
    static std::string HEADER_FILENAME; /**< The filename of the header file to read. **/

    //------------------------------------------Function Separator ----------------------------------------------------
    //Directory Specifications
    static QString NAME_DIR; /**< The starting character for the wave direction directory.**/
    static QString NAME_RES; /**< The starting character for the resonant frequency directory.**/
    static QString NAME_BOD; /**< The starting characters for the body directory. **/

    //------------------------------------------Function Separator ----------------------------------------------------
    //Key-Value Markers
    static QString KEY_NAME; /**< Key to mark name of object being described. */
    static QString KEY_DATA; /**< Key to mark data for an object */
    static QString KEY_RAO; /**< Key to mark RAO data for an object. */
    static QString KEY_VALUE; /**< Key to mark the values for an object */
    static QString KEY_FREQUENCY; /**< Key to mark a frequency value */
    static QString KEY_DIRECTION; /**< Key to mark a direction value */
    static QString KEY_BODY; /**< Key to mark a body object */


};

}   //Namespace ofreq

}   //Namespace osea

#endif // REPORTMANAGER_H
