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

#include "dictdata.h"
#include "filereader.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Class Names
string dictData::OBJECT_HYDROFILE = "hydrofiles"; /**< Keyword for the hydrofiles object.*/

//------------------------------------------Function Separator ----------------------------------------------------
//Keyword Names
string dictData::KEY_FILELOCATION = "filelocation"; /**< Keyword for the file location list. */


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictData::dictData()
{
    ObjectSet = false;
}

//------------------------------------------Function Separator --------------------------------------------------------
dictData::dictData(osea::FileReader *parent)
{
    pParent = parent;
    ObjectSet = false;
}

//==========================================Section Separator =========================================================
//Signals

//==========================================Section Separator =========================================================
//Public Slots

//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
int dictData::defineKey(std::string keyIn, std::vector<std::string> valIn)
{
    //Keyword for file locations
    //----------------------------------------------
    if (keyIn == KEY_FILELOCATION)
    {
        try
        {
            //Check that the object was first declared.
            if (!ObjectSet)
                throw std::runtime_error("File:  data.in \n  Object not declared.");

            //Reset the boolean
            ObjectSet = false;

            //Scan through each of the files listed in the vector.  Check for trailing slashes.
            for (unsigned int i = 0; i < valIn.size(); i++)
            {
                int q = valIn.at(i).rfind(SLASH);
                if (valIn.at(i).rfind(SLASH) == valIn.at(i).length() - 1)
                {
                    //Remove trailing slash.
                    valIn.at(i).pop_back();
                }
            }

            //First clear the list.
            pParent->listDataFiles().clear();

            //Write back to the main filereader.
            pParent->listDataFiles() = valIn;

            return 0;
        }
        catch(const std::exception &err)
        {
            return 99;
            logStd.Notify();
            logErr.Write(ID + err.what());
        }
    }

    //----------------------------------------------
    else
    {
        logStd.Notify();
        logErr.Write(string("File:  data.in \n Unknown key found.  Key:  ") + keyIn);
        return 1;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int dictData::defineClass(std::string nameIn)
{
    //Key for hydrofiles object.
    //----------------------------------------------
    if (nameIn == OBJECT_HYDROFILE)
    {
        ObjectSet = true;

        return 0;
    }

    //----------------------------------------------
    else
    {
        logStd.Notify();
        logErr.Write(string("File:  data.in \n Unknown object declared.  Object:  ") + nameIn);
        return 1;
    }
}




