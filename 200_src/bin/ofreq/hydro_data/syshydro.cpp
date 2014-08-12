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

#include "syshydro.h"

using namespace osea::ofreq;
using namespace std;

//==========================================Section Separator =========================================================
//Static initialization
double sysHydro::DISTMAX = 0.7853981634;  //Set to 45 deg.

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
sysHydro::sysHydro()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
sysHydro::~sysHydro()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector< std::vector< hydroData > > &sysHydro::listHydroData()
{
    return plistHydroData;
}

//------------------------------------------Function Separator --------------------------------------------------------
hydroData &sysHydro::listHydroData(int ampInd, int dirInd)
{
    return plistHydroData.at(ampInd).at(dirInd);
}

//------------------------------------------Function Separator --------------------------------------------------------
void sysHydro::addHydroData(int ampInd, ofreq::hydroData dataIn)
{
    if (ampInd < 0)
    {
        //no amp index specified.  Add new index.
        plistHydroData.push_back(vector< hydroData >(dataIn));
    }
    else
    {
        //amp index specified.  Add new wave direction at given amplitude index.
        plistHydroData.at(ampInd).push_back(dataIn);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string sysHydro::getHydroBodyName()
{
    return pHydroBodyName;
}

//------------------------------------------Function Separator --------------------------------------------------------
void sysHydro::setHydroBodyName(std::string nameIn)
{
    pHydroBodyName = nameIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void sysHydro::calcHydroData(double waveAmp, double waveFreq)
{
    //First get the indices of the two wave amplitudes that match the set amplitude.
    vector<int> indWaveAmp = findMatchAmplitude(waveAmp);

    //Create two vectors of hydrodata.  Interpolate each of the hydrodata.
    vector<hydroData> DataAmp1;
    vector<hydroData> DataAmp2;

    //Interpolate for first vector.  The first entry is wave direction 1.  Second entry is wave direction two.
    if (plistDirSubset.at(indWaveAmp[0]).size() < 2)
    {
        //No two wave directions.  This will be quick. . .
        //Just get the hydrodata for the one wave direction.
        DataAmp1.push_back(
                    plistDirSubset.at(indWaveAmp[0]).at(0)->interpHydroData(waveFreq)
                );
    }
    else
    {
        //More than one wave direction.  Good.
        //Interpolate for wave frequency on each wave direction.
        for (int i = 0; i < plistDirSubset.at(indWaveAmp[0]).size(); i++)
        {
            //Get hydrodata, interpolated for wave frequency.
            DataAmp1.push_back(
                        plistDirSubset.at(indWaveAmp[0]).at(i)->interpHydroData(waveFreq)
                    );
        }
    }

    //Repeat the process.  But only if there is a second wave amplitude index declared.
    if (indWaveAmp.size() > 1)
    {
        //Interpolate for first vector.  The first entry is wave direction 1.  Second entry is wave direction two.
        if (plistDirSubset.at(indWaveAmp[1]).size() < 2)
        {
            //No two wave directions.  This will be quick. . .
            //Just get the hydrodata for the one wave direction.
            DataAmp1.push_back(
                        plistDirSubset.at(indWaveAmp[1]).at(0)->interpHydroData(waveFreq)
                    );
        }
        else
        {
            //More than one wave direction.  Good.
            //Interpolate for wave frequency on each wave direction.
            for (int i = 0; i < plistDirSubset.at(indWaveAmp[1]).size(); i++)
            {
                //Get hydrodata, interpolated for wave frequency.
                DataAmp1.push_back(
                            plistDirSubset.at(indWaveAmp[1]).at(i)->interpHydroData(waveFreq)
                        );
            }
        }
    }

    //Successfully interpolated all four data sets for wave frequency.  Now interpolate for wave direction.



    if (DataAmp2.size() > 0)
    {
        //Only one wave amplitude defined.  Proceed with that.

        //First wave amplitude.
        //Interpolate if there is more than one wave direction.
        if (DataAmp1.size() > 1)
        {
            //Interpolate the given wave directions.
            hydroData DataDir1 =
                    interpWaveDir(
                        DataAmp1[0],
                        DataAmp1[1]);
        }
        else
        {
            hydroData DataDir1 = DataAmp1[0];
        }

        //Free up memory and clear the vector.
        DataAmp1.clear();

        //Now proceed to wave scaling.
    }
    else
    {
        //Two wave amplitudes.  Go forward with that.

        //First wave amplitude.
        //Interpolate if there is more than one wave direction.
        if (DataAmp1.size() > 1)
        {
            //Interpolate the given wave directions.
            hydroData DataDir1 =
                    interpWaveDir(
                        DataAmp1[0],
                        DataAmp1[1]);
        }
        else
        {
            hydroData DataDir1 = DataAmp1[0];
        }

        //Free up memory and clear the vector.
        DataAmp1.clear();

        //Second wave amplitude.
        //Interpolate if there is more than one wave direction.
        if (DataAmp2.size() > 1)
        {
            //Interpolate the given wave directions.
            hydroData DataDir2 =
                    interpWaveDir(
                        DataAmp2[0],
                        DataAmp2[1]);
        }
        else
        {
            hydroData DataDir2 = DataAmp2[0];
        }

        //Free up memory and clear the vector.
        DataAmp2.clear();

        //Now proceed to wave scaling.
    }

}

//------------------------------------------Function Separator --------------------------------------------------------
ForceActive sysHydro::getForceActive()
{




}

//------------------------------------------Function Separator --------------------------------------------------------
ForceReact sysHydro::getForceReact()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
ForceCross sysHydro::getForceCross(std::string linkedBodName)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void sysHydro::setWaveDir(double dirIn)
{
    pWaveDir = dirIn;

    //Build the wave subset
    buildWaveSubSet();
}

//------------------------------------------Function Separator --------------------------------------------------------
double sysHydro::getWaveDir()
{
    return pWaveDir;
}


//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
void sysHydro::buildWaveSubset()
{
    //Searches through the list of wave data and creates the subset of hydrodata that matches
    //The criteria for wave direction.

    //First clear the list.
    plistDirSubset.clear();

    for(int i = 0; i < plistHydroData.size(); i++)
    {
        //Outer loop.  Going through wave amplitudes.
        double dist[2];         //The distance between wave directions.

        dist[0] = 1E6;
        dist[1] = 1E6;
        int index[2];
        index[0] = -1;
        index[1] = -1;

        for (int j = 0; j < plistHydroData.at(i).size(); j++)
        {
            //Calculate distance for first point.
            dist[0] = fabs(pWaveDir - plistHydroData.at(i).at(j).getWaveDir());

            //Check distance.
            if ((dist[0] < dist[1]) &&
                    (dist[0] < DISTMAX))
            {
                dist[1] = dist[0];
                index[0] = j;
            }
        }

        //Check to see if there is any point in going forward.
        if (index[0] != -1)
        {
            //Reset distance counters.
            dist[0] = 1E6;
            dist[1] = 1E6;

            //Calculate the distance for the second point.
            for (int j = 0; j < plistHydroData.at(i).size(); j++)
            {
                //Calculate distance for second point.
                dist[0] = fabs(pWaveDir - plistHydroData.at(i).at(j).getWaveDir());

                //Check distance.
                if ((dist[0] < dist[1]) &&
                        (dist[0] < DISTMAX) &&
                        (j != index[0]))
                {
                    dist[1] = dist[0];
                    index[1] = j;
                }
            }

            //Now found index of the two closest points.  create them in the subset vector.
            //Create vector that will be added to the list of subsets.
            vector<ofreq::hydroData*> tempWaveDir;
            tempWaveDir.push_back(&(plistHydroData.at(i).at(index[0])));
            tempWaveDir.push_back(&(plistHydroData.at(i).at(index[1])));

            //Add the vector to the total list of subsets.
            plistDirSubset.push_back(tempWaveDir);
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
double sysHydro::waveScale(double ampIn, double amp1, double Data1, double amp2, double Data2)
{
    //First check if Data2 was supplied.
    if ((amp2 == 0) &&
            (Data2 == 0))
    {
        //Linear Wave scaling.
        double A;       //Wave scale factor

        //Calculate scale factor
        A = ampIn / amp1;

        //Scale and write output
        return A * Data1;
    }
    else
    {
        //Power function wave scaling
        double A;       //Wave scale factor
        double n;       //Wave scale exponent.

        //Calculate exponent n.
        n = log(Data2 / Data1) / log(amp2 / amp1);

        //Calcuate amplitude A
        A = Data1 / pow(amp1, n);

        //Scale and write output.
        return A * pow(ampIn, n);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<int> sysHydro::findMatchAmplitude(double ampIn)
{
    vector<int> output;         //The output vector.

    //Check if the subset is only one size.
    if (plistDirSubset.size() == 1)
    {
        //Yes.  Return that index.
        output.push_back(0);
        return output;
    }
    else
    {
        //No.  Find the closest match.
        double dist[2];         //The search terms.

        dist[0] = 1E6;
        dist[1] = 1E6;

        //Add a term to the vector.
        output.push_back(0);

        //Search through the list and find the closest match.
        for (int i = 0; i < plistDirSubset.size(); i++)
        {
            dist[0] = fabs(ampIn - plistDirSubset.at(i).at(0).getWaveAmp());

            if (dist[0] < dist[1])
            {
                dist[1] = dist[0];
                output[0] = i;
            }
        }

        //Reset distance indicators.
        dist[0] = 1E6;
        dist[1] = 1E6;

        //Add another index.
        output.push_back(1);

        //Search through the list and find the second closest match.
        for (int i = 0; i < plistDirSubset.size(); i++)
        {
            dist[0] = fabs(ampIn - plistDirSubset.at(i).at(0).getWaveAmp());

            if (dist[0] < dist[1] &&
                    (i != output[0]))
            {
                dist[1] = dist[0];
                output[1] = i;
            }
        }

        //Return result.
        return output;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
hydroData sysHydro::interpWaveDir(ofreq::hydroData &Data1, ofreq::hydroData &Data2)
{
    //Interpolates the two data sets for the wave direction defined within pWaveDir.

    //Create output object
    hydroData output;

    //Set the wave amplitude.
    output.setWaveAmp(
                (Data1.getWaveAmp() + Data2.getWaveAmp()) / 2
                );

    //Set the hydrobody name.
    output.setHydroBodyName(
                Data1.getHydroBodyName());

    //Set the wave direction to the interpolation result.
    output.setWaveDir(
                this->pWaveDir);

    //Set the wave frequency.
    output.addWaveFreq(
                Data1.listWaveFreq(0));

    //Calculate the interpolation fraction.
    double frac = (output.getWaveDir() - Data1.getWaveDir()) /
                  (Data2.getWaveDir() - Data1.getWaveDir());

    //Interpolate the active force.
    matForceActive interpActive =
            iePolate(output.getWaveDir(),
                     Data1.getWaveDir(),
                     Data2.getWaveDir(),
                     Data1.listDataActive(0),
                     Data2.listDataActive(0)
                     );
    //Add to the output.
    output.addDataActive(interpActive);

    //Interpolate the reactive force.
    matForceReact interpReact =
            iePolate(output.getWaveDir(),
                     Data1.getWaveDir(),
                     Data2.getWaveDir(),
                     Data1.listDataReact(0),
                     Data2.listDataReact(0)
                     );
    //Add to the output.
    output.addDataReact(interpReact);

    for (int i = 0; i < Data1.listDataCross().at(0).size(); i++)
    {
        //Get hydrobody name from first data set.
        std::string Name = Data1.listDataCross().at(i).getLinkedName();

        //Interpolate the two hydrodata sets.
        matForceCross interpCross =
                iePolate(output.getWaveDir(),
                         Data1.getWaveDir(),
                         Data2.getWaveDir(),
                         Data1.listDataCross(0,Name),
                         Data2.listDataCross(0,Name)
                         );

        //Set the name.
        interpCross.setLinkedName(Name);

        //Add the interpolated object to the hydrodata set.
        output.addDataCross(interpCross,0);
    }

    //Write out the output.
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
hydroData sysHydro::ScaleHydroData(double ampIn, ofreq::hydroData &Data1, ofreq::hydroData &Data2 = NULL)
{
    //Iterate through each object in the wave set and scale that object.
}
