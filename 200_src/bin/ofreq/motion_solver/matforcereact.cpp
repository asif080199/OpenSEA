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

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact::matForceReact()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact::~matForceReact()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact::matForceReact(vector<Derivative> forceIn)
{
    for (unsigned int i; i <= forceIn.size(); i++)
    {
        this->pderiv.push_back(forceIn[i]);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact matForceReact::operator+(const matForceReact& forceOther)
{
    //define the output vector
    matForceReact output;
    //find the max order of the derivatives.
    int maxOrdTwo;
    if (this->maxOrder() > forceOther.maxOrder())
        maxOrdTwo = this->maxOrder();
    else
        maxOrdTwo = forceOther.maxOrder();

    //Expand the size of the vector
    output.pderiv.resize(maxOrdTwo);

    //Get size of matrices.
    maxSizeTwo;
    if (this->matSize() > forceOther.matSize())
        maxSizeTwo = this-> matSize();
    else
        maxSizeTwo = forceOther.matSize();

    //Check that the maximum size is correct with the matrix size.
    if (this->pderiv[0].n_rows > maxSizeTwo)
        maxSizeTwo = this->pderiv[0].n_rows;
    if (forceOther.pderiv[0].n_rows > maxSizeTwo)
        maxSizeTwo = forceOther.pderiv[0].n_rows;

    //Resize Matrices, initialize with zeros.
    for (int i = 0; i <= maxOrdTwo; i++)
    {
        output.pderiv[i].zeros(maxSizeTwo);

        //Iterate through the matrix and add values.
        for (unsigned int j = 0; j < maxSizeTwo; j++)
        {
            for (unsigned int k = 0; k < maxSizeTwo; k++)
            {
                output.pderiv[i](k,j) = this->pderiv[i](k,j) + forceOther.pderiv[i](k,j);
            }
        }
    }

    //return output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceReact matForceReact::operator-(const matForceReact& forceOther)
{
    //define the output vector
    matForceReact output;
    //find the max order of the derivatives.
    int maxOrdTwo;
    if (this->maxOrder() > forceOther.maxOrder())
        maxOrdTwo = this->maxOrder();
    else
        maxOrdTwo = forceOther.maxOrder();

    //Expand the size of the vector
    output.pderiv.resize(maxOrdTwo);

    //Get size of matrices.
    maxSizeTwo;
    if (this->matSize() > forceOther.matSize())
        maxSizeTwo = this-> matSize();
    else
        maxSizeTwo = forceOther.matSize();

    //Check that the maximum size is correct with the matrix size.
    if (this->pderiv[0].n_rows > maxSizeTwo)
        maxSizeTwo = this->pderiv[0].n_rows;
    if (forceOther.pderiv[0].n_rows > maxSizeTwo)
        maxSizeTwo = forceOther.pderiv[0].n_rows;

    //Resize Matrices, initialize with zeros.
    for (int i = 0; i <= maxOrdTwo; i++)
    {
        output.pderiv[i].zeros(maxSizeTwo);

        //Iterate through the matrix and add values.
        for (unsigned int j = 0; j < maxSizeTwo; j++)
        {
            for (unsigned int k = 0; k < maxSizeTwo; k++)
            {
                output.pderiv[i](k,j) = this->pderiv[i](k,j) - forceOther.pderiv[i](k,j);
            }
        }
    }

    //return output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
int matForceReact::getMaxOrder()
{
    return pderiv.size() - 1;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat matForceReact::getDerivative(int order)
{
    return pderiv[order];
}

//------------------------------------------Function Separator --------------------------------------------------------
void matForceReact::setDerivative(int order, cx_mat Coeff)
{
    pderiv[order] = Coeff;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<cx_mat> &matForceReact::listDerivatives()
{
    return pderiv;
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
    return pderiv[0].n_rows;
}
