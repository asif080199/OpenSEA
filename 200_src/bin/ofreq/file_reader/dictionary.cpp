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

    //Create variable to catch return value
    int returnVal;

    //Proceed through each key / value pair defined for the ObjectGroup object.
    for (unsigned int i = 0; i <= input.listKey().size(); i++)
    {
        //define the key
        returnVal = defineKey(input.getKey(i), input.getVal(i));
        //Need to create error handler later to handle what happens if returnVal not 0
    }

    //Next proceed through each of the sub classes defined for the ObjectGroup object.
    for (unsigned int i = 0; i <= input.listObject().size(); i++)
    {
        //define the class
        returnVal = setObject(input.listObject()[i]);
        //Need to create error handler later to handle what happens if returnVal not 0
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
virtual void Dictionary::setSystem(System* ptInput)
{
    ptSystem = ptInput;
}

//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> Dictionary::convertComplex(string input)
{
    complex<double> output;

    const string BRACKET = "<";
    const string IMAGINARY = "i";
    const string PLUS = "+";
    const string MINUS = "-";

    //First check if defined by amplitude and angle
    int indexSep;
    indexSep = input.find(BRACKET);
    if (indexSep == std::string::npos)
    {
        //Not using bracket
        int indPlus;
        int indMinus;
        int indImag;

        //Get positions of plus, minus, and imaginary.
        //Ignore the first character to avoid picking up any plus/minus indicators for real component.
        indImag = input.find(IMAGINARY,1);
        indPlus = input.find(PLUS,1);
        indMinus = input.find(MINUS,1);

        //Separate string by position of plus and minus signs.
        if (indPlus == std::string::npos)
        {
            //No plus involved.  Minus sign.
            //Check if imaginary symbol is at middel or end of input.
            if (indImag == input.end())
            {
                //End of input
                //Assign real part.
                output.real() = atof(input.substr(0, indMinus).c_str());
                //Assign imaginary part
                output.imag() = -1.0 * atof(input.substr(indMinus + 1, indImag - indPlus).c_str());
            }
            else
            {
                //Middle of input
                //Assign real part.
                output.real() = atof(input.substr(0, indMinus).c_str());
                //Assign imaginary part
                output.imag() = -1.0 * atof(input.substr(indImag + 1, input.end() - indImag).c_str());
            }
        }
        else
        {
            //Plus sign used.
            //Check if imaginary symbol is at middle or end of input.
            if (indImag == input.end())
            {
                //End of input.
                //Assign real part.
                output.real() = atof(input.substr(0, indPlus).c_str());
                //Assign imaginary part
                output.imag() = atof(input.substr(indPlus + 1, indImag - indPlus).c_str());
            }
            else
            {
                //Middle of input.
                //Assign real part.
                output.real() = atof(input.substr(0,indPlus).c_str());
                //Assign imaginary part
                output.imag() = atof(input.substr(indImag + 1, input.end() - indImag).c_str());
            }
        }
    }
    else
    {
        //using bracket.  Process as polar notation.
        double amp;
        double phase;

        //Get amplitude
        amp = atof(input.substr(0, indexSep).c_str());
        //Get phase
        phase = atof(input.substr(indexSep + 1).c_str());

        //Convert to real and imaginary parts.
        output.real() = amp * cos(phase);
        output.imag() = amp * sin(phase);
    }

    //Return output
    return output;
}


//==========================================Section Separator =========================================================
//Private Functions

