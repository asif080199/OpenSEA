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
using namespace osea;
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

    //Set starting value for SeaModel index.
    pSeaModelIndex = -1;
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

    //Delete the list of wave spectra.
    for (unsigned int i = 0; i < plistWaveSpec.size(); i++)
    {
        delete plistWaveSpec[i];
    }
    plistWaveSpec.clear();

    //Delete the list of sea models.
    for (unsigned int i = 0; i < plistSeaModel.size(); i++)
    {
        delete plistSeaModel[i];
    }
    plistSeaModel.clear();
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::setLogFiles(std::string pathIn, bool append)
{
    if (!append)
    {
        //Don't append output files.  Start by deleting them.
        string filename = pathIn + SLASH + "standard.log";
        logStd.deleteLog(filename);

        filename = pathIn + SLASH + "error.log";
        logErr.deleteLog(filename);

        filename = pathIn + SLASH + "monitor.log";
        logMon.deleteLog(filename);
    }

    //Set the standard output directory options.
    logStd.setLogFile(pathIn + SLASH + "standard.log");
    logErr.setLogFile(pathIn + SLASH + "error.log");
    logMon.setLogFile(pathIn + SLASH + "monitor.log");

    //Set default output locations
    logStd.setDefaultOutput("Both");
    logErr.setDefaultOutput("File");
    logMon.setDefaultOutput("File");

    //Set default timestamp options.
    logStd.setDefaultTimeStamp(false);
    logErr.setDefaultTimeStamp(true);
    logMon.setDefaultTimeStamp(false);
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
ForceActive* System::refForceActive_user(string forceName)
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
        return &(plistForceActive_user[i]);
    }
    else
    {
        //return error
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
ForceReact *System::refForceReact_user(string forceName)
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
        return &(plistForceReact_user[i]);
    }
    else
    {
        //return error
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
ForceCross* System::refForceCross_user(string forceName)
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
        return &(plistForceCross_user[i]);
    }
    else
    {
        //return error
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

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<osea::SeaModel *> &System::listSeaModel()
{
    return plistSeaModel;
}

//------------------------------------------Function Separator --------------------------------------------------------
osea::SeaModel* System::listSeaModelPt(int index)
{
    return plistSeaModel.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
osea::SeaModel &System::listSeaModel(int index)
{
    return *(listSeaModelPt(index));
}

//------------------------------------------Function Separator --------------------------------------------------------
osea::SeaModel* System::listSeaModelPt(std::string NameIn)
{
    //Search through and find the right one.
    int i = 0;
    try
    {
        //Check that the list contains entries.
        if (plistSeaModel.size() == 0)
        {
            std::string msg;
            msg = "No sea models defined.";
            throw std::runtime_error(msg);
        }
        for (i = 0; i < plistSeaModel.size(); i++ )
        {
            if(plistSeaModel.at(i)->getName() == NameIn)
                break;
        }

        return plistSeaModel.at(i);
    }
    catch(std::exception &err)
    {
        logStd.Notify();
        logErr.Write(string("Object:  System, Function:  listSeaModel() \n") + err.what());
    }
    catch(...)
    {
        logStd.Notify();
        logErr.Write(string("Object:  System, Function:  listSeaModel() \n Unknown error."));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
osea::SeaModel &System::listSeaModel(std::string NameIn)
{
    return *(listSeaModelPt(NameIn));
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<osea::WaveSpecBase *> &System::listWaveSpec()
{
    return plistWaveSpec;
}

//------------------------------------------Function Separator --------------------------------------------------------
osea::WaveSpecBase* System::listWaveSpecPt(int index)
{
    return plistWaveSpec.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
osea::WaveSpecBase &System::listWaveSpec(int index)
{
    return *(listWaveSpecPt(index));
}

//------------------------------------------Function Separator --------------------------------------------------------
osea::WaveSpecBase* System::listWaveSpecPt(std::string NameIn)
{
    int i = 0;
    try
    {
        if (plistWaveSpec.size() == 0)
        {
            std::string msg;
            msg = "No wave spectra defined.";
            throw std::runtime_error(msg);
        }

        for (i = 0; i < plistWaveSpec.size(); i++)
        {
            if (plistWaveSpec.at(i)->getName() == NameIn)
                break;
        }

        return plistWaveSpec.at(i);
    }
    catch(std::exception &err)
    {
        logStd.Notify();
        logErr.Write(string("Object:  System, Function:  listWaveSpec() \n") + err.what());
    }
    catch(...)
    {
        logStd.Notify();
        logErr.Write(string("Object:  System, Function:  listSeaModel() \n Unknown error."));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
osea::WaveSpecBase &System::listWaveSpec(std::string NameIn)
{
    return *(listWaveSpecPt(NameIn));
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::setActiveSeaModel(std::string NameIn)
{
    pActSeaModelName = NameIn;

    //Match the active sea model
    SearchActiveSeaModel();
}

//------------------------------------------Function Separator --------------------------------------------------------
osea::SeaModel &System::refActiveSeaModel()
{
    return *(plistSeaModel.at(pSeaModelIndex));
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

//------------------------------------------Function Separator --------------------------------------------------------
void System::addSeaModel(osea::SeaModel modelIn)
{
    //Create new sea model and assign it to the heap.
    osea::SeaModel* newModel = new osea::SeaModel(modelIn);

    plistSeaModel.push_back(newModel);

    //Search for matching active sea models.
    SearchActiveSeaModel();
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addSeaModel(osea::SeaModel_DualDirection modelIn)
{
    //Create new sea model and assign it to the heap.
    osea::SeaModel_DualDirection* newModel = new osea::SeaModel_DualDirection(modelIn);

    plistSeaModel.push_back(newModel);

    //Search for matching active sea models.
    SearchActiveSeaModel();
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addSeaModel(osea::SeaModel_SingleDirection modelIn)
{
    //Create new sea model and assign it to the heap.
    osea::SeaModel_SingleDirection* newModel = new osea::SeaModel_SingleDirection(modelIn);

    plistSeaModel.push_back(newModel);

    //Search for matching active sea models.
    SearchActiveSeaModel();
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addSeaModel(osea::SeaModel_LongCrest modelIn)
{
    //Create new sea model and assign it to the heap.
    osea::SeaModel_LongCrest* newModel = new osea::SeaModel_LongCrest(modelIn);

    plistSeaModel.push_back(newModel);

    //Search for matching active sea models.
    SearchActiveSeaModel();
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addSeaModel(osea::SeaModel_Uniform modelIn)
{
    //Create new sea model and assign it to the heap.
    osea::SeaModel_Uniform* newModel = new osea::SeaModel_Uniform(modelIn);

    plistSeaModel.push_back(newModel);

    //Search for matching active sea models.
    SearchActiveSeaModel();
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addSeaModel()
{
    //Create new sea model and assign it to the heap.
    osea::SeaModel* newModel = new osea::SeaModel();

    plistSeaModel.push_back(newModel);

    //Search for matching active sea models.
    SearchActiveSeaModel();
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addWaveSpec(osea::WaveSpecBase specIn)
{
    //Create new wave spectra base and assign it to the heap.
    osea::WaveSpecBase* newSpec = new osea::WaveSpecBase(specIn);

    plistWaveSpec.push_back(newSpec);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addWaveSpec(osea::WaveSpec specIn)
{
    //Create new wave spectra base and assign it to the heap.
    osea::WaveSpec* newSpec = new osea::WaveSpec(specIn);

    plistWaveSpec.push_back(newSpec);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addWaveSpec(osea::SpecBretschneider specIn)
{
    //Create new wave spectra base and assign it to the heap.
    osea::SpecBretschneider* newSpec = new osea::SpecBretschneider(specIn);

    plistWaveSpec.push_back(newSpec);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addWaveSpec(osea::SpecJONSWAP specIn)
{
    //Create new wave spectra base and assign it to the heap.
    osea::SpecJONSWAP* newSpec = new osea::SpecJONSWAP(specIn);

    plistWaveSpec.push_back(newSpec);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addWaveSpec(osea::SpecPM specIn)
{
    //Create new wave spectra base and assign it to the heap.
    osea::SpecPM* newSpec = new osea::SpecPM(specIn);

    plistWaveSpec.push_back(newSpec);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addWaveSpec()
{
    //Create new wave spectra and assign it to the heap.
    osea::WaveSpec* newSpec = new osea::WaveSpec();

    plistWaveSpec.push_back(newSpec);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::SearchActiveSeaModel()
{
    bool foundit = false;       //Boolean to determine if found a match.

    try
    {
//        if (plistSeaModel.size() == 0)
//        {
//            std::string msg;
//            msg = "No sea models defined.";
//            throw std::runtime_error(msg);
//        }
        if (plistSeaModel.size() != 0)
        {
            int i;
            for (i = 0; i < plistSeaModel.size(); i++ )
            {
                if (plistSeaModel.at(i)->getName() == pActSeaModelName)
                {
                    foundit = true;
                    break;
                }
            }

            //Write out the result, if a match was found.
            if (foundit)
                pSeaModelIndex = i;
            else
                pSeaModelIndex = -1;
        }
    }
    catch(std::exception &err)
    {
        logStd.Notify();
        logErr.Write(string("Object:  System, Function:  SearchActiveSeaModel() \n") + err.what());
    }
    catch(...)
    {
        logStd.Notify();
        logErr.Write(string("Object:  System, Function:  SearchActiveSeaModel() \n Unknown error."));
    }

}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<ofreq::HydroManager> &System::listHydroManager()
{
    return plistHydroManager;
}

//------------------------------------------Function Separator --------------------------------------------------------
ofreq::HydroManager &System::listHydroManager(int index)
{
    return plistHydroManager[index];
}

//------------------------------------------Function Separator --------------------------------------------------------
ofreq::HydroManager &System::listHydroManager(std::string bodNameIn)
{
    bool matchFound;        //Checks if a match is found for the body name.
    unsigned int i;

    //Search through list of bodiex.
    for (i = 0; i < plistHydroManager.size(); i++)
    {
        if (bodNameIn.compare(plistHydroManager[i].getHydroBodyName()) == 0)
        {
            matchFound = true;
            break;
        }
    }

    try {
        if (!matchFound)
            throw std::runtime_error("Hydrobody name not found.");

        return plistHydroManager[i];
    }
    catch(std::exception &err)
    {
        logStd.Notify();
        logErr.Write(string("Object:  System, Function:  listHydroManager()  \n") + err.what());
    }
    catch(...)
    {
        logStd.Notify();
        logErr.Write(string("Object:  System, Function:  listHydroManager() \n Unknown error."));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addHydroManager(ofreq::HydroManager HydroIn)
{
    plistHydroManager.push_back(HydroIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addHydroManager()
{
    plistHydroManager.push_back(ofreq::HydroManager());
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


