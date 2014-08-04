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

#include "matforcereact.h"

using namespace arma;
using namespace std;
using namespace osea::ofreq;

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact::matForceReact()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact::~matForceReact()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact::matForceReact(vector<cx_mat> forceIn)
{
    for (unsigned int i; i <= forceIn.size(); i++)
    {
        this->pderiv.push_back(forceIn[i]);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact matForceReact::operator+(matForceReact& forceOther)
{
    //define the output vector
    matForceReact output;

    if (this->pderiv.size() == 0)
    {
        //Nothing defined for first object.
        output = forceOther;
    }

    else if (forceOther.pderiv.size() == 0)
    {
        //Nothing defined for the other object.
        output = *this;
    }

    else
    {
        //find the max order of the derivatives.
        int maxOrdTwo;
        if (this->getMaxOrder() > forceOther.getMaxOrder())
            maxOrdTwo = this->getMaxOrder();
        else
            maxOrdTwo = forceOther.getMaxOrder();

        //Expand the size of the vector
        output.pderiv.resize(maxOrdTwo + 1);

        //Get size of matrices.
        unsigned int maxSizeTwo;
        if (this->getMatSize() > forceOther.getMatSize())
            maxSizeTwo = this-> getMatSize();
        else
            maxSizeTwo = forceOther.getMatSize();

        //Check that the maximum size is correct with the matrix size.
        if (this->pderiv[0].n_rows > maxSizeTwo)
            maxSizeTwo = this->pderiv[0].n_rows;
        if (forceOther.pderiv[0].n_rows > maxSizeTwo)
            maxSizeTwo = forceOther.pderiv[0].n_rows;

        //Resize Matrices, initialize with zeros.
        for (unsigned int i = 0; i <= maxOrdTwo; i++)
        {
            output.pderiv[i].zeros(maxSizeTwo, maxSizeTwo);

            //Check if either matrix does not have enough derivatives
            if (this->pderiv.size() - 1 < i)
            {
                //This force object does not have enough derivatives.
                output.pderiv[i] = forceOther.pderiv[i];
            }
            else if (forceOther.pderiv.size() - 1 < i)
            {
                //Other force object does not have enough derivatives.
                output.pderiv[i] = this->pderiv[i];
            }
            else
            {
                //Both have enough derivatives.  Add together.
                output.pderiv[i] = this->pderiv[i] + forceOther.pderiv[i];
            }
        }
    }

    //return output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact matForceReact::operator-(matForceReact& forceOther)
{
    //define the output vector
    matForceReact output;

    if (this->pderiv.size() == 0)
    {
        //Nothing defined for first object.
        output = forceOther;
    }

    else if (forceOther.pderiv.size() == 0)
    {
        //Nothing defined for the other object.
        output = *this;
    }

    else
    {
        //find the max order of the derivatives.
        int maxOrdTwo;
        if (this->getMaxOrder() > forceOther.getMaxOrder())
            maxOrdTwo = this->getMaxOrder();
        else
            maxOrdTwo = forceOther.getMaxOrder();

        //Expand the size of the vector
        output.pderiv.resize(maxOrdTwo + 1);

        //Get size of matrices.
        unsigned int maxSizeTwo;
        if (this->getMatSize() > forceOther.getMatSize())
            maxSizeTwo = this-> getMatSize();
        else
            maxSizeTwo = forceOther.getMatSize();

        //Check that the maximum size is correct with the matrix size.
        if (this->pderiv[0].n_rows > maxSizeTwo)
            maxSizeTwo = this->pderiv[0].n_rows;
        if (forceOther.pderiv[0].n_rows > maxSizeTwo)
            maxSizeTwo = forceOther.pderiv[0].n_rows;

        //Resize Matrices, initialize with zeros.
        for (unsigned int i = 0; i <= maxOrdTwo; i++)
        {
            output.pderiv[i].zeros(maxSizeTwo, maxSizeTwo);

            //Check if either matrix does not have enough derivatives
            if (this->pderiv.size() - 1 < i)
            {
                //This force object does not have enough derivatives.
                output.pderiv[i] = forceOther.pderiv[i];
            }
            else if (forceOther.pderiv.size() - 1 < i)
            {
                //Other force object does not have enough derivatives.
                output.pderiv[i] = this->pderiv[i];
            }
            else
            {
                //Both have enough derivatives.  Add together.
                output.pderiv[i] = this->pderiv[i] - forceOther.pderiv[i];
            }
        }
    }

    //return output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact matForceReact::operator*(double scalar)
{
    //Matrix multiplication with scalar
    matForceReact output;

    //Test for zero size
    if ( !(pderiv.size() == 0) )
    {
        //Get max size of matrices
        int maxSize = pderiv[0].n_rows;

        //Resize output matrix.
        output.pderiv.resize(this->pderiv.size());

        //Iterate through each derivative of matrix
        for (int i = 0; i < pderiv.size(); i++)
        {
            //Create matrix of zeros
            output.pderiv[i].zeros(maxSize, maxSize);

            //Multiply for each element
            for (int j = 0; j < pderiv[i].n_rows; j++)
            {
                for (int k = 0; k < pderiv[i].n_cols; k++)
                {
                    output.pderiv[i].at(j, k) = this->pderiv[i].at(j,k) * scalar;
                }
            }
        }
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact matForceReact::operator/(double scalar)
{
    return this->operator *(1 / scalar);
}

//------------------------------------------Function Separator --------------------------------------------------------
int matForceReact::getMaxOrder()
{
    if (pderiv.size() == 0)
        return 0;
    else
        return pderiv.size() - 1;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat matForceReact::getDerivative(int order)
{
    return pderiv[order];
}

//------------------------------------------Function Separator --------------------------------------------------------
void matForceReact::setDerivative(unsigned int order, cx_mat Coeff)
{
    //Check if vector is large enough for requested order size
    if ((order > pderiv.size() - 1) ||
            (pderiv.size() == 0))
    {
        arma::cx_mat mat1 = zeros<cx_mat>(Coeff.n_rows, Coeff.n_cols);

        //Resize list
        pderiv.resize(order + 1, mat1);
    }

    //Assign matrix.
    pderiv[order] = Coeff;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<cx_mat> &matForceReact::listDerivative()
{
    return pderiv;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat &matForceReact::listDerivative(unsigned int index)
{
    try
    {
        //Check if the requested derivative exists.
        if ((index > pderiv.size() - 1) ||
                (pderiv.size() == 0))
            throw 1;

        //Return derivative
        return pderiv[index];
    }
    catch (int err)
    {
        //Error handler.  Resize and return matrix of zeros.

        //First check if requested derivative exceeds limits.
        if ((index > pderiv.size() - 1) ||
                (pderiv.size() == 0))
        {
            //Check if zero
            if (pderiv.size() == 0)
            {
                //No size to go from.
                arma::cx_mat mat1 = zeros<cx_mat>(1,1);

                //Resize list
                pderiv.resize(index + 1, mat1);
            }
            else
            {
                //Get matrix size
                arma::cx_mat mat1 = zeros<cx_mat>(this->getMatSize(), this->getMatSize());

                //Resize list
                pderiv.resize(index + 1, mat1);
            }
        }
        else
        {
            //Case where vector was large enough, but requested index has
            //no matrix
            arma::cx_mat mat1 = zeros<cx_mat>(this->getMatSize(), this->getMatSize());
            pderiv[index] = mat1;
        }

        //Return matrix of zeros, newly created.
        return pderiv[index];
    }
    catch(...)
    {
        //Unknown error handler.
        logStd.Notify();
        logErr.Write("Uknown error occurred.  Object:  matForceReact, Function:  listDerivative");
        exit(1);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void matForceReact::setId(int num)
{
    pId = num;
}

//------------------------------------------Function Separator --------------------------------------------------------
int matForceReact::getId()
{
    return pId;
}

//------------------------------------------Function Separator --------------------------------------------------------
int matForceReact::getMatSize()
{
    if (pderiv.size() == 0)
        return 0;
    else
        return pderiv[0].n_rows;
}
