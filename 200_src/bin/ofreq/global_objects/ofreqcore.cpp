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

#include "ofreqcore.h"

using namespace osea::ofreq;
using namespace std;

//==========================================Section Separator =========================================================
//Static initialization
std::string oFreqCore::FILE_OUTPUT = "output.log";
std::string oFreqCore::FILE_ERROR = "error.log";
std::string oFreqCore::DIR = "";
#ifdef Q_OS_WIN
    std::string oFreqCore::SLASH = "\\";  /**< Directory separator in a string path., windows version**/
#elif defined Q_OS_LINUX
    std::string oFreqCore::SLASH = "/";   /**< Directory separator in a string path., linux version**/
#endif

std::ofstream oFreqCore::OutLog;
std::ofstream oFreqCore::ErrLog;
std::ofstream &oFreqCore::seaout = OutLog;
std::ofstream &oFreqCore::seaerr = ErrLog;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator ----------------------------------------------------
oFreqCore::oFreqCore()
{

}

//------------------------------------------Function Separator ----------------------------------------------------
oFreqCore::~oFreqCore()
{

}

//==========================================Section Separator =========================================================
//Public Slots


//==========================================Section Separator =========================================================
//Public Signals


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator ----------------------------------------------------
void oFreqCore::setOutLog(string dirIn)
{
    //check if input has a slash at the end.
    //All functions assume no slash at the end.
    if (dirIn[dirIn.length() - 1] == SLASH[0])
    {
        //End slash found.  Remove it.
        dirIn.erase(dirIn.length() - 1, 1);
    }
    DIR = dirIn;

    string filename = DIR + SLASH + FILE_OUTPUT;
    OutLog.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);
}

//------------------------------------------Function Separator ----------------------------------------------------
void oFreqCore::setErrLog(string dirIn)
{
    //check if input has a slash at the end.
    //All functions assume no slash at the end.
    if (dirIn[dirIn.length() - 1] == SLASH[0])
    {
        //End slash found.  Remove it.
        dirIn.erase(dirIn.length() - 1, 1);
    }
    DIR = dirIn;

    string filename = DIR + SLASH + FILE_ERROR;
    OutLog.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);
}

//------------------------------------------Function Separator ----------------------------------------------------
void oFreqCore::writeLog(string mesIn)
{
    //First write date and time to log file.
    OutLog << Time() << "\t\t";
    //Then write message and end in a carriage return.
    OutLog << mesIn << endl;

    //Write same message to output screen
    std::cout << Time() << "\t\t";
    //Write message to output screen and end in a carriage return.
    cout << mesIn << endl;
}

//------------------------------------------Function Separator ----------------------------------------------------
void oFreqCore::writeError(string mesIn)
{
    //First write date and time to log file, plus class name.
    ErrLog << Time() << "\t" << typeid(*this).name() << "\t\t\t";
    //Then write message and end in a carriage return.
    ErrLog << mesIn << endl;

    //Write output to screen.
    std::cout << endl << mesIn << endl << "Check Error log for details." << endl << endl;
}

//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator ----------------------------------------------------
const std::string oFreqCore::Time()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
