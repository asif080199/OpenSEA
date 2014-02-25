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

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static Initialization
string System::valForceActive_user = "forceActive_user";
string System::valForceReact_user = "forceReact_user";
string System::valForceCross_user = "forceCross_user";

//==========================================Section Separator =========================================================
//Public methods

//------------------------------------------Function Separator --------------------------------------------------------
System::System() : pWaveDirections(), pWaveFrequencies()
{   
    //Create the list of motion models
    DefineModels();
}

//------------------------------------------Function Separator --------------------------------------------------------
System::~System()
{
    //Delete the list of motion models
    for (unsigned int i = 0; i < plistModels.size(); i++)
    {
        delete plistModels[i];
    }
    plistModels.clear();
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

}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> System::getWaveFrequencies()
{
    return pWaveFrequencies;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &System::listWaveFrequencies()
{
    return pWaveFrequencies;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> System::getWaveDirections()
{
    return pWaveDirections;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &System::listWaveDirections()
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
vector<Body>& System::listBody()
{
    return plistBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
Body &System::listBody(int input)
{
    return plistBody.at(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<OutputsBody>& System::listOutput()
{
    return plistOutputs;
}

//------------------------------------------Function Separator --------------------------------------------------------
OutputsBody &System::listOutput(int input)
{
    return plistOutputs.at(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::clearForce(string forceClass)
{
    //Clears the vector of force objects.
    if (forceClass == valForceActive_user)
    {
        //clear ForceActive objects class, under scope of user
        plistForceActive_user.clear();
    }
    else if (forceClass == valForceReact_user)
    {
        //Clear ForceReact objects class, under scope of user
        plistForceReact_user.clear();
    }
    else if (forceClass == valForceCross_user)
    {
        //Clear ForceCross objects class, under scope of user
        plistForceCross_user.clear();
    }
    else if (forceClass == "")
    {
        //Clear all three object types.
        plistForceActive_user.clear();
        plistForceReact_user.clear();
        plistForceCross_user.clear();
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceActive System::getForceActive_user(string forceName)
{
    //returns forceActive_user specified by forcename
    unsigned int i = 0;      //counting variable
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
    else
    {
        //return error
        return ForceActive();
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<ForceActive> &System::listForceActive_user()
{
    //return vector
    return plistForceActive_user;
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceActive &System::listForceActive_user(unsigned int forceIndex)
{
    //Check if index is out of bounds.  If so, resize vector.
    if ((forceIndex > plistForceActive_user.size() - 1) || (plistForceActive_user.size() == 0))
    {
        plistForceActive_user.resize(forceIndex + 1);
        plistForceActive_user[forceIndex].setSystemIndex(forceIndex);
    }

    //Return object.
    return plistForceActive_user[forceIndex];
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceReact System::getforceReact_user(string forceName)
{
    //returns forceReact_user specified by forcename
    unsigned int i = 0;      //counting variable
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
    else
    {
        //return error
        return ForceReact();
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<ForceReact> &System::listForceReact_user()
{
    return plistForceReact_user;
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceReact &System::listForceReact_user(unsigned int forceIndex)
{
    //Check if index is out of bounds.  If so, resize vector.
    if ((forceIndex > plistForceReact_user.size() - 1) || (plistForceReact_user.size() == 0))
    {
        plistForceReact_user.resize(forceIndex + 1);
        plistForceReact_user[forceIndex].setSystemIndex(forceIndex);
    }

    return plistForceReact_user[forceIndex];
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceCross System::getforceCross_user(string forceName)
{
    //returns ForceCross_user specified by forcename
    unsigned int i = 0;      //counting variable
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
    else
    {
        //return error
        return ForceCross();
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<ForceCross> &System::listForceCross_user()
{
    return plistForceCross_user;
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceCross &System::listForceCross_user(unsigned int forceIndex)
{
    //Check if index is out of bounds.  If so, resize vector.
    if ((forceIndex > plistForceCross_user.size() - 1) || (plistForceCross_user.size() == 0))
    {
        plistForceCross_user.resize(forceIndex + 1);
        plistForceCross_user[forceIndex].setSystemIndex(forceIndex);
    }

    return plistForceCross_user[forceIndex];
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
void System::addForceActive_user(ForceActive input)
{
    plistForceActive_user.push_back(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addForceActive_user()
{
    plistForceActive_user.push_back(ForceActive());
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addForceReact_user(ForceReact input)
{
    plistForceReact_user.push_back(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addForceReact_user()
{
    plistForceReact_user.push_back(ForceReact());
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addForceCross_user(ForceCross input)
{
    plistForceCross_user.push_back(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addForceCross_user()
{
    plistForceCross_user.push_back(ForceCross());
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::linkBodies(int bodID)
{
    //Iterate through all user linked bodies
    for (unsigned int i = 0; i < plistBody[bodID].listNamedLink_user().size(); i++)
    {
        //Search for body with correct name
        string name = plistBody[bodID].listNamedLink_user(i);   //Name of the body to search for.

        for (unsigned int j = 0; j < listBody().size(); j++)
        {
            if (name == listBody(j).refBodyName())
            {
                //Assign the pointer for the body
                plistBody[bodID].listCrossBody_user(i) = listBody(j);
                //Quit the loop
                break;
            }
        }
    }
    //Clear the list of named links.  Don't need it anymore.
    plistBody[bodID].listNamedLink_user().clear();

    //Iterate through all hydro linked bodies
    for (unsigned int i = 0; i < plistBody[bodID].listNamedLink_hydro().size(); i++)
    {
        //Search for body with correct name
        string name = plistBody[bodID].listNamedLink_hydro(i);   //Name of the body to search for.

        for (unsigned int j = 0; j < listBody().size(); j++)
        {
            if (name == listBody(j).refBodyName())
            {
                //Assign the pointer for the body
                plistBody[bodID].listCrossBody_hydro(i) = listBody(j);
                //Quit the loop
                break;
            }
        }
    }
    //Clear the list of named links.  Don't need it anymore.
    plistBody[bodID].listNamedLink_hydro().clear();
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<MotionModel *> &System::listModel()
{
    return plistModels;
}

//------------------------------------------Function Separator --------------------------------------------------------
MotionModel &System::listModel(unsigned int index)
{
    return *(plistModels[index]);
}

//------------------------------------------Function Separator --------------------------------------------------------
MotionModel &System::listModel(std::string modelName)
{
    for (unsigned int i = 0; i < plistModels.size(); i++)
    {
        if (modelName.compare(plistModels[i]->getName()) == 0)    //Equals zero when string are identical.
        {
            return *(plistModels[i]);
            break;
        }

    }
}

//==========================================Section Separator =========================================================
//Signals

//==========================================Section Separator =========================================================
//Protected methods

//==========================================Section Separator =========================================================
//Private methods

//------------------------------------------Function Separator --------------------------------------------------------
void System::DefineModels()
{
    //Create each motion model and add it to the list.
    //Don't need to set any properties for the models.  All those get set at object creation.

    plistModels.push_back(new Model6DOF());
}
