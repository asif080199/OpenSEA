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

#include "log.h"

using namespace osea::ofreq;
using namespace std;

//==========================================Section Separator =========================================================
//Static initialization
#ifdef Q_OS_WIN
    std::string Log::SLASH = "\\";  /**< Directory separator in a string path., windows version**/
#elif defined Q_OS_LINUX
    std::string Log::SLASH = "/";   /**< Directory separator in a string path., linux version**/
#endif


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
Log::Log()
{
    defaultOutput = "Both";
    defaultTimestamp = 1;
}

//------------------------------------------Function Separator --------------------------------------------------------
Log::~Log()
{
    outFile.close();
}

//------------------------------------------Function Separator --------------------------------------------------------
void Log::Write(std::string msg, int severity, int timestamp, std::string output)
{
    //Write output of file
    std::string msgOut = "";        //THe output message.
    std::string prefix = "";        //THe prefix to put in front of output message.

    //Decide which type of prefix to create.
    switch (severity)
    {
    case 0:
        prefix = "ERROR:  ";
        break;
    case 1:
        prefix = "WARNING:  ";
        break;
    case 2:
        prefix = "INFORMATION:  ";
        break;
    case 3:
        prefix = "";
        break;
    default:
        prefix = "";
    }

    //Check if using default output
    if (output == "Default")
        output = defaultOutput;

    //Check if using default timestamp.
    if (timestamp == 0)
    {
        timestamp = defaultTimestamp;
    }

    //Step 1:  Decide which type of output to go with.
    if ((output == "Log") ||
            (output == "log") ||
            (output == "File") ||
            (output == "file"))
    {
        //Case for writing to log file only.

        //Check if should write a timestamp.
        if (timestamp == 1)
        {
            //First write date and time.
            time_t rawtime;
            struct tm * timeinfo;
            char buffer[21];

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(buffer,21,"%Y-%b-%d %H:%M:%S",timeinfo);
            outFile << buffer << "\t";
        }

        //Write out the message and append carriage return
        outFile << prefix << msg << endl;
    }
    else if ((output == "Screen") ||
             (output == "screen") ||
             (output == "Message") ||
             (output == "message"))
    {
        //Case for writing to screen only

        //Check if should write a timestamp.
        if (timestamp == 1)
        {
            //First write date and time.
            time_t rawtime;
            struct tm * timeinfo;
            char buffer[21];

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(buffer,21,"%Y-%b-%d %H:%M:%S",timeinfo);
            outFile << buffer << "\t";
        }

        //Write out the message and append carriage return
        std::cout << prefix << msg << endl;
    }
    else if ((output == "Both") ||
             (output == "both") ||
             (output == "Dual") ||
             (output == "dual") ||
             (output == "All") ||
             (output == "all"))
    {
        //Case for writing to both log file and screen.

        //Check if should write a timestamp.
        if (timestamp == 1)
        {
            //First write date and time.
            time_t rawtime;
            struct tm * timeinfo;
            char buffer[21];

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(buffer,21,"%Y-%b-%d %H:%M:%S",timeinfo);
            outFile << buffer << "\t";
            outFile << buffer << "\t";
        }

        //Write out the message and append carriage return
        outFile << prefix << msg << endl;
        std::cout << prefix << msg << endl;
    }

    //Finally cancel program execution, if required.
    if (severity == 0)
        exit(1);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Log::WriteLog(std::string msg, int severity, int timestamp)
{
    //Write to the log file only
    Write(msg, severity, timestamp, "Log");
}

//------------------------------------------Function Separator --------------------------------------------------------
void Log::WriteScreen(std::string msg, int severity, int timestamp)
{
    //Write to the screen only
    Write(msg, severity, timestamp, "Screen");
}

//------------------------------------------Function Separator --------------------------------------------------------
void Log::setLogFile(std::string pathIn)
{
    try
    {
        //Parse the log file
        parsePath(pathIn);

        //Open file
        outFile.close();
        outFile.open(fileDir + SLASH + fileName, std::ofstream::app);
        //File automatically appends to end of any existing files.

        //Check if the file failed to open successfully.
        if (!outFile.is_open())
            throw std::runtime_error(fileName + string(":  Failed to open file successfully."));
    }
    catch(const std::exception &err)
    {
        std::cout << err.what() << endl;
        cout << fileDir << SLASH << fileName << endl;
    }
    catch(...)
    {
        throw std::runtime_error(__FUNCTION__);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void Log::deleteLog(std::string pathIn)
{
    try
    {
        //Parse the log file, if specified
        if (pathIn != "")
            parsePath(pathIn);

        //Get the Qstring for filename
        QString filename = QString::fromStdString(fileDir + SLASH + fileName);
        QFile file(filename);

        //Check if the file exists.  If so, delete it.
        if (file.exists())
            file.remove();

        //Check if the file still exists.
        if (file.exists())
            throw std::runtime_error("Failed to delete log file.");
    }
    catch(const std::exception &err)
    {
        std::cout << string("Error:  ") << string(__FUNCTION__) << string(">>  ") << err.what() << endl;
    }
    catch(...)
    {
        throw std::runtime_error(__FUNCTION__);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void Log::cls()
{
    //Print many carriage returns.
    cout << string(100, '\n');
}

//------------------------------------------Function Separator --------------------------------------------------------
void Log::setDefaultOutput(std::string output)
{
    defaultOutput = output;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Log::setDefaultTimeStamp(bool timein)
{
    if (timein)
        defaultTimestamp = 1;
    else
        defaultTimestamp = -1;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Log::Notify(std::string input)
{
    string msg;
    string def = "Errors found.  Please check the error log for details.";

    if (input == "")
        msg = def;
    else
        msg = input;

    //Write the notify message.
    Write(msg, 3);
}


//==========================================Section Separator =========================================================
//Protected Funcitons


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
void Log::parsePath(std::string pathIn)
{
    //Parse the path to pull out the file name.

    //Get position of last diretory separator.
    std::size_t pos = pathIn.find_last_of(SLASH);

    //Parse out directory
    if (pos == std::string::npos)
    {
        //no directory included.
        fileDir = ".";
    }
    else
    {
        fileDir = pathIn.substr(0,pos);
    }

    //Parse out filename
    if (pos == std::string::npos)
    {
        fileName = pathIn;
    }
    else
    {
        fileName = pathIn.substr(pos+1);
    }
}
