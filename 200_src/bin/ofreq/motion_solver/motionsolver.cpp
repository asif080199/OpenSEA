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

#include "motionsolver.h"

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
MotionSolver::MotionSolver()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
MotionSolver::MotionSolver(vector<matBody> listBodIn)
{
    for (unsigned int i; i < listBodIn.size(); i++)
    {
        this->addBody(listBodIn[i]);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
MotionSolver::~MotionSolver()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionSolver::addBody(matBody bodIn)
{
    this->plistBody.push_back(bodIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact MotionSolver::sumReactSet(vector<matForceReact> listForces)
{
    //Create output object.
    matForceReact output;

    //Iterate through the list and Sum the forces in each item.
    for (unsigned int i; i < listForces.size(); i++)
    {
        output = output + listForces[i];
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matForceCross> MotionSolver::sumCrossSet(vector<matForceCross> listForces)
{
    //Create output object
    vector<matForceCross> output(this->plistBody.size());

    //Run through the list and set the link id to the index of the cell.
    for (unsigned int i = 0; i < output.size(); i++)
    {
        output[i].setLinkedId(i);
    }

    //Go through the list.  For each entry in the list, also run through
    //the full forces list and add all forces to each entry.
    //The class for matForceCross automatically handles the filtering
    //to ensure only the correct items are added together.
    //(those with matching linkedid's).
    for (unsigned int i = 0; i < output.size(); i++)
    {
        for (unsigned int j = 0; j < listForces.size(); j++)
        {
            output[i] = output[i] + listForces[j];
        }
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat MotionSolver::sumActiveSet(vector<matForceActive> listForces)
{
    int MaxMatrixSize;
    //Get max size of matrix
    for (unsigned int i = 0; i < listForces.size(); i++)
    {
        if (MaxMatrixSize > listForces[i].getMatSize())
        {
            MaxMatrixSize = listForces[i].getMatSize();
        }
    }

    matForceActive tempOutput;
    cx_mat singleForceMarix(MaxMatrixSize,1);

    //Sum each force entry.
    for(unsigned int i = 0; i < listForces.size(); i++)
    {
        tempOutput = tempOutput + listForces[i];
    }

    singleForceMarix = tempOutput.listCoefficients();

    return singleForceMarix;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat MotionSolver::sumDerivative(matForceReact forceIn)
{
    //complex version of current wave frequency.
    complexDouble waveFreq(this->curWaveFrequency, 0.0);
    //Complex version of imaginary variable i.
    complexDouble imagI(0.0, 1.0);
    //complex scalar multiple.  Will calculate later.
    complexDouble scalarMult;

    cx_mat output;

    //resize output matrix
    output.zeros(forceIn.getMatSize());

    //Iterate through each derivative order and add them together.
    for (int i = 0 ; i <= forceIn.getMaxOrder() ; i++)
    {
        //Calculate scalar product.
        scalarMult = pow(waveFreq, i) * pow(imagI, i);

        //Multiply through to derivative terms and add to total.
        for (unsigned int row = 0 ; row < forceIn.getDerivative(i).n_rows ; row++)
        {
            for (unsigned int col = 0 ; col < forceIn.getDerivative(i).n_cols ; col++)
            {
                output.at(row,col) += scalarMult * forceIn.getDerivative(i).at(row,col);
            }
        }
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<cx_mat> MotionSolver::sumDerivative(vector<matForceCross> forceIn)
{
    //complex version of current wave frequency.
    complexDouble waveFreq(this->curWaveFrequency, 0.0);
    //Complex version of imaginary variable i.
    complexDouble imagI(0.0, 1.0);
    //complex scalar multiple.  Will calculate later.
    complexDouble scalarMult;

    vector<cx_mat> output;

    for (unsigned int x = 0 ; x < forceIn.size() ; x++)
    {
        //resize output matrix
        output[x].zeros(forceIn[x].getMatSize());

        //Iterate through each derivative order and add them together.
        for (int i = 0 ; i <= forceIn[x].getMaxOrder() ; i++)
        {
            //Calculate scalar product.
            scalarMult = pow(waveFreq, i) * pow(imagI, i);

            //Multiply through to derivative terms and add to total.
            for (unsigned int row = 0 ; row < forceIn[x].getDerivative(i).n_rows ; row++)
            {
                for (unsigned int col = 0 ; col < forceIn[x].getDerivative(i).n_cols ; col++)
                {
                    output[x].at(row,col) += scalarMult * forceIn[x].getDerivative(i).at(row,col);
                }
            }
        }
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionSolver::setWaveFreq(double freqIn)
{
    //Input wave frequency.
    curWaveFrequency = freqIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionSolver::calculateOutputs()
{
    //create a temporary vector to hold outputs of summations - user forces
    vector<matForceReact> tempReactList_usr(plistBody.size());
    vector<cx_mat> ActiveList_usr(plistBody.size());
    vector < vector<matForceCross> > tempCrossList_usr(plistBody.size(), vector<matForceCross>(plistBody.size()));

    //create a temporary vector to hold outputs of summations - hydro forces
    vector<matForceReact> tempReactList_hydro(plistBody.size());
    vector<cx_mat> ActiveList_hydro(plistBody.size());
    vector < vector<matForceCross> > tempCrossList_hydro(plistBody.size(), vector<matForceCross>(plistBody.size()));

    //Sum forces for each body in the list.
    for (unsigned int i = 0; i< plistBody.size(); i++)
    {
        //Add user forces.
        tempReactList_usr[i] = sumReactSet(plistBody[i].listForceReact_user());
        ActiveList_usr[i] = sumActiveSet(plistBody[i].listForceActive_user());
        tempCrossList_usr[i] = sumCrossSet(plistBody[i].listForceCross_user());

        //Add hydro forces.
        tempReactList_hydro[i] = sumReactSet(plistBody[i].listForceReact_hydro());
        ActiveList_hydro[i] = sumActiveSet(plistBody[i].listForceActive_hydro());
        tempCrossList_hydro[i] = sumCrossSet(plistBody[i].listForceCross_hydro());
    }
    //Each set now reduced to a single force object, for each body.

    //Create temporary matrices to hold outputs of derivative summation - user forces
    vector<cx_mat> ReactList_usr;
    // No need to Sum for active forces.  Already independent of derivative.
    vector< vector<cx_mat> > CrossList_usr;

    //Create temporary matrices to hold outputs ot derivative summation - hydro forces
    vector<cx_mat> ReactList_hydro;
    // No need to Sum for active forces.  Already independent of derivative.
    vector< vector<cx_mat> > CrossList_hydro;

    //Sum derivatives within each force.
    for (unsigned int i = 0; i < plistBody.size(); i++)
    {
        //Sum for user forces
        ReactList_usr.push_back(sumDerivative(tempReactList_usr[i]));
        CrossList_usr.push_back(sumDerivative(tempCrossList_usr[i]));

        //Sum for hydro forces
        ReactList_hydro.push_back(sumDerivative(tempReactList_hydro[i]));
        CrossList_hydro.push_back(sumDerivative(tempCrossList_hydro[i]));
    }

    //Delete uneeded variables
    tempReactList_usr.clear();
    tempCrossList_usr.clear();
    tempReactList_hydro.clear();
    tempCrossList_hydro.clear();

    //Create temporary variables for total of all forces.
    vector<cx_mat> ReactList;
    vector<cx_mat> ActiveList;
    vector< vector<cx_mat> > CrossList;
    vector<cx_mat> tempCross;

    //Add different force types for each body object.
    for (unsigned int i = 0; i < plistBody.size(); i++)
    {
        ReactList.push_back(ReactList_usr[i] + ReactList_hydro[i]);
        ActiveList.push_back(ActiveList_usr[i] + ActiveList_hydro[i]);
        for (unsigned int j = 0; j < plistBody.size(); j++)
        {
            tempCross.push_back(CrossList_usr[i][j] + CrossList_hydro[i][j]);
        }
        CrossList.push_back( tempCross );
        tempCross.clear();
    }

    //Delete uneeded variables
    ReactList_usr.clear();
    ActiveList_usr.clear();
    CrossList_usr.clear();
    ReactList_hydro.clear();
    ActiveList_hydro.clear();
    CrossList_hydro.clear();
    tempCross.clear();

    //"Assemble Global Matrix"
    vector<int> matStart;
    vector<int> matEnd;
    if(plistBody.size() == 1) //Single Body Matrix
    {
        //Dont include cross body forces if only 1 matrix and just use first index of vector
        globReactiveMat = ReactList[0]; //A Matrix
        globActiveMat = ActiveList[0];     //F Matrix

        //Create values for matStart and matEnd
        matStart.push_back(0);
        matEnd.push_back(ActiveList[0].n_rows);
    }
    else //Multiple Bodies, must resize the matrices
    {
        int newMatrixSize = 0;

        for(unsigned int j = 0; j < plistBody.size(); j++)
        {
            //get the size of new matrix by summing all reactiveForceMatrices
            newMatrixSize += ReactList[j].n_rows;

            //Add the sizes to get the starting position of each matrix
            if (j == 0) {
                matStart.push_back(0);
            }
            else
            {
                matStart.push_back(matStart[j-1] + ReactList[j].n_rows + 1);
            }

            //Add the sizes to get the ending positions of each matrix.
            matEnd.push_back(matStart[j] + ReactList[j].n_rows);
        }

        //A Matrix
        //resize matrix to include all bodies reactiveforces & cross Body Forces, matrix must be square (ex. 6x6)
        //At the same time, fill matrix with zero values.
        globReactiveMat.zeros(newMatrixSize, newMatrixSize);
        //F Matrix
        globActiveMat.zeros(newMatrixSize, 1); //column Matrix

        for (unsigned int j = 0; j < plistBody.size(); j++)
        {
            //Iterate through and insert the reactive force matrices for each body
            globReactiveMat.submat(matStart[j], matStart[j], matEnd[j], matEnd[j]) = ReactList[j];

            //Iterate through and insert the active force matrices for each body
            globActiveMat.submat(matStart[j], 0, matEnd[j], 0) = ActiveList[j];
        }

        //Add in the cross body forces for each body.
        //Iterate through each body.
        for (unsigned int i = 0; i < plistBody.size(); i++)
        {
            //Iterate through the cross-body forces listed for each body
            for(unsigned int j = 0; j < plistBody.size(); j++)
            {
                globReactiveMat.submat(matStart[i], matStart[j], matEnd[i], matEnd[j]) = CrossList[i][j];
            }
        }
    }

    //Solve for Unknown Matrix (the X Matrix) --    A*X=B where X is the unknown
    globSolnMat = solve(globReactiveMat, globActiveMat, true); //true arg for more precise calculations

    //Split into vector of solutions, each represents per body
    for(unsigned int i = 0; i < plistBody.size(); i++)
    {
        plistSolution.push_back(globSolnMat.submat(matStart[i], 0, matEnd[i], 0));
    }
}


//------------------------------------------Function Separator --------------------------------------------------------
vector<cx_mat> &MotionSolver::listSolution()
{
    return plistSolution;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat &MotionSolver::listSolution(unsigned int bod)
{
    //Check if outside bounds
    if (bod < plistSolution.size())
    {
        return plistSolution[bod];
    }
    else
    {
        //Throw and error
        cx_mat out;
        out.zeros();
        return out;
    }
}

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions
