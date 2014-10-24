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

//==========================================Section Separator =========================================================
//Inclusions and namespaces
#include "seamodel.h"

using namespace std;
using namespace osea;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
SeaModel::SeaModel()
{
    //Set starting value sort sorting
    SortedFreq = false;
    SortedDir = false;
}

//------------------------------------------Function Separator --------------------------------------------------------
SeaModel::~SeaModel()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<double> &SeaModel::listWaveDir()
{
    return plistWaveDir;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &SeaModel::listWaveDir(int index)
{
    return plistWaveDir.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<osea::WaveSpecBase*> &SeaModel::listWaveSpec()
{
    return plistWaveSpec;
}

//------------------------------------------Function Separator --------------------------------------------------------
osea::WaveSpecBase* &SeaModel::listWaveSpec(int index)
{
    return plistWaveSpec.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<double> &SeaModel::listWaveFreq()
{
    return plistWaveFreq;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &SeaModel::listWaveFreq(int index)
{
    return plistWaveFreq.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<double> &SeaModel::listWaveScale()
{
    return plistWaveScale;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &SeaModel::listWaveScale(int index)
{
    return plistWaveScale.at(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
void SeaModel::addWaveDir(double dirIn)
{
    plistWaveDir.push_back(dirIn);

    //Reset sorted variable
    SortedDir = false;
}

//------------------------------------------Function Separator --------------------------------------------------------
void SeaModel::addWaveSpec(WaveSpecBase *specIn)
{
    plistWaveSpec.push_back(specIn);

    //Reset sorted variable
    SortedDir = false;

    //Also add to the wave scaling list.
    if (plistWaveScale.size() < plistWaveSpec.size())
    {
        //Add to the wave scale list.
        plistWaveScale.push_back(1);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void SeaModel::addWaveFreq(double freqIn)
{
    //Reset the sorted variable
    SortedFreq = false;

    plistWaveFreq.push_back(freqIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void SeaModel::addWaveFreq(std::vector<double> freqIn)
{
    //Reset the sorted variable
    SortedFreq = false;

    //Clear the list of wave frequencies.
    plistWaveFreq.clear();

    //Add the list of wave frequencies
    plistWaveFreq = freqIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void SeaModel::addWaveScale(double scaleIn)
{
    //Reset sorted variable.
    SortedDir = false;

    //Check if the list sizes match.
    if (plistWaveScale.size() == plistWaveSpec.size())
    {
        //Sizes match.  Just override the last index.
        plistWaveScale.at(plistWaveScale.size() - 1) = scaleIn;
    }
    else
    {
        //Sizes don't match.  Add another to the index.
        plistWaveScale.push_back(scaleIn);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string SeaModel::getName()
{
    return pName;
}

//------------------------------------------Function Separator --------------------------------------------------------
void SeaModel::setName(std::string nameIn)
{
    pName = nameIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string &SeaModel::refName()
{
    return pName;
}

//------------------------------------------Function Separator --------------------------------------------------------
double SeaModel::getWaveEnergy(double dirIn, int freqIndex)
{
    try
    {
        //Get the wave frequency.
        double freqIn = plistWaveFreq.at(freqIndex);

        //Calculate the result
        return getWaveEnergy(dirIn, freqIn);
    }
    catch (const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  ") + err.what());
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
double SeaModel::getWaveEnergy(double dirIn, double freqIn)
{
    //First check if the frequencies and wave directions are sorted.

    if (!SortedFreq)
    {
        //Sort wave frequencies
        SortWaveFreq();
        SortedFreq = true;
    }

    if (!SortedDir)
    {
        //Sort wave directions
        SortWaveDir();
        SortedDir = true;
    }

    //Next check if the list of wave directions is greater than one.
    if (plistWaveDir.size() > 1)
    {
        //Greater than one.  Interpolate between items in the list.
        //Next, get the index of the wave direction which is closest to the requested direction.
        double dist[2];                //The distance of separation for the closest index.
        int indexDir[2];                   //The index of the wave direction closest.

        dist[0] = 1E6;
        dist[1] = 1E6;

        for (int i = 0; i < plistWaveDir.size(); i++)
        {
            //Find the index closest to the requested wave direction.
            dist[0] = abs(dirIn - plistWaveDir[i]);

            //Check for the closest dist.
            if (dist[0] < dist[1])
            {
                dist[1] = dist[0];
                indexDir[0] = i;
            }
        }

        //Check for the second closest distance.  It must be the index either above or below the closest.
        dist[0] = abs(dirIn - plistWaveDir[indexDir[0] - 1]);
        dist[1] = abs(dirIn - plistWaveDir[indexDir[0] + 1]);

        if (dist[0] < dist[1])
            indexDir[1] = indexDir[0] - 1;
        else
            indexDir[1] = indexDir[0] + 1;

        //Get the wave energy from the respective wave spectra
        //Include the scaling multipliers.
        double WaveEnergy[2];

        WaveEnergy[0] = plistWaveScale[indexDir[0]] * plistWaveSpec.at(indexDir[0])->getSpecEnergy(freqIn);
        WaveEnergy[1] = plistWaveScale[indexDir[1]] * plistWaveSpec.at(indexDir[1])->getSpecEnergy(freqIn);


        //Interpolate between the results of the two wave spectra to get the result.
        //First check if the requested wave direction is exactly on top of either direction.
        if (dirIn == plistWaveDir[indexDir[0]])
        {
            //Just return the result of the first wave direction.
            return WaveEnergy[0];
        }

        else if (dirIn == plistWaveDir[indexDir[1]])
        {
            //Just return the result of the second wave direction.
            return WaveEnergy[1];
        }

        else
        {
            //Interpolate between the two results.
            //Use simple linear interpolation.
            double slope;           //The slope of the line.
            double intercept;       //The y-intercept of the two results.

            //Get the slope
            slope = (WaveEnergy[1] - WaveEnergy[0]) / (plistWaveDir[indexDir[0]] - plistWaveDir[indexDir[1]]);

            //Get the y-intercept.
            intercept = WaveEnergy[0] - slope * plistWaveDir[indexDir[0]];

            //Calculate the result.
            return slope * dirIn + intercept;
        }
    }

    else
    {
        //Not greater than one.  Just return the value of wave spectra for that item.
        return plistWaveScale[0] * plistWaveSpec.at(0)->getSpecEnergy(freqIn);
    }
}


//------------------------------------------Function Separator --------------------------------------------------------
double SeaModel::getWaveAmp(double dirIn, int freqIndex)
{
    //First calculate the wave energy for the requested direction and frequency.
    double WaveEnergy = getWaveEnergy(dirIn, freqIndex);

    //Next get the frequency step for the defined frequency index.
    double stepsize = getFreqStep(freqIndex);

    //Finally, calculate the wave amplitude
    try
    {
        //Check for negative number.
        if (WaveEnergy < 0)
        {
            std::string msg;                    //Error message.

            msg = string("Interpolated wave energy was negative for the requested wave direction and") +
                  string("frequency.  Please check the sea model.\nWave Direction:  ");
                  std::to_string(dirIn) + string(" rad\n") + string("Wave Frequency:  ") +
                  std::to_string(plistWaveFreq[freqIndex]) + string(" rad/s");

            //Throw the error message.
            throw std::runtime_error(msg);
        }


        //Calculate wave amplitude
        return sqrt(2 * WaveEnergy * stepsize);
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(string("Object:  SeaModel, Function:  getWaveAmp()\nError Message:  ") + err.what());
    }
    catch(...)
    {
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  Unknown error occurred."));
    }
}


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
void SeaModel::SortWaveDir()
{
    //Sort the wave directions into numerical order.

    //First check if the list is larger than one.
    if (plistWaveDir.size() > 1)
    {
        //Larger than one.  Sort the list.

        int n = plistWaveDir.size() - 1;   //The size of the the list to sort.
        bool swapped = true;               //Boolean to track if any variables swapped.
        double swapDir;                    //Temporary variable to hold swapping of wave directions.
        osea::WaveSpecBase *swapSpec;           //Temporary variable to hold swapping of wave spectra.
        double swapScale;                   //Temporary variable to hold swapping of wave scaling terms.

        while (swapped)
        {
            swapped = false;            //Reset swap value
            for (int i = 0; i < n; i++)
            {
                if (plistWaveDir[i] > plistWaveDir[i+1])
                {
                    //Swap the wave directions
                    swapDir = plistWaveDir[i];
                    plistWaveDir[i] = plistWaveDir[i+1];
                    plistWaveDir[i+1] = swapDir;

                    //Swap the wave spectra
                    swapSpec = plistWaveSpec[i];
                    plistWaveSpec[i] = plistWaveSpec[i+1];
                    plistWaveSpec[i+1] = swapSpec;

                    //Swap the wave scaling terms
                    swapScale = plistWaveScale[i];
                    plistWaveScale[i] = plistWaveScale[i+1];
                    plistWaveScale[i+1] = swapScale;

                    //Recognize that swapping happend
                    swapped = true;
                }
            }
            n = n - 1;          //Lower the number of iterations for next time.
        }

        //Set the sorted parameter to true.
        SortedDir = true;
    }

    else
    {
        //Set the sorted parameter to true.
        SortedDir = true;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void SeaModel::SortWaveFreq()
{
    //First check if the list is larger than one.
    if (plistWaveFreq.size() > 1)
    {
        //Larger than one.  Sort the list.

        //Sort the wave frequencies into numerical order.
        int n = plistWaveFreq.size() - 1;   //The size of the the list to sort.
        bool swapped = true;               //Boolean to track if any variables swapped.
        double swapFreq;                    //Temporary variable to hold the swapping.

        while (swapped)
        {
            swapped = false;            //Reset swap value
            for (int i = 0; i < n; i++)
            {
                if (plistWaveFreq[i] > plistWaveFreq[i+1])
                {
                    //Swap the elements
                    swapFreq = plistWaveFreq[i];                //Temporary storage of larger variable.
                    plistWaveFreq[i] = plistWaveFreq[i+1];      //Push the upper one down one index.
                    plistWaveFreq[i+1] = swapFreq;              //Save temporary variable.

                    //Recognize that swapping happend
                    swapped = true;
                }
            }
            n = n - 1;          //Lower the number of iterations for next time.
        }

        //Set the sorted parameter to true.
        SortedFreq = true;
    }
    else
    {
        //Only one entry.  List is already sorted.

        //Set the sorted parameter to true.
        SortedFreq = true;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
double SeaModel::getFreqStep(int index)
{
    //Calculates the frequency step depending on which index is specified.

    try
    {
        //Check if the frequency list has two or more entries.
        if (plistWaveFreq.size() == 2)
        {
            //only two entries.  Very simple calculation.
            return (plistWaveFreq[1] - plistWaveFreq[0]) / 2;
        }

        else if (plistWaveFreq.size() > 2)
        {
            //Frequency list greater than two entries.
            //Three possible cases
            if (index == 0)
            {
                //Case for beginning of list.
                return (plistWaveFreq[1] - plistWaveFreq[0]) / 2;
            }

            else if ((index > 0) &&
                     (index < plistWaveFreq.size() - 1))
            {
                //Case for any frequency index in the middle of the list.
                return (plistWaveFreq[index + 1] - plistWaveFreq[index - 1]) / 2;
            }

            else if (index == (plistWaveFreq.size() - 1))
            {
                //Case for frequency index at the end of the list.
                return (plistWaveFreq[plistWaveFreq.size() - 1] - plistWaveFreq[plistWaveFreq.size() - 2]) / 2;
            }
        }

        else
        {
            //Not good.  This situation will not work.
            throw std::runtime_error("Frequency step requires a minimum of two frequencies definitions to calculate.");
        }
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(string("Object:  SeaModel, Function:  getFreqStep()\nError Message:  ") + err.what());
    }
    catch (...)
    {
        logStd.Notify();
        logErr.Write("Uknown Error:  Object:  SeaModel, Function:  getFreqStep()");
    }
}

//==========================================Section Separator =========================================================
//Private Functions

