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

//==========================================Section Separator =========================================================
//Static Initialization
string Parser::COMMENT_LINE = "//"; /**< Line Comment. */
string Parser::COMMENT_BLOCK_BEGIN = "/*"; /**< Block Comment Begin. */
string Parser::COMMENT_BLOCK_END = "*/"; /**< Block comment end. */
string Parser::END_STATEMENT = ";"; /**< End of statement. */
string Parser::OBJECT_BEGIN = "{"; /**< Object scope begin. */
string Parser::OBJECT_END = "}"; /**< Object scope end */
string Parser::LIST_BEGIN = "("; /**< List scope begin. */
string Parser::LIST_END = ");"; /**< List scope end. */
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
    curObject = 0;
    //Resize list of objects.
    plistObject.resize(curObject + 1);
}

//------------------------------------------Function Separator --------------------------------------------------------
Parser::~Parser()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void Parser::Parse(istream &infile)
{
    string curString = "";
    string prevString = "";

    //Loop through each line in the file and apply a series of possibilities for how to process the file.
    for(;;)
    {
        //Read a line from the input file.
        infile >> curString; //curSring is the current string in the text file

        //Check for a variety of conditions

        //Check for end of file
        //-----------------------------------
        if (infile.eof()) break; //End of File, end parsing

        //Check for a single comment line mark.
        //-----------------------------------
        else if (curString.find(COMMENT_LINE) != std::string::npos)
        {
            infile.ignore(MAX_IGNORE, EOL);
            if (infile.eof()) break;
        }

        //Check for a block comment.
        //-----------------------------------
        else if (curString == COMMENT_BLOCK_BEGIN)
        {
            for(;;)
            {
                infile >> curString;
                if (infile.eof()) break;

                if(curString == COMMENT_BLOCK_END)
                {
                    break;
                }
            }
        }

        //Found valid pairs of text that are true input.
        //-----------------------------------
        else
        {
            ParseCommands(infile, prevString);  //Valid inputs detected.  Parse them with the set of rules
                                                //For valid inputs.
        }

        prevString = curString;  //Records the string from the last loop.  Used for list recognition and object
                                 //name recognition.
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
void Parser::ParseCommands(istream &infile, string prevString)
{
    string curString = "";      //Current string from input.
    char ignoreChars[2];        //Characters that will be ignored by the file
    ignoreChars[0] = END_STATEMENT[0];     //Ignore the characters for the end statement
    ignoreChars[1] = QUOTE[0];             //Ignore the characters for the quotation.

    infile >> curString;
    if ( !infile.eof())
    {
        //Check for object definition
        //-----------------------------------
        if (curString == OBJECT_BEGIN)
        {
            //create new object and store the name
            plistObject.push_back(ObjectGroup());
            curObject += 1;
            plistObject[curObject].setClassName(prevString);
            //Initialize the subparser
            subParse = new Parser();

            int bracket_count;      //THe number of object definition brackets.  Must add to zero
                                    //in the end.
            bracket_count += 1;     //Add one for the bracket already found.

            while (bracket_count != 0)
            {
                //Loop through the object definition and isolate the object defined.

                //Check for another object declaration
                bracket_count += ObjectCheck(infile);

                //Feed the information to a recursive instance of Parse function.
                subParse->Parse(infile);
            }

            //Get the keysets from the parsed object.
            for (unsigned int i = 0; i < subParse->getObject(0).listKey().size(); i++)
            {
                plistObject[curObject].addKeySet(subParse->getObject(0).getKey(i), subParse->getObject(0).getVal(i));
            }

            //Parsing complete.  Get the parsed subobjects back.
            for (unsigned int i = 0; i < subParse->listObject().size(); i++)
            {
                plistObject[curObject].addSubObject(subParse->getObject(i));
            }
        }

        //Check for string enclosed in quotation marks.
        //-----------------------------------
        if(curString[0] == QUOTE[0]) //the string will be enclosed in quotes
        {
            int quoteCount = std::count(curString.begin(), curString.end(), QUOTE[0]);//the count of quotes in this string

            if(quoteCount == 1) //if only 1 then need to get the rest of string enclosed in end quote
            {
                string tempInput;
                infile >> tempInput;
                while((tempInput.at(tempInput.length() - 1) != *END_STATEMENT.data()) &&
                      (tempInput.at(tempInput.length() - 1) != *QUOTE.data())) //keep going until find ";" or '\"'
                {
                    curString += " " + tempInput; //add to curString the rest of string
                    infile >> tempInput;
                }
                curString += " " + tempInput; //add the last part
            }
        }

        //remove ignore chars from string
        //-----------------------------------
        for(unsigned int i = 0; i < 2; i++)
            curString.erase(
                        std::remove(curString.begin(), curString.end(), ignoreChars[i]),
                        curString.end());

        //Check if this is a list
        //-----------------------------------
        vector<string> theList;     //list of output items

        if (curString == LIST_BEGIN) //check if is a list
        {
            while(curString != LIST_END)
            {
                infile >> curString;

                int index = curString.find(KEY_VAL_SEPARATOR);  //Find the separator mark.
                unsigned int listindex;                         //Index of the value in the list of values.
                string listval;                                 //Value in the list of values.

                //Check if this is a direct list.
                if ((curString != LIST_END) && (index != std::string::npos))
                {
                    //Direct list
                    theList.push_back(curString);
                }
                else if(curString != LIST_END)
                {
                    //Not direct list

                    //split the string into index and value
                    listindex = atoi(curString.substr(0, index + 1).c_str());
                    listval = curString.substr(index + 1, curString.length() - 1 - index);

                    if (listindex > theList.size() - 1)
                    {
                        //resize list vector
                        theList.resize(listindex + 1);
                    }

                    //Assign value
                    theList[listindex] = listval;
                }

                if (infile.eof())
                {
                    cerr << "ERROR: ILLEGAL LIST FORMAT" << endl;
                        break;
                }
            }

            if (keySet)
            {
                //Write the output to the key value.
                plistObject[curObject].addKeyVal(theList);

                //Turn off the key value
                keySet = false;
            }
            else
            {
                cerr << "Error:  No key word found to match key value." << endl;
            }

            theList.clear();
        }

        //Process as normal set of key words and values.
        //----------------------------------------------------------
        else
        {
            //Check if processing key word or key value
            if (keySet)
            {
                //Key already set.  Processing as key value.
                plistObject[curObject].addKeyVal(curString);

                //Reset key set value.
                keySet = false;
            }

            else
            {
                //Key not set.  Process as key word.
                plistObject[curObject].addKeyWord(curString);

                //Switch boolean to indicate key word set.
                keySet = true;
            }

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
