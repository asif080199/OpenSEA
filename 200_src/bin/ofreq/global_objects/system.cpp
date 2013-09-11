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

#include "system.h"

//==========================================Section Separator =========================================================
//Public methods

//------------------------------------------Function Separator --------------------------------------------------------
System::System() : pWaveDirections(), pWaveFrequencies()
{
	/*waveDirections = new WaveDirections();
	waveFrequencies = new WaveFrequencies();*/
}

//------------------------------------------Function Separator --------------------------------------------------------
System::~System()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::testPrint()
{
	cout << "Analysis Type: " << analysisType << endl;
	pWaveDirections.testPrint();
	pWaveFrequencies.testPrint();
	cout << endl;
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::setWaveFrequencies(vector<double> vecIn)
{
    pWaveFrequencies = vecIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::setWaveDirections(vector<double> vecIn)
{
    pWaveDirections = vecIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::setSpreadModel(string spreadIn)
{
	pWaveDirections.setSpreadModel(spreadIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> System::getWaveFrequencies()
{
    return pWaveFrequencies;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &System::refWaveFrequencies()
{
    return pWaveFrequencies;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> System::getWaveDirections()
{
    return pWaveDirections;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &System::refWaveDirections()
{
    return pWaveDirections;
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::setCurWaveDirInd(int input)
{
    pCurWaveDir = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::setCurFreqInd(int input)
{
    pCurWaveFreq = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
int System::getCurWaveDirInd()
{
    return pCurWaveDir;
}

//------------------------------------------Function Separator --------------------------------------------------------
int System::getCurFreqInd()
{
    return pCurWaveFreq;
}

//------------------------------------------Function Separator --------------------------------------------------------
double System::getCurWaveDir()
{
    return pWaveDirections[pCurWaveDir];
}

//------------------------------------------Function Separator --------------------------------------------------------
double System::getCurFreq()
{
    return pWaveFrequencies[pCurWaveFreq];
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<Body>& System::reflistBody()
{
    return plistBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<OutputsBody>& System::reflistOutputs()
{
    return plistOutputs;
}

//------------------------------------------Function Separator --------------------------------------------------------
Body& System::refBody(int input = 0)
{
    return plistBody.at(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
OutputsBody& System::refOutput(int input = 0)
{
    return plistOutputs.at(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::clearForce(string forceClass)
{
    //Clears the vector of force objects.
    switch (forceClass)
    {
    case valForceActive_user:
        //clear forceActive objects class, under scope of user
        plistForceActive_user.clear();
        break;
    case valForceReact_user:
        //Clear forceReact objects class, under scope of user
        plistForceReact_user.clear();
        break;
    case valForceCross_user:
        //Clear forceCross objects class, under scope of user
        plistForceCross_user.clear();
        break;
    case "":
        //Clear all three object types.
        plistForceActive_user.clear();
        plistForceReact_user.clear();
        plistForceCross_user.clear();
        break;
    default:
        //Case for any unknown inputs.
        //do nothing.
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
forceActive System::getforceActive_user(string forceName)
{
    //returns forceActive_user specified by forcename
    int i = 0;      //counting variable
    while (i <= plistForceActive_user.size() - 1)
    {
        //Check if forcename matches specified force.
        if (plistForceActive_user[i].getForceName() == forceName)
            break;

        i += 1;
    }

    //Return value
    if (i <= (plistForceActive_user.size() - 1))
    {
        //return specified object
        return plistForceActive_user[i];
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<forceActive> &System::listforceActive_user()
{
    //return vector
    return plistForceActive_user;
}

//------------------------------------------Function Separator --------------------------------------------------------
forceReact System::getforceReact_user(string forceName)
{
    //returns forceReact_user specified by forcename
    int i = 0;      //counting variable
    while (i <= plistForceReact_user.size() - 1)
    {
        //Check if forcename matches specified force.
        if (plistForceReact_user[i].getForceName() == forceName)
            break;

        i += 1;
    }

    //Return value
    if (i <= (plistForceReact_user.size() - 1))
    {
        //return specified object
        return plistForceReact_user[i];
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<forceReact> &System::listforceReact_user()
{
    return plistForceReact_user;
}

//------------------------------------------Function Separator --------------------------------------------------------
forceActive System::getforceCross_user(string forceName)
{
    //returns ForceCross_user specified by forcename
    int i = 0;      //counting variable
    while (i <= plistForceCross_user.size() - 1)
    {
        //Check if forcename matches specified force.
        if (plistForceCross_user[i].getForceName() == forceName)
            break;

        i += 1;
    }

    //Return value
    if (i <= (plistForceCross_user.size() - 1))
    {
        //return specified object
        return plistForceCross_user[i];
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<forceActive> &System::listforceCross_user()
{
    return plistForceCross_user;
}

//==========================================Section Separator =========================================================
//Public Slots

//------------------------------------------Function Separator --------------------------------------------------------
void System::setAnalysisType(string analysisTypeIn)
{
    analysisType = analysisTypeIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addBody(Body input)
{
    plistBody.push_back(input);

    //Return the system object.
    emit ReferenceSystem(this);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addBody()
{
    plistBody.push_back(Body());

    //Return the system object.
    emit ReferenceSystem(this);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addOutput(OutputsBody input)
{
    plistOutputs.push_back(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addOutput()
{
    plistOutputs.push_back(OutputsBody());
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addforceActive_user(forceActive input)
{
    plistForceActive_user.push_back(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addforceActive_user()
{
    plistForceActive_user.push_back(forceActive());
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addforceReact_user(forceReact input)
{
    plistForceReact_user.push_back(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addforceReact_user()
{
    plistForceReact_user.push_back(forceReact());
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addforceCross_user(forceCross input)
{
    plistForceCross_user.push_back(forceCross());
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addforceCross_user()
{
    plistForceCross_user.push_back(forceReact());
}

//==========================================Section Separator =========================================================
//Signals

//==========================================Section Separator =========================================================
//Protected methods

//==========================================Section Separator =========================================================
//Private methods
