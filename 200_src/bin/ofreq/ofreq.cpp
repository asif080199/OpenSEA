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
#include "./system_objects/system.h"
#include "./file_reader/dictcontrol.h"
#include "./file_reader/dictforces.h"
#include "./file_reader/dictbodies.h"
#include "./file_reader/filereader.h"
#include "./file_reader/dictseaenv.h"
#include "./file_reader/dictdata.h"
#include "./file_reader/dictoutputs.h"
#include "./hydro_reader/hydroreader.h"
#include "./system_objects/ofreqcore.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <QtGlobal>
#ifdef Q_OS_WIN
    //----------- Windows Inclusions ------------
    #include <direct.h>
    #define GetCurrentDir _getcwd

    #include <shlwapi.h>
    #pragma comment(lib, "shlwapi.lib")
#elif defined Q_OS_LINUX
    //----------- Linux Inclusions ------------
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

//System object.  Used to control entire execution.
System sysofreq;

//Name of executable file itself
const std::string EXECNAME = "ofreq";

//Name of executable folder.  Specific to the ofreq executable.
const std::string EXECFOLDER = "ofreq_debug";

//Name of var folder.
const std::string VARFOLDER = "var";

//Name of lib folder
const std::string LIBFOLDER = "lib";

//Name of etc folder
const std::string ETCFOLDER = "etc";

//Name of binaries folder
const std::string BINFOLDER = "bin";

//######################################### Function Prototypes #######################################################

//------------------------------------------Function Separator --------------------------------------------------------
/**
 * @brief Builds a matrix body object for the body specified by the integer.  Uses the motion model identified by the
 * Body object.
 * @param bod Which body to use for building the matix body.
 * @return Returns a matBody object, fully provisioned with all necessary data.
 */
void buildMatBody(int bod, bool useCoeff=true);

//------------------------------------------Function Separator --------------------------------------------------------
/**
 * @brief Calculates derived outputs using the OutputsBody object and then writes those outputs to files.
 * @param OutputIn The OutputsBody object that will calculate the derived outputs.  All properties for the OutputsBody
 * object must be set by the time the function is called.  Variable passed by reference.
 * @param WriterIn The FileWriter object that will receive the outputs from the OutputsBody object and write those
 * outputs to a file.
 */
void calcOutput(OutputsBody &OutputIn, FileWriter &WriterIn);

//------------------------------------------Function Separator --------------------------------------------------------
/**
 * @brief Reads in all the input files.  Creates the necessary objects for file reading.  And connects those objects
 * using Qt slots and signals.  Finally proceeds through each file and reads it.  All parsing is accomplished by
 * the FileReader object.  File interpretation is processed through the Dictionary objects.
 * @param runPath String.  The path to the root directory of the input files.
 * @sa Dictionary
 */
void ReadFiles(string runPath);

//------------------------------------------Function Separator --------------------------------------------------------
/**
 * @brief Finds the path of the critical files for the program.
 *
 * Finds the path of one of four possible folders that are critical to the oSea program.  Includes platform
 * dependant code so that this function should work both under Windows or Linux.
 * @param typePath String that specifies which path to get.  Options are:
 * "exec":      Path to the directory of the executable file.  NOT the directory the program was called from.
 * "lib":       Path to the lib directory that is common to all oSea programs.
 * "var":       Path to the var directory that is common to all oSea programs.
 * "etc":       Path to the etc directory that is common to all oSea programs.
 * "bin":       Path to the binaries directory.  Binaries for individual programs are included in sub folders.
 * @return Returns std::string that is the full absolute path to the specified .  Returned variable passed by
 * value.
 */
std::string getPath(std::string typePath = "exec");

//------------------------------------------Function Separator --------------------------------------------------------
/**
 * @brief The write monitor function writes out the critical variables for monitoring solution progress.
 *
 * The following outputs are included in the writeMonitor function:
 * - current wave direction
 * - current wave frequency
 * - current total iteration (the function uses this to calculate percent complete).
 * @param dirIn Integer.  Variable passed by value.  The index of the current wave direction.  Start numbering from 0.
 * @param freqIn Integer.  Variable passed by value.  The index of the current wave frequency.  Start numbering from 0.
 * @param iterIn Integer.  Variable passed by value.  The index of the current iteration.  Start numbering from 0.
 */
void writeMonitor(int dirIn, int freqIn, int iterIn);

//------------------------------------------Function Separator --------------------------------------------------------
/**
 * @brief Writes the headers of the log files.
 */
void writeLogHeader();

//############################################ Class Prototypes #######################################################


//##################################### Constant Variables Declaration ################################################
#ifdef Q_OS_WIN
    const string seperator = "\\";  /**< Directory separator in a string path., windows version**/
#elif defined Q_OS_LINUX
    const string seperator = "/";   /**< Directory separator in a string path., linux version**/
#endif

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
    //Main Input Section
    //=================================================================================================================
	//if command line arg supplied, use that directory
	//or assume the current working directory

    if (argc == 2)
    {
        sysofreq.setPath(argv[1]);
    }
    else if (argc == 1)
    {
        //Get current working directory
        char cCurrentPath[FILENAME_MAX];
        if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
        {
            //Return error
        }
        //Set the path to the current working directory
        sysofreq.setPath(cCurrentPath);
    }

    //Setup the output log files
    //---------------------------------------------------------------------------
    sysofreq.setLogFiles(sysofreq.getPath());

    //Clear the screen and create an introductory header
    //---------------------------------------------------------------------------
    sysofreq.logStd.cls();
    writeLogHeader();

    try {
        //Read input files and interpret data.
        //---------------------------------------------------------------------------
        sysofreq.logStd.Write("Reading input files",3);
        sysofreq.logStd.Write("=================================================================================",3);
        ReadFiles(sysofreq.getPath());
        sysofreq.logStd.Write("\n\n\n",3);

        //Set the active sea model
        //---------------------------------------------------------------------------
        sysofreq.setActiveSeaModel();

        //Clear out any previous output files
        //---------------------------------------------------------------------------
        sysofreq.refReportManager().clearFiles();





    //Pre Processing Section
    //=================================================================================================================

        //Start creating main objects
        //---------------------------------------------------------------------------
        //Resize the solution object.
        for (unsigned int i = 0; i < sysofreq.listBody().size(); i++)
        {
            sysofreq.listSolutionSet().push_back(SolutionSet(sysofreq.listWaveDirections().size(),
                                                 sysofreq.listWaveFrequencies().size()));
        }

        //Iterate through each wave direction and wave frequency to solve
        //---------------------------------------------------------------------------
        //Create an iterator to track the loops.
        int itertrack = 0;

        sysofreq.logStd.Write("Solving equations",3);
        sysofreq.logStd.Write("=================================================================================",3);

        for(unsigned int i = 0; i < sysofreq.listWaveDirections().size(); i++)
        {
            //Set the current wave direction
            sysofreq.setCurWaveDirInd(i);

            for(unsigned int j = 0; j < sysofreq.listWaveFrequencies().size(); j++)
            {
                //Set the current wave frequency
                sysofreq.setCurFreqInd(j);

                //Update hydrodynamic forces
                sysofreq.updateHydroForce();

                //Build the matrix bodies
                //---------------------------------------------------------------------------
                //Build operation stuck inside the wave iteration loop because the forces may change with
                //wave direction.  Also inside the wave frequency loop because the forces may change
                //with wave frequency.

                //Clear the list of matrix bodies.
                listMatBody.clear();

                //Resize matrix bodies vector.
                listMatBody.resize(sysofreq.listBody().size());

                bool coeffonly = true;              //Boolean to tell the motion model to only use coefficients.
                //Iterate through each body and build.
                for (unsigned int i2 = 0; i2 < sysofreq.listBody().size(); i2++)
                {
                    //Build the body.
                    //Will automatically use the correct bodies.
                    buildMatBody(i2, coeffonly);
                }




    //Matrix Solution Section
    //=================================================================================================================

                //Create motion solver and feed in the body data.
                MotionSolver theMotionSolver(listMatBody);
                //Set the current wave frequency
                theMotionSolver.setWaveFreq(sysofreq.getCurFreq());
                //Solve the system of equations.
                theMotionSolver.calculateOutputs();

                //assign each solution per frequency to a body
                for(unsigned int k = 0; k < sysofreq.listBody().size(); k++)
                {
                    Solution soln;
                    //soln.refBody() = sysofreq.listBody(k);
                    soln.setBody(&sysofreq.listBody(k));
                    soln.setSolnMat(theMotionSolver.listSolution(k));
                    sysofreq.listSolutionSet().at(k).setSolnMat(i, j, soln);
                }

                //Update the iterator
                itertrack += 1;

                //Update the monitor log
                writeMonitor(i,j,itertrack);

                //Write output to standard log
                string msg;
                ostringstream convert;
                msg = "Wave Direction:  ";
                convert << (i+1);
                msg += convert.str() + " of ";
                convert.str("");
                convert << sysofreq.listWaveDirections().size();
                msg += convert.str();
                convert.str("");
                msg += "\t\tFrequency:  ";
                convert << (j+1);
                msg += convert.str() + " of ";
                convert.str("");
                convert << sysofreq.listWaveFrequencies().size();
                msg += convert.str();
                sysofreq.logStd.Write(msg,3);
                convert.str("");
            }
        }




    //Post Processing Section
    //=================================================================================================================

        //Read in header file for output reports
        sysofreq.refReportManager().setHeader(
                    getPath("var")
                    );


        //Write outputs
        //---------------------------------------------------------------------------
        //Writes reports for all wave directions and all bodies.
        sysofreq.refReportManager().writeReport();

        sysofreq.logStd.Write("\n\n\n",3);
        sysofreq.logStd.Write("=================================================================================",3);
        sysofreq.logStd.Write("oFreq completed successfully.",3);
        return 0;
    }
    catch(const std::exception &err)
    {
        sysofreq.logStd.Notify();
        return 1;
        sysofreq.logErr.Write(ID + std::string(err.what()));
    }
}

//####################################### buildMatBody Function #######################################################
void buildMatBody(int bod, bool useCoeff)
{
    try
    {
        //First assign the basic properties for the matbody.
        listMatBody.at(bod).setId(bod);
        Body* MyBod;         //The current body that I am working with
        MotionModel* MyModel; //The current motion model that I am working with

        MyBod = &(sysofreq.listBody(bod));   //Get the current body to work with.
        MyModel = &(MyBod->getMotionModel());  //Get the current motion model to work with.

        //Now know the correct motion model to use.
        //Create initial setup.
        MyModel->setlistBody(sysofreq.listBody());   //Feed the list of bodies
        MyModel->setBody(bod);       //Set which body to use as the current body
        MyModel->CoefficientOnly() = useCoeff;  //Let it know to only calculate coefficients.
        MyModel->Reset();   //Give it a reset just for good measure.

        //Iterate through all the active forces, user
        //------------------------------------------
        for (unsigned int i = 0; i < MyBod->listForceActive_user().size(); i++)
        {
            listMatBody.at(bod).listForceActive_user().push_back(matForceActive());
            listMatBody.at(bod).listForceActive_user(i).listCoefficient() = MyModel->getMatForceActive_user(i);
            //Print out active force matrix.  For Debugging.
            //listMatBody.at(bod).listForceActive_user(i).listCoefficient().print("Active Force: " + i);

            //Create force ID.
            listMatBody.at(bod).listForceActive_user(i).setId(i);
        }

        //Iterate through all the active forces, hydro
        //------------------------------------------
        for(unsigned int i = 0; i < MyBod->listForceActive_hydro().size(); i++)
        {
            listMatBody.at(bod).listForceActive_hydro().push_back(matForceActive());
            listMatBody.at(bod).listForceActive_hydro(i).listCoefficient() = MyModel->getMatForceActive_hydro(i);
            //Print out active force matrix.  For Debugging.
            //listMatBody.at(bod).listForceActive_hydro(i).listCoefficient().print("Active Force: " + i);

            //Create force ID.
            listMatBody.at(bod).listForceActive_hydro(i).setId(i);
        }

        //Use this pointer for referencing the forces
        matForceReact* ptForce;
        ForceReact* ptReact;

        //Iterate through all the reactive forces, user
        //------------------------------------------
        for (unsigned int i = 0; i < MyBod->listForceReact_user().size(); i++)
        {
            listMatBody.at(bod).listForceReact_user().push_back(matForceReact());
            //Create pointer
            ptForce = &listMatBody.at(bod).listForceReact_user(i);

            //Assign id for force.
            ptForce->setId(i);

            ptReact = MyBod->listForceReact_user(i);

            //Iterate through each derivative.
            for (int j = 0; j <= ptReact->getMaxOrd(); j++)
            {
                //Check for a zero entry.
                if (ptReact->listDerivative().size() == 0)
                    break;

                //Assign matrices
                ptForce->listDerivative().push_back(MyModel->getMatForceReact_user(i,j));

    //            //Print out Matrix.  For debugging.
    //            listMatBody.at(bod).listForceReact_user(0).listDerivative(0).print("Reactive Force:  ");
            }
        }

        //Iterate through all the reactive forces, hydro
        //------------------------------------------
        for (unsigned int i = 0; i < MyBod->listForceReact_hydro().size(); i++)
        {
            listMatBody.at(bod).listForceReact_hydro().push_back(matForceReact());
            //Create pointer
            ptForce = & listMatBody.at(bod).listForceReact_hydro(i);

            //Assign id for force.
            ptForce->setId(i);

            ptReact = MyBod->listForceReact_hydro(i);

            //Iterate through each derivative.
            for (int j = 0; j <= ptReact->getMaxOrd(); j++)
            {
                //Check for a zero entry.
                if (ptReact->listDerivative().size() == 0)
                    break;

                //Assign matrices
                ptForce->listDerivative().push_back(MyModel->getMatForceReact_hydro(i,j));
            }
        }

        matForceCross* ptForce2;
        ForceCross* ptCross;

        //Iterate through all the cross body forces, user
        //------------------------------------------
        for (unsigned int i = 0; i < MyBod->listForceCross_user().size(); i++)
        {
            listMatBody.at(bod).listForceCross_user().push_back(matForceCross());
            //Create pointer
            ptForce2 = &listMatBody.at(bod).listForceCross_user(i);

            //Assign id for force.
            ptForce2->setId(i);

            //Assign cross body
            for (unsigned int k = 0; k < sysofreq.listBody().size(); k++)
            {
                if (&sysofreq.listBody(k) == &(MyBod->listCrossBody_user(i)))
                {
                    //Assign cross body
                    ptForce2->setLinkedBody(&(listMatBody.at(k)));
                    //Set linked body id.  This should be automatic, but the program produced unreliable results.
                    ptForce2->setLinkedId(k);
                    break;
                }
            }

            //Assign pointer
            ptCross = sysofreq.listBody(bod).listForceCross_user(i);

            //Iterate through each derivative.
            for (int j = 0; j <= ptCross->getMaxOrd(); j++)
            {
                //Check for a zero entry.
                if (ptCross->listDerivative().size() == 0)
                    continue;

                //Assign matrices
                ptForce2->listDerivative().push_back(MyModel->getMatForceCross_user(i,j));
            }
        }

        //Iterate through all the cross body forces, hydro
        //------------------------------------------
        for (unsigned int i = 0; i < MyBod->listForceCross_hydro().size(); i++)
        {
            listMatBody.at(bod).listForceCross_hydro().push_back(matForceCross());
            //Create pointer
            ptForce2 = &listMatBody.at(bod).listForceCross_hydro(i);

            //Assign id for force.
            ptForce2->setId(i);

            //Assign cross body
            for (unsigned int k = 0; k < sysofreq.listBody().size(); k++)
            {
                if (&sysofreq.listBody(k) == &(MyBod->listCrossBody_hydro(i)))
                {
                    //Assign cross body
                    ptForce2->setLinkedBody(&(listMatBody.at(k)));
                    //Set linked body id.  This should be automatic, but the program produced unreliable results.
                    ptForce2->setLinkedId(k);
                    break;
                }
            }

            //Assign pointer
            ptCross = sysofreq.listBody(bod).listForceCross_hydro(i);

            //Iterate through each derivative.
            for (int j = 0; j <= ptCross->getMaxOrd(); j++)
            {
                //Check for a zero entry.
                if (ptCross->listDerivative().size() == 0)
                    continue;

                //Assign matrices
                ptForce2->listDerivative().push_back(MyModel->getMatForceCross_hydro(i,j));
            }
        }

        //Get the mass matrix
        //------------------------------------------
        listMatBody.at(bod).refMass() = MyModel->getMatForceMass();
        //print out mass matrix.  For debugging.
        //listMatBody.at(bod).refMass().print("Mass:");
    }
    catch(const std::exception &err)
    {
        sysofreq.logStd.Notify();
        sysofreq.logErr.Write(ID + std::string(err.what()));
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
    dictSeaEnv dictSea;                 //Create dictionary object for seaenv.in
    dictData dictDat(&fileIn);           //Create dictionary object for data.in
    dictOutputs dictOut;                //Create dictionary object for outputs.in

    try {
        //Create pointer to System object for each of the filereader objects
        fileIn.setSystem( &sysofreq);
        dictBod.setSystem( &sysofreq);
        dictForce.setSystem( &sysofreq);
        dictCont.setSystem( &sysofreq);
        dictSea.setSystem( &sysofreq);
        dictDat.setSystem( &sysofreq);
        dictOut.setSystem( &sysofreq);

        //Set path for file reading
        fileIn.setPath(runPath);

        //Read input files
        //Sequence of file reading is important.
        //Read control file.
        fileIn.setDictionary(dictCont);
        fileIn.readControl();       //Must be first.
        //Read sea environment file.
        fileIn.setDictionary(dictSea);
        fileIn.readSeaEnv();
        //Read user forces
        fileIn.setDictionary(dictForce);
        fileIn.readForces();        //Must come before reading Bodies
        //Read Bodies
        fileIn.setDictionary(dictBod);
        fileIn.readBodies();        //Must come after reading forces.
        //Read hydrodynamic data
        fileIn.setDictionary(dictDat);
        fileIn.readData();
        //Read in output controls
        fileIn.setDictionary(dictOut);
        fileIn.readOutputs();

        //At the end, force system to check one last time for the active sea model.
        sysofreq.SearchActiveSeaModel();

        //Write output to screen.
        sysofreq.logStd.Write("Hydrodynamic Input Files",3);
        sysofreq.logStd.Write("-----------------------------",3);

        //So far, input files only specified the location of the hydrodynamic data.  Now need to actually  read it.
        HydroReader hydroIn;            //Create hydroreader.
        hydroIn.setSystem( &sysofreq);

        if (fileIn.listDataFiles().size() > 0)
        {
            for (unsigned int i = 0; i < fileIn.listDataFiles().size(); i++)
            {
                //Iterate through each of the items on the list of hydro files and read them.
                hydroIn.setPath(fileIn.listDataFiles(i));           //Set the path to the hydro system.
                hydroIn.readHydroSys();
            }
        }
    }
    catch(const std::exception &err)
    {
        sysofreq.logStd.Notify();
        sysofreq.logErr.Write(ID + std::string(err.what()));
    }
}

//######################################## ReadFiles Function #########################################################
std::string getPath(string typePath)
{
    std::string output;     //Output of full path
    char buff[1024];        //Buffer for reading path

    //First get executable path.  Includes dependancy code for both windows and linux.
    #ifdef Q_OS_WIN
    //----------------------------------- Windows Code ----------------------------------------------------------------
    ssize_t len = GetModuleFileNameA(NULL, buff, sizeof(buff) - 1);
    std::string SLASH = "\\";
    #elif defined Q_OS_LINUX
    //------------------------------------ Linux Code -----------------------------------------------------------------
    ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff) - 1);
    std::string SLASH = "/";
    #endif

    //--------------------------- Platform Independent Code -----------------------------------------------------------
    if (len != -1)
    {
        buff[len] = '\0';
        output.assign(buff);
    }
    else
    {
        sysofreq.logErr.Write(string("Function:  ofreq \n Error Message:  Could not find filepath for executable."));
        sysofreq.logStd.Write("Errors found.  Please check the error log.",3);
    }

    //Strip off name of executable itself.
    std::string strip = SLASH + EXECNAME;
    output.erase(output.size() - strip.size(), strip.size());

    //Strip back the name of the executable directory.
    strip = BINFOLDER + SLASH + EXECFOLDER;
    output.erase(output.size() - strip.size(), strip.size());

    //Change output depending on which path was requested
    if (typePath == "exec")
    {
        //Add on the executable path
        output.append(BINFOLDER);
        output.append(SLASH);
        output.append(EXECFOLDER);
    }
    else if (typePath == "bin")
    {
        //Add on the binary folder
        output.append(BINFOLDER);
    }
    else if (typePath == "lib")
    {
        //Add on library path
        output.append(LIBFOLDER);
    }
    else if (typePath == "var")
    {
        //Add on variable folder
        output.append(VARFOLDER);
    }
    else if (typePath == "etc")
    {
        //Add on etc folder
        output.append(ETCFOLDER);
    }

    //Write output
    return output;
}

//######################################## writeMonitor Function ######################################################
void writeMonitor(int dirIn, int freqIn, int iterIn)
{
    int total;          //Total number of iterations
    double perc;        //Percentage complete of solving
    ofreq::oFreqCore convert;

    //get total number of iterations
    total = sysofreq.listWaveDirections().size() * sysofreq.listWaveFrequencies().size();

    perc = double(iterIn) / double(total);

    string msg;
    msg = convert.itoa(iterIn) + "       \t";
    msg += convert.itoa(dirIn + 1) + "        \t";
    msg += convert.itoa(freqIn + 1) + "        \t";
    msg += convert.ftoa(perc);
    sysofreq.logMon.Write(msg,3);
}

//######################################## writeLogHeader Function ####################################################
void writeLogHeader()
{
    #ifdef Q_OS_WIN
        std::string SLASH = "\\";  /**< Directory separator in a string path., windows version**/
    #elif defined Q_OS_LINUX
        std::string SLASH = "/";   /**< Directory separator in a string path., linux version**/
    #endif

    std::string directory = getPath("var");
    std::string HEADER_FILENAME = "openseaheader.txt";
    std::string fileIn = directory + SLASH + HEADER_FILENAME;
    std::string header;

    //Writes the headers to each of the log files
    ifstream header_fileInput(fileIn.c_str(), std::ios::in);

    try
    {
        if (!header_fileInput)
            throw std::ios_base::failure("Header file does not exist:  " + fileIn);

        header_fileInput.seekg(0, std::ios::end);
        header.resize(header_fileInput.tellg());
        header_fileInput.seekg(0, std::ios::beg);
        header_fileInput.read(&header.at(0), header.size());
        //Close file
        header_fileInput.close();
    }
    catch(const std::exception &err)
    {
        sysofreq.logStd.Notify();
        sysofreq.logErr.Write(ID + std::string(err.what()));
    }

    //Now that the header is read in, write it to each log file.
    sysofreq.logStd.Write(header + "\n\n\n",3,-1);
    sysofreq.logErr.Write(header + "\n\n\n",3,-1);
    sysofreq.logMon.Write(header + "\n\n\n",3,-1);

    //Write header information for monitor log
    sysofreq.logMon.Write("Monitors",3);
    sysofreq.logMon.Write("=================================================================================",3);
    sysofreq.logMon.Write("Iteration \tWave      \tWave      \tPercent",3);
    sysofreq.logMon.Write("          \tDirection \tFrequency \tComplete",3);
    sysofreq.logMon.Write("=================================================================================",3);

    //Write header information for error log
    sysofreq.logErr.Write("Error Log",3,-1);
    sysofreq.logErr.Write("=================================================================================",3,-1);

    //Write header information for standard log
    sysofreq.logStd.Write("System Status",3,-1);
    sysofreq.logStd.Write("=================================================================================",3,-1);
}
