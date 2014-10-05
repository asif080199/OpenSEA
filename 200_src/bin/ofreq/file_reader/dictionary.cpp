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
#include "filereader.h"

using namespace std;
using namespace osea;

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

//------------------------------------------Function Separator --------------------------------------------------------
Dictionary::Dictionary(osea::FileReader *parent)
{
    //set pointer object
    pParent = parent;
}

//==========================================Section Separator =========================================================
//Signal Functions


//==========================================Section Separator =========================================================
//Public Slots
//------------------------------------------Function Separator --------------------------------------------------------
void Dictionary::setObject(ObjectGroup input)
{
    //Define the object based on its name
    defineClass(input.getClassName());

    //Create variable to catch return value
    int returnVal;

    //Proceed through each key / value pair defined for the ObjectGroup object.
    for (unsigned int i = 0; i < input.listKey().size(); i++)
    {
        //define the key
        try {
            returnVal = defineKey(input.getKey(i), input.getVal(i));

            if (returnVal != 0)
                throw std::runtime_error("Error reading in key and value pair.\n" +
                                         string("Key:  ") + string(input.getKey(i)) + string("\nValue:  ") +
                                         string(input.getVal(i)[0]));
        }
        catch(const std::exception &err)
        {
            //Write out error message and stop execution.
            logErr.Write(string(err.what()));
            logStd.Notify();
            exit(1);
        }
        catch(...)
        {
            //Write out error message and stop execution.
            logErr.Write(string(ID) + string(">>  Unknown error occurred."));
            logStd.Notify();
            exit(1);
        }
    }

    //Next proceed through each of the sub classes defined for the ObjectGroup object.
    for (unsigned int i = 0; i < input.listObject().size(); i++)
    {
        //define the class
        try {
            setObject(*(input.listObject(i)));
        }
        catch(const std::exception &err)
        {
            //Write out error message and stop execution.
            logErr.Write("Error reading in object definition.\n" +
                         string("Object Name:  ") + string(input.listObject(i)->getClassName()) +
                         string("\nError Code:  ") + string(err.what()));
            logStd.Notify();
            exit(1);
        }
        catch(...)
        {
            //Write out error message and stop execution.
            logErr.Write("Error reading in object definition.\n" +
                         string("Object Name:  ") + string(input.listObject(i)->getClassName()));
            logStd.Notify();
            exit(1);
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void Dictionary::setSystem(ofreq::System* ptInput)
{
    ptSystem = ptInput;
}

//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
int Dictionary::defineKey(string keyIn, vector<string> valIn)
{
    //Simple definition of function to keep the compiler happy.
    //This will be overriden by children classes.
    return 99;
}

//------------------------------------------Function Separator --------------------------------------------------------
int Dictionary::defineClass(string nameIn)
{
    //Simple definition of function to keep the compiler happy.
    //This will be overriden by children classes.
    return 99;
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> Dictionary::convertComplex(string input)
{
    complex<double> output;

    const string BRACKET = "<";
    const string IMAGINARY = "i";
    const string PLUS = "+";
    const string MINUS = "-";
    const string COMMA = ",";

    //First check if defined by amplitude and angle
    int indexPolar;
    indexPolar = input.find(BRACKET);

    //Next check if defined by comma.
    int indexCoord;
    indexCoord = input.find(COMMA);

    //Next check if defined by rectangular notation.
    int indImag;
    indImag = input.find(IMAGINARY,1);

    try
    {

        if (indexPolar != std::string::npos)
        {
            //using bracket.  Process as polar notation.
            double amp;
            double phase;

            //Get amplitude
            amp = atof(input.substr(0, indexPolar).c_str());
            //Get phase
            phase = atof(input.substr(indexPolar + 1).c_str());

            //Convert to real and imaginary parts.
            output.real(amp * cos(phase));
            output.imag(amp * sin(phase));
        }

        else if (indexCoord != std::string::npos)
        {
            //Using comma.  Process as coordinate system notation.

            //Process real part.
            output.real(
                        atof(input.substr(0,indexCoord).c_str())
                        );

            //Process imaginary part.
            output.imag(
                        atof(input.substr(indexCoord + 1).c_str())
                        );
        }

        else if (indImag != std::string::npos)
        {
            //using rectangular notation
            int indPlus;
            int indMinus;


            //Get positions of plus, minus, and imaginary.
            //Ignore the first character to avoid picking up any plus/minus indicators for real component.
            indPlus = input.find(PLUS,1);
            indMinus = input.find(MINUS,1);

            //Separate string by position of plus and minus signs.
            if (indPlus == std::string::npos)
            {
                //No plus involved.  Minus sign.
                //Check if imaginary symbol is at middle or end of input.
                if (indImag == input.length() - 1)
                {
                    //End of input
                    //Assign real part.
                    output.real(
                                atof(input.substr(0, indMinus + 1).c_str())
                                );
                    //Assign imaginary part
                    output.imag(
                                -1.0 * atof(input.substr(indMinus + 1, indImag - indPlus).c_str())
                                );
                }
                else
                {
                    //Middle of input
                    //Assign real part.
                    output.real(
                                atof(input.substr(0, indMinus).c_str())
                                );
                    //Assign imaginary part
                    output.imag(
                                -1.0 * atof(input.substr(indImag + 1, input.length() - 1 - indImag).c_str())
                                );
                }
            }
            else
            {
                //Plus sign used.
                //Check if imaginary symbol is at middle or end of input.
                if (indImag == input.length() - 1)
                {
                    //End of input.
                    //Assign real part.
                    output.real(
                                atof(input.substr(0, indPlus).c_str())
                                );
                    //Assign imaginary part
                    output.imag(
                                atof(input.substr(indPlus + 1, indImag - indPlus).c_str())
                                );
                }
                else
                {
                    //Middle of input.
                    //Assign real part.
                    output.real(
                                atof(input.substr(0,indPlus).c_str())
                                );
                    //Assign imaginary part
                    output.imag(
                                atof(input.substr(indImag + 1, input.length() - 1 - indImag).c_str())
                                );
                }
            }
        }

        else
            throw std::runtime_error(string("Complex variable format not recognized.  ") +
                                     string("Offending Entry:  ") + input);
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  ") + err.what());
        exit(1);
    }
    catch(...)
    {
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  Unknown error occurred."));
        exit(1);
    }

    //Return output
    return output;
}


//==========================================Section Separator =========================================================
//Private Functions

