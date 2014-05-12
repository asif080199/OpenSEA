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
 *Aug 25 2013       Nicholas Barczak    Initially created
 *Oct 31, 2013      Nicholas Barczak    Debugged and updated to Rev 1.0
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
#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <string>
#include <istream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include "objectgroup.h"
#include "../global_objects/ioword.h"
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

//######################################### Class Separator ###########################################################
//Constant variables
/**
 * @brief Type definition used to store key values.  Must be a vector of vectors because a value may also be a list
 * of values.
 */
typedef std::vector< std::vector<std::string> > vecValue;

/**
 * @brief Type defintion used to store keywords.
 */
typedef std::vector<std::string> vecKeyword;

//######################################### Class Separator ###########################################################
//Prototype class definitions
class Parser;       //This forward declaration is required because the class definition involves recursion of the
                    //class.

//######################################### Class Separator ###########################################################
/**
 * @brief The Parser class takes an input segment of strings and segments that segment.  It strips out comments.
 * It recognizes quotation marks and groups those segments together.  Parser finally returns a series of ObjectGroup
 * objects.  Each ObjectGroup contains the classname and a list of keyword value pairs.
 *
 * The parser and ObjectGroup are both setup to allow endless recursion of class definitions.  You can easily
 * define classes within classes in the input files, with no limits.
 *
 */
class Parser : public osea::ofreq::ioword
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Nothing happens here.
     */
    Parser();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Default destructor.  Nothing happens here.
     */
    ~Parser();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Parses the input files into a vector of key-value pairs.
     * @param infile The input file to parse.  Istream.  Variable passed by reference.
     * @param bracket_count The count of object definition brackets.  Used as a termination condition in the parsing
     * process.  Provides control for any recursive instances of the Parse() function.  Default is 0.  The bracket
     * count updates while parsing, as the Parser finds new opening ({) and closing (}) brackets.  Parsing continues
     * as long as the bracket count is equal to or above the bracket count fed into the Parser as an argument.  At
     * this point, the function behavior does not change with what the actual value of bracket_count is.  Parsing
     * only requires that bracket_count is >= its starting value.  However, several development attempts indicated
     * that the behavior the Parse function may become dependent on the value of bracket_count.  So it is left in
     * as an argument to allow functionality expansion in any future development.
     * @sa ParseCommands()
     */
    void Parse(std::istream &infile, int bracket_count = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the ObjectGroup object.
     * @param index Index of which ObjectGroup object to retrieve.  If not specified, the default is the first object
     * in the list.
     * @return Returns vector of ObjectGroup objects.  Each entry in the vector represents an object.  Variable
     * passed by value.
     */
    ObjectGroup getObject(int index = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the vector of ObjectGroup objects processed by the Parser object.
     * @return Returns reference to the vector of ObjectGroup objects.  Variable passed by reference.
     */
    std::vector<ObjectGroup> &listObject();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the ObjectGroup object specified by index.
     *
     * Returns entry from the list of ObjectGroup objects.  Returned entry is specified by index.
     * @param index Unsigned integer.  The index of the ObjectGroup object to return from the list.
     * @return Returns ObjectGroup object.  Returned variable is passed by reference.
     * @sa ObjectGroup
     */
    ObjectGroup &listObject(unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to thelist of key words.
     * @return Returns a reference to the list of key words.  Variable passed by reference.
     */
    vecKeyword &listKey();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to an individual item in the list of key words.
     *
     * Returns the key word specified by the index.
     * @param index Integer.  The index of the key word to retrieve.
     * @return std::string.  The key word specified by index.  Returned variable passed by reference.
     */
    std::string &listKey(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of key values.
     * @return Returns a reference to the list of key values.  Variable passed by reference.
     */
    vecValue &listVal();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to an individual item in the list of key values.
     *
     * Returns the key value specified by the index.
     * @param index Integer.  THe index of the key value to retrieve.
     * @return Vector of strings.  The key value specified by the index.  Returned variable passed by reference.
     */
    std::vector<std::string> &listVal(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of SubObjects.
     * @param index1 Index of which ObjectGroup object to retrieve.  If not specified, the default is the first object
     * in the list.
     * @return Returns a reference to the list of SubObjects detected by the parser.  Returned variable passed by
     * reference.  Returned variable is a vector of pointers to the SubObjects.
     */
    std::vector<ObjectGroup*> &refSubObject(int index1 = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the subObject referenced by the index.
     * @param index Specifies which SubObject to return from the list of SubObjects.
     * @param index1 Index of which ObjectGroup object to retrieve.  If not specified, the default is the first object
     * in the list.
     * @return Returns a SubObject from the list of SubObjects.  Returned variable is passed by value.  Contains
     * the actual object and not a pointer to the object.
     */
    ObjectGroup getSubObject(int index, int index1 = 0);

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The objects segmented by the parser.  Contains the list of any sub objects retrieved from a recursive
     * instance of the parser.
     */
    std::vector<ObjectGroup> plistObject;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Vector of keywords.
     */
    vecKeyword plistKey;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Vector of vectors.  Each entry in the outer vector is a key value and corresponds to a keyword in the
     * plistKey vector.  The inner vector represents situations of lists.  Each entry in the inner vector is an
     * entry in the list of key values.
     */
    vecValue plistVal;

//    //------------------------------------------Function Separator ----------------------------------------------------
//    /**
//     * @brief ObjectGroup object to contain the OpenSEA object that is the header at the beginning of every input file.
//     */
//    ObjectGroup oSeaObject;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the recursive object of this class.  This is necessary to allow recursive parsing of an object.
     */
    Parser* subParse;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Private function to filter out comments from the input file.  Returns a string that stripped out all
     * comments.  Returns only the strings that do not contain comments.  Returns only a blank string if comments were
     * found.  For multi-line and single-line comments, it will read out all the comment text from the istream infile.
     * @param curString The string of the input to filter for comments.  Variable passed by value.
     * @param infile The istream object containing the file that will be read.  Allows the function to read off full
     * comment text.  Variable passed by reference.
     * @return Returns string.  Returned string is the input text stripped of all comments.  Returns a blank string
     * if only comments were contained in the file.
     */
    std::string CommentFilter(std::string curString, std::istream &infile);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Another segment of the Parse() function.  This is the section of the Parse function specific to
     * parsing out valid commands from the input file, after all comments and extraneous detail was filtered out.
     * @param infile The input file to parse.
     * @param curString
     * @param prevString The std::string from the previous loop of parsing the input stream.
     * @param bracket_count The count of object definition brackets.  Used as a termination condition in the parsing
     * process.  Provides control for any recursive instances of the Parse() function.  Default is 0.  Parsing
     * continues as long as bracket count is not less than zero.  Variable passed by reference.
     * @sa Parse()
     */
    void ParseCommands(std::istream &infile, std::string curString, std::string prevString, int &bracket_count);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Variable to track if the key word was already set.  This lets the Parser function know whether the
     * processed value should go into the key Word list or the key value list.
     * keySet = True:  Key already set and process as key value.
     * keySet = False:  Key not set and process as key word.
     */
    bool keySet;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Tracks which object the parser is currently adding data to.  Handles situations of possible multiple
     * object declarations.
     */
    int curObject;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Tracks which keyset the parser is currently adding data to.  Ensures that key words and key values are
     * always in sync.  Uses line end statement to trigger advancement to next key set.
     */
    int curKeySet;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief This function reads an istream input and determines if it qualifies as an object declaration.
     * Most importantly, it does this in a way to not eliminate the characters from the istream object.  It also ignores
     * white spaces.
     * @param infile The istream input to check for designated object characters.  Variable passed by reference.
     * @return Returns an integer counter to add or decrease the count of object designators.  Valid returned values
     * are:
     * +1:  OBJECT_BEGIN detected.  Object count added.
     * 0:   No object designator detected.  Nothing done.
     * -1:  OBJECT_END detected.  Object count decreased.
     */
    int ObjectCheck(std::istream &infile);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief This functions reads a string object and determines if it qualifies as an object declaration.  It also
     * ignores white spaces.
     * @param inString The string input to check for designated object characters.  Variable passed by reference.
     * @return Returns an integer counter to add or decrease the count of object designators.  Valid returned values
     * are:
     * +1:  OBJECT_BEGIN detected.  Object count added.
     * 0:   No object designator detected.  Nothing done.
     * -1:  OBJECT_END detected.  Object count decreased.
     */
    int ObjectCheck(std::string &inString);   
};

}   //Namespace osea

#endif
