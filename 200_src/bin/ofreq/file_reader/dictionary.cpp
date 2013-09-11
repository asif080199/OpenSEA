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

#include "dictionary.h"

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
Dictionary::Dictionary(QObject *parent) :
    QObject(parent)
{
}

//------------------------------------------Function Separator --------------------------------------------------------
Dictionary::Dictionary()
{

}

//==========================================Section Separator =========================================================
//Signal Functions


//==========================================Section Separator =========================================================
//Public Slots
//------------------------------------------Function Separator --------------------------------------------------------
virtual void Dictionary::setObject(ObjectGroup input)
{
    //Define the object based on its name
    defineClass(input.getClassName());

    //Proceed through each key / value pair defined for the ObjectGroup object.
    for (unsigned int i = 0; i <= input.refListKey().size(); i++)
    {
        //define the key
        defineKey(input.getKey(i), input.getVal(i));
    }

    //Next proceed through each of the sub classes defined for the ObjectGroup object.
    for (unsigned int i = 0; i <= input.refListObject().size(); i++)
    {
        //define the class
        setObject(input.refListObject()[i]);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
virtual void Dictionary::setSystem(System* ptInput)
{
    ptSystem = ptInput;
}

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

