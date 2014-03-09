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

#include "parser.h"

using namespace std;
using namespace osea;

//==========================================Section Separator =========================================================
//Static Initialization
string Parser::COMMENT_LINE = "//"; /**< Line Comment. */
string Parser::COMMENT_BLOCK_BEGIN = "/*"; /**< Block Comment Begin. */
string Parser::COMMENT_BLOCK_END = "*/"; /**< Block comment end. */
string Parser::END_STATEMENT = ";"; /**< End of statement. */
string Parser::OBJECT_BEGIN = "{"; /**< Object scope begin. */
string Parser::OBJECT_END = "}"; /**< Object scope end */
string Parser::LIST_BEGIN = "("; /**< List scope begin. */
string Parser::LIST_END = ")"; /**< List scope end. */
string Parser::KEY_VAL_SEPARATOR= ":"; /**< Key/Val pair seperator. */
char Parser::EOL = '\n'; /**< newline. */
int Parser::MAX_IGNORE = 150000; /**< Max # of chars to ignore. */
string Parser::OBJ_SEAFILE = "seafile"; /**< seafile object name, used to ignore seafile block. */
string Parser::QUOTE = "\"";
string Parser::SPACE = " ";
string Parser::RETURN = "\n";

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
Parser::Parser()
{
    //Set initial value for curObject
    curObject = -1;

    //Set initial value for curKeySet
    curKeySet = 0;
}

//------------------------------------------Function Separator --------------------------------------------------------
Parser::~Parser()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void Parser::Parse(istream &infile, int bracket_count)
{
    string curString = "";  //curSring is the current string in the text file
    string prevString = ""; //The previously read string in the text file
    int minBracket = bracket_count;     //The current required number of minimum brackets to continue iterating.

    //Ensure the keySet variable is reset
    keySet = false;

    //Read a line from the input file
    infile >> curString;
    //Set the previous string.
    prevString = curString;

    //Loop through each line in the file and apply a series of possibilities for how to process the file.
    //Check condition for bracket count.  Used as a termination criteria for recursive instances.
    while (bracket_count >= minBracket)
    {
        //Check for end of file
        //-----------------------------------
        if (infile.eof()) break; //End of File, end parsing

        //Check for comments
        //-----------------------------------
        curString = CommentFilter(curString, infile);

        //Found valid pairs of text that are true input.
        //-----------------------------------
        if (!curString.empty())
            ParseCommands(infile, curString, prevString, bracket_count);  //Valid inputs detected.

        //If bracket count is below current count, don't pull out the next term.  Just go
        //to the next loop.
        if (bracket_count < minBracket)
            continue;

        //Only do this if the bracket count is not negative, and curString is not negative.
        if (!curString.empty())
            prevString = curString;  //Records the string from the last loop.  Used for list recognition and object
                                 //name recognition.

        //Read a line from the input file.
        infile >> curString; //curSring is the current string in the text file
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
ObjectGroup Parser::getObject(int index)
{
    return plistObject.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<ObjectGroup> &Parser::listObject()
{
    return plistObject;
}

//------------------------------------------Function Separator --------------------------------------------------------
ObjectGroup &Parser::listObject(unsigned int index)
{
    return plistObject[index];
}

//------------------------------------------Function Separator --------------------------------------------------------
vecKeyword& Parser::listKey()
{
    return plistKey;
}

//------------------------------------------Function Separator --------------------------------------------------------
string &Parser::listKey(int index)
{
    return plistKey[index];
}

//------------------------------------------Function Separator --------------------------------------------------------
vecValue& Parser::listVal()
{
    return plistVal;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<string> &Parser::listVal(int index)
{
    return plistVal[index];
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<ObjectGroup*> &Parser::refSubObject(int index1)
{
    return plistObject[index1].listObject();
}

//------------------------------------------Function Separator --------------------------------------------------------
ObjectGroup Parser::getSubObject(int index, int index1)
{
    return *(plistObject.at(index1).listObject(index));
}

//==========================================Section Separator =========================================================
//Protected Functions

//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
std::string Parser::CommentFilter(std::string curString, std::istream &infile)
{
    //Check for a single comment line mark.
    //-----------------------------------
    if (curString.find(COMMENT_LINE) != std::string::npos)
    {
        infile.ignore(MAX_IGNORE, EOL);

        //return blank string
        return "";
    }

    //Check for a block comment.
    //-----------------------------------
    else if ( curString.find(COMMENT_BLOCK_BEGIN) != std::string::npos)
    {
        int commentcount = 1;       //Number of comment markers

        for(;;)
        {
            infile >> curString;
            if (infile.eof()) break;

            if (curString.find(COMMENT_BLOCK_BEGIN) != std::string::npos)
                commentcount += 1;

            if( curString.find(COMMENT_BLOCK_END) != std::string::npos)
                commentcount -= 1;

            if (commentcount <= 0)
                break;
        }
        //return blank string
        return "";
    }

    //Valid input.  Return string.
    else
    {
        return curString;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void Parser::ParseCommands(istream &infile, string curString, string prevString, int &bracket_count)
{
    char ignoreChars[2];        //Characters that will be ignored by the file
    ignoreChars[0] = END_STATEMENT[0];     //Ignore the characters for the end statement
    ignoreChars[1] = QUOTE[0];             //Ignore the characters for the quotation.
    bool advanceKeySet = false;                     //Tracks whether we need to advance to the next key set.

    if (!infile.eof())
    {
        //Check for string enclosed in quotation marks.
        //-----------------------------------
        if(curString.find(QUOTE) != std::string::npos) //the string will be enclosed in quotes.  Evaluates to 0 if equal.
        {
            int quoteCount = std::count(curString.begin(), curString.end(), QUOTE[0]);//the count of quotes in this string

            if(quoteCount == 1) //if only 1 then need to get the rest of string enclosed in end quote
            {
                string tempInput;
                infile >> tempInput;
                while(tempInput.find(QUOTE) == std::string::npos)  //keep going until find '\"'
                {
                    curString += " " + tempInput; //add to curString the rest of string
                    infile >> tempInput;
                }
                curString += " " + tempInput; //add the last part
            }
        }

        //Check if need to advance to the next key set.  Check for it now because the next step is to strip
        //out the end statement data.
        //-----------------------------------
        if (curString.find(END_STATEMENT) != std::string::npos)
            advanceKeySet = true;
        else
            advanceKeySet = false;

        //remove ignore chars from string
        //-----------------------------------
        for(unsigned int i = 0; i < 2; i++)
            curString.erase(
                        std::remove(curString.begin(), curString.end(), ignoreChars[i]),
                        curString.end());

        vector<string> theList;     //list of output items

        //Check for object definition
        //-----------------------------------
        if (curString.find(OBJECT_BEGIN) != std::string::npos)
        {
            //Only create new object if not already in an object.
            //Create new object and store the name
            plistObject.push_back(ObjectGroup());
            curObject += 1;
            plistObject[curObject].setClassName(prevString);

            //Initialize the subparser
            subParse = new Parser();

            //Feed the information to a recursive instance of Parse function.
            subParse->Parse(infile, bracket_count + 1);

            //Get the keysets from the parsed object
            for (unsigned int i = 0; i < subParse->listKey().size(); i++)
            {
                plistObject[curObject].addKeySet(subParse->listKey(i), subParse->listVal(i));
            }

            //Get the sub-objects from the parsed object
            for (unsigned int i = 0; i < subParse->listObject().size(); i++)
            {
                plistObject[curObject].addSubObject(subParse->listObject(i));
            }

            //Delete the subparser
            delete subParse;
        }

        //Check if this is an object closing
        //-----------------------------------
        else if (curString.find(OBJECT_END) != std::string::npos)
        {
            //Reduce the bracket count
            bracket_count -= 1;
        }

        //Check if this is a list
        //-----------------------------------
        else if (curString.find(LIST_BEGIN) != std::string::npos)
        {
            //Get next statement in list
            infile >> curString;

            //Filter out any comments
            //curString = CommentFilter(curString, infile);

            while(curString.find(LIST_END) == std::string::npos) //Evaluates to zero if equal
            {
                //Filter out any comments
                curString = CommentFilter(curString, infile);
                if (curString.empty())
                {
                    infile >> curString;
                    continue;
                }

                int index = curString.find(KEY_VAL_SEPARATOR);  //Find the separator mark.
                int listindex;                         //Index of the value in the list of values.
                string listval;                                 //Value in the list of values.

                //Check if this is a direct list.
                if (index == std::string::npos)
                {
                    //Direct list
                    theList.push_back(curString);
                }
                else if(curString[0] != LIST_END[0])
                {
                    //Not direct list

                    //split the string into index and value
                    listindex = atoi(curString.substr(0, index + 1).c_str());
                    listval = curString.substr(index + 1, curString.length() - 1 - index);

                    //Remember, vector list starts counting at zero.
                    //But user list starts counting at 1.

                    if (listindex > theList.size())
                    {
                        //resize list vector
                        theList.resize(listindex);
                    }

                    //Assign value
                    theList[listindex - 1] = listval;
                }

                if (infile.eof())
                {
                    writeError("Error:  Illegal input list format");
                        break;
                }

                infile >> curString;    //Get the next character
            }

            if (keySet)
            {
                //Write the output to the key value.
                if ((curKeySet > plistVal.size() - 1) || (plistVal.size() == 0))
                    plistVal.resize(curKeySet + 1);
                plistVal[curKeySet] = theList;
            }
            else
            {
                writeError("Error:  No key word found to match key value.");
            }

            theList.clear();

            //Check if need to advance to the next key set.
            //-----------------------------------
            if (curString.find(END_STATEMENT) != std::string::npos)
                advanceKeySet = true;
            else
                advanceKeySet = false;
        }

        //Process as normal set of key words and values.
        //----------------------------------------------------------
        else
        {
            //First check if the next input will be an object declaration.
            if (ObjectCheck(infile) != 1)
            {
                //Next input is not an object declaration.
                //Safe to proceed.

                //Check if processing key word or key value
                if (keySet)
                {
                    //Key already set.  Processing as key value.

                    //create vector of input
                    vector<string> input(1);
                    input[0] = curString;

                    //Add input to the list of values
                    if ((curKeySet > plistVal.size() - 1) || (plistVal.size() == 0))
                        plistVal.resize(curKeySet + 1);
                    plistVal[curKeySet] = input;
                }

                else
                {
                    //Key not set.  Process as key word.
                    //Check if list is big enough
                    if ((curKeySet > plistKey.size() - 1) || (plistKey.size() == 0))
                        plistKey.resize(curKeySet + 1);
                    plistKey[curKeySet] = curString;

                    //Switch boolean to indicate key word set.
                    keySet = true;
                }
            }
        }

        //Handle values for statement ends.
        //----------------------------------------------------------
        if (advanceKeySet)
        {
            //Advance to next keyValueSet
            curKeySet += 1;

            //Reset key word set
            keySet = false;

            //Reset advanceKeySet
            advanceKeySet = false;
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int Parser::ObjectCheck(istream &infile)
{
    char junk;  //Junk variable to read off white spaces.
    char curChar = infile.peek();

    while (curChar == SPACE[0] || curChar == RETURN[0])
    {
        //Eliminate junk variable.
        infile.read(&junk,1);
        curChar = infile.peek();
    }

    //Now read the extracted data to see if it contains an object declaration
    if (curChar == OBJECT_BEGIN[0])
        return 1;
    else if (curChar == OBJECT_END[0])
        return -1;
    else
        return 0;
}

//------------------------------------------Function Separator --------------------------------------------------------
int Parser::ObjectCheck(std::string &inString)
{
    //read the data to see if it contains an object declaration.
    if (inString.find(OBJECT_BEGIN) != std::string::npos)
        return 1;
    else if (inString.find(OBJECT_END) != std::string::npos)
        return -1;
    else
        return 0;
}
