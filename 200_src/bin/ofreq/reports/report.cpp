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
#include "report.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
Report::Report(ofreq::ReportManager *ptIn)
{
    setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
Report::Report()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
Report::~Report()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::setSystem(ofreq::System *ptIn)
{
    ptSystem = ptIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
System *Report::getSystem()
{
    return ptSystem;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::setBody(ofreq::Body *ptIn)
{
    ptBody = ptIn;

    //Search through the list of Body objects in the System object to try and find the Body name
    try
    {
        for (unsigned int i = 0; i < ptSystem->listBody().size(); i++)
        {
            if (ptBody->getBodyName().compare(ptSystem->listBody(i).getBodyName()) == 0)
            {
                pBodIndex = i;
                break;
            }
        }
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
Body* Report::getBody()
{
    return ptBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
int Report::getBodIndex()
{
    return pBodIndex;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::setManager(ofreq::ReportManager *ptIn)
{
    ptManager = ptIn;

    //Set the system oject as well.
    this->setSystem(
                ptManager->getSystem()
                );
}

//------------------------------------------Function Separator --------------------------------------------------------
ReportManager* Report::getManager()
{
    return ptManager;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::setName(std::string nameIn)
{
    pName = nameIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string Report::getName()
{
    return pName;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string Report::getClass()
{
    const std::string &rawID = __PRETTY_FUNCTION__;

    size_t colons = rawID.find("::");
    if (colons == std::string::npos)
    {
        return "::";
    }

    size_t begin = rawID.substr(0,colons).rfind(" ") + 1;
    size_t end = colons - begin;
    return rawID.substr(begin, end);

    //For the debug version, print out the name.
    logStd.Write(string("Class Name:  ") + rawID.substr(begin, end),
                 3);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::calcReport(int freqInd)
{
    //Just a default function to keep the compiler happy until child functions insert real code.
}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::clearData()
{
    plistData.clear();
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector< Data > &Report::listData()
{
    return plistData;
}

//------------------------------------------Function Separator --------------------------------------------------------
Data &Report::listData(int indexIn)
{
    return plistData.at(indexIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
int Report::getDataType()
{
    return pDataType;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::setCurWaveInd(int indIn)
{
    curWaveInd = indIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::addConst(std::string keyIn, std::string valIn)
{
    plistConst_Key.push_back(keyIn);

    //Create new data value and add it to the list.
    Data tempData;

    tempData.addString(valIn);

    plistConst_Val.push_back(tempData);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::addConst(std::string keyIn, int valIn)
{
    plistConst_Key.push_back(keyIn);

    //Create new data value and add it to the list.
    Data tempData;
    tempData.addValue(valIn);

    plistConst_Val.push_back(tempData);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::addConst(std::string keyIn, double valIn)
{
    plistConst_Key.push_back(keyIn);

    //Create new data value and add it to the list.
    Data tempData;
    tempData.addValue(valIn);

    plistConst_Val.push_back(tempData);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::addConst(std::string keyIn, std::complex<double> valIn)
{
    plistConst_Key.push_back(keyIn);

    //Create new data value and add it to the list.
    Data tempData;
    tempData.addValue(valIn);

    plistConst_Val.push_back(tempData);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::addConst(std::string keyIn, vector<double> valIn)
{
    plistConst_Key.push_back(keyIn);

    //Create new data value and add it to the list.
    Data tempData;
    for (unsigned int i = 0; i < valIn.size(); i++)
    {
        tempData.addValue(valIn.at(i));
    }
    plistConst_Val.push_back(tempData);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::addConst(std::string keyIn, vector<int> valIn)
{
    plistConst_Key.push_back(keyIn);

    //Create new data value and add it to the list.
    Data tempData;
    for (unsigned int i = 0; i < valIn.size(); i++)
    {
        tempData.addValue(valIn.at(i));
    }
    plistConst_Val.push_back(tempData);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Report::addConst(std::string keyIn, vector< std::complex<double> > valIn)
{
    plistConst_Key.push_back(keyIn);

    //Create new data value and add it to the list.
    Data tempData;
    for (unsigned int i = 0; i < valIn.size(); i++)
    {
        tempData.addValue(valIn.at(i));
    }
    plistConst_Val.push_back(tempData);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<std::string> &Report::listConstKey()
{
    return plistConst_Key;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string &Report::listConstKey(int indexIn)
{
    try
    {
        return plistConst_Key.at(indexIn);
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<Data> &Report::listConstVal()
{
    return plistConst_Val;
}

//------------------------------------------Function Separator --------------------------------------------------------
Data &Report::listConstVal(int indexIn)
{
    try
    {
        return plistConst_Val.at(indexIn);
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}


//==========================================Section Separator =========================================================
//Protected Functions

//==========================================Section Separator =========================================================
//Private Functions
