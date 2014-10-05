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

#include "matforcecross.h"
#include "matbody.h"    //Include the matBody header here to avoid circular header inclusions.

using namespace arma;
using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static initialization

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
matForceCross::matForceCross()
{
    //Add 3 matrices to derivative matrix
//    for(unsigned int i = 0; i < 3; i++) //<---Fix, change to const
//    {
//        cx_mat temp(6,6);
//        derivativeMatrix.push_back(temp);
//    }
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceCross::matForceCross(vector<cx_mat> forceIn)
{
    for (unsigned int i; i <= forceIn.size(); i++)
    {
        this->pderiv.push_back(forceIn[i]);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceCross::~matForceCross()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceCross matForceCross::operator+(matForceCross& forceOther)
{
    //define the output vector
    matForceCross output;

    if (this->pderiv.size() == 0)
    {
        //Nothing defined for first object.
        output = forceOther;

        //linked body parameters did not match.  Return only this object.
        return *this;
    }

    else if (forceOther.pderiv.size() == 0)
    {
        //Nothing defined for the other object.
        output = *this;

        //linked body parameters did not match.  Return only this object.
        return *this;
    }

    else
    {
        //Check that the lnkedbody parameter is the same for both objects.
        if (this->plinkid == forceOther.plinkid)
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
            //return output.
            return output;
        }
        else
        {
            //linked body parameters did not match.  Return only this object.
            return *this;
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceCross matForceCross::operator-(matForceCross& forceOther)
{
    //define the output vector
    matForceCross output;

    if (this->pderiv.size() == 0)
    {
        //Nothing defined for first object.
        output = forceOther;

        //linked body parameters did not match.  Return only this object.
        return *this;
    }

    else if (forceOther.pderiv.size() == 0)
    {
        //Nothing defined for the other object.
        output = *this;

        //linked body parameters did not match.  Return only this object.
        return *this;
    }

    else
    {
        //Check that the lnkedbody parameter is the same for both objects.
        if (this->plinkid == forceOther.plinkid)
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
            //return output
            return output;
        }
        else
        {
            //linked body parameters did not match.  Return only this object.
            return *this;
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceCross matForceCross::operator*(double scalar)
{
    //Matrix multiplication with scalar
    matForceCross output;

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
                    output.pderiv[i].at(j,k) = this->pderiv[i].at(j,k) * scalar;
                }
            }
        }
    }

    //Set the linked body parameter
    output.plinkbody = this->plinkbody;
    output.plinkbodyName = this->plinkbodyName;
    output.plinkid = this->plinkid;

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceCross matForceCross::operator/(double scalar)
{
    return this->operator *(1 / scalar);
}

//------------------------------------------Function Separator --------------------------------------------------------
void matForceCross::setLinkedBody(matBody &BodIn)
{
    plinkbody = &BodIn;

    //Also set the linkedId to the same thing.  Ensure the two are always in sync.
    this->plinkid = BodIn.getId();
}

//------------------------------------------Function Separator --------------------------------------------------------
matBody* matForceCross::getLinkedBody()
{
    return plinkbody;
}

//------------------------------------------Function Separator --------------------------------------------------------
void matForceCross::setLinkedId(int bodId)
{
    plinkid = bodId;
}

//------------------------------------------Function Separator --------------------------------------------------------
int matForceCross::getLinkedId()
{
    return plinkid;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<int> matForceCross::getMatDims()
{
    vector<int> output(2);

    //Get number of rows and number of columns
    output[0] = pderiv[0].n_rows;
    output[1] = pderiv[0].n_cols;

    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
void matForceCross::setLinkedName(std::string nameIn)
{
    plinkbodyName = nameIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string matForceCross::getLinkedName()
{
    return plinkbodyName;
}

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions
