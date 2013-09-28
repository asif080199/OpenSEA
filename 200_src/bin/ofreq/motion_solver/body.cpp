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

#include "body.h"

//------------------------------------------Function Separator --------------------------------------------------------
Body::Body()
{
    //Initialize private variables with zeros
    pmassMat.zeros(6,6);
    pCentroid.zeros(3,1);
    pPosn.zeros(3,1);
}

//------------------------------------------Function Separator --------------------------------------------------------
Body::~Body()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
bool Body::operator ==(Body &bodIn)
{
    //Create output and start with the initial assumption that bodies are equal.
    bool bodEq = true;

    //1.)  Check if they have the same body name.
    if (this->bodyName == bodIn.bodyName)
        bodEq *= true;
    else
        bodEq *= false;

    //2.)  Check if they have the same force reference.
    //2.1.1)  Check All active hydro forces
    if (bodEq && (this->plistForceActive_hydro.size() == bodIn.plistForceActive_hydro.size()))
    {
        for (unsigned int i = 0; i < this->plistForceActive_hydro.size(); i++)
        {
            if(this->plistForceActive_hydro.at(i) == bodIn.plistForceActive_hydro.at(i))
                bodEq *= true;
            else
                bodEq *= false;
        }
    }
    //2.1.2)  Check all active user forces
    if (bodEq && (this->plistForceActive_usr.size() == bodIn.plistForceActive_usr.size()))
    {
        for (unsigned int i = 0; i < this->plistForceActive_usr.size(); i++)
        {
            if(this->plistForceActive_usr.at(i) == bodIn.plistForceActive_usr.at(i))
                bodEq *= true;
            else
                bodEq *= false;
        }
    }
    //2.2.1)  Check all reactive hydro forces
    if (bodEq && (this->plistForceReact_hydro.size() == bodIn.plistForceReact_hydro.size()))
    {
        for (unsigned int i = 0; i < this->plistForceReact_hydro.size(); i++)
        {
            if(this->plistForceReact_hydro.at(i) == bodIn.plistForceReact_hydro.at(i))
                bodEq *= true;
            else
                bodEq *= false;
        }
    }
    //2.2.2)  Check All reactive user forces
    if (bodEq && (this->plistForceReact_usr.size() == bodIn.plistForceReact_usr.size()))
    {
        for (unsigned int i = 0; i < this->plistForceReact_usr.size(); i++)
        {
            if(this->plistForceReact_usr.at(i) == bodIn.plistForceReact_usr.at(i))
                bodEq *= true;
            else
                bodEq *= false;
        }
    }
    //2.3.1)  Check All cross-body hydro forces
    if (bodEq && (this->plistForceCross_hydro.size() == bodIn.plistForceCross_hydro.size()))
    {
        for (unsigned int i = 0; i < this->plistForceCross_hydro.size(); i++)
        {
            if(this->plistForceCross_hydro.at(i) == bodIn.plistForceCross_hydro.at(i))
                bodEq *= true;
            else
                bodEq *= false;
        }
    }
    //2.3.2)  Check All cross-body user forces
    if (bodEq && (this->plistForceCross_usr.size() == bodIn.plistForceCross_usr.size()))
    {
        for (unsigned int i = 0; i < this->plistForceCross_usr.size(); i++)
        {
            if(this->plistForceCross_usr.at(i) == bodIn.plistForceCross_usr.at(i))
                bodEq *= true;
            else
                bodEq *= false;
        }
    }

    //3.)  Check that the bodies have the same mass
    if (bodEq)
    {
        for (unsigned int i = 0; i < this->MassMatrix().n_rows; i++)
        {
            for (unsigned int j = 0; j < this->MassMatrix().n_cols; j++)
            {
                if(this->MassMatrix()(i,j) == bodIn.MassMatrix()(i,j))
                    bodEq *= true;
                else
                    bodEq *= false;
            }
        }
    }

    //Checks complete.  Return result.
    return bodEq;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setBodyName(string newName)
{
	bodyName = newName;
}

//------------------------------------------Function Separator --------------------------------------------------------
string &Body::refBodyName()
{
    return bodyName;
}

//------------------------------------------Function Separator --------------------------------------------------------
string &Body::refHydroBodName()
{
    return hydroBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setHydroBodName(string newName)
{
	hydroBody = newName;
}

//------------------------------------------Function Separator --------------------------------------------------------
string Body::getHydroBodName()
{
    return hydroBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setHeading(double newHeading)
{
    phead = newHeading;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getHeading()
{
    return phead;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &Body::refHeading()
{
    return phead;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setListForceActive_user(vector<string> newForceList)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setListForceReact_user(vector<string> newForceList)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setListForceActive_hydro(vector<string> newForceList)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setListForceReact_hydro(vector<string> newForceList)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setListForceCross_hydro(vector<string> newForceList)
{

}

//==========================================Section Separator =========================================================
//Mass Functions

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMass(double newMass)
{
    pmassMat(0,0) = newMass;   //X position mass.
    pmassMat(1,1) = newMass;   //Y position mass.
    pmassMat(2,2) = newMass;   //Z position mass.
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMass()
{
    return pmassMat(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIxx(double newXX)
{
    pmassMat(3,3) = newXX;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIxx()
{
    return pmassMat(3,3);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIyy(double newYY)
{
    pmassMat(4,4) = newYY;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIyy()
{
    return pmassMat(4,4);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIzz(double newZZ)
{
    pmassMat(5,5) = newZZ;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIzz()
{
    return pmassMat(5,5);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIxy(double newXY)
{
    pmassMat(3,4) = newXY;
    pmassMat(4,3) = newXY;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIxy()
{
    return pmassMat(3,4);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIxz(double newXZ)
{
    pmassMat(3,5) = newXZ;
    pmassMat(5,3) = newXZ;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIxz()
{
    return pmassMat(3,5);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIyz(double newYZ)
{
    pmassMat(4,5) = newYZ;
    pmassMat(5,4) = newYZ;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIyz()
{
    return pmassMat(4,5);
}

//------------------------------------------Function Separator --------------------------------------------------------
Mat<double> Body::getMassMatrix()
{
    return pmassMat;
}

//------------------------------------------Function Separator --------------------------------------------------------
Mat<double> &Body::MassMatrix()
{
    return pmassMat;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMassMatrix(Mat<double> MassMatIn)
{
    pmassMat = MassMatIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setCenX(double newCenX)
{
    pCentroid(0,0) = newCenX;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getCenX()
{
    return pCentroid(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setCenY(double newCenY)
{
    pCentroid(1,0) = newCenY;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getCenY()
{
    return pCentroid(1,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setCenZ(double newCenZ)
{
    pCentroid(2,0) = newCenZ;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getCenZ()
{
    return pCentroid(2,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
Mat<double> Body::getCen()
{
    return pCentroid;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setPosnX(double input)
{
    pPosn(0,0) = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getPosnX()
{
    return pPosn(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setPosnY(double input)
{
    pPosn(1,0) = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getPosnY()
{
    return pPosn(1,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setPosnZ(double input)
{
    pPosn(2,0) = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getPosnZ()
{
    return pPosn(2,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
Mat<double> Body::getPosn()
{
    return pPosn;
}

//------------------------------------------Function Separator --------------------------------------------------------
Mat<double> &Body::refPosn()
{
    return pPosn;
}


//==========================================Section Separator =========================================================
//Body Stuff

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setCrossBodyName(string newName)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
string Body::getBodyName()
{
	return bodyName;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setSolnMat(cx_mat input)
{
    pSoln = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat Body::getSolution()
{
    return pSoln;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat &Body::refSolution()
{
    return pSoln;
}

//------------------------------------------Function Separator --------------------------------------------------------
Body Body::Copy()
{
    return *this;
}


//==========================================Section Separator =========================================================
//Force Lists

//------------------------------------------Function Separator --------------------------------------------------------
vector<ForceActive *> Body::listForceActive_user()
{
    return plistForceActive_usr;
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceActive* Body::listForceActive_user(int forceIn)
{
    return plistForceActive_usr[forceIn];
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<ForceActive *> &Body::listForceActive_hydro()
{
    return plistForceActive_hydro;
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceActive *Body::listForceActive_hydro(int forceIn)
{
    return plistForceActive_hydro[forceIn];
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<ForceReact *> &Body::listForceReact_user()
{
    return plistForceReact_usr;
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceReact* Body::listForceReact_user(int forceIn)
{
    return plistForceReact_usr[forceIn];
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceReact *Body::listForceReact_hydro(int forceIn)
{
    return plistForceReact_usr[forceIn];
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<ForceReact *> &Body::listForceReact_hydro()
{
    return plistForceReact_hydro;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<ForceCross* > &Body::listForceCross_user()
{
    return plistForceCross_usr;
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceCross* Body::listForceCross_user(int forceIn)
{
    return plistForceCross_usr[forceIn];
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<ForceCross* > &Body::listForceCross_hydro()
{
    return plistForceCross_hydro;
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceCross* Body::listForceCross_hydro(int forceIn)
{
    return plistForceCross_hydro[forceIn];
}


//==========================================Section Separator =========================================================
//Miscellaneous

//------------------------------------------Function Separator --------------------------------------------------------
vector<Body *> &Body::listCrossBody_user()
{
    return plistLinkedBody_usr;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<Body *> &Body::listCrossBody_hydro()
{
    return plistLinkedBody_hydro;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMotionModel(string modelName)
{
    motModel = modelName;
}

//------------------------------------------Function Separator --------------------------------------------------------
string Body::getMotionModel()
{
    return motModel;
}

//------------------------------------------Function Separator --------------------------------------------------------
int Body::getEquationCount()
{
    return eqnCount;
}
