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

#include "hydromanager.h"

using namespace arma;
using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static initialization
double HydroManager::DISTMAX = 0.7853981634;  //Set to 45 deg.

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
HydroManager::HydroManager()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
HydroManager::~HydroManager()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector< std::vector< hydroData > > &HydroManager::listHydroData()
{
    return plistHydroData;
}

//------------------------------------------Function Separator --------------------------------------------------------
hydroData &HydroManager::listHydroData(int ampInd, int dirInd)
{
    try
    {
        return plistHydroData.at(ampInd).at(dirInd);
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(err.what());
    }
    catch(...)
    {
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  Unknown error occurred."));
    }

}

//------------------------------------------Function Separator --------------------------------------------------------
void HydroManager::addHydroData(int ampInd, ofreq::hydroData dataIn)
{
    if (ampInd < 0)
    {
        //create new vector of hydrodata.
        vector<hydroData> datalist;
        datalist.push_back(dataIn);
        //no amp index specified.  Add new index.
        plistHydroData.push_back(datalist);
    }
    else
    {
        //amp index specified.  Add new wave direction at given amplitude index.
        plistHydroData.at(ampInd).push_back(dataIn);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void HydroManager::addHydroData(ofreq::hydroData dataIn)
{
    this->addHydroData(-1, dataIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string HydroManager::getHydroBodyName()
{
    return pHydroBodyName;
}

//------------------------------------------Function Separator --------------------------------------------------------
void HydroManager::setHydroBodyName(std::string nameIn)
{
    pHydroBodyName = nameIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void HydroManager::calcHydroData(double waveAmp, double waveFreq)
{      
    try
    {
        //Start by reseting the outputs.
        ResetOutput();

        //Get the indices of the two wave amplitudes that match the set amplitude.
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
            for (unsigned long i = 0; i < plistDirSubset.at(indWaveAmp[0]).size(); i++)
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
                DataAmp2.push_back(
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
                    DataAmp2.push_back(
                                plistDirSubset.at(indWaveAmp[1]).at(i)->interpHydroData(waveFreq)
                            );
                }
            }
        }

        //Successfully interpolated all four data sets for wave frequency.  Now interpolate for wave direction.
        if (!(DataAmp2.size() > 0))
        {
            //Only one wave amplitude defined.  Proceed with that.
            hydroData DataDir1;

            //First wave amplitude.
            //Interpolate if there is more than one wave direction.
            if (DataAmp1.size() > 1)
            {
                //Interpolate the given wave directions.
                DataDir1 =
                        interpWaveDir(
                            DataAmp1[0],
                            DataAmp1[1]);
            }
            else
            {
                DataDir1 = DataAmp1[0];
            }

            //Free up memory and clear the vector.
            DataAmp1.clear();

            //Now proceed to wave scaling.
            //Write output.
            pHydroFinal = ScaleHydroData(waveAmp, DataDir1);
        }
        else
        {
            //Two wave amplitudes.  Go forward with that.
            hydroData DataDir1;
            hydroData DataDir2;

            //First wave amplitude.
            //Interpolate if there is more than one wave direction.
            if (DataAmp1.size() > 1)
            {
                //Interpolate the given wave directions.
                DataDir1 =
                        interpWaveDir(
                            DataAmp1[0],
                            DataAmp1[1]);
            }
            else
            {
                DataDir1 = DataAmp1[0];
            }

            //Free up memory and clear the vector.
            DataAmp1.clear();

            //Second wave amplitude.
            //Interpolate if there is more than one wave direction.
            if (DataAmp2.size() > 1)
            {
                //Interpolate the given wave directions.
                DataDir2 =
                        interpWaveDir(
                            DataAmp2[0],
                            DataAmp2[1]);
            }
            else
            {
                DataDir2 = DataAmp2[0];
            }

            //Free up memory and clear the vector.
            DataAmp2.clear();

            //Now proceed to wave scaling.
            //Write output
            pHydroFinal = ScaleHydroData(waveAmp, DataDir1, DataDir2);
        }
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  ") + string(err.what()));
        exit(1);
    }
    catch(...)
    {
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  Unknown error occurred."));
        exit(1);
    }

}

//------------------------------------------Function Separator --------------------------------------------------------
ForceActive *HydroManager::getForceActive()
{
    //Create the forceActive object from the interpolated set of hydrodata.

    //Create output
    ptForceActive = new ForceActive();

    for (int i = 0; i < pHydroFinal.listDataActive(0).listCoefficient().n_rows; i++)
    {
        //Add the coefficient to the output.
        ptForceActive->addEquation(
                    pHydroFinal.listDataActive(0).listCoefficient()(i,0),
                    i);
    }

    //Write output
    return ptForceActive;
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceReact *HydroManager::getForceReact()
{
    //Create ptForceReact
    ptForceReact = new ForceReact();

    //Iterate through each order of derivative.
    for (int k = 0; k <= pHydroFinal.listDataReact(0).getMaxOrder(); k++)
    {
        //Create new derivative object.
        Derivative tempDeriv;

        //Iterate through each equation.
        for (int i = 0; i < pHydroFinal.listDataReact(0).getMatSize(); i++)
        {
            //Create new vector of variables.
            vector<double> listCoeff;

            //Iterate through each coefficient in the equation.
            for (int j = 0; j < pHydroFinal.listDataReact(0).getMatSize(); j++)
            {
                //Fill the vector with variables.
                listCoeff.push_back(
                            real(pHydroFinal.listDataReact(0).
                                 listDerivative(k)(i,j)
                                )
                            );
            }

            //Add the vector to the list.
            tempDeriv.addModelEquation(listCoeff, i);
        }

        //Add to the list.
        ptForceReact->addDerivative(tempDeriv, k);
    }

    //Write ptForceReact.
    return ptForceReact;
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceCross *HydroManager::getForceCross(std::string linkedBodName)
{
    //Test if the specified body name exists.
    bool test = false;      //Tests if a matching body name was found.
    for (int i = 0; i < pHydroFinal.listDataCross().at(0).size(); i++)
    {
        if (pHydroFinal.listDataCross(0,i).getLinkedName() == linkedBodName)
        {
            test = true;
            break;
        }
    }

    //if no match was found, throw an error.
    if (!test)
        throw std::invalid_argument("Bodyname not found.");

    //If body name was found, proceed to locating and returning it.

    //Create ptForceCross
    ptForceCross = new ForceCross();

    //Iterate through each order of derivative.
    for (int k = 0; k <= pHydroFinal.listDataCross(0, linkedBodName).getMaxOrder(); k++)
    {
        //Create new derivative object.
        Derivative tempDeriv;

        //Iterate through each equation.
        for (int i = 0; i < pHydroFinal.listDataCross(0,linkedBodName).getMatSize(); i++)
        {
            //Create new vector of variables.
            vector<double> listCoeff;

            //Iterate through each coefficient in the equation.
            for (int j = 0; j < pHydroFinal.listDataCross(0,linkedBodName).getMatSize(); j++)
            {
                //Fill the vector with variables.
                listCoeff.push_back(
                            real(pHydroFinal.listDataCross(0,linkedBodName)
                                 .getDerivative(k)(i,j)
                                )
                            );
            }

            //Add the vector to the list.
            tempDeriv.addModelEquation(listCoeff, i);
        }

        //Add to the list.
        ptForceCross->addDerivative(tempDeriv, k);
    }

    //Set the name for the linked body.
    ptForceCross->setForceName(linkedBodName);

    //Write ptForceCross.
    return ptForceCross;
}

//------------------------------------------Function Separator --------------------------------------------------------
void HydroManager::setWaveDir(double dirIn)
{
    pWaveDir = checkAngle(dirIn);

    //Reset the outputs
    ResetOutput();

    //Check the list for any need to duplicate.
    this->checkDirList();

    //Build the wave subset
    this->buildWaveSubset();
}

//------------------------------------------Function Separator --------------------------------------------------------
double HydroManager::getWaveDir()
{
    return pWaveDir;
}

//------------------------------------------Function Separator --------------------------------------------------------
void HydroManager::setGravity(std::vector<double> gravIn)
{
    pGravity = gravIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<double> HydroManager::getGravity()
{
    return pGravity;
}


//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
void HydroManager::buildWaveSubset()
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

        try
        {
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

            //Check if none of the distances were within the allowed range.
            if (dist[1] > DISTMAX)
                throw std::runtime_error("No data sets were within the allowed distance from the specified wave direction.");
        }
        catch(const std::exception &err)
        {
            logStd.Notify();
            logErr.Write(string(ID) + string(">>  ") + string(err.what()));
            break;
        }
        catch(...)
        {
            logStd.Notify();
            logErr.Write(string(ID) + string(">>  Unknown error occurred."));
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
double HydroManager::waveScale(double ampIn, double amp1, double Data1, double amp2, double Data2)
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
complex<double> HydroManager::waveScale(double ampIn, double amp1, std::complex<double> Data1, double amp2,
                 std::complex<double> Data2)
{
    //Get the phase angle
    double phase = arg(Data1);

    //Get the amplitudes.
    double Amps[2];
    Amps[0] = abs(Data1);

    if (amp2 == 0)
        Amps[1] = 0;
    else
        Amps[1] = abs(Data2);

    double ampOut;

    //Scale the amplitude of the complex number.
    if (amp2 == 0)
        ampOut = waveScale(ampIn, amp1, Amps[0]);
    else
        ampOut = waveScale(ampIn, amp1, Amps[0], amp2, Amps[1]);

    //Recombine into a single complex number.
    complex<double> output;

    output.real(ampOut * cos(phase));
    output.imag(ampOut * sin(phase));

    //Write output.
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceActive HydroManager::waveScale(double ampIn, double amp1, ofreq::matForceActive &mat1)
{
    //Create output object
    matForceActive output;

    //Resize the output matrix.
    output.listCoefficient().set_size(mat1.listCoefficient().n_rows, mat1.listCoefficient().n_cols);

    //Iterate through each entry in the matrix
    try
    {
        for (int i = 0; i < mat1.listCoefficient().n_rows; i++)
        {
            for (int j = 0; j < mat1.listCoefficient().n_cols; j++)
            {
                //Perform linear wave scaling
                output.listCoefficient()(i,j) =
                        waveScale(ampIn,
                                  amp1,
                                  mat1.listCoefficient()(i,j)
                                  );
            }
        }
    }
    catch(const std::exception &err)
    {
        logErr.Write(string(err.what()));
        logStd.Notify();
    }
    catch (...)
    {
        //Notify
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  Unknown error occurred."));
    }

    //Write output.
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceActive HydroManager::waveScale(double ampIn, double amp1, ofreq::matForceActive &mat1, double amp2,
                                ofreq::matForceActive &mat2)
{
    //Create output object
    matForceActive output;

    //Resize the output matrix.
    output.listCoefficient().set_size(mat1.listCoefficient().n_rows, mat1.listCoefficient().n_cols);

    //Iterate through each entry in the matrix
    try
    {
        //Check that both matrices are the same size.
        if ((mat1.listCoefficient().n_rows != mat2.listCoefficient().n_rows)
                || (mat1.listCoefficient().n_cols != mat2.listCoefficient().n_cols))
            throw std::runtime_error("Matrix sizes do not match.");

        for (int i = 0; i < mat1.listCoefficient().n_rows; i++)
        {
            for (int j = 0; j < mat1.listCoefficient().n_cols; j++)
            {
                //Perform non-linear wave scaling.
                output.listCoefficient()(i,j) =
                        waveScale(ampIn,
                                  amp1,
                                  mat1.listCoefficient()(i,j),
                                  amp2,
                                  mat2.listCoefficient()(i,j)
                                  );
            }
        }

        //Write output.
        return output;
    }
    catch(const std::exception &err)
    {
        logErr.Write(string(err.what()));
        logStd.Notify();
    }
    catch (...)
    {
        //Notify
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  Unknown error occurred."));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact HydroManager::waveScale(double ampIn, double amp1, ofreq::matForceReact &mat1)
{
    //Create output
    matForceReact output;

    try {
        for (int k = 0; k <= mat1.getMaxOrder(); k++)
        {
            //Create a new matrix to add results to.
            arma::cx_mat result(mat1.getMatSize(),
                                     mat1.getMatSize(),
                                     fill::zeros);

            for (int i = 0; i < mat1.getDerivative(k).n_rows; i++)
            {
                for (int j = 0; j < mat1.getDerivative(k).n_cols; j++)
                {
                    //Perform linear wave scaling
                    result(i,j) =
                            waveScale(ampIn,
                                      amp1,
                                      mat1.getDerivative(k)(i,j)
                                      );
                }
            }

            //Add matrix into object.
            output.listDerivative().push_back(result);
        }

        //Write output.
        return output;
    }
    catch(const std::exception &err)
    {
        logErr.Write(string("Error. Object:  HydroManager.  \nFunction:  waveScale(matForceReact) \n")
                     + string(err.what()));
        logStd.Notify();
    }
    catch (...)
    {
        //Notify
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  Unknown error occurred."));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact HydroManager::waveScale(double ampIn, double amp1, ofreq::matForceReact &mat1, double amp2,
                                ofreq::matForceReact &mat2)
{
    //Create output
    matForceReact output;

    try {
        //Check that the two objects have the same order of derivative and same matrix size.
        if ((mat1.getMaxOrder() != mat2.getMaxOrder())
                || (mat1.getMatSize() != mat2.getMatSize()))
        {
            throw std::runtime_error("Matrices do not match.");
        }

        for (int k = 0; k <= mat1.getMaxOrder(); k++)
        {
            //Create a new matrix to add results to.
            arma::cx_mat result(mat1.getMatSize(),
                                     mat1.getMatSize(),
                                     fill::zeros);

            for (int i = 0; i < mat1.getDerivative(k).n_rows; i++)
            {
                for (int j = 0; j < mat1.getDerivative(k).n_cols; j++)
                {
                    //Perform non-linear wave scaling.
                    result(i,j) =
                            waveScale(ampIn,
                                      amp1,
                                      mat1.getDerivative(k)(i,j),
                                      amp2,
                                      mat2.getDerivative(k)(i,j)
                                      );
                }
            }

            //Add matrix into object.
            output.listDerivative().push_back(result);
        }

        //Write output.
        return output;
    }
    catch(const std::exception &err)
    {
        logErr.Write(string("Error. Object:  HydroManager.  \nFunction:  waveScale(matForceReact) \n")
                     + string(err.what()));
        logStd.Notify();
    }
    catch (...)
    {
        //Notify
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  Unknown error occurred."));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceCross HydroManager::waveScale(double ampIn, double amp1, ofreq::matForceCross &mat1)
{
    //Create output
    matForceCross output;

    //Set linkedbody name for new matrix.
    output.setLinkedName(
                mat1.getLinkedName());

    try {
        for (int k = 0; k <= mat1.getMaxOrder(); k++)
        {
            //Create a new matrix to add results to.
            arma::cx_mat result(mat1.getMatSize(),
                                     mat1.getMatSize(),
                                     fill::zeros);

            for (int i = 0; i < mat1.getDerivative(k).n_rows; i++)
            {
                for (int j = 0; j < mat1.getDerivative(k).n_cols; j++)
                {
                    //Perform linear wave scaling
                    result(i,j) =
                            waveScale(ampIn,
                                      amp1,
                                      mat1.getDerivative(k)(i,j)
                                      );
                }
            }

            //Add matrix into object.
            output.listDerivative().push_back(result);
        }

        //Write output.
        return output;
    }
    catch(const std::exception &err)
    {
        logErr.Write(string("Error. Object:  HydroManager.  \nFunction:  waveScale(matForceCross) \n")
                     + string(err.what()));
        logStd.Notify();
    }
    catch (...)
    {
        //Notify
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  Unknown error occurred."));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceCross HydroManager::waveScale(double ampIn, double amp1, ofreq::matForceCross &mat1, double amp2,
                                ofreq::matForceCross &mat2)
{
    //Create output
    matForceCross output;

    //Set linkedbody name for new matrix.
    output.setLinkedName(
                mat1.getLinkedName());


    try {
        //Check that the two objects have the same order of derivative and same matrix size.
        if ((mat1.getMaxOrder() != mat2.getMaxOrder())
                || (mat1.getMatSize() != mat2.getMatSize()))
        {
            throw std::runtime_error("Matrices do not match.");
        }

        //Check that the two objects have the same linked body name.
        if (mat1.getLinkedName() != mat2.getLinkedName())
        {
            throw std::runtime_error("Linked body names of crossbody objects do not match.");
        }

        for (int k = 0; k <= mat1.getMaxOrder(); k++)
        {
            //Create a new matrix to add results to.
            arma::cx_mat result(mat1.getMatSize(),
                                     mat1.getMatSize(),
                                     fill::zeros);

            for (int i = 0; i < mat1.getDerivative(k).n_rows; i++)
            {
                for (int j = 0; j < mat1.getDerivative(k).n_cols; j++)
                {
                    //Perform non-linear wave scaling.
                    result(i,j) =
                            waveScale(ampIn,
                                      amp1,
                                      mat1.getDerivative(k)(i,j),
                                      amp2,
                                      mat2.getDerivative(k)(i,j)
                                      );
                }
            }

            //Add matrix into object.
            output.listDerivative().push_back(result);
        }
    }
    catch(const std::exception &err)
    {
        logErr.Write(string("Error. Object:  HydroManager.  \nFunction:  waveScale(matForceCross) \n")
                     + string(err.what()));
        logStd.Notify();
    }
    catch (...)
    {
        //Notify
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  Unknown error occurred."));
    }

    //Write output.
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<int> HydroManager::findMatchAmplitude(double ampIn)
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
            dist[0] = fabs(ampIn - plistDirSubset.at(i).at(0)->getWaveAmp());

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
            dist[0] = fabs(ampIn - plistDirSubset.at(i).at(0)->getWaveAmp());

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
hydroData HydroManager::interpWaveDir(ofreq::hydroData &Data1, ofreq::hydroData &Data2)
{
    //Interpolates the two data sets for the wave direction defined within pWaveDir.

    try
    {
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

        //Interpolate the active force and add to the output.
        output.addDataActive(
                    iePolate(output.getWaveDir(),
                         Data1.getWaveDir(),
                         Data2.getWaveDir(),
                         Data1.listDataActive(0),
                         Data2.listDataActive(0)
                         )
                    );

        //Interpolate the reactive force and add to the output.
        output.addDataReact(
                    iePolate(output.getWaveDir(),
                         Data1.getWaveDir(),
                         Data2.getWaveDir(),
                         Data1.listDataReact(0),
                         Data2.listDataReact(0)
                         )
                    );

        for (int i = 0; i < Data1.listDataCross().at(0).size(); i++)
        {
            //Get hydrobody name from first data set.
            std::string Name = Data1.listDataCross().at(0).at(i).getLinkedName();

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
            output.addDataCross(interpCross);
        }

        //Write out the output.
        return output;
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  ") + err.what());
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
hydroData HydroManager::ScaleHydroData(double ampIn, ofreq::hydroData &Data1)
{
    //Iterate through each object in the wave set and scale that object.
    try
    {
        //Create output object.
        hydroData output;

        //Set the wave amplitude.
        output.setWaveAmp(ampIn);

        //Set the hydrobody name.
        output.setHydroBodyName(
                    Data1.getHydroBodyName());

        //Set the wave direction to the interpolation result.
        output.setWaveDir(
                    this->pWaveDir);

        //Set the wave frequency.
        output.addWaveFreq(
                    Data1.listWaveFreq(0));

        //Scale the active force object.
        //Scale linearly.
        output.addDataActive(
                    waveScale(
                        ampIn,
                        Data1.getWaveAmp(),
                        Data1.listDataActive(0)
                        )
                    );

        //Scale the reactive force object.
        //Scale linearly
        output.addDataReact(
                    waveScale(
                        ampIn,
                        Data1.getWaveAmp(),
                        Data1.listDataReact(0)
                        )
                    );

        //Scale the crossbody force objects.
        for (int i = 0; i < Data1.listDataCross().at(0).size(); i++)
        {
            //Linear scaling
            output.addDataCross(
                        waveScale(
                            ampIn,
                            Data1.getWaveAmp(),
                            Data1.listDataCross(0,i)
                            )
                        );
        }

        //Write output
        return output;
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  ") + err.what());
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
hydroData HydroManager::ScaleHydroData(double ampIn, ofreq::hydroData &Data1, ofreq::hydroData &Data2)
{
    //Iterate through each object in the wave set and scale that object.

    try
    {
        //Create output object.
        hydroData output;

        //Set the wave amplitude.
        output.setWaveAmp(ampIn);

        //Set the hydrobody name.
        output.setHydroBodyName(
                    Data1.getHydroBodyName());

        //Set the wave direction to the interpolation result.
        output.setWaveDir(
                    this->pWaveDir);

        //Set the wave frequency.
        output.addWaveFreq(
                    Data1.listWaveFreq(0));

        //Scale the active force object.
        //Scale nonlinearly.
        output.addDataActive(
                    waveScale(
                        ampIn,
                        Data1.getWaveAmp(),
                        Data1.listDataActive(0),
                        Data2.getWaveAmp(),
                        Data2.listDataActive(0)
                        )
                    );

        //Scale the reactive force object.
        //Scale nonlinearly
        output.addDataReact(
                    waveScale(
                        ampIn,
                        Data1.getWaveAmp(),
                        Data1.listDataReact(0),
                        Data2.getWaveAmp(),
                        Data2.listDataReact(0)
                        )
                    );

        //Scale the crossbody force objects.
        for (int i = 0; i < Data1.listDataCross().at(0).size(); i++)
        {
            //Nonlinear wave scaling
            output.addDataCross(
                        waveScale(
                            ampIn,
                            Data1.getWaveAmp(),
                            Data1.listDataCross(0,i),
                            Data2.getWaveAmp(),
                            Data2.listDataCross(0,
                                                Data1.listDataCross(0,i).getLinkedName())
                            )
                        );
        }

        //Write output
        return output;
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(string(ID) + string(">>  ") + err.what());
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void HydroManager::checkDirList()
{
    for (unsigned int i = 0; i < plistHydroData.size(); i++)
    {
        //Each loop is a different wave amplitude.  Check through all directions in each loop.
        int entries = 0;        //The number of entries found at the zero rad area.
        int original;           //THe index of the original function.

        //Search through list of wave directions to see if a copy needs to be made.
        for (unsigned int j = 0; j < plistHydroData.at(i).size(); j++)
        {
            if (entries == 0)
            {
                double test1[3];
                test1[0] = cos(plistHydroData[i][j].getWaveDir());
                test1[1] = sin(plistHydroData[i][j].getWaveDir());
                //Searching for first entry.
                if((cos(plistHydroData[i][j].getWaveDir()) > 0.99)
                        && (sin(plistHydroData[i][j].getWaveDir()) < 0.04)
                        && (sin(plistHydroData[i][j].getWaveDir()) >= 0.00))
                {
                    entries += 1;
                    original = j;
                }
            }
            else
            {
                //Searching for second entry.
                //Add a conditional statement to ensure we don't count entries that are within a few
                //degrees of the first entry.
                if((plistHydroData[i][j].getWaveDir() > DISTMAX)
                        && (cos(plistHydroData[i][j].getWaveDir()) > 0.99)
                        && (sin(plistHydroData[i][j].getWaveDir()) < 0.04)
                        && (sin(plistHydroData[i][j].getWaveDir()) >= 0.00))
                    entries += 1;
            }
        }

        //Search complete.
        //If no entries found, do nothing.  This situation does not apply.
        //If one entry found, copy data over.
        //If more than one entry found, do nothing.
        if (entries == 1)
        {
            //Copy over data.
            plistHydroData[i].push_back(
                        plistHydroData[i][original]);

            //Change Wave Direction
            double dirNew = (2 * PI) - plistHydroData[i][original].getWaveDir();
            plistHydroData[i].back().setWaveDir(dirNew);
        }

    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void HydroManager::ResetOutput()
{
    //Delete the data on the heap.
    delete ptForceActive;
    delete ptForceReact;
    delete ptForceCross;

    ptForceActive = NULL;
    ptForceReact = NULL;
    ptForceCross = NULL;
}
