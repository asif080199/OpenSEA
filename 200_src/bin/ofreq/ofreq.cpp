/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date              Author				Description
 *---------------------------------------------------------------------------------------------------------------------
 *May 15, 2013      Shane Honanie       Initially created.
 *Aug 03, 2013      Nicholas Barczak    Added comments and changed behavior of motion solver.
 *                                      Motion model now called in the main ofreq function.  Output passed to the
 *                                      motion solver.
 *
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

//######################################### Include Statements ########################################################
#include <QCoreApplication>
#include "./motion_solver/motionsolver.h"
#include "./motion_model/motionmodel.h"
#include "./motion_solver/matbody.h"
#include "./motion_solver/matforceactive.h"
#include "./motion_solver/matforcecross.h"
#include "./motion_solver/matforcereact.h"
#include "./file_writer/filewriter.h"
#include "./derived_outputs/outputsbody.h"
#include "./global_objects/solutionset.h"
#include "./global_objects/solution.h"
#include "./global_objects/system.h"
#include "./file_reader/dictcontrol.h"
#include "./file_reader/dictforces.h"
#include "./file_reader/dictbodies.h"
#include "./file_reader/filereader.h"
#include "./global_objects/ofreqcore.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <QtGlobal>
#ifdef Q_OS_WIN
    #include <direct.h>
    #define GetCurrentDir _getcwd
#elif defined Q_OS_LINUX
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

//Motion Models
//---------------------------------------
#include "./motion_model/model6dof.h"

//########################################## Global Variables #########################################################
//Namespaces
using namespace osea;
using namespace osea::ofreq;
using namespace std;

//########################################## Global Variables #########################################################

//Create matrix bodies.
vector<matBody> listMatBody;

//Vector containing various motion models available
vector<MotionModel> listModel;

//List of solutions from the motion model.  Each solution object is the list of solutions for one body.
vector<SolutionSet> listSolutions;

//System object.  Used to control entire execution.
System sysofreq;

//######################################### Function Prototypes #######################################################

//------------------------------------------Function Separator ----------------------------------------------------
/**
 * @brief Builds a matrix body object for the body specified by the integer.  Uses the motion model identified by the
 * Body object.
 * @param bod Which body to use for building the matix body.
 * @return Returns a matBody object, fully provisioned with all necessary data.
 */
void buildMatBody(int bod, bool useCoeff=true);

//------------------------------------------Function Separator ----------------------------------------------------
/**
 * @brief Calculates derived outputs using the OutputsBody object and then writes those outputs to files.
 * @param OutputIn The OutputsBody object that will calculate the derived outputs.  All properties for the OutputsBody
 * object must be set by the time the function is called.  Variable passed by reference.
 * @param WriterIn The FileWriter object that will receive the outputs from the OutputsBody object and write those
 * outputs to a file.
 */
void calcOutput(OutputsBody &OutputIn, FileWriter &WriterIn);

//------------------------------------------Function Separator ----------------------------------------------------
/**
 * @brief Reads in all the input files.  Creates the necessary objects for file reading.  And connects those objects
 * using Qt slots and signals.  Finally proceeds through each file and reads it.  All parsing is accomplished by
 * the FileReader object.  File interpretation is processed through the Dictionary objects.
 * @param runPath String.  The path to the root directory of the input files.
 * @sa Dictionary
 */
void ReadFiles(string runPath);

//------------------------------------------Function Separator ----------------------------------------------------
/**
 * @brief Defines all motion models.
 *
 * Adds all defined motion models to the list of available motion models.  Each
 * class of motion model has only one object defined in the list.
 */
void DefineModels();

//############################################ Class Prototypes #######################################################


//##################################### Constant Variables Declaration ################################################
#ifdef Q_OS_WIN
    const string seperator = "\\";  /**< Directory separator in a string path., windows version**/
#elif defined Q_OS_LINUX
    const string seperator = "/";   /**< Directory separator in a string path., linux version**/
#endif
string oFreq_Directory = "";

//########################################### Main Function ###########################################################
/**
 * @brief The main function that runs ofreq program.
 *
 * The main function that runs ofreq program.  It proceeds through in several stages.  This briefly outlines them.
 * 0.  Initialize a few variables.
 * 1.  Read in input files.
 * 2.  Interpret / parse input files and use them to build the program objects.
 * 3.  Use the motion model to convert input objects into matrix force objects.  The particular motion model used
 *     changes with each Body object.
 * 4.  Setup the motion solver.  Feed all the data in.
 * 5.  Set the operating frequency and use the motion solver to solve equations of motion.
 * 6.  Store results in Solution object.
 * 7.  Repeat steps 4 through 6 for each wave direction and wave frequency.
 * 8.  Use the results to calculate derived outputs.
 * 9.  Write the calculated outputs to output files.
 */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ofreq::oFreqCore freqCore;

//    //Create a header for output to the screen
//    oFreqCore::OutLog << string(100,'\n');
//    oFreqCore::OutLog << "==========================================================\n";
//    oFreqCore::OutLog << "OpenSEA:  oFreq";
//    oFreqCore::OutLog << "==========================================================\n";
//    oFreqCore::OutLog << string(5,'\n');

	//if command line arg supplied, use that directory
	//or assume the current working directory
    if (argc == 2)
		oFreq_Directory = argv[1];
    else if (argc == 1)
    {
        //Get current working directory
        char cCurrentPath[FILENAME_MAX];
        if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
        {
            //Return error
        }
        //Set the path to the current working directory
        oFreq_Directory = cCurrentPath;
    }

    //Generate motion models
    //---------------------------------------------------------------------------
    DefineModels();

    //Read input files and interpret data.
    //---------------------------------------------------------------------------
    ReadFiles(oFreq_Directory);

    //Start creating main objects
    //---------------------------------------------------------------------------
    //Resize matrix bodies vector.
    listMatBody.resize(sysofreq.listBody().size());
    //Resize the solution object.
    for (unsigned int i = 0; i < sysofreq.listBody().size(); i++)
    {
        listSolutions.push_back(SolutionSet(sysofreq.listWaveDirections().size(),
                                             sysofreq.listWaveFrequencies().size()));
    }

    //Iterate through each wave direction and wave frequency to solve
    //---------------------------------------------------------------------------
    for(unsigned int i = 0; i < sysofreq.listWaveDirections().size(); i++)
	{
        //Set the current wave direction
        sysofreq.setCurWaveDirInd(i);

        for(unsigned int j = 0; j < sysofreq.listWaveFrequencies().size(); j++)
		{
            //Set the current wave frequency
            sysofreq.setCurFreqInd(j);

            //Build the matrix bodies
            //---------------------------------------------------------------------------
            //Build operation stuck inside the wave iteration loop because the forces may change with
            //wave direction.  Also inside the wave frequency loop because the forces may change
            //with wave frequency.
            bool coeffonly = true;              //Boolean to tell the motion model to only use coefficients.
            //Iterate through each body and build.
            for (unsigned int i2 = 0; i2 < sysofreq.listBody().size(); i2++)
            {
                //Build the body.
                //Will automatically use the correct bodies.
                buildMatBody(i2, coeffonly);
            }

            //Create motion solver and feed in the body data.
            MotionSolver theMotionSolver(listMatBody);
            //Set the current wave frequency
            theMotionSolver.setWaveFreq(sysofreq.getCurFreq());
            //Solve the system of equations.
            theMotionSolver.calculateOutputs();

			//asign each solution per frequency to a body
            for(unsigned int k = 0; k < sysofreq.listBody().size(); k++)
			{
                Solution soln;
                soln.refBody() = sysofreq.listBody(k);
                soln.setSolnMat(theMotionSolver.listSolution(k));
                listSolutions[k].setSolnMat(i, j, soln);
			}
		}
    }

    //Write outputs
    //---------------------------------------------------------------------------
    //Iterate through each body output.  Setup the output and calculate outputs.
    //Resize list of outputs
    if (sysofreq.listOutput().size() != sysofreq.listBody().size())
        sysofreq.listOutput().resize(sysofreq.listBody().size());

    for (unsigned int i = 0; i < sysofreq.listOutput().size(); i++)
    {
        //Setup variables for each OutputBody object.
        sysofreq.listOutput(i).setListBody(sysofreq.listBody());
        sysofreq.listOutput(i).setSolutionSet(listSolutions);
        sysofreq.listOutput(i).setListFreq(sysofreq.listWaveFrequencies());
        sysofreq.listOutput(i).setListWaveDir(sysofreq.listWaveDirections());
        sysofreq.listOutput(i).setCurBody(i);

        //Setup filewriter for outputs
        FileWriter Writer(oFreq_Directory, sysofreq.listOutput(i));

        //Write frequency and wave direction list if this is the first iteration.
        if (i == 0)
        {
            bool testwrite;     //test to see if file writing was sucessful.

            //Write frequency list
            testwrite = Writer.writeFrequency();

            if (!testwrite)
                cerr << "Error Writing Frequency Outputs\n";

            //Write wave directions list
            testwrite = Writer.writeWaveDirection();

            if (!testwrite)
                cerr << "Error Writing Frequency Outputs\n";
        }

        //Calculate outputs and write to file
        //Everything handled with the following function.
        calcOutput(sysofreq.listOutput(i), Writer);
    }

    return a.exec();
}

//####################################### buildMatBody Function #######################################################
void buildMatBody(int bod, bool useCoeff)
{
    //First assign the basic properties for the matbody.
    listMatBody[bod].setId(bod);
    int modelnum;       //Tracks which model to use

    //Search through the set of motion models to find the matching name.
    for (unsigned int i = 0; i < listModel.size(); i++)
    {
        if (listModel[i].getName() == sysofreq.listBody(bod).getMotionModel())
        {
            listMatBody[bod].setModelId(i);
            modelnum = i;
            break;
        }
    }

    //Now know the correct motion model to use.
    //Create initial setup.
    listModel[modelnum].setlistBody(sysofreq.listBody());        //Feed the list of bodies
    listModel[modelnum].setBody(bod);                   //Set which body to use as the current body
    listModel[modelnum].CoefficientOnly() = useCoeff;      //Let it know to only calculate coefficients.
    listModel[modelnum].Reset();                        //Give it a reset just for good measure.

    //Iterate through all the active forces, user
    //------------------------------------------
    for (unsigned int i = 0; i < sysofreq.listBody(bod).listForceActive_user().size(); i++)
    {
        listMatBody[bod].listForceActive_user().push_back(matForceActive());
        listMatBody[bod].listForceActive_user(i).listCoefficients() = listModel[modelnum].getMatForceActive_user(i);
        //Create force ID.
        listMatBody[bod].listForceActive_user(i).setId(i);
    }

    //Iterate through all the active forces, hydro
    //------------------------------------------
    for(unsigned int i = 0; i < sysofreq.listBody(bod).listForceActive_hydro().size(); i++)
    {
        listMatBody[bod].listForceActive_hydro().push_back(matForceActive());
        listMatBody[bod].listForceActive_hydro(i).listCoefficients() = listModel[modelnum].getMatForceActive_hydro(i);
        //Create force ID.
        listMatBody[bod].listForceActive_hydro(i).setId(i);
    }

    //Use this pointer for referencing the forces
    matForceReact* ptForce;
    ForceReact* ptReact;

    //Iterate through all the reactive forces, user
    //------------------------------------------
    for (unsigned int i = 0; i < sysofreq.listBody(bod).listForceReact_user().size(); i++)
    {
        listMatBody[bod].listForceReact_user().push_back(matForceReact());
        //Create pointer
        ptForce = &listMatBody[bod].listForceReact_user(i);

        //Assign id for force.
        ptForce->setId(i);

        ptReact = sysofreq.listBody(bod).listForceReact_user(i);

        //Iterate through each derivative.
        for (int j = 0; j <= ptReact->getMaxOrd(); j++)
        {
            //Assign matrices
            ptForce->listDerivative().push_back(listModel[modelnum].getMatForceReact_user(i,j));
        }
    }

    //Iterate through all the reactive forces, hydro
    //------------------------------------------
    for (unsigned int i = 0; i < sysofreq.listBody(bod).listForceReact_hydro().size(); i++)
    {
        listMatBody[bod].listForceReact_hydro().push_back(matForceReact());
        //Create pointer
        ptForce = & listMatBody[bod].listForceReact_hydro(i);

        //Assign id for force.
        ptForce->setId(i);

        ptReact = sysofreq.listBody(bod).listForceReact_user(i);

        //Iterate through each derivative.
        for (int j = 0; j <= ptReact->getMaxOrd(); j++)
        {
            //Assign matrices
            ptForce->listDerivative().push_back(listModel[modelnum].getMatForceReact_hydro(i,j));
        }
    }

    delete ptForce;
    delete ptReact;
    matForceCross* ptForce2;
    ForceCross* ptCross;

    //Iterate through all the cross body forces, user
    //------------------------------------------
    for (unsigned int i = 0; i < sysofreq.listBody(bod).listForceCross_user().size(); i++)
    {
        listMatBody[bod].listForceCross_user().push_back(matForceCross());
        //Create pointer
        ptForce2 = &listMatBody[bod].listForceCross_user(i);

        //Assign id for force.
        ptForce2->setId(i);

        //Assign cross body
        for (unsigned int k = 0; k < sysofreq.listBody().size(); k++)
        {
            if (&sysofreq.listBody(k) == &(sysofreq.listBody(k).listCrossBody_user(bod)))
            {
                //Assign cross body
                ptForce2->setLinkedBody(listMatBody[k]);
                //Linked ID is automatically set.
                break;
            }
        }

        //Assign pointer
        ptCross = sysofreq.listBody(bod).listForceCross_user(i);

        //Iterate through each derivative.
        for (int j = 0; j <= ptCross->getMaxOrd(); j++)
        {
            //Assign matrices
            ptForce2->listDerivative().push_back(listModel[modelnum].getMatForceCross_user(i,j));
        }
    }

    //Iterate through all the cross body forces, hydro
    //------------------------------------------
    for (unsigned int i = 0; i < sysofreq.listBody(bod).listForceCross_hydro().size(); i++)
    {
        listMatBody[bod].listForceCross_hydro().push_back(matForceCross());
        //Create pointer
        ptForce2 = &listMatBody[bod].listForceCross_hydro(i);

        //Assign id for force.
        ptForce2->setId(i);

        //Assign cross body
        for (unsigned int k = 0; k < sysofreq.listBody().size(); k++)
        {
            if (&sysofreq.listBody(k) == &(sysofreq.listBody(k).listCrossBody_hydro(bod)))
            {
                //Assign cross body
                ptForce2->setLinkedBody(listMatBody[k]);
                //Linked Id is automatically set
                break;
            }
        }

        //Assign pointer
        ptCross = sysofreq.listBody(bod).listForceCross_user(i);

        //Iterate through each derivative.
        for (int j = 0; j <= ptCross->getMaxOrd(); j++)
        {
            //Assign matrices
            ptForce2->listDerivative().push_back(listModel[modelnum].getMatForceCross_hydro(i,j));
        }
    }

    //Get the mass matrix
    //------------------------------------------
    listMatBody[bod].refMass() = listModel[modelnum].getMatForceMass();
}

//######################################## CalcOutput Function ########################################################
void calcOutput(OutputsBody &OutputIn, FileWriter &WriterIn)
{
    bool filetest;      //Checks whether filewriting was sucessful.

    //Setup the FileWriter with the OutputsBody object.
    WriterIn.refOutputsBody() = OutputIn;

    //Iterate through each wave direction and calculate outputs.
    for (unsigned int j = 0; j < sysofreq.listWaveDirections().size(); j++)
    {
        //Set current wave direction
        sysofreq.setCurWaveDirInd(j);
        OutputIn.setCurWaveDir(j);

        //Write output for Global Motion
        filetest = WriterIn.writeGlobalMotion();

        if (!filetest)
        {
            cerr << "Error writing output to Global Motion file." << endl;
        }

        //Write output for Global Velocity
        filetest = WriterIn.writeGlobalVelocity();

        if (!filetest)
        {
            cerr << "Error writing output to Global Velocity file." << endl;
        }

        //Write output for Global Acceleration
        filetest = WriterIn.writeGlobalAcceleration();

        if (!filetest)
        {
            cerr << "Error writing output to Global Acceleration file." << endl;
        }

        //Write output for Global Solution;
        filetest = WriterIn.writeGlobalSolution();

        if (!filetest)
        {
            cerr << "Error writing output to Global Solution file." << endl;
        }
    }
}

//######################################## ReadFiles Function #########################################################
void ReadFiles(string runPath)
{
    //Reads in all the input files

    FileReader fileIn;                  //Create file reading objects
    dictBodies dictBod;                 //Create dictionary object for bodies.in
    dictForces dictForce;               //Create dictionary object for forces.in
    dictControl dictCont;               //Create dictionary object for control.in

    //Connect the dictionary and FileReader objects.
//    QObject::connect(&fileIn, SIGNAL(outputBodiesFile(ObjectGroup)), &dictBod, SLOT(setObject(ObjectGroup)));
//    QObject::connect(&fileIn, SIGNAL(outputForcesFile(ObjectGroup)), &dictForce, SLOT(setObject(ObjectGroup)));
//    QObject::connect(&fileIn, SIGNAL(outputControlFile(ObjectGroup)), &dictCont, SLOT(setObject(ObjectGroup)));

    //Create pointer to System object for each of the filereader objects
    fileIn.setSystem( &sysofreq);
    dictBod.setSystem( &sysofreq);
    dictForce.setSystem( &sysofreq);
    dictCont.setSystem( &sysofreq);

    //Set path for file reading
    fileIn.setPath(runPath);

    //Read input files
    //Sequence of file reading is important.
    fileIn.setDictionary(dictCont);
    fileIn.readControl();       //Must be first.
    fileIn.setDictionary(dictForce);
    fileIn.readForces();        //Must come before reading Bodies
    fileIn.setDictionary(dictBod);
    fileIn.readBodies();        //Must come after reading forces.
}

//####################################### DefineModels Function #######################################################
void DefineModels()
{
    //Create each motion model and add it to the list.
    //Don't need to set any properties for the models.  All those get set at object creation.

    listModel.push_back(Model6DOF());
}
