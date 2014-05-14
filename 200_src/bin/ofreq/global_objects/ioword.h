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
 *May 7, 2014       Nicholas Barczak    Initially Created
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
//Put header files
#ifndef IOWORD_H
#define IOWORD_H
#include <string>
#include "../system_objects/ofreqcore.h"

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
 * @brief The list of common symbols and words used to parse input and output files in oFreq.
 *
 * This is a pure virtual class.  It provides static definitions of common symbols and words used to define items
 * in input and output files.  These are not the definitions for file interpretation.  This is only the information
 * specific to the parsing of the files.  Anything specific to interpretation should be included in the definitions
 * for individual classes.  All the inputs and outputs are defined with static variables to allow easier refactoring
 * and changes to the code if the need ever arises in the future.
 *
 * All classes for input and output files should inherit this class to pickup the static variables.
 */
class ioword : public osea::ofreq::oFreqCore
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Nothing happens here.
     */
    ioword();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.  Nothing happens here.
     */
    ~ioword();

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    //Commenting marks
    static std::string COMMENT_LINE; /**< Line Comment. */
    static std::string COMMENT_BLOCK_BEGIN; /**< Block Comment Begin. */
    static std::string COMMENT_BLOCK_END; /**< Block comment end. */
    static std::string BREAK_TOP; /**< String of characters to visually separate the top in an output file. */
    static std::string BREAK_BOTTOM; /**< String of characters to visually separate the bottom in an output file. */

    //------------------------------------------Function Separator ----------------------------------------------------
    //General file formatting marks
    static std::string END_STATEMENT; /**< End of statement. */
    static std::string OBJECT_BEGIN; /**< Object scope begin. */
    static std::string OBJECT_END; /**< Object scope end */
    static std::string LIST_BEGIN; /**< List scope begin. */
    static std::string LIST_END; /**< List scope end. */
    static std::string END; /**< End Statement. */
    static std::string TAB_REF; /**< Tab spacing.  Entered as a series of spaces.*/
    static std::string SPACE; /**< Just a space " " */
    static std::string EOL; /**< newline. */
    static std::string QUOTE;    /**< Quotation symbol " */
    static std::string LIST_EXPAND; /**< Mark to note expansion of a list */
    /**
     * Mark to separate real from imaginary values in a complex number, using coordinate system notation.
     */
    static std::string COMPLEX_COORD;

    //------------------------------------------Function Separator ----------------------------------------------------
    //General object names
    static std::string OBJ_SEAFILE; /**< seafile object name*/

    //------------------------------------------Function Separator ----------------------------------------------------
    //Key value separator symbol.
    static std::string KEY_VAL_SEPARATOR; /**< Key/Val pair seperator. */

    //------------------------------------------Function Separator ----------------------------------------------------
    //Number constants
    static int MAX_IGNORE; /**< Max # of chars to ignore. */
    static int DIGIT; /** The number of digits to use in precision of floating point numbers.*/

//==========================================Section Separator =========================================================
private:


};

}   //Namespace ofreq
}   //Namespace osea

#endif // ioword_H
