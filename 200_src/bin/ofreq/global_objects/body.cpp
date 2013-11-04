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

using namespace std;
using namespace arma;
using namespace osea::ofreq;

//------------------------------------------Function Separator --------------------------------------------------------
Body::Body()
{
    //Initialize private variables with zeros
    pMassMat.zeros(6,6);
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
    if (this->pBodyName == bodIn.pBodyName)
        bodEq *= true;
    else
        bodEq *= false;

//    //2.)  Check if they have the same force reference.
//    //2.1.1)  Check All active hydro forces
//    if (bodEq && (this->plistForceActive_hydro.size() == bodIn.plistForceActive_hydro.size()))
//    {
//        for (unsigned int i = 0; i < this->plistForceActive_hydro.size(); i++)
//        {
//            if(this->plistForceActive_hydro.at(i) == bodIn.plistForceActive_hydro.at(i))
//                bodEq *= true;
//            else
//                bodEq *= false;
//        }
//    }
//    //2.1.2)  Check all active user forces
//    if (bodEq && (this->plistForceActive_usr.size() == bodIn.plistForceActive_usr.size()))
//    {
//        for (unsigned int i = 0; i < this->plistForceActive_usr.size(); i++)
//        {
//            if(this->plistForceActive_usr.at(i) == bodIn.plistForceActive_usr.at(i))
//                bodEq *= true;
//            else
//                bodEq *= false;
//        }
//    }
//    //2.2.1)  Check all reactive hydro forces
//    if (bodEq && (this->plistForceReact_hydro.size() == bodIn.plistForceReact_hydro.size()))
//    {
//        for (unsigned int i = 0; i < this->plistForceReact_hydro.size(); i++)
//        {
//            if(this->plistForceReact_hydro.at(i) == bodIn.plistForceReact_hydro.at(i))
//                bodEq *= true;
//            else
//                bodEq *= false;
//        }
//    }
//    //2.2.2)  Check All reactive user forces
//    if (bodEq && (this->plistForceReact_usr.size() == bodIn.plistForceReact_usr.size()))
//    {
//        for (unsigned int i = 0; i < this->plistForceReact_usr.size(); i++)
//        {
//            if(this->plistForceReact_usr.at(i) == bodIn.plistForceReact_usr.at(i))
//                bodEq *= true;
//            else
//                bodEq *= false;
//        }
//    }
//    //2.3.1)  Check All cross-body hydro forces
//    if (bodEq && (this->plistForceCross_hydro.size() == bodIn.plistForceCross_hydro.size()))
//    {
//        for (unsigned int i = 0; i < this->plistForceCross_hydro.size(); i++)
//        {
//            if(this->plistForceCross_hydro.at(i) == bodIn.plistForceCross_hydro.at(i))
//                bodEq *= true;
//            else
//                bodEq *= false;
//        }
//    }
//    //2.3.2)  Check All cross-body user forces
//    if (bodEq && (this->plistForceCross_usr.size() == bodIn.plistForceCross_usr.size()))
//    {
//        for (unsigned int i = 0; i < this->plistForceCross_usr.size(); i++)
//        {
//            if(this->plistForceCross_usr.at(i) == bodIn.plistForceCross_usr.at(i))
//                bodEq *= true;
//            else
//                bodEq *= false;
//        }
//    }

//    //3.)  Check that the bodies have the same mass
//    if (bodEq)
//    {
//        for (unsigned int i = 0; i < this->MassMatrix().n_rows; i++)
//        {
//            for (unsigned int j = 0; j < this->MassMatrix().n_cols; j++)
//            {
//                if(this->MassMatrix()(i,j) == bodIn.MassMatrix()(i,j))
//                    bodEq *= true;
//                else
//                    bodEq *= false;
//            }
//        }
//    }

    //Checks complete.  Return result.
    return bodEq;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setBodyName(string newName)
{
    pBodyName = newName;
}

//------------------------------------------Function Separator --------------------------------------------------------
string &Body::refBodyName()
{
    return pBodyName;
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
    pHead = newHeading;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getHeading()
{
    return pHead;
}

//------------------------------------------Function Separator --------------------------------------------------------
double &Body::refHeading()
{
    return pHead;
}

//==========================================Section Separator =========================================================
//Mass Functions

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMass(double newMass)
{
    pMassMat(0,0) = newMass;   //X position mass.
    pMassMat(1,1) = newMass;   //Y position mass.
    pMassMat(2,2) = newMass;   //Z position mass.

    //Calculate mass coupling.
    setMassCouple();
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMass()
{
    return pMassMat(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIxx(double newXX)
{
    pMassMat(3,3) = newXX;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIxx()
{
    return pMassMat(3,3);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIyy(double newYY)
{
    pMassMat(4,4) = newYY;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIyy()
{
    return pMassMat(4,4);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIzz(double newZZ)
{
    pMassMat(5,5) = newZZ;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIzz()
{
    return pMassMat(5,5);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIxy(double newXY)
{
    pMassMat(3,4) = newXY;
    pMassMat(4,3) = newXY;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIxy()
{
    return pMassMat(3,4);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIxz(double newXZ)
{
    pMassMat(3,5) = newXZ;
    pMassMat(5,3) = newXZ;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIxz()
{
    return pMassMat(3,5);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIyz(double newYZ)
{
    pMassMat(4,5) = newYZ;
    pMassMat(5,4) = newYZ;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIyz()
{
    return pMassMat(4,5);
}

//------------------------------------------Function Separator --------------------------------------------------------
Mat<double> Body::getMassMatrix()
{
    return pMassMat;
}

//------------------------------------------Function Separator --------------------------------------------------------
Mat<double> &Body::MassMatrix()
{
    return pMassMat;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMassMatrix(Mat<double> MassMatIn)
{
    pMassMat = MassMatIn;

    //Add mass coupling
    setMassCouple();
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setCenX(double newCenX)
{
    pCentroid(0,0) = newCenX;

    //Add mass coupling
    setMassCouple();
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

    //Add mass coupling
    setMassCouple();
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

    //Add mass coupling
    setMassCouple();
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

    //Add mass coupling
    setMassCouple();
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

    //Add mass coupling
    setMassCouple();
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

    //Add mass coupling
    setMassCouple();
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
string Body::getBodyName()
{
    return pBodyName;
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
vector<ForceActive *> &Body::listForceActive_user()
{
    return plistForceActive_usr;
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceActive* Body::listForceActive_user(int forceIn)
{
    //Resize if the vector is too small.
    if ((forceIn > plistForceActive_usr.size() - 1) || (plistForceActive_usr.size() == 0))
    {
        plistForceActive_usr.resize(forceIn + 1);
    }

    //Add one if no index specified.
    if (forceIn < 0)
    {
        plistForceActive_usr.resize(plistForceActive_usr.size() + 1);
        //Get the index of the last element.
        forceIn = plistForceActive_usr.size() - 1;
    }

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
    //Resize if the vector is too small.
    if ((forceIn > plistForceActive_hydro.size() - 1) || (plistForceActive_hydro.size() == 0))
    {
        plistForceActive_hydro.resize(forceIn + 1);
    }

    //Add one if no index specified.
    if (forceIn < 0)
    {
        plistForceActive_hydro.resize(plistForceActive_hydro.size() + 1);
        //Get the index of the last element.
        forceIn = plistForceActive_hydro.size() - 1;
    }

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
    //Resize if the vector is too small.
    if ((forceIn > plistForceReact_usr.size() - 1) || (plistForceReact_usr.size() == 0))
    {
        plistForceReact_usr.resize(forceIn + 1);
    }

    //Add one if no index specified.
    if (forceIn < 0)
    {
        plistForceReact_usr.resize(plistForceReact_usr.size() + 1);
        //Get the index of the last element.
        forceIn = plistForceReact_usr.size() - 1;
    }

    return plistForceReact_usr[forceIn];
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceReact *Body::listForceReact_hydro(int forceIn)
{
    //Resize if the vector is too small.
    if ((forceIn > plistForceReact_hydro.size() - 1) || (plistForceReact_hydro.size() == 0))
    {
        plistForceReact_hydro.resize(forceIn + 1);
    }

    //Add one if no index specified.
    if (forceIn < 0)
    {
        plistForceReact_hydro.resize(plistForceReact_hydro.size() + 1);
        //Get the index of the last element.
        forceIn = plistForceReact_hydro.size() - 1;
    }

    return plistForceReact_hydro[forceIn];
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
    //Resize if the vector is too small.
    if ((forceIn > plistForceCross_usr.size() - 1) || (plistForceCross_usr.size() == 0))
    {
        plistForceCross_usr.resize(forceIn + 1);
    }

    //Add one if no index specified.
    if (forceIn < 0)
    {
        plistForceCross_usr.resize(plistForceCross_usr.size() + 1);
        //Get the index of the last element.
        forceIn = plistForceCross_usr.size() - 1;
    }

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
    //Resize if the vector is too small.
    if ((forceIn > plistForceCross_hydro.size() - 1) || (plistForceCross_hydro.size() == 0) || (forceIn < 0))
    {
        plistForceCross_hydro.resize(forceIn + 1);
    }

    //Add one if no index specified.
    if (forceIn < 0)
    {
        plistForceCross_hydro.resize(plistForceCross_hydro.size() + 1);
        //Get the index of the last element.
        forceIn = plistForceCross_hydro.size() - 1;
    }

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
Body &Body::listCrossBody_user(int index)
{
    return *(plistLinkedBody_usr[index]);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<Body *> &Body::listCrossBody_hydro()
{
    return plistLinkedBody_hydro;
}

//------------------------------------------Function Separator --------------------------------------------------------
Body &Body::listCrossBody_hydro(int index)
{
    return *(plistLinkedBody_hydro[index]);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<string> &Body::listNamedLink_user()
{
    return plistNamedLink_usr;
}

//------------------------------------------Function Separator --------------------------------------------------------
string &Body::listNamedLink_user(unsigned int varIn)
{
    //Check if need to resize the vector
    if ((varIn > plistLinkedBody_usr.size() - 1) || (plistLinkedBody_usr.size() == 0))
    {
        plistLinkedBody_usr.resize(varIn + 1);
    }

    return plistNamedLink_usr[varIn];
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<string> &Body::listNamedLink_hydro()
{
    return plistNamedLink_hydro;
}

//------------------------------------------Function Separator --------------------------------------------------------
string &Body::listNamedLink_hydro(unsigned int varIn)
{
    //Check if need to resize the vector
    if ((varIn > plistLinkedBody_hydro.size() - 1) || (plistLinkedBody_hydro.size() == 0))
    {
        plistLinkedBody_hydro.resize(varIn + 1);
    }

    return plistNamedLink_hydro[varIn];
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

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMassCouple()
{
    //Sets the coupling between mass and moment terms.
    //Should be run any time the mass is set or the centroid is set.

    //Mass coupling for force X
    pMassMat(0,3) = 0;
    pMassMat(0,4) = pMassMat(0,0) * pCentroid(2,0);
    pMassMat(0,5) = -1 * pMassMat(0,0) * pCentroid(1,0);

    //Mass coupling for force Y
    pMassMat(1,3) = -1 * pMassMat(1,1) * pCentroid(2,0);
    pMassMat(1,4) = 0;
    pMassMat(1,5) = pMassMat(1,1) * pCentroid(0,0);

    //Mass coupling for force Z
    pMassMat(2,3) = pMassMat(2,2) * pCentroid(1,0);
    pMassMat(2,4) = -1 * pMassMat(2,2) * pCentroid(0,0);
    pMassMat(2,5) = 0;

    //Mass coupling for moment X
    pMassMat(3,0) = 0;
    pMassMat(3,1) = -1 * pMassMat(0,0) * pCentroid(2,0);
    pMassMat(3,2) = pMassMat(0,0) * pCentroid(1,0);

    //Mass coupling for moment Y
    pMassMat(4,0) = -1 * pMassMat(1,1) * pCentroid(2,0);
    pMassMat(4,1) = 0;
    pMassMat(4,2) = pMassMat(1,1) * pCentroid(0,0);

    //Mass coupling for moment Z
    pMassMat(5,0) = -1 * pMassMat(2,2) * pCentroid(1,0);
    pMassMat(5,1) = pMassMat(2,2) * pCentroid(0,0);
    pMassMat(5,2) = 0;
}
