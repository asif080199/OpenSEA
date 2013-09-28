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
cx_mat &matBody::refMass()
{
    return pMass;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matForceReact> &matBody::listForceReact_user()
{
    return plistForceReact_user;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matForceCross> &matBody::listForceCross_user()
{
    return plistForceCross_user;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matForceActive> &matBody::listForceActive_user()
{
    return plistForceActive_user;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matForceReact> &matBody::listForceReact_hydro()
{
    return plistForceReact_hydro;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matForceCross> &matBody::listForceCross_hydro()
{
    return plistForceCross_hydro;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<matForceActive> &matBody::listForceActive_hydro()
{
    return plistForceActive_hydro;
}
