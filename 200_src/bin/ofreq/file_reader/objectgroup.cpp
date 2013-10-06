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

#include "objectgroup.h"

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
ObjectGroup::ObjectGroup()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
ObjectGroup::~ObjectGroup()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void ObjectGroup::setClassName(string input)
{
    pClassName = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
string ObjectGroup::getClassName()
{
    return pClassName;
}

//------------------------------------------Function Separator --------------------------------------------------------
vecKeyword& ObjectGroup::listKey()
{
    return plistKey;
}

//------------------------------------------Function Separator --------------------------------------------------------
vecValue& ObjectGroup::listVal()
{
    return plistVal;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<ObjectGroup *> &ObjectGroup::listObject()
{
    return plistObject;
}

//------------------------------------------Function Separator --------------------------------------------------------
void ObjectGroup::addSubObject(ObjectGroup objIn)
{
    plistObject.push_back(&objIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void ObjectGroup::addSubObject()
{
    plistObject.push_back(new ObjectGroup());
}

//------------------------------------------Function Separator --------------------------------------------------------
void ObjectGroup::addKeySet(string key, string val)
{
    //create vector of input
    vector<string> input(1);
    input[1] = val;

    //Add to the list, using the full vector method
    this->addKeySet(key, input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void ObjectGroup::addKeySet(string key, vector<string> val)
{
    plistKey.push_back(key);
    plistVal.push_back(val);
}

//------------------------------------------Function Separator --------------------------------------------------------
void ObjectGroup::addKeyWord(string word, int index)
{
    if (index < 0)
    {
        //Specify key at specific index.
        plistKey.at(index) = word;
    }
    else
    {
        //Add key to the end of the list.
        plistKey.push_back(word);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void ObjectGroup::addKeyVal(string val, int index)
{
    vector<string> input(1);
    input[0] = val;

    this->addKeyVal(input, index);
}

//------------------------------------------Function Separator --------------------------------------------------------
void ObjectGroup::addKeyVal(vector<string> val, int index)
{
    if (index < 0)
    {
        //Specify value at specific index.
        plistVal.at(index) = val;
    }
    else
    {
        //Add value to the end of the list.
        plistVal.push_back(val);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
string ObjectGroup::getKey(int index)
{
    return plistKey[index];
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<string> ObjectGroup::getVal(int index)
{
    return plistVal[index];
}

//------------------------------------------Function Separator --------------------------------------------------------
void ObjectGroup::setVersion(string input)
{
    pVersion = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
string ObjectGroup::getVersion()
{
    return pVersion;
}

//------------------------------------------Function Separator --------------------------------------------------------
void ObjectGroup::setFormat(string input)
{
    pFormat = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
string ObjectGroup::getFormat()
{
    return pFormat;
}

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions
