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

#include "matforceactive.h"

using namespace arma;
using namespace std;
using namespace osea::ofreq;

//------------------------------------------Function Separator --------------------------------------------------------
matForceActive::matForceActive()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceActive::~matForceActive()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceActive matForceActive::operator+(matForceActive &forceOther)
{
    //Add two force objects together.
    matForceActive output;

    //Find max size
    int maxsize;

    if (this->getMatSize() > forceOther.getMatSize())
        maxsize = this->getMatSize();
    else
        maxsize = forceOther.getMatSize();

    //Resize matrix and initialize with zeros.
    output.pCoeff.zeros(maxsize,1);

    //Iterate through matrix and add the two together.
    for (unsigned int i = 0; i < pCoeff.n_rows; i++)
    {
        output.pCoeff(i,1) = pCoeff(i,1) + forceOther.pCoeff(i,1);
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
matForceActive matForceActive::operator-(matForceActive &forceOther)
{
    //Add two force objects together.
    matForceActive output;

    //Find max size
    int maxsize;

    if (this->getMatSize() > forceOther.getMatSize())
        maxsize = this->getMatSize();
    else
        maxsize = forceOther.getMatSize();

    //Resize matrix and initialize with zeros.
    output.pCoeff.zeros(maxsize,1);

    //Iterate through matrix and add the two together.
    for (unsigned int i = 0; i < pCoeff.n_rows; i++)
    {
        output.pCoeff(i,1) = pCoeff(i,1) - forceOther.pCoeff(i,1);
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat &matForceActive::listCoefficients()
{
    return pCoeff;
}

//------------------------------------------Function Separator --------------------------------------------------------
void matForceActive::setId(int num)
{
    pId = num;
}

//------------------------------------------Function Separator --------------------------------------------------------
int matForceActive::getId()
{
    return pId;
}

//------------------------------------------Function Separator --------------------------------------------------------
int matForceActive::getMatSize()
{
    return pCoeff.n_rows;
}
