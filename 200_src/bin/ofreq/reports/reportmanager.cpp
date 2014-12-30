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
QString ReportManager::KEY_RAO = "RAO";
QString ReportManager::KEY_VALUE = "value";
QString ReportManager::KEY_FREQUENCY = "frequency";
QString ReportManager::KEY_DIRECTION = "direction";
QString ReportManager::KEY_BODY = "body";


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
ReportManager::~ReportManager()
{
    //Iterate through list of reports and delete each one.
    for (unsigned int i = 0; i < plistReport.size(); i++)
    {
        delete plistReport.at(i);
    }
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
    //Write some user output.
    logStd.Write(EOL + EOL + EOL,3);
    logStd.Write("Calculating Outputs",3);
    logStd.Write("=================================================================================",3);

    //Create a wave directions report and execute it.
    Report *RepPt = new repDirections(this);
    writeReport(0, RepPt);
    delete RepPt;

    //Create a wave frequency report and execute it.
    RepPt = new repFrequencies(this);
    writeReport(0, RepPt);
    delete RepPt;

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
    //Provide user output
    logStd.Write("Wave Direction:  "
                 + itoa(waveIndexIn + 1)
                 + " of "
                 + itoa(ptSystem->listWaveDirections().size() + 1 - 1) //+1 = conversion to 1-based count. -1 = offset for vector count.
                 ,3);

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
        //Write some user output.
        std::string msg;

        //Start by clearing report data.
        //This safeguards against any interactions between reports.
        ReportIn->clearData();

        msg = TAB_REF + TAB_REF + ReportIn->getClass() + string("::");
        //Test for null body pointer.
        if (ReportIn->getBody())
        {
            msg += ReportIn->getBody()->getBodyName();
            msg += string("::  ");
        }
        msg += ReportIn->getName();

        logStd.Write(msg, 3);

        //Calculate the report for the specified wave direction and specified report.
        calcReport(waveIndexIn, ReportIn);

        //Setup the correct directory.
        QString path = setDirectory(ReportIn);

        //Setup the correct file.
        setFile(ReportIn, path);

        //Use the current output file to get an output stream.
        pFileOutput.open(QIODevice::Append);
        QTextStream FileOut(&pFileOutput);

        //Parse the report into the output file.
        bool writeTrue = parseReport(ReportIn, FileOut);

        //Check if wrote correctly.
        if (!writeTrue)
        {
            throw std::runtime_error(string("Report did no write correctly.  Report name:  ")
                                     + ReportIn->getName());
        }
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

        //Calculate report, for all wave frequencies.
        for (unsigned int i = 0; i < ptSystem->listWaveFrequencies().size(); i++)
        {
            ReportIn->calcReport(i);
        }

        //Calculate RAO, for all wave frequencies.
        for (int i = 0; i < ptSystem->listWaveFrequencies().size(); i++)
        {
            ReportIn->calcRAO(i);
        }

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
    QSys = a2Qstr(ptSystem->getPath() + SLASH);

    //Remove the wave directions file.
    repDirections tempDir;
    Qpath = QSys + a2Qstr(tempDir.getFileName());
    result = result * QFile::remove(Qpath);

    //Remove the frequency direction file.
    repFrequencies tempFreq;
    Qpath = QSys + a2Qstr(tempFreq.getFileName());
    result = result * QFile::remove(Qpath);

    //Iterate through each wave directory and remove it.
    for (unsigned int i = 0; i < ptSystem->listWaveDirections().size(); i++)
    {
        Qpath = QSys
                + NAME_DIR
                + a2Qstr(
                    itoa(i + 1)
                    );

        result = removeDir(Qpath);

        //Write any warnings to log.
        if (!result)
        {
            std::string msg = "Some files not deleted from the following directory:  ";
            msg = msg + NAME_DIR.toStdString() + itoa(i + 1);
            logErr.Write(msg,2);
        }
    }

    //Return output
    return result;
}

//------------------------------------------Function Separator --------------------------------------------------------
void ReportManager::addReport(ofreq::Report *ReportIn)
{
    plistReport.push_back(ReportIn);
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
        output = a2Qstr(
                    ptSystem->getPath() + SLASH);

        //Get report class
        QString className = a2Qstr(ReportIn->getClass());

        //Iterate through different possibilities for class names.
        //-------------------------------------------------------
        repDirections tempDir;
        repFrequencies tempFreq;

        //--------------------------------------
        //Wave Directions
        if (className == a2Qstr(tempDir.getClass()))
        {
            //just the current directory.
        }

        //--------------------------------------
        //Wave Frequencies
        else if (className == a2Qstr(tempFreq.getClass()))
        {
            //just the current directory.
        }

        //--------------------------------------
        //Default behavior for all reports
        else
        {
            //Add in the current wave direction and body name.
            subdir = subdir
                     + NAME_DIR
                     + a2Qstr(
                         itoa(curWaveInd + 1) +
                         SLASH)
                     + NAME_BOD
                     + a2Qstr(
                         itoa(ReportIn->getBodIndex() + 1) +
                         SLASH);
        }

        //Create subdirectories
        try
        {
            createDir(subdir);
        }
        catch(const std::exception &err)
        {
            //Error handler.
            logStd.Notify();
            logErr.Write(ID + string("Error creating subdirectory.  Directory name:  ")
                         + subdir.toStdString()
                         + string("Details:  ")
                         + string(err.what()));
        }

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
        //Append filename to path.
        filePath += a2Qstr(ReportIn->getFileName());

        //Check if file already exists.
        if (QFile::exists(filePath))
        {
            //File already exists.  Just open and append a new starting line to it.

            //Close filename to be safe.
            pFileOutput.close();

            //Open file, as an append.
            pFileOutput.setFileName(filePath);
            pFileOutput.open(QIODevice::Append);
        }

        else
        {
            //File does not exist.  Create it and append header.
            //Open file, as an append.
            pFileOutput.setFileName(filePath);
            pFileOutput.open(QIODevice::Append);

            //Assign a text stream
            QTextStream output(&pFileOutput);

            //Append header information.
            output << a2Qstr(header);
            output << getInfoBlock(ReportIn->getClass());
        }

        //Assign a text stream
        QTextStream output(&pFileOutput);

        //Append in a few carriage returns.
        output << a2Qstr(EOL + EOL + EOL);

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
    fileOut << a2Qstr(BREAK_TOP + EOL);
    fileOut << a2Qstr(ReportIn->getClass()
                      + SPACE
                      + OBJECT_BEGIN
                      + EOL);

    //Write out object key and name.
    fileOut << TAB() << KEY_NAME << a2Qstr(SPACE + QUOTE + ReportIn->getName() + QUOTE + END + EOL);

    //---------------------------------------
    //Write out each of the object constants.
    for (unsigned int i = 0; i < ReportIn->listConstKey().size(); i++)
    {
        //---------------------------------------
        //Write out key name
        fileOut << TAB() << a2Qstr(ReportIn->listConstKey(i) + SPACE);

        if ((ReportIn->listConstVal(i).listValue().size() > 1)
                || (ReportIn->listConstVal(i).listString().size() > 1))
        {
            //---------------------------------------
            //Multiple key values

            //First create the opening bracket
            fileOut << a2Qstr(LIST_BEGIN + EOL);

            //Check Data type
            if (ReportIn->listConstVal(i).getDataType() == 0)
            {
                //Complex list.
                for (unsigned int j = 0; j < ReportIn->listConstVal(i).listValue().size(); j++)
                {
                    fileOut << TAB(2)
                               + Val2String(
                                          ReportIn->listConstVal(i).listValue(j))
                               + a2Qstr(EOL);
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
                               + a2Qstr(EOL);
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
                               + a2Qstr(EOL);
                }
            }

            else if (ReportIn->listConstVal(i).getDataType() == 3)
            {
                //String list
                for (unsigned int j = 0; j < ReportIn->listConstVal(i).listString().size(); j++)
                {
                    fileOut << TAB(2)
                            << a2Qstr(
                                   QUOTE)
                                   + Val2String(
                                       ReportIn->listConstVal(i).listString(j))
                                   + a2Qstr(
                                        QUOTE
                                        + EOL
                                   );
                }
            }

            //Create the closing bracket
            fileOut << TAB() << a2Qstr(LIST_END + END + EOL);
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
                            + a2Qstr(END + EOL);
                }
            }

            else if (ReportIn->listConstVal(i).getDataType() == 1)
            {
                //Double list
                for (unsigned int j = 0; j < ReportIn->listConstVal(i).listValue().size(); j++)
                {
                    fileOut << Val2String(
                                    ReportIn->listConstVal(i).listValueDouble(j))
                                + a2Qstr(END + EOL);
                }
            }

            else if (ReportIn->listConstVal(i).getDataType() == 2)
            {
                //Integer list
                for (unsigned int j = 0; j < ReportIn->listConstVal(i).listValue().size(); j++)
                {
                    fileOut << Val2String(
                                    ReportIn->listConstVal(i).listValueInt(j))
                                + a2Qstr(END + EOL);
                }
            }

            else if (ReportIn->listConstVal(i).getDataType() == 3)
            {
                //String list
                for (unsigned int j = 0; j < ReportIn->listConstVal(i).listString().size(); j++)
                {
                    fileOut << a2Qstr(
                                   QUOTE)
                                   + Val2String(
                                       ReportIn->listConstVal(i).listString(j))
                               + a2Qstr(
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
                << a2Qstr(SPACE
                                          + OBJECT_BEGIN
                                          + EOL);

        //Write out the frequency index keyword and value.
        fileOut << TAB(2)
                + KEY_FREQUENCY
                + a2Qstr(SPACE)
                + Val2String(
                          ReportIn->listData(i).getIndex() + 1)
                + a2Qstr(
                       END + EOL);

        //---------------------------------------
        //Write out the values from the data object.

        //First create keyword
        fileOut << TAB(2) + KEY_VALUE + a2Qstr(SPACE);

        //---------------------------------------
        //Check if writing single value or multiple.
        if (ReportIn->listData(i).listValue().size() > 1)
        {
            //---------------------------------------
            //Multiple values

            //Write out beginning of list
            fileOut << a2Qstr(LIST_BEGIN + EOL);

            //Check Data type

            if (ReportIn->listData(i).getDataType() == 0)
            {
                //Complex data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listValue().size(); j++)
                {
                    fileOut << TAB(3) + Val2String(
                                  ReportIn->listData(i).listValue(j))
                            + a2Qstr(EOL);
                }
            }

            else if (ReportIn->listData(i).getDataType() == 1)
            {
                //Double data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listValue().size(); j++)
                {
                    fileOut << TAB(3) + Val2String(
                                    ReportIn->listData(i).listValueDouble(j))
                                + a2Qstr(EOL);
                }
            }

            else if (ReportIn->listData(i).getDataType() == 2)
            {
                //Integer data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listValue().size(); j++)
                {
                    fileOut << TAB(3) + Val2String(
                                    ReportIn->listData(i).listValueInt(j))
                                + a2Qstr(EOL);
                }
            }

            else if (ReportIn->listData(i).getDataType() == 3)
            {
                //String data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listString().size(); j++)
                {
                    fileOut << TAB(3) + a2Qstr(
                                   QUOTE)
                                   + Val2String(
                                       ReportIn->listData(i).listString(j))
                               + a2Qstr(
                                   QUOTE
                                   + EOL
                                   );
                }
            }

            //Write out end of list
            fileOut << TAB(2) + a2Qstr(LIST_END + END + EOL);
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
                            + a2Qstr(END + EOL);
                }
            }

            else if (ReportIn->listData(i).getDataType() == 1)
            {
                //Double data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listValue().size(); j++)
                {
                    fileOut << Val2String(
                                    ReportIn->listData(i).listValueDouble(j))
                                + a2Qstr(END + EOL);
                }
            }

            else if (ReportIn->listData(i).getDataType() == 2)
            {
                //Integer data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listValue().size(); j++)
                {
                    fileOut << Val2String(
                                    ReportIn->listData(i).listValueInt(j))
                                + a2Qstr(END + EOL);
                }
            }

            else if (ReportIn->listData(i).getDataType() == 3)
            {
                //String data type
                for (unsigned int j = 0; j < ReportIn->listData(i).listString().size(); j++)
                {
                    fileOut << a2Qstr(
                                   QUOTE)
                                   + Val2String(
                                       ReportIn->listData(i).listString(j))
                               + a2Qstr(
                                   QUOTE
                                   + END
                                   + EOL
                                   );
                }
            }
        }

        //---------------------------------------
        //Closing bracket to object.
        fileOut << TAB() + a2Qstr(OBJECT_END + EOL + EOL);
    }

    //---------------------------------------
    //Write out the RAO data

    if (ReportIn->listRAO().size() > 0)
    {
        //Write out a small separator.
        fileOut << a2Qstr(EOL + EOL) + TAB()
                + a2Qstr(BREAK_MIDDLE + EOL);
        fileOut << TAB()
                + QString("//RAO Data") + a2Qstr(EOL);
    }

    for (unsigned int i = 0; i < ReportIn->listRAO().size(); i++)
    {
        //Start with the data keyword and object starter.
        fileOut << TAB()
                << KEY_RAO
                << a2Qstr(SPACE + OBJECT_BEGIN + EOL);

        //Write out the frequency index keyword and value.
        fileOut << TAB(2)
                + KEY_FREQUENCY
                + a2Qstr(SPACE)
                + Val2String(ReportIn->listRAO(i).getIndex() + 1)
                + a2Qstr(END + EOL);

        //---------------------------------------
        //Write out the values from the data object.

        //First create keyword
        fileOut << TAB(2) + KEY_VALUE + a2Qstr(SPACE);

        //---------------------------------------
        //Check if writing single value or multiple.
        if (ReportIn->listRAO(i).listValue().size() > 1)
        {
            //---------------------------------------
            //Multiple values

            //Write out beginning of list
            fileOut << a2Qstr(LIST_BEGIN + EOL);

            //Check Data type

            if (ReportIn->listRAO(i).getDataType() == 0)
            {
                //Complex data type
                for (unsigned int j = 0; j < ReportIn->listRAO(i).listValue().size(); j++)
                {
                    fileOut << TAB(3) + Val2String(
                                  ReportIn->listRAO(i).listValue(j))
                            + a2Qstr(EOL);
                }
            }

            else if (ReportIn->listRAO(i).getDataType() == 1)
            {
                //Double data type
                for (unsigned int j = 0; j < ReportIn->listRAO(i).listValue().size(); j++)
                {
                    fileOut << TAB(3) + Val2String(
                                    ReportIn->listRAO(i).listValueDouble(j))
                                + a2Qstr(EOL);
                }
            }

            else if (ReportIn->listRAO(i).getDataType() == 2)
            {
                //Integer data type
                for (unsigned int j = 0; j < ReportIn->listRAO(i).listValue().size(); j++)
                {
                    fileOut << TAB(3) + Val2String(
                                    ReportIn->listRAO(i).listValueInt(j))
                                + a2Qstr(EOL);
                }
            }

            else if (ReportIn->listRAO(i).getDataType() == 3)
            {
                //String data type
                for (unsigned int j = 0; j < ReportIn->listRAO(i).listString().size(); j++)
                {
                    fileOut << TAB(3) + a2Qstr(
                                   QUOTE)
                                   + Val2String(
                                       ReportIn->listRAO(i).listString(j))
                               + a2Qstr(
                                   QUOTE
                                   + EOL
                                   );
                }
            }

            //Write out end of list
            fileOut << TAB(2) + a2Qstr(LIST_END + END + EOL);
        }
        else
        {
            //---------------------------------------
            //Single value

            //Check Data type

            if (ReportIn->listRAO(i).getDataType() == 0)
            {
                //Complex data type
                for (unsigned int j = 0; j < ReportIn->listRAO(i).listValue().size(); j++)
                {
                    fileOut << Val2String(
                                  ReportIn->listRAO(i).listValue(j))
                            + a2Qstr(END + EOL);
                }
            }

            else if (ReportIn->listRAO(i).getDataType() == 1)
            {
                //Double data type
                for (unsigned int j = 0; j < ReportIn->listRAO(i).listValue().size(); j++)
                {
                    fileOut << Val2String(
                                    ReportIn->listRAO(i).listValueDouble(j))
                                + a2Qstr(END + EOL);
                }
            }

            else if (ReportIn->listRAO(i).getDataType() == 2)
            {
                //Integer data type
                for (unsigned int j = 0; j < ReportIn->listRAO(i).listValue().size(); j++)
                {
                    fileOut << Val2String(
                                    ReportIn->listRAO(i).listValueInt(j))
                                + a2Qstr(END + EOL);
                }
            }

            else if (ReportIn->listRAO(i).getDataType() == 3)
            {
                //String data type
                for (unsigned int j = 0; j < ReportIn->listRAO(i).listString().size(); j++)
                {
                    fileOut << a2Qstr(
                                   QUOTE)
                                   + Val2String(
                                       ReportIn->listRAO(i).listString(j))
                               + a2Qstr(
                                   QUOTE
                                   + END
                                   + EOL
                                   );
                }
            }
        }

        //---------------------------------------
        //Closing bracket to object.
        fileOut << TAB() + a2Qstr(OBJECT_END + EOL + EOL);
    }

    //---------------------------------------
    //Closing bracket to object.
    fileOut << a2Qstr(OBJECT_END + EOL);

    //End mark.
    fileOut << a2Qstr(BREAK_BOTTOM + EOL + EOL + EOL + EOL);

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
    return a2Qstr(output);
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
                //Search through the list of reports and see if it matches any of the
                //report filenames.

                for (unsigned int i = 0; i < plistReport.size(); i++)
                {
                    if (a2Qstr(this->listReportPt(i)->getFileName()) == info.fileName())
                    {
                        result = QFile::remove(info.absoluteFilePath());
                        break;
                    }
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
QString ReportManager::getInfoBlock(string nameIn)
{
    return a2Qstr(VAL_SEAFILE + SPACE + OBJECT_BEGIN + EOL)
            + TAB()
            + a2Qstr(KEY_VERSION)
            + TAB()
            + a2Qstr(VAL_VERSION + END + EOL)
            + TAB()
            + a2Qstr(KEY_FORMAT)
            + TAB()
            + a2Qstr(VAL_FORMAT + END + EOL)
            + TAB()
            + a2Qstr(KEY_OBJECT)
            + TAB()
            + a2Qstr(nameIn + END + EOL + OBJECT_END + EOL + EOL);
}

//------------------------------------------Function Separator --------------------------------------------------------
bool ReportManager::createDir(QString path)
{
    //Create Qt directory object.
    path = a2Qstr(ptSystem->getPath())
           + a2Qstr(SLASH)
           + path;

    QDir dir(path);    //Qt object that is the output directory.

    //Check if directory exists and create if not.
    return dir.mkpath(".");
}

//------------------------------------------Function Separator --------------------------------------------------------
QString ReportManager::Val2String(std::string valIn)
{
    //Convert string to QString
    return a2Qstr(valIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
QString ReportManager::Val2String(int valIn)
{
    //Convert to QString
    return a2Qstr(itoa(valIn));
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
    convert << a2Qstr(LIST_BEGIN);

    //Enter real number
    convert << valIn.real();

    //Enter separation mark
    convert << a2Qstr(COMPLEX_COORD);

    //Enter imaginary part
    convert << valIn.imag();

    //Enter closing brackets
    convert << a2Qstr(LIST_END);

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
QString ReportManager::a2Qstr(std::string valIn)
{
    return QString::fromAscii(valIn.data(), valIn.size());
}

//------------------------------------------Function Separator --------------------------------------------------------
string ReportManager::Qstr2a(QString valIn)
{
    return std::string(valIn.toAscii());
}
