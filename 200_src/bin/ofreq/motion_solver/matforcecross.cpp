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
    }

    else if (forceOther.pderiv.size() == 0)
    {
        //Nothing defined for the other object.
        output = *this;
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
            output.pderiv.resize(maxOrdTwo);

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
    }

    else if (forceOther.pderiv.size() == 0)
    {
        //Nothing defined for the other object.
        output = *this;
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
            output.pderiv.resize(maxOrdTwo);

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
        else
        {
            //linked body parameters did not match.  Return only this object.
            return *this;
        }
    }
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
