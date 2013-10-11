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
#include <istream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include "objectgroup.h"
#include "../global_objects/ofreqcore.h"

using namespace std;

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
class Parser : public oFreqCore
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
    void Parse(istream &infile);

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
    vector<ObjectGroup> &listObject();

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
     * @brief Provides direct access to the list of SubObjects.
     * @param index1 Index of which ObjectGroup object to retrieve.  If not specified, the default is the first object
     * in the list.
     * @return Returns a reference to the list of SubObjects detected by the parser.  Returned variable passed by
     * reference.  Returned variable is a vector of pointers to the SubObjects.
     */
    vector<ObjectGroup*> &refSubObject(int index1 = 0);

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
    Parser* subParse;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Another segment of the Parse() function.  This is the section of the Parse function specific to
     * parsing out valid commands from the input file, after all comments and extraneous detail was filtered out.
     * @param infile The input file to parse.
     * @param prevString The string from the previous loop of parsing the input stream.
     */
    void ParseCommands(istream &infile, string prevString);

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

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief This function reads an istream input and counts the number of occurrences of an object declaration.
     * Most importantly, it does this in a way to not eliminate the characters from the istream object.  It also ignores
     * white spaces.
     * @param infile The istream input to check for designated object characters.
     * @return Returns an integer counter to add or decrease the count of object designators.  Valid returned values
     * are:
     * +1:  OBJECT_BEGIN detected.  Object count added.
     * 0:   No object designator detected.  Nothing done.
     * -1:  OBJECT_END detected.  Object count decreased.
     */
    int ObjectCheck(istream &infile);

    //------------------------------------------Function Separator ----------------------------------------------------
    // Characters within Input files
    static string COMMENT_LINE; /**< Line Comment. */
    static string COMMENT_BLOCK_BEGIN; /**< Block Comment Begin. */
    static string COMMENT_BLOCK_END; /**< Block comment end. */
    static string END_STATEMENT; /**< End of statement. */
    static string OBJECT_BEGIN; /**< Object scope begin. */
    static string OBJECT_END; /**< Object scope end */
    static string LIST_BEGIN; /**< List scope begin. */
    static string LIST_END; /**< List scope end. */
    static string KEY_VAL_SEPARATOR; /**< Key/Val pair seperator. */
    static char EOL; /**< newline. */
    static int MAX_IGNORE; /**< Max # of chars to ignore. */
    static string OBJ_SEAFILE; /**< seafile object name, used to ignore seafile block. */
    static string QUOTE;    /**< Quotation symbol " */
    static string SPACE; /**< Just a space " " */
    static string RETURN; /**< The return character "\n" */
};

#endif // PARSER_H
