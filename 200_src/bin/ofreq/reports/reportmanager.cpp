/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
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

//==========================================Section Separator =========================================================
//Inclusions and namespaces
#include "reportmanager.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Filename Specification
string ReportManager::HEADER_FILENAME = "openseaheader.txt";

//------------------------------------------Function Separator ----------------------------------------------------
//Directory Specifications
QString ReportManager::NAME_DIR = "d"; /**< The starting character for the wave direction directory.**/
QString ReportManager::NAME_RES = "r"; /**< The starting character for the resonant frequency directory.**/
QString ReportManager::NAME_BOD = "body"; /**< The starting characters for the body directory. **/

//------------------------------------------Function Separator ----------------------------------------------------
//General File Content Marks
QString ReportManager::KEY_NAME = "name";
QString ReportManager::KEY_DATA = "data";
QString ReportManager::KEY_VALUE = "value";
QString ReportManager::KEY_FREQUENCY = "frequency";
QString ReportManager::KEY_DIRECTION = "direction";
QString ReportManager::KEY_BODY = "body";


//==========================================Section Separator =========================================================
//Filename Markers
QString ReportManager::FILE_DIRECTIONS = "directions.out";
QString ReportManager::FILE_FREQUENCIES = "frequencies.out";
QString ReportManager::FILE_GLOBAL_ACCELERATION = "accglobal.out";
QString ReportManager::FILE_GLOBAL_MOTION = "motglobal.out";
QString ReportManager::FILE_GLOBAL_VELOCITY = "velglobal.out";
QString ReportManager::FILE_GLOBAL_SOLUTION = "solglobal.out";

//------------------------------------------Function Separator ----------------------------------------------------
//Class Name Markers
QString ReportManager::CLASS_DIRECTIONS = "repDirections";    /**< Class name for wave directions output file. */
QString ReportManager::CLASS_FREQUENCIES = "repFrequencies";   /**< Class name for wave frequencies output file. */
QString ReportManager::CLASS_GLOBAL_ACCELERATION = "repGlobAcceleration";   /**< Class name for global acceleration output file. */
QString ReportManager::CLASS_GLOBAL_MOTION = "repGlobMotion"; /**< Class name for global motion output file. */
QString ReportManager::CLASS_GLOBAL_VELOCITY = "repGlobVelocity"; /**< Class name for global velocity output file. */
QString ReportManager::CLASS_GLOBAL_SOLUTION = "repGlobSolution"; /**< Class name for global solution output file. */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
ReportManager::ReportManager()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
ReportManager::ReportManager(ofreq::System *ptIn) : ReportManager()
{   
    setSystem(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void ReportManager::setSystem(ofreq::System *ptIn)
{
    ptSystem = ptIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
System* ReportManager::getSystem()
{
    return ptSystem;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector< Report* > &ReportManager::listReport()
{
    return plistReport;
}

//------------------------------------------Function Separator --------------------------------------------------------
Report* &ReportManager::listReportPt(int indexIn)
{
    return plistReport.at(indexIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void ReportManager::writeReport()
{
    //Iterate through each wave direction and write out reports.
    for (unsigned int i = 0; i < ptSystem->listWaveDirections().size(); i++)
    {
        try
        {
            writeReport(i);
        }
        catch(const std::exception &err)
        {
            //Error handler.
            logStd.Notify();
            logErr.Write(ID + string(err.what()));
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void ReportManager::writeReport(int waveIndexIn)
{
    //Iterate through each report in the list of reports and write out report.
    for (unsigned int i = 0; i < plistReport.size(); i++)
    {
        try
        {
            //Calculate each report.
            writeReport(waveIndexIn, plistReport.at(i));
        }
        catch(const std::exception &err)
        {
            //Error handler.
            logStd.Notify();
            logErr.Write(ID + string(err.what()));
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void ReportManager::writeReport(int waveIndexIn, ofreq::Report *ReportIn)
{
    try
    {
        //Calculate the report for the specified wave direction and specified report.
        calcReport(waveIndexIn, ReportIn);

        //Setup the correct directory.
        QString path = setDirectory(ReportIn);

        //Setup the correct file.
        setFile(ReportIn, path);

        //Use the current output file to get an output stream.
        QTextStream FileOut(&pFileOutput);

        //Parse the report into the output file.
        bool writeTrue = parseReport(ReportIn, FileOut);
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }

    try
    {
        //Close the file.
        pFileOutput.close();

        //Clear the results data from the report.
        ReportIn->clearData();
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void ReportManager::calcReport(int waveIndexIn)
{
    //Iterate through each report in the list of reports and calculate data.
    for (unsigned int i = 0; i < plistReport.size(); i++)
    {
        try
        {
            //Calculate each report.
            calcReport(waveIndexIn, plistReport.at(i));
        }
        catch(const std::exception &err)
        {
            //Error handler.
            logStd.Notify();
            logErr.Write(ID + string(err.what()));
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void ReportManager::calcReport(int waveIndexIn, ofreq::Report *ReportIn)
{
    //Calculate report
    try
    {
        //Set the current wave index
        setWaveInd(waveIndexIn);

        ReportIn->calcReport();
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void ReportManager::clearReports()
{
    //Clear all reports in the list of reports, to save on memory.
    for (unsigned int i = 0; i < plistReport.size(); i++)
    {
        plistReport.at(i)->clearData();
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
bool ReportManager::clearFiles()
{
    //Clear all output files generated.
    bool result = true;
    QString QSys;
    QString Qpath;

    //Convert system path to QString.
    QSys = QString::fromStdString(ptSystem->getPath() + SLASH);

    //Remove the wave directions file.
    Qpath = QSys + FILE_DIRECTIONS;
    result = result * QFile::remove(Qpath);

    //Remove the frequency direction file.
    Qpath = QSys + FILE_FREQUENCIES;
    result = result * QFile::remove(Qpath);

    //Iterate through each wave directory and remove it.
    for (unsigned int i = 0; i < ptSystem->listWaveDirections().size(); i++)
    {
        Qpath = QSys
                + NAME_DIR
                + QString::fromStdString(
                    itoa(i)
                    );

        result = removeDir(Qpath);

        //Write any warnings to log.
        if (!result)
        {
            std::string msg = "Some files not deleted from the following directory:  ";
            msg = msg + NAME_DIR.toStdString() + itoa(i);
            logErr.Write(msg,2);
        }
    }

    //Return output
    return result;
}


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
QString ReportManager::setDirectory(ofreq::Report *ReportIn)
{
    try
    {
        //Set the correct directory, relative to report class type.
        QString output;
        QString subdir;         //Subdirectory under the current path.

        //Set current working directory.
        output.fromStdString(
                    ptSystem->getPath() + SLASH);

        //Get report class
        QString className = QString::fromStdString(ReportIn->getClass());

        //Iterate through different possibilities for class names.
        //-------------------------------------------------------

        //--------------------------------------
        //Wave Directions
        if (className == CLASS_DIRECTIONS)
        {
            //just the current directory.
        }

        //--------------------------------------
        //Wave Frequencies
        else if (className == CLASS_FREQUENCIES)
        {
            //just the current directory.
        }

        //--------------------------------------
        //Global Solution
        else if (className == CLASS_GLOBAL_SOLUTION)
        {
            //Add in the current wave direction and body name.
            subdir = subdir
                     + NAME_DIR
                     + QString::fromStdString(
                         itoa(curWaveInd) +
                         SLASH)
                     + NAME_BOD
                     + QString::fromStdString(
                         itoa(ReportIn->getBodIndex()) +
                         SLASH);
        }

        //--------------------------------------
        //Global Motion
        else if (className == CLASS_GLOBAL_MOTION)
        {
            //Add in the current wave direction and body name.
            subdir = subdir
                     + NAME_DIR
                     + QString::fromStdString(
                         itoa(curWaveInd) +
                         SLASH)
                     + NAME_BOD
                     + QString::fromStdString(
                         itoa(ReportIn->getBodIndex()) +
                         SLASH);
        }

        //--------------------------------------
        //Global Velocity
        else if (className == CLASS_GLOBAL_VELOCITY)
        {
            //Add in the current wave direction and body name.
            subdir = subdir
                     + NAME_DIR
                     + QString::fromStdString(
                         itoa(curWaveInd) +
                         SLASH)
                     + NAME_BOD
                     + QString::fromStdString(
                         itoa(ReportIn->getBodIndex()) +
                         SLASH);
        }

        //--------------------------------------
        //Global Acceleration
        else if (className == CLASS_GLOBAL_ACCELERATION)
        {
            //Add in the current wave direction and body name.
            subdir = subdir
                     + NAME_DIR
                     + QString::fromStdString(
                         itoa(curWaveInd) +
                         SLASH)
                     + NAME_BOD
                     + QString::fromStdString(
                         itoa(ReportIn->getBodIndex()) +
                         SLASH);
        }

        //Create subdirectories
        createDir(subdir);

        //Add to output directory.
        output += subdir;

        //Write output
        return output;
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void ReportManager::setFile(ofreq::Report *ReportIn, QString dirIn)
{
    try
    {
        QString filePath = dirIn;
        //Search through class list and find matching object for filename.
        if (QString::fromStdString(ReportIn->getClass()) == CLASS_DIRECTIONS)
        {
            //Append to filepath.
            filePath += FILE_DIRECTIONS;
        }
        else if (QString::fromStdString(ReportIn->getClass()) == CLASS_FREQUENCIES)
        {
            //Append to filepath.
            filePath += FILE_FREQUENCIES;
        }
        else if (QString::fromStdString(ReportIn->getClass()) == CLASS_GLOBAL_ACCELERATION)
        {
            //Append to filepath.
            filePath += FILE_GLOBAL_ACCELERATION;
        }
        else if (QString::fromStdString(ReportIn->getClass()) == CLASS_GLOBAL_MOTION)
        {
            //Append to filepath.
            filePath += FILE_GLOBAL_MOTION;
        }
        else if (QString::fromStdString(ReportIn->getClass()) == CLASS_GLOBAL_VELOCITY)
        {
            //Append to filepath.
            filePath += FILE_GLOBAL_VELOCITY;
        }
        else if (QString::fromStdString(ReportIn->getClass()) == CLASS_GLOBAL_SOLUTION)
        {
            //Append to filepath.
            filePath += FILE_GLOBAL_SOLUTION;
        }
        else
        {
            throw std::runtime_error("No matching class type found.");
        }

        //Check if file already exists.
        if (QFile::exists(filePath))
        {
            //File already exists.  Just open and append a new starting line to it.

            //Close filename to be safe.
            pFileOutput.close();

            //Open file, as an append.
            pFileOutput.setFileName(filePath);
        }

        else
        {
            //File does not exist.  Create it and append header.
            //Open file, as an append.
            pFileOutput.setFileName(filePath);

            //Attach file to output stream.
            QTextStream output1(&pFileOutput);

            //Append header information.
            output1 << getInfoBlock(ReportIn->getClass());
        }

        QTextStream output(&pFileOutput);

        //Append in a few carriage returns and a new section break.
        output << "\n\n\n";
        output << QString::fromStdString(BREAK_BOTTOM + EOL);

        //Close the output file.
        pFileOutput.close();
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
bool ReportManager::parseReport(ofreq::Report *ReportIn, QTextStream &fileOut)
{
    //Set output
    bool status = false;
    //Write report class name and opening brackets.
    fileOut << QString::fromStdString(ReportIn->getClass()
                                      + EOL
                                      + OBJECT_BEGIN);

    //Write out object key and name.
    fileOut << TAB() << KEY_NAME << QString::fromStdString(SPACE + ReportIn->getName() + END + EOL);

    //---------------------------------------
    //Write out each of the object constants.
    for (unsigned int i = 0; i < ReportIn->listConstKey().size(); i++)
    {
        //---------------------------------------
        //Write out key name
        fileOut << TAB() << QString::fromStdString(ReportIn->listConstKey(i) + SPACE);

        if ((ReportIn->listConstVal(i).listValue().size() > 1)
                || (ReportIn->listConstVal(i).listString().size() > 1))
        {
            //---------------------------------------
            //Multiple key values

            //First create the opening bracket
            fileOut << QString::fromStdString(LIST_BEGIN + EOL);

            //Check Data type
            if (ReportIn->listConstVal(i).getDataType() == 0)
            {
                //Complex list.
                for (unsigned int j = 0; j < ReportIn->listConstVal(i).listValue().size(); j++)
                {
                    fileOut << TAB(2)
                               + Val2String(
                                          ReportIn->listConstVal(i).listValue(j))
                               + QString::fromStdString(EOL);
                }
            }

            else if (ReportIn->listConstVal(i).getDataType() == 1)
            {
                //Double list
                for (unsigned int j = 0; j < ReportIn->listConstVal(i).listValue().size(); j++)
                {
                    fileOut << TAB(2)
                               + Val2String(
                                          ReportIn->listConstVal(i).listValueDouble(j))
                               + QString::fromStdString(EOL);
                }
            }

            else if (ReportIn->listConstVal(i).getDataType() == 2)
            {
                //Integer list
                for (unsigned int j = 0; j < ReportIn->listConstVal(i).listValue().size(); j++)
                {
                    fileOut << TAB(2)
                               + Val2String(
                                          ReportIn->listConstVal(i).listValueInt(j))
                               + QString::fromStdString(EOL);
                }
            }

            else if (ReportIn->listConstVal(i).getDataType() == 3)
            {
                //String list
                for (unsigned int j = 0; j < ReportIn->listConstVal(i).listString().size(); j++)
                {
                    fileOut << TAB(2)
                            << QString::fromStdString(
                                   QUOTE)
                                   + Val2String(
                                       ReportIn->listConstVal(i).listString(j))
                                   + QString::fromStdString(
                                        QUOTE
                                        + EOL
                                   );
                }
            }

            //Create the closing bracket
            fileOut << TAB() << QString::fromStdString(LIST_END + END + EOL);
        }
        else
        {
            //---------------------------------------
            //Single key value

            //Check Data Type
            if (ReportIn->listConstVal(i).getDataType() == 0)
            {
                //Complex list.
                for (unsigned int j = 0; j < ReportIn->listConstVal(i).listValue().size(); j++)
                {
                    fileOut << Val2String(
                                  ReportIn->listConstVal(i).listValue(j))
                            + QString::fromStdString(END + EOL);
                }
            }

            else if (ReportIn->listConstVal(i).getDataType() == 1)
            {
                //Double list
                for (unsigned int j = 0; j < ReportIn->listConstVal(i).listValue().size(); j++)
                {
                    fileOut << Val2String(
                                    ReportIn->listConstVal(i).listValueDouble(j))
                                + QString::fromStdString(END + EOL);
                }
            }

            else if (ReportIn->listConstVal(i).getDataType() == 2)
            {
                //Integer list
                for (unsigned int j = 0; j < ReportIn->listConstVal(i).listValue().size(); j++)
                {
                    fileOut << Val2String(
                                    ReportIn->listConstVal(i).listValueInt(j))
                                + QString::fromStdString(END + EOL);
                }
            }

            else if (ReportIn->listConstVal(i).getDataType() == 3)
            {
                //String list
                for (unsigned int j = 0; j < ReportIn->listConstVal(i).listString().size(); j++)
                {
                    fileOut << QString::fromStdString(
                                   QUOTE)
                                   + Val2String(
                                       ReportIn->listConstVal(i).listString(j))
                               + QString::fromStdString(
                                   QUOTE
                                   + END
                                   + EOL
                                   );
                }
            }
        }
    }

    //---------------------------------------
    //Write out the object data

    for (unsigned int i = 0; i < ReportIn->listData().size(); i++)
    {
        //Start with the data keyword and object starter.
        fileOut << TAB()
                << KEY_DATA
                << QString::fromStdString(SPACE
                                          + OBJECT_BEGIN
                                          + EOL);

        //Write out the frequency index keyword and value.
        fileOut << TAB(2)
                + KEY_FREQUENCY
                + QString::fromStdString(SPACE)
                + Val2String(
                          ReportIn->listData(i).getIndex())
                + QString::fromStdString(
                       END + EOL);

        //---------------------------------------
        //Write out the values from the data object.

        //First create keyword
        fileOut << TAB(2) + KEY_VALUE + QString::fromStdString(SPACE);

        //---------------------------------------
        //Check if writing single value or multiple.
        if (ReportIn->listData(i).listValue().size() > 1)
        {
            //---------------------------------------
            //Multiple values

            //Write out beginning of list
            fileOut << QString::fromStdString(LIST_BEGIN + EOL);

            //Check Data type

            if (ReportIn->listData(i).getDataType() == 0)
            {
                //Complex data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listValue().size(); j++)
                {
                    fileOut << TAB(3) + Val2String(
                                  ReportIn->listData(i).listValue(j))
                            + QString::fromStdString(EOL);
                }
            }

            else if (ReportIn->listData(i).getDataType() == 1)
            {
                //Double data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listValue().size(); j++)
                {
                    fileOut << TAB(3) + Val2String(
                                    ReportIn->listData(i).listValueDouble(j))
                                + QString::fromStdString(EOL);
                }
            }

            else if (ReportIn->listData(i).getDataType() == 2)
            {
                //Integer data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listValue().size(); j++)
                {
                    fileOut << TAB(3) + Val2String(
                                    ReportIn->listData(i).listValueInt(j))
                                + QString::fromStdString(EOL);
                }
            }

            else if (ReportIn->listData(i).getDataType() == 3)
            {
                //String data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listString().size(); j++)
                {
                    fileOut << TAB(3) + QString::fromStdString(
                                   QUOTE)
                                   + Val2String(
                                       ReportIn->listData(i).listString(j))
                               + QString::fromStdString(
                                   QUOTE
                                   + EOL
                                   );
                }
            }

            //Write out end of list
            fileOut << TAB(2) + QString::fromStdString(LIST_END + EOL);
        }
        else
        {
            //---------------------------------------
            //Single value

            //Check Data type

            if (ReportIn->listData(i).getDataType() == 0)
            {
                //Complex data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listValue().size(); j++)
                {
                    fileOut << Val2String(
                                  ReportIn->listData(i).listValue(j))
                            + QString::fromStdString(END + EOL);
                }
            }

            else if (ReportIn->listData(i).getDataType() == 1)
            {
                //Double data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listValue().size(); j++)
                {
                    fileOut << Val2String(
                                    ReportIn->listData(i).listValueDouble(j))
                                + QString::fromStdString(END + EOL);
                }
            }

            else if (ReportIn->listData(i).getDataType() == 2)
            {
                //Integer data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listValue().size(); j++)
                {
                    fileOut << Val2String(
                                    ReportIn->listData(i).listValueInt(j))
                                + QString::fromStdString(END + EOL);
                }
            }

            else if (ReportIn->listData(i).getDataType() == 3)
            {
                //String data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listString().size(); j++)
                {
                    fileOut << QString::fromStdString(
                                   QUOTE)
                                   + Val2String(
                                       ReportIn->listData(i).listString(j))
                               + QString::fromStdString(
                                   QUOTE
                                   + END
                                   + EOL
                                   );
                }
            }
        }

        //---------------------------------------
        //Closing bracket to object.
        fileOut << TAB() + QString::fromStdString(OBJECT_END + EOL + EOL);
    }

    //---------------------------------------
    //Closing bracket to object.
    fileOut << QString::fromStdString(OBJECT_END + EOL);

    //Set status to success.
    status = true;

    //Return result.
    return status;
}


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
void ReportManager::setWaveInd(int indexIn)
{
    curWaveInd = indexIn;

    ptSystem->setCurWaveDirInd(curWaveInd);
}

//------------------------------------------Function Separator --------------------------------------------------------
QString ReportManager::TAB(int num)
{
    string output;  //Output string
    for (int i = 0; i < num; i++)
        output.append(TAB_REF);

    //Write output
    return QString::fromStdString(output);
}

//------------------------------------------Function Separator --------------------------------------------------------
bool ReportManager::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else
            {
                //Check if filename matches one of the output files that should be in there.
                if (info.fileName() == FILE_DIRECTIONS)
                {
                    result = QFile::remove(info.absoluteFilePath());
                }
                else if (info.fileName() == FILE_FREQUENCIES)
                {
                    result = QFile::remove(info.absoluteFilePath());
                }
                else if (info.fileName() == FILE_GLOBAL_ACCELERATION)
                {
                    result = QFile::remove(info.absoluteFilePath());
                }
                else if (info.fileName() == FILE_GLOBAL_MOTION)
                {
                    result = QFile::remove(info.absoluteFilePath());
                }
                else if (info.fileName() == FILE_GLOBAL_VELOCITY)
                {
                    result = QFile::remove(info.absoluteFilePath());
                }
                else if (info.fileName() == FILE_GLOBAL_SOLUTION)
                {
                    result = QFile::remove(info.absoluteFilePath());
                }
            }

            if (!result)
                return result;
        }
        result = dir.rmdir(dirName);
    }
    return result;
}

//------------------------------------------Function Separator --------------------------------------------------------
void ReportManager::setHeader(string filePathIn)
{
    std::string fileIn;     //Full path of input file that contains the header to write.

    //Combine file path and filename.
    fileIn = filePathIn;

    if (fileIn.substr(fileIn.size(),1) == SLASH)
    {
        //Just add filename
        fileIn.append(HEADER_FILENAME);
    }
    else
    {
        //Add filename plus slash.
        fileIn.append(SLASH);
        fileIn.append(HEADER_FILENAME);
    }

    ifstream header_fileInput(fileIn.c_str(), std::ios::in);

    try {
        //Check if file exists and was open
        if (!header_fileInput)
            throw ios_base::failure("Could not read header file.  Location:  " + fileIn +
                                    "\n This can occurr if the file does not exist or does not have read permissions.");

        header_fileInput.seekg(0, std::ios::end);
        header.resize(header_fileInput.tellg());
        header_fileInput.seekg(0, std::ios::beg);
        header_fileInput.read(&header.at(0), header.size());
        //Close file
        header_fileInput.close();
    }
    catch (const std::exception &err)
    {
        //Write out error message
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
QString ReportManager::getInfoBlock(string nameIn)
{
    return QString::fromStdString(VAL_SEAFILE + EOL + OBJECT_BEGIN + EOL)
            + TAB()
            + QString::fromStdString(KEY_VERSION)
            + TAB()
            + QString::fromStdString(VAL_VERSION + END + EOL)
            + TAB()
            + QString::fromStdString(KEY_FORMAT)
            + TAB()
            + QString::fromStdString(VAL_FORMAT + END + EOL)
            + TAB()
            + QString::fromStdString(KEY_OBJECT)
            + TAB()
            + QString::fromStdString(nameIn + END + EOL + OBJECT_END + EOL + EOL);
}

//------------------------------------------Function Separator --------------------------------------------------------
bool ReportManager::createDir(QString path)
{
    //Create Qt directory object.
    path = QString::fromStdString(ptSystem->getPath())
           + QString::fromStdString(SLASH)
           + path;

    QDir dir(path);    //Qt object that is the output directory.

    //Check if directory exists and create if not.
    return dir.mkpath(".");
}

//------------------------------------------Function Separator --------------------------------------------------------
QString ReportManager::Val2String(std::string valIn)
{
    //Convert string to QString
    return QString::fromStdString(valIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
QString ReportManager::Val2String(int valIn)
{
    //Convert to QString
    return QString::fromStdString(itoa(valIn));
}

//------------------------------------------Function Separator --------------------------------------------------------
QString ReportManager::Val2String(double valIn)
{
    QString output;
    QTextStream convert;
    convert.setString(&output);
    convert.setRealNumberNotation(QTextStream::ScientificNotation);
    convert << valIn;

    //Write output.
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
QString ReportManager::Val2String(std::complex<double> valIn)
{
    QString output;
    QTextStream convert;
    convert.setString(&output);
    convert.setRealNumberNotation(QTextStream::ScientificNotation);

    //Create opening brackets
    convert << QString::fromStdString(LIST_BEGIN);

    //Enter real number
    convert << valIn.real();

    //Enter separation mark
    convert << QString::fromStdString(COMPLEX_COORD);

    //Enter imaginary part
    convert << valIn.imag();

    //Enter closing brackets
    convert << QString::fromStdString(LIST_END);

    //Write output
    return output;
}
