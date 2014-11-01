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
#include "dicthydroreact.h"
#include "hydroreader.h"

using namespace std;
using namespace osea;
using namespace arma;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Class Name constants
string dictHydroReact::OBJECT_BODY = "body"; /**< Body object designator. */
string dictHydroReact::OBJECT_DATA = "data"; /**< Datat object designator. */

//------------------------------------------Function Separator ----------------------------------------------------
//Keyword name constants
string dictHydroReact::KEY_NAME = "name"; /**< Keyword for body name */
string dictHydroReact::KEY_FREQUENCY = "frequency"; /**< Keyword for frequency. */
string dictHydroReact::KEY_VALUE = "value"; /**< Keyword for matrix of coefficients. */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictHydroReact::dictHydroReact()
{
    pBodOn = false;
    pHydroBod = NULL;
    ORD = 0;
}

//------------------------------------------Function Separator --------------------------------------------------------
dictHydroReact::dictHydroReact(osea::HydroReader *parent)
{
    pParent = parent;
    pBodOn = false;
    pHydroBod = NULL;
    ORD = 0;
}

//==========================================Section Separator =========================================================
//Signal Functions


//==========================================Section Separator =========================================================
//Public Slots Functions


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
int dictHydroReact::defineKey(std::string keyIn, std::vector<std::string> valIn)
{
    //Key for hydrobody name
    //-----------------------------------------------
    if (keyIn == KEY_NAME)
    {
        //Check if body was declared.
        try
        {
            if (!pBodOn)
            {
                if (ORD == 0)
                {
                    //Hydrostiffness input file.
                    throw std::runtime_error(string("hydrostiffness.out file.  Body:  ") + valIn.at(0)
                        + string("\n No body object declared before body data was supplied."));
                }
                else if (ORD == 1)
                {
                    //Hydrodamping input file.
                    throw std::runtime_error(string("hydrodamp.out file.  Body:  ") + valIn.at(0)
                        + string("\n No body object declared before body data was supplied."));
                }
                else if (ORD == 2)
                {
                    //Hydromass input file.
                    throw std::runtime_error(string("hydromass.out file.  Body:  ") + valIn.at(0)
                        + string("\n No body object declared before body data was supplied."));
                }

            }


            //Reset the body signafier for the next body.
            pBodOn = false;

            //Search for a hydroData object with that body name.
            int temp = pParent->findHydroDataTemp(valIn.at(0));

            //Setup a pointer to the hydroBody object, for convenience.
            pHydroBod = &(pParent->plistTempHydro.at(temp));

            //Do not need to set the hydrobody name.  That gets handled by the findHydroDataTemp() function.
            return 0;
        }
        catch(std::runtime_error &err)
        {
            //Throw an error.
            logStd.Notify();
            logErr.Write(ID + std::string(err.what()));
            return 2;
            exit(1);
        }
    }

    //Key for frequency index
    //-----------------------------------------------
    else if (keyIn == KEY_FREQUENCY)
    {
        //Convert value
        int out = atoi(valIn.at(0).c_str());

        //Set frequency index.
        pFreqInd = out - 1;

        //Resize the list of reactive forces for that size.
        if ((pHydroBod->listDataReact().size() - 1 < pFreqInd)
                || (pHydroBod->listDataReact().size() == 0))
        {
            pHydroBod->listDataReact().resize(pFreqInd + 1);
        }

        return 0;
    }

    //Key for value list.
    //-----------------------------------------------
    else if (keyIn == KEY_VALUE)
    {
        try
        {
            //First check if the frequency index was set.
            if (pFreqInd < 0)
                throw std::runtime_error(string("Hydrodynamic input file.\n Frequency index was not declared before ")
                                         +string("entered data.  No frequency associated with this data."));

            /* The data is a matrix of values.  But it gets input as a sequential list.  However, there is a key
             * feature.  All matrices of reactive forces must be square.  Number of rows equals number of columns.
             * So if you know the total number of elements, you can work out the dimensions of the matrix.
             * n_row = sqrt(n_elem)
             */

            int nRows = int(sqrt(valIn.size()));

            //Create a matrix.
            cx_mat input(nRows, nRows);

            int count = 0;

            //Add items to the matrix.
            for (int i = 0; i < nRows; i++)
            {
                for (int j = 0; j < nRows; j++)
                {
                    //Convert value
                    complex<double> out;
                    out.real(atof(valIn.at(count).c_str()));

                    count += 1;

                    //Add to matrix.
                    input(i,j) = out;
                }
            }

            //Add the matrix to the output.
            pHydroBod->listDataReact(pFreqInd).setDerivative(ORD, input);

            //Reset the frequency index
            pFreqInd = -1;

            return 0;
        }
        catch (std::runtime_error &err)
        {
            logStd.Notify();
            logErr.Write(ID + std::string(err.what()));
            return 2;
            exit(1);
        }

    }

    //-----------------------------------------------
    else
    {
        logStd.Notify();
        logErr.Write(string("Hydrodynamic input file \n Unknown key found.  Key:  ") + keyIn);
        return 1;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int dictHydroReact::defineClass(std::string nameIn)
{
    //Body class Name
    //-----------------------------------------------
    if (nameIn == OBJECT_BODY)
    {
        //Turn on the body object to signal that all data is contained.
        pBodOn = true;
        pHydroBod = NULL;

        return 0;
    }

    //Data class name
    //-----------------------------------------------
    else if (nameIn == OBJECT_DATA)
    {
        //reset the frequency index
        pFreqInd = -1;

        return 0;
    }

    //-----------------------------------------------
    else
    {
        //throw an error
        logStd.Notify();
        logErr.Write(string("Hydrodynamic input file \n Unknown object name found.  Object:  ") + nameIn);
        return 1;
    }
}

//==========================================Section Separator =========================================================
//Private Functions
