#include "dictcrossreact.h"

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
#include "dictcrossreact.h"
#include "hydroreader.h"

using namespace std;
using namespace osea;
using namespace arma;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Class Name constants
string dictCrossReact::OBJECT_BODY = "body"; /**< Body object designator. */
string dictCrossReact::OBJECT_DATA = "data"; /**< Datat object designator. */
string dictCrossReact::OBJECT_LINKBODY = "linkedbody"; /**< Object designator for a linked body. */

//------------------------------------------Function Separator ----------------------------------------------------
//Keyword name constants
string dictCrossReact::KEY_NAME = "name"; /**< Keyword for body name */
string dictCrossReact::KEY_FREQUENCY = "frequency"; /**< Keyword for frequency. */
string dictCrossReact::KEY_VALUE = "value"; /**< Keyword for matrix of coefficients. */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictCrossReact::dictCrossReact()
{
    pBodOn = false;
    pLinkOn = false;
    pHydroBod = NULL;
    ORD = 0;
}

//------------------------------------------Function Separator --------------------------------------------------------
dictCrossReact::dictCrossReact(osea::HydroReader *parent)
{
    pParent = parent;
    pBodOn = false;
    pLinkOn = false;
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
int dictCrossReact::defineKey(std::string keyIn, std::vector<std::string> valIn)
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
                    throw std::runtime_error(string("crossstiffness.out file.  Body:  ") + valIn.at(0)
                        + string("\n No body object declared before body data was supplied."));
                }
                else if (ORD == 1)
                {
                    //Hydrodamping input file.
                    throw std::runtime_error(string("crossdamp.out file.  Body:  ") + valIn.at(0)
                        + string("\n No body object declared before body data was supplied."));
                }
                else if (ORD == 2)
                {
                    //Hydromass input file.
                    throw std::runtime_error(string("crossmass.out file.  Body:  ") + valIn.at(0)
                        + string("\n No body object declared before body data was supplied."));
                }

            }

            //Check if setting the name for the body or the linked body.
            if (pLinkOn)
            {
                //Setting name for linked body.
                pLinkName = valIn.at(0);
            }
            else
            {
                //Search for a hydroData object with that body name.
                int temp = pParent->findHydroDataTemp(valIn.at(0));

                //Setup a pointer to the hydroBody object, for convenience.
                pHydroBod = &(pParent->plistTempHydro.at(temp));

                //Do not need to set the hydrobody name.  That gets handled by the findHydroDataTemp() function.
            }

            return 0;
        }
        catch(const std::exception &err)
        {
            //Throw an error.
            logStd.Notify();
            logErr.Write(ID + std::string(err.what()));
            return 2;   
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

        //First search to see if the list of frequencies is the right size.
        if ((pHydroBod->listDataCross().size() -1 < pFreqInd)
                || (pHydroBod->listDataCross().size() == 0))
        {
            //Resize the list
            pHydroBod->listDataCross().resize(pFreqInd + 1);
        }

        //Make sure the list of linked bodies is at least one element in size.
        if (pHydroBod->listDataCross().at(pFreqInd).size() > 0)
        {
            //Now search through the list of linked bodies to see if I can find the one with the matching name.
            bool Match = false;

            for (unsigned int i = 0; i < pHydroBod->listDataCross().at(pFreqInd).size(); i++)
            {
                if (pHydroBod->listDataCross(pFreqInd, i).getLinkedName() == pLinkName)
                {
                    Match = true;
                    break;
                }
            }

            //If no match was found, need to add a crossbody force.
            if (!Match)
            {
                pHydroBod->addDataCross(pFreqInd);

                //Assign the hydrobody name.
                pHydroBod->listDataCross(pFreqInd,
                                         pHydroBod->listDataCross().at(pFreqInd).size() - 1)
                        .setLinkedName(pLinkName);
            }
        }
        else
        {
            //Nothing assigned.  Need to add a new crossbody force.
            pHydroBod->addDataCross(pFreqInd);

            //Assign the hydrobody name.
            pHydroBod->listDataCross(pFreqInd,0).setLinkedName(pLinkName);
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
                throw std::runtime_error(string("crossbody input file.\n Frequency index was not declared before ")
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
            pHydroBod->listDataCross(pFreqInd, pLinkName).setDerivative(ORD, input);

            //Reset the frequency index
            pFreqInd = -1;

            return 0;
        }
        catch (std::runtime_error &err)
        {
            logStd.Notify();
            logErr.Write(ID + std::string(err.what()));
            return 2;
           
        }
    }

    //-----------------------------------------------
    else
    {
        logStd.Notify();
        logErr.Write(string("Crossbody input file \n Unknown key found.  Key:  ") + keyIn);
        return 1;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int dictCrossReact::defineClass(std::string nameIn)
{
    //Body class Name
    //-----------------------------------------------
    if (nameIn == OBJECT_BODY)
    {
        //Turn on the body object to signal that all data is contained.
        pBodOn = true;
        pLinkOn = false;
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

    //Linked Body class name.
    //-----------------------------------------------
    else if (nameIn == OBJECT_LINKBODY)
    {
        //Turn on the linked body object to signal all data is within the linked body.
        pLinkOn = true;

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
