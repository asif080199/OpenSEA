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

#include "objectgroup.h"

using namespace std;

//######################################### Class Separator ###########################################################
//Prototype class definitions
class Parser;       //This forward declaration is required because the class definition involves recursion of the
                    //class.

//######################################### Class Separator ###########################################################
//Constant Variables

// Characters within Input files
// ---------------------------------
const int HEADER_LENGTH = 6; /**< Length of header, This is temp needs to be fixed. */
const string COMMENT_LINE = "//"; /**< Line Comment. */
const string COMMENT_BLOCK_BEGIN = "/*"; /**< Block Comment Begin. */
const string COMMENT_BLOCK_END = "*/"; /**< Block comment end. */
const string END_STATEMENT = ";"; /**< End of statement. */
const string OBJECT_BEGIN = "{"; /**< Object scope begin. */
const string OBJECT_END = "}"; /**< Object scope end */
const string LIST_BEGIN = "("; /**< List scope begin. */
const string LIST_END = ");"; /**< List scope end. */
const string KEY_VAL_SEPARATOR= ":"; /**< Key/Val pair seperator. */
const char EOL = '\n'; /**< newline. */
const int MAX_IGNORE = 150000; /**< Max # of chars to ignore. */
const string OBJ_SEAFILE = "seafile"; /**< seafile object name, used to ignore seafile block. */
const string QUOTE = "\"";

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
class Parser
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
     * @param The input file to parse.
     */
    void Parse(istream& infile);

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
    vector<ObjectGroup>& refObject();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of SubObjects.
     * @param index1 Index of which ObjectGroup object to retrieve.  If not specified, the default is the first object
     * in the list.
     * @return Returns a reference to the list of SubObjects detected by the parser.  Returned variable passed by
     * reference.  Returned variable is a vector of pointers to the SubObjects.
     */
    vector<ObjectGroup> &refSubObject(int index1 = 0);

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
     * @brief The object segmented by the parser.
     */
    vector<ObjectGroup> plistObject;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief ObjectGroup objec to contain the OpenSEA object that is the header at the beginning of every input file.
     */
    ObjectGroup oSeaObject;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the recursive object of this class.  This is necessary to allow recursive parsing of an object.
     */
    Parser subParse;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Another segment of the Parse() function.  This is the section of the Parse function specific to
     * parsing out valid commands from the input file, after all comments and extraneous detail was filtered out.
     * @param infile The input file to parse.
     * @param prevString The string from the previous loop of parsing the input stream.
     */
    void ParseCommands(istream& infile, string prevString);

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
     */
    int curObject;
};

#endif // PARSER_H
