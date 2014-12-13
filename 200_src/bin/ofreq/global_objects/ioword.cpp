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

#include "ioword.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Commenting marks
string ioword::COMMENT_LINE = "//"; /**< Line Comment. */
string ioword::COMMENT_BLOCK_BEGIN = "/*"; /**< Block Comment Begin. */
string ioword::COMMENT_BLOCK_END = "*/"; /**< Block comment end. */
string ioword::BREAK_TOP = "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //\n";
string ioword::BREAK_BOTTOM = "// ************************************************************************* //";

//------------------------------------------Function Separator ----------------------------------------------------
//General file formatting marks
string ioword::OBJECT_BEGIN = "{"; /**< Object scope begin. */
string ioword::OBJECT_END = "}"; /**< Object scope end */
string ioword::LIST_BEGIN = "("; /**< List scope begin. */
string ioword::LIST_END = ")"; /**< List scope end. */
string ioword::END = ";";
string ioword::TAB_REF = "  ";
string ioword::SPACE = " ";
string ioword::EOL = "\n"; /**< newline. */
string ioword::QUOTE = "\"";
string ioword::LIST_EXPAND = "..."; /**< Mark to note expansion of a list */
string ioword::COMPLEX_COORD = ","; /** < Mark to separate real from imaginary values in a complex number */

//------------------------------------------Function Separator ----------------------------------------------------
// Directory Names
#ifdef Q_OS_WIN
    string ioword::SLASH = "\\";  /**< Directory separator in a string path., windows version**/
#elif defined Q_OS_LINUX
    string ioword::SLASH = "/";   /**< Directory separator in a string path., linux version**/
#endif

//------------------------------------------Function Separator ----------------------------------------------------
//General object names
string ioword::OBJ_SEAFILE = "seafile"; /**< seafile object name, used to ignore seafile block. */

//------------------------------------------Function Separator ----------------------------------------------------
//Key value separator symbol.
string ioword::KEY_VAL_SEPARATOR= ":"; /**< Key/Val pair seperator. */
string ioword::KEY_FORMAT = "format";  /**< The key designator for a format value in the seafile object. */
string ioword::KEY_VERSION = "version"; /**< The key designator for a version value in the seafile object. */
string ioword::KEY_OBJECT = "object";  /**< The key designator for a object value in the seafile object. */
string ioword::VAL_VERSION = "1.0"; /**< Value to denote current program version. */
string ioword::VAL_FORMAT = "ascii"; /**< Value to denote file formatting. */
string ioword::VAL_SEAFILE = "seafile"; /**< Value to denote a seafile object. */

//------------------------------------------Function Separator ----------------------------------------------------
//Number constants
int ioword::MAX_IGNORE = 150000; /**< Max # of chars to ignore. */
int ioword::DIGIT = 6; /** The number of digits to use in precision of floating point numbers.*/

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator ----------------------------------------------------
ioword::ioword()
{
}

ioword::~ioword()
{
}

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions
