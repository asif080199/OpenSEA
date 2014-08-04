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

#include "hydrodata.h"

using namespace osea::ofreq;
using namespace std;

//==========================================Section Separator =========================================================
//Static initialization

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
hydroData::hydroData()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
hydroData::~hydroData()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void hydroData::setWaveAmp(double ampIn)
{
    pWaveAmp = ampIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
double hydroData::getWaveAmp()
{
    return pWaveAmp;
}

//------------------------------------------Function Separator --------------------------------------------------------
void hydroData::setWaveDir(double dirIn)
{
    pWaveDir = dirIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
double hydroData::getWaveDir()
{
    return pWaveDir;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<matForceReact> &hydroData::listDataReact()
{
    return plistDataReact;
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact &hydroData::listDataReact(int index)
{
    return plistDataReact.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
void hydroData::addDataReact(ofreq::matForceReact forceIn)
{
    plistDataReact.push_back(forceIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<matForceActive> &hydroData::listDataActive()
{
    return plistDataActive;
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceActive &hydroData::listDataActive(int index)
{
    return plistDataActive.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
void hydroData::addDataActive(matForceActive forceIn)
{
    plistDataActive.push_back(forceIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector< std::vector< matForceCross> > &hydroData::listDataCross()
{
    return plistDataCross;
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceCross &hydroData::listDataCross(int freqInd, int bodIndex)
{
    return plistDataCross.at(freqInd).at(bodIndex);
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceCross &hydroData::listDataCross(int freqInd, std::string bodyName)
{
    int bodIndex;

    //Find index of bodyname
    for (int i = 0; i < plistDataCross.at(freqInd).size(); i++)
    {
        if (plistDataCross.at(freqInd).at(i).getLinkedName() == bodyName)
        {
            bodIndex = i;
            break;
        }
    }

    //Return value.
    return this->listDataCross(freqInd, bodIndex);
}

//------------------------------------------Function Separator --------------------------------------------------------
void hydroData::addDataCross(matForceCross forceIn, int freqInd)
{
    if (freqInd == -1)
    {
        //No frequency index specified.  Add a new index.
        plistDataCross.push_back(
                    vector<matForceCross>(forceIn)
                    );
    }
    else
    {
        //Frequency index specified.  Add a new hydrobody.
        plistDataCross.at(freqInd).push_back(forceIn);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<double> &hydroData::listWaveFreq()
{
    return plistWaveFreq;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &hydroData::listWaveFreq(int index)
{
    return plistWaveFreq.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
void hydroData::addWaveFreq(double freqIn)
{
    plistWaveFreq.push_back(freqIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact hydroData::getDataReact(double freqIn)
{
    //Find the closest distance indices
    vector<int> index = FindMatch(freqIn, plistWaveFreq);

    //Make sure the two frequencies are in the correct order.
    if (plistWaveFreq[index[1]] < plistWaveFreq[index[0]])
    {
        //Swap
        int swap = index[0];
        index[0] = index[1];
        index[1] = swap;
    }

    //Interpolate
    return iPolate(freqIn,
            plistWaveFreq[index[0]],
            plistWaveFreq[index[1]],
            plistDataReact[index[0]],
            plistDataReact[index[1]]
            );
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceActive hydroData::getDataActive(double freqIn)
{
    //Find the closest distance indices
    vector<int> index = FindMatch(freqIn, plistWaveFreq);

    //Make sure the two frequencies are in the correct order.
    if (plistWaveFreq[index[1]] < plistWaveFreq[index[0]])
    {
        //Swap
        int swap = index[0];
        index[0] = index[1];
        index[1] = swap;
    }

    //Interpolate
    return iPolate(freqIn,
            plistWaveFreq[index[0]],
            plistWaveFreq[index[1]],
            plistDataActive[index[0]],
            plistDataActive[index[1]]
            );
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceCross hydroData::getDataCross(double freqIn, int hydroInd)
{
    double dist[2];         //The distance between the search term and terms considered.
    vector<int> output(2);      //The vector of output terms.

    //Set initial values
    dist[0] = 1E9;
    dist[1] = 1E9;

    //Iterate through data set and find the closest match
    for (int i = 0; i < plistWaveFreq.size(); i++)
    {
        //Calculate distance
        dist[0] = abs(freqIn - plistWaveFreq[i]);

        //Check if distance is smaller
        //And make sure that vector is large enough for specified index.
        if ((dist[0] < dist[1]) &&
                (plistDataCross[i].size() >= hydroInd + 1))
        {
            dist[1] = dist[0];
            output[0] = i;
        }
    }

    //Reset distance values
    dist[0] = 1E9;
    dist[1] = 1E9;

    //Iterate through and find the 2nd closest match
    for (int i = 0; i < plistWaveFreq.size(); i++)
    {
        //Calculate distance
        dist[0] = abs(freqIn - plistWaveFreq[i]);

        //Check if distance is smaller and not the other index.
        //Also be sure that vector is large enough for the specified index.
        if ((dist[0] < dist[1]) &&
                (i != output[0]) &&
                (plistDataCross[i].size() >= hydroInd + 1))
        {
            dist[1] = dist[0];
            output[1] = i;
        }
    }

    //Perform interpolation
    return iPolate(freqIn,
            plistWaveFreq[index[0]],
            plistWaveFreq[index[1]],
            plistDataCross[index[0]][hydroInd],
            plistDataCross[index[1]][hydroInd]
            );
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceCross hydroData::getDataCross(double freqIn, std::string hydroName)
{
    double dist[2];         //The distance between the search term and terms considered.
    vector<int> output(2);      //The vector of output terms.

    //Set initial values
    dist[0] = 1E9;
    dist[1] = 1E9;

    //Iterate through data set and find the closest match
    //Include requirement that searched terms must match hydroName.
    for (int i = 0; i < plistWaveFreq.size(); i++)
    {
        for (int j = 0; j < plistDataCross[i].size(); j++)
        {
            //Calculate distance
            dist[0] = abs(freqIn - plistWaveFreq[i]);

            //Check if distance is smaller
            //And require that frequency contains a hydrobody with the matching name.
            if ((dist[0] < dist[1]) &&
                    (plistDataCross[i][j].getLinkedName().compare(hydroName) == 0))
            {
                dist[1] = dist[0];
                output[0] = i;
            }
        }
    }

    //Reset distance values
    dist[0] = 1E9;
    dist[1] = 1E9;

    //Iterate through and find the 2nd closest match
    for (int i = 0; i < plistWaveFreq.size(); i++)
    {
        for (int j = 0; j < plistDataCross[i].size(); j++)
        {
            //Calculate distance
            dist[0] = abs(freqIn - plistWaveFreq[i]);

            //Check if distance is smaller and not the other index.
            //And require that frequency contains a hydrobody with the matching name.
            if ((dist[0] < dist[1]) &&
                    (i != output[0]) &&
                    (plistDataCross[i][j].getLinkedName().compare(hydroName) == 0))
            {
                dist[1] = dist[0];
                output[1] = i;
            }
        }
    }

    //Now find the indices for the actual hydrobody names.
    int hydroInd[2];              //Index of hydrobody names
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < plistDataCross[i].size(); j++)
        {
            if (plistDataCross[i][j].getLinkedName().compare(hydroName) == 0)
            {
                hydroInd[i] = j;
                break;
            }
        }
    }

    //Perform interpolation
    return iPolate(freqIn,
            plistWaveFreq[index[0]],
            plistWaveFreq[index[1]],
            plistDataCross[index[0]][hydroInd[0]],
            plistDataCross[index[1]][hydroInd[1]]
            );
}


//==========================================Section Separator =========================================================
//Protected Functions



//==========================================Section Separator =========================================================
//Private Functions
