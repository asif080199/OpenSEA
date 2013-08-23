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

#include "matbody.h"


matBody::matBody()
{
}

matBody::~matBody()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
void matBody::setId(int num)
{
    pId = num;
}

//------------------------------------------Function Separator --------------------------------------------------------
int matBody::getId()
{
    return pId;
}

//------------------------------------------Function Separator --------------------------------------------------------
void matBody::setModelId(int num)
{
    pModelId = num;
}

//------------------------------------------Function Separator --------------------------------------------------------
int matBody::getModelId()
{
    return pModelId;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat &matBody::Mass()
{
    return pMass;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matReactForce> &matBody::listReactForce_user()
{
    return plistReactForce_user;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matCrossForce> &matBody::listCrossForce_user()
{
    return plistCrossForce_user;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matActiveForce> &matBody::listActiveForce_user()
{
    return plistActiveForce_user
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matReactForce> &matBody::listReactForce_hydro()
{
    return plistReactForce_hydro;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matCrossForce> &matBody::listCrossForce_hydro()
{
    return plistCrossForce_hydro;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matActiveForce> &matBody::listActiveForce_hydro()
{
    return plistActiveForce_hydro
}
