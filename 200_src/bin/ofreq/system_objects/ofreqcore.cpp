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

#ifdef Q_OS_WIN
    string oFreqCore::SLASH = "\\";  /**< Directory separator in a string path., windows version**/
#elif defined Q_OS_LINUX
    string oFreqCore::SLASH = "/";   /**< Directory separator in a string path., linux version**/
#endif

Log oFreqCore::logStd;      /**< Standard log file declaration */
Log oFreqCore::logErr;      /**< Error log file declaration. */
Log oFreqCore::logMon;      /**< Monitor log file declaration */

double oFreqCore::GRAVITY = 9.80665; /**< Acceleration due to gravity.  Normal earth gravity.*/
double oFreqCore::PI = 3.14159265359;   /**< Mathematical constant pi.*/

string oFreqCore::DOT = "."; /**< Relative reference to same directory */
string oFreqCore::DDOT = ".."; /**< Relative reference to a directory one up*/

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
oFreqCore::oFreqCore()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
oFreqCore::~oFreqCore()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
string oFreqCore::itoa(int input)
{
    std::stringstream convert;
    convert << input;
    return convert.str();
}

//------------------------------------------Function Separator --------------------------------------------------------
string oFreqCore::ftoa(double input)
{
    std::stringstream convert;
    convert << input;
    return convert.str();
}

//------------------------------------------Function Separator --------------------------------------------------------
string oFreqCore::ctoa(std::complex<double> input)
{
    std::stringstream convert;
    convert << "(";
    convert << input.real() << "," << input.imag();
    convert << ")";
    return convert.str();
}

//==========================================Section Separator =========================================================
//Public Slots


//==========================================Section Separator =========================================================
//Public Signals


//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

