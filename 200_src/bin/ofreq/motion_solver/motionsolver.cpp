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

using namespace arma;
using namespace std;
using namespace osea::ofreq;

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
        this->addBody(listBodIn.at(i));
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
matForceReact *MotionSolver::sumReactSet(vector<matForceReact> listForces)
{
    //Check if the list of forces is empty
    if (listForces.size() == 0)
    {
        return NULL;
    }

    else
    {
        //Create output object.
        matForceReact *output = new matForceReact();

        //Iterate through the list and Sum the forces in each item.
        for (unsigned int i = 0; i < listForces.size(); i++)
        {
            *output = *output + listForces.at(i);
        }

        //Write output
        return output;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matForceCross *> MotionSolver::sumCrossSet(vector<matForceCross> listForces)
{
    vector<matForceCross *> output;     //The output from the function.

    //Resize output list.
    output.resize(plistBody.size(), NULL);

    //Check if the list of forces is empty
    if (listForces.size() == 0)
    {
        return output;
    }

    else
    {
        //Each entry in the list matches a body id.  If no forces are found that match
        //the body ID, leave that entry as NULL.

        for (unsigned int j = 0; j < plistBody.size(); j++)
        {
            for (unsigned int i = 0; i < listForces.size(); i++)
            {
                //Check if body id matches.
                if (listForces.at(i).getLinkedId() == plistBody.at(j).getId())
                {
                    matForceCross *ptOutput;

                    //Create a new output force and add it in at that specific entry.
                    if (output.at(j) == NULL)
                    {
                        ptOutput = new matForceCross();

                        //Assign to the output.
                        output.at(j) = ptOutput;

                        //Assign link id.
                        output.at(j)->setLinkedId(plistBody.at(j).getId());
                    }
                    else
                    {
                        ptOutput = output.at(j);
                    }

                    //Add the forces to the output.
                    *ptOutput = *ptOutput + listForces.at(i);
                }
            }
        }       

        //Write output.
        return output;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat *MotionSolver::sumActiveSet(vector<matForceActive> listForces)
{
    //Check for empty list of forces
    if (listForces.size() == 0)
    {
        return NULL;
    }

    else
    {
        int MaxMatrixSize;
        //Get max size of matrix
        for (unsigned int i = 0; i < listForces.size(); i++)
        {
            if (MaxMatrixSize > listForces.at(i).getMatSize())
            {
                MaxMatrixSize = listForces.at(i).getMatSize();
            }
        }

        matForceActive tempOutput;
        cx_mat* singleForceMatrix = new cx_mat(MaxMatrixSize,1);

        //Sum each force entry.
        for(unsigned int i = 0; i < listForces.size(); i++)
        {
            tempOutput = tempOutput + listForces.at(i);
        }

        *singleForceMatrix = tempOutput.listCoefficient();

        return singleForceMatrix;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat *MotionSolver::sumDerivative(matForceReact *forceIn)
{
    if (forceIn == NULL)
    {
        //No inputs.  Return NULL
        return NULL;
    }

    else
    {
        //complex version of current wave frequency.
        complexDouble waveFreq(this->curWaveFrequency, 0.0);
        //Complex version of imaginary variable i.
        complexDouble imagI(0.0, 1.0);
        //complex scalar multiple.  Will calculate later.
        complexDouble scalarMult;

        cx_mat *output = new cx_mat;

        //resize output matrix
        output->zeros(forceIn->getMatSize(), forceIn->getMatSize());

        //Iterate through each derivative order and add them together.
        for (int i = 0; i <= forceIn->getMaxOrder(); i++)
        {
            //Calculate scalar product.
            scalarMult = pow(waveFreq, i) * pow(imagI, i);

            //Multiply through to derivative terms and add to total.
            for (unsigned int row = 0; row < forceIn->getDerivative(i).n_rows; row++)
            {
                for (unsigned int col = 0; col < forceIn->getDerivative(i).n_cols; col++)
                {
                    output->at(row,col) += scalarMult * forceIn->getDerivative(i).at(row,col);
                }
            }
        }
        //Write output
        return output;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<cx_mat *> MotionSolver::sumDerivative(std::vector<matForceCross *> forceIn)
{
    vector<cx_mat *> output;

    //Resize output matrix.
    output.resize(forceIn.size(), NULL);

    //Fill vector with new matrices.
    for (unsigned int i = 0; i < forceIn.size(); i++)
    {
        if ((forceIn.at(i) != NULL)
                && (forceIn.at(i)->listDerivative().size() != 0))
        {
            //complex version of current wave frequency.
            complexDouble waveFreq(this->curWaveFrequency, 0.0);
            //Complex version of imaginary variable i.
            complexDouble imagI(0.0, 1.0);
            //complex scalar multiple.  Will calculate later.
            complexDouble scalarMult;

            //resize output matrix
            cx_mat *ptOutput = new cx_mat(forceIn.at(i)->getMatSize(), forceIn.at(i)->getMatSize());
            output.at(i) = ptOutput;

            //Iterate through each derivative order and add them together.
            for (int j = 0; j <= forceIn.at(i)->getMaxOrder(); j++)
            {
                //Calculate scalar product.
                scalarMult = pow(waveFreq, j) * pow(imagI, j);

                //Multiply through to derivative terms and add to total.
                for (unsigned int row = 0; row < forceIn.at(i)->getDerivative(j).n_rows; row++)
                {
                    for (unsigned int col = 0; col < forceIn.at(i)->getDerivative(j).n_cols; col++)
                    {
                        output.at(i)->at(row,col) += scalarMult * forceIn.at(i)->getDerivative(j).at(row,col);
                    }
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
    vector<matForceReact*> tempReactList_usr(plistBody.size());
    vector<cx_mat*> ActiveList_usr(plistBody.size());
    vector<matForceCross *> input(plistBody.size()); //Temporary vector for input into cross-body
    vector < vector<matForceCross *> > tempCrossList_usr(plistBody.size(), input);

    //create a temporary vector to hold outputs of summations - hydro forces
    vector<matForceReact*> tempReactList_hydro(plistBody.size());
    vector<cx_mat*> ActiveList_hydro(plistBody.size());
    vector<matForceCross *> input2(plistBody.size()); //Temporary vector for input into cross-body
    vector < vector<matForceCross *> > tempCrossList_hydro(plistBody.size(), input2);

    //Sum forces for each body in the list.
    for (curSumBody = 0; curSumBody < plistBody.size(); curSumBody++)
    {
        //Add user forces.
        tempReactList_usr.at(curSumBody) = sumReactSet(plistBody.at(curSumBody).listForceReact_user());
        ActiveList_usr.at(curSumBody) = sumActiveSet(plistBody.at(curSumBody).listForceActive_user());
        tempCrossList_usr.at(curSumBody) = sumCrossSet(plistBody.at(curSumBody).listForceCross_user());

        //Add hydro forces.
        tempReactList_hydro.at(curSumBody) = sumReactSet(plistBody.at(curSumBody).listForceReact_hydro());
        ActiveList_hydro.at(curSumBody) = sumActiveSet(plistBody.at(curSumBody).listForceActive_hydro());
        tempCrossList_hydro.at(curSumBody) = sumCrossSet(plistBody.at(curSumBody).listForceCross_hydro());

        //Create reactive force matrices for adding in mass
        matForceReact *curReactSet;
        matForceReact mass = plistBody.at(curSumBody).getForceReact_mass();

        //Assign reactive force matrices
        if (tempReactList_usr.at(curSumBody) == NULL)
        {
            //Reactive force set has no forces defined.  Create one.
            curReactSet = new matForceReact();
            tempReactList_usr.at(curSumBody) = curReactSet;
        }
        else
        {
            //Reactive force set has some forces defined.  Get their current location.
            curReactSet = tempReactList_usr.at(curSumBody);
        }

        //Add the mass matrix
        *curReactSet = *curReactSet + mass;
    }
    //Each set now reduced to a single force object, for each body.

    //Create temporary matrices to hold outputs of derivative summation - user forces
    vector<cx_mat*> ReactList_usr;
    vector< vector<cx_mat*> > CrossList_usr;
    // No need to Sum for active forces.  Already independent of derivative.

    //Create temporary matrices to hold outputs of derivative summation - hydro forces
    vector<cx_mat*> ReactList_hydro;
    // No need to Sum for active forces.  Already independent of derivative.
    vector< vector<cx_mat*> > CrossList_hydro;

    //Sum derivatives within each force.
    for (curSumBody = 0; curSumBody < plistBody.size(); curSumBody++)
    {
        //Sum for user forces
        ReactList_usr.push_back(sumDerivative(tempReactList_usr.at(curSumBody)));
        CrossList_usr.push_back(sumDerivative(tempCrossList_usr.at(curSumBody)));

        //Sum for hydro forces
        ReactList_hydro.push_back(sumDerivative(tempReactList_hydro.at(curSumBody)));
        CrossList_hydro.push_back(sumDerivative(tempCrossList_hydro.at(curSumBody)));
    }

    //Delete uneeded variables
    for (curSumBody = 0; curSumBody < plistBody.size(); curSumBody++)
    {
        delete tempReactList_hydro.at(curSumBody);
        delete tempReactList_usr.at(curSumBody);
        delete input.at(curSumBody);
        delete input2.at(curSumBody);

        for (unsigned int p = 0; p < tempCrossList_hydro.at(curSumBody).size(); p++)
            delete tempCrossList_hydro.at(curSumBody).at(p);

        for (unsigned int p = 0; p < tempCrossList_usr.at(curSumBody).size(); p++)
            delete tempCrossList_usr.at(curSumBody).at(p);
    }
    tempReactList_hydro.clear();
    tempReactList_usr.clear();
    input.clear();
    input2.clear();
    tempCrossList_hydro.clear();
    tempCrossList_usr.clear();

    //Create temporary variables for total of all forces.
    vector<cx_mat> ReactList;
    vector<cx_mat> ActiveList;
    vector< vector<cx_mat> > CrossList;
    vector<cx_mat> tempCross;

    //Add different force types for each body object.
    for (curSumBody = 0; curSumBody < plistBody.size(); curSumBody++)
    {
        //Add active force objects
        ActiveList.push_back(
                    SumSingle(ActiveList_usr.at(curSumBody), ActiveList_hydro.at(curSumBody), -2)
                    );

        //Add reactive force objects
        ReactList.push_back(
                    SumSingle(ReactList_usr.at(curSumBody), ReactList_hydro.at(curSumBody), -1)
                    );

        //Add cross-body force objects
        for (unsigned int j = 0; j < plistBody.size(); j++)
        {
            tempCross.push_back(
                        SumSingle(CrossList_usr.at(curSumBody).at(j), CrossList_hydro.at(curSumBody).at(j), j)
                        );
        }
        CrossList.push_back( tempCross );
        tempCross.clear();
    }

    //Delete uneeded variables
    for (curSumBody = 0; curSumBody < plistBody.size(); curSumBody++)
    {
        delete ReactList_hydro.at(curSumBody);
        delete ReactList_usr.at(curSumBody);
        delete ActiveList_hydro.at(curSumBody);
        delete ActiveList_usr.at(curSumBody);

        for (unsigned int p = 0; p < CrossList_hydro.at(curSumBody).size(); p++)
            delete CrossList_hydro.at(curSumBody).at(p);

        for (unsigned int p = 0; p < CrossList_usr.at(curSumBody).size(); p++)
            delete CrossList_usr.at(curSumBody).at(p);
    }
    tempCross.clear();
    ReactList_hydro.clear();
    ReactList_usr.clear();
    ActiveList_hydro.clear();
    ActiveList_usr.clear();
    CrossList_hydro.clear();
    CrossList_usr.clear();

    //"Assemble Global Matrix"
    vector<int> matStart;
    vector<int> matEnd;
    if(plistBody.size() == 1) //Single Body Matrix
    {
        //Dont include cross body forces if only 1 matrix and just use first index of vector
        globReactiveMat = ReactList.at(0); //A Matrix
        globActiveMat = ActiveList.at(0);     //F Matrix

        //Create values for matStart and matEnd
        matStart.push_back(0);
        matEnd.push_back(ActiveList.at(0).n_rows - 1);
    }
    else //Multiple Bodies, must resize the matrices
    {
        int newMatrixSize = 0;

        for(unsigned int j = 0; j < plistBody.size(); j++)
        {
            //get the size of new matrix by summing all reactiveForceMatrices
            newMatrixSize += ReactList.at(j).n_rows;

            //Add the sizes to get the starting position of each matrix
            if (j == 0) {
                matStart.push_back(0);
            }
            else
            {
                matStart.push_back(matStart.at(j-1) + ReactList.at(j).n_rows);
            }

            //Add the sizes to get the ending positions of each matrix.
            matEnd.push_back(matStart.at(j) + ReactList.at(j).n_rows - 1);
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
            globReactiveMat.submat(matStart.at(j), matStart.at(j), matEnd.at(j), matEnd.at(j)) = ReactList.at(j);

            //Iterate through and insert the active force matrices for each body
            globActiveMat.submat(matStart.at(j), 0, matEnd.at(j), 0) = ActiveList.at(j);
        }

        //Add in the cross body forces for each body.
        //Iterate through each body.
        for (unsigned int i = 0; i < plistBody.size(); i++)
        {
            //Iterate through the cross-body forces listed for each body
            for(unsigned int j = 0; j < plistBody.size(); j++)
            {
                //Skip if matching indices.
                if (j == i)
                    continue;

                globReactiveMat.submat(matStart.at(i), matStart.at(j), matEnd.at(i), matEnd.at(j)) = CrossList.at(i).at(j);
            }
        }
    }

    //Solve for Unknown Matrix (the X Matrix) --    A*X=B where X is the unknown
    try
    {
        globSolnMat = solve(globReactiveMat, globActiveMat, true); //true arg for more precise calculations
    }
    catch (const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(string("Error:  ") + ID + std::string("Failed to solve the global motion matrix.  Matrix values printed below.\n") +
                     string("Error Message:  ") + string(err.what()) + "\n\n",3);

        //Debug print out matrix values
        DebugMatrix("Global Reactive Matrix", globReactiveMat);
        DebugMatrix("Global Active Matrix", globActiveMat);
    }

    //Split into vector of solutions, each represents per body
    for(unsigned int i = 0; i < plistBody.size(); i++)
    {
        plistSolution.push_back(globSolnMat.submat(matStart.at(i), 0, matEnd.at(i), 0));
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
        return plistSolution.at(bod);
    }
    else
    {
        //Throw an error
        cx_mat out;
        out.zeros();
        return out;
    }
}

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat MotionSolver::SumSingle(cx_mat *Input1, cx_mat *Input2, int ForceType)
{
    //Create default matrix of zeros.
    cx_mat output;
    if (ForceType == -2)
    {
        //Active force matrix were the inputs.  Create a column matrix.
        //Get the dimensions for the output matrix.
        int rows = plistBody.at(curSumBody).refMass().n_rows;
        output.zeros(rows, 1);
    }

    else if (ForceType == -1)
    {
        //Reactive force matrix.  Create a square matrix.
        int rows = plistBody.at(curSumBody).refMass().n_rows;
        output.zeros(rows,rows);
    }

    else
    {
        //Cross Body force matrix.  The integer of the input specifies the second body index.
        int rows = plistBody.at(curSumBody).refMass().n_rows;
        int cols = plistBody.at(ForceType).refMass().n_rows;
        output.zeros(rows,cols);
    }

    //Add in inputs, depending on whether any of them are NULL values.
    if (Input1 != NULL)
    {
        if ((Input1->n_rows == output.n_rows)
                && (Input1->n_cols == output.n_cols))
            output = output + *Input1;
    }

    if (Input2 != NULL)
    {
        if ((Input2->n_rows == output.n_rows)
                && (Input2->n_cols == output.n_cols))
            output = output + *Input2;
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionSolver::DebugMatrix(std::string Name, cx_mat& input)
{
    //Write output of name
    logErr.Write(Name + " = ",3);

    //Print out matrix.
    input.print(logErr.outFile);
}
