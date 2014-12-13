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

//==========================================Section Separator =========================================================
//Inclusions and namespaces
#include "data.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
Data::Data()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
Data::Data(int IndexIn)
{
    setIndex(IndexIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
int Data::getIndex()
{
    return pIndex;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Data::setIndex(int indexIn)
{
    pIndex = indexIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<std::complex<double> > &Data::listValue()
{
    return plistValue;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> &Data::listValue(int indexIn)
{
    try
    {
        return plistValue.at(indexIn);
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int Data::listValueInt(int indexIn)
{
    return int(listValueDouble(indexIn));
}

//------------------------------------------Function Separator --------------------------------------------------------
double Data::listValueDouble(int indexIn)
{
    try
    {
        return plistValue.at(indexIn).real();
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void Data::clearValue()
{
    plistValue.clear();
}

//------------------------------------------Function Separator --------------------------------------------------------
void Data::addValue(std::complex<double> valIn)
{
    plistValue.push_back(valIn);
    pDataType = 0;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Data::addValue(double valIn)
{
    std::complex<double> temp;
    temp.real(valIn);
    plistValue.push_back(temp);

    pDataType = 1;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Data::addValue(int valIn)
{
    std::complex<double> temp;
    temp.real( double(valIn) );
    plistValue.push_back(temp);

    pDataType = 2;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Data::addString(std::string valIn)
{
    plistString.push_back(valIn);

    pDataType = 3;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<std::string> &Data::listString()
{
    return plistString;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string &Data::listString(int indexIn)
{
    try
    {
        return plistString.at(indexIn);
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int Data::getDataType()
{
    return pDataType;
}

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions
