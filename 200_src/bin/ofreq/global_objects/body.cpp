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
#include "../motion_model/motionmodel.h"        //Include motion model in .cpp file to prevent recursive definitions.

using namespace std;
using namespace arma;
using namespace osea::ofreq;

//------------------------------------------Function Separator --------------------------------------------------------
//Static variable intialization
int Body::Tx = 0;      //Index of translation in X-axis
int Body::Ty = 1;      //Index of translation in Y-axis
int Body::Tz = 2;      //Index of translation in Z-axis
int Body::Rx = 3;      //Index of rotation about X-axis
int Body::Ry = 4;      //Index of rotation about Y-axis
int Body::Rz = 5;      //Index of rotation about Z-axis

//------------------------------------------Function Separator --------------------------------------------------------
Body::Body()
{
    //Initialize private variables with zeros
//    initMassMat();
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
    int index;

    //initialize mass matrix
    initMassMat();

    try
    {
        //Set mass for equation 0:  X-axis translation
        index = findIndex(Tx);
        pMassMat(index, index) = newMass;
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMass()\n" +
                     string("Operation:  set mass for X-axis translation\n") +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMass()\n" +
                     string("Operation:  set mass for X-axis translation"));
        logStd.Notify();
    }

    try
    {
        //Set mass for equation 1:  Y-axis translation
        index = findIndex(1);
        pMassMat(index, index) = newMass;
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMass()\n" +
                     string("Operation:  set mass for Y-axis translation\n") +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMass()\n" +
                     string("Operation:  set mass for Y-axis translation"));
        logStd.Notify();
    }

    try
    {
        //Set mass for equation 2:  Z-axis translation
        index = findIndex(Tz);
        pMassMat(index, index) = newMass;
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMass()\n" +
                     string("Operation:  set mass for Z-axis translation\n") +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMass()\n" +
                     string("Operation:  set mass for Z-axis translation"));
        logStd.Notify();
    }

    //Calculate mass coupling.
    setMassCouple();
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMass()
{
    //Find an index with translation in it.
    int index;

    //initialize mass matrix
    initMassMat();

    try
    {
        index = findIndex(Tx);   //Check for X-axis translation.
    }
    catch(...)
    {
        try
        {
            //Check the next index
            index = findIndex(1);   //Check for Y-axis translation.
        }
        catch(...)
        {
            try
            {
                //Check the next index
                index = findIndex(Tz);   //Check for Z-axis translation.
            }
            catch(std::exception &err)
            {
                //Error handler.
                logErr.Write("Error occurred.  Object:  Body, Function:  getMass()\n" +
                             string("Operation:  find index for mass matrix\n") +
                             string("Error Message:  ") + string(err.what()));
                logStd.Notify();
                return 0;
            }
            catch(...)
            {
                //Error handler.
                logErr.Write("Error occurred.  Object:  Body, Function:  getMass()\n" +
                             string("Operation:  find index for mass matrix"));
                logStd.Notify();
                return 0;
            }
        }
    }

    return pMassMat(index, index);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIxx(double newXX)
{
    int index;

    //initialize mass matrix
    initMassMat();

    try
    {
        //Get index of rotation in XX-axis.
        index = findIndex(Rx);
        pMassMat(index, index) = newXX;
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMomIxx()\n" +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMomIxx()");
        logStd.Notify();
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIxx()
{
    int index;

    //initialize mass matrix
    initMassMat();

    try
    {
        //Get index of rotation in the XX-axis.
        index = findIndex(Rx);

        return pMassMat(index, index);
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  getMomIxx()\n" +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
        return 0;
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  getMomIxx()");
        logStd.Notify();
        return 0;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIyy(double newYY)
{
    int index;

    //initialize mass matrix
    initMassMat();

    try
    {
        //Get the index of rotation in the YY-axis.
        index = findIndex(Ry);

        pMassMat(index, index) = newYY;
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMomIyy()\n" +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMomIyy()");
        logStd.Notify();
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIyy()
{
    int index;

    //initialize mass matrix
    initMassMat();

    try
    {
        //Get index of rotation in the YY-axis.
        index = findIndex(Ry);

        return pMassMat(index, index);
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  getMomIyy()\n" +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
        return 0;
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  getMomIyy()");
        logStd.Notify();
        return 0;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIzz(double newZZ)
{
    int index;

    //initialize mass matrix
    initMassMat();

    try
    {
        //Get the index of rotation in the ZZ-axis.
        index = findIndex(Rz);

        pMassMat(index, index) = newZZ;
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMomIzz()\n" +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMomIzz()");
        logStd.Notify();
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIzz()
{
    int index;

    //initialize mass matrix
    initMassMat();

    try
    {
        //Get index of rotation in the ZZ-axis.
        index = findIndex(Rz);

        return pMassMat(index, index);
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  getMomIzz()\n" +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
        return 0;
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  getMomIzz()");
        logStd.Notify();
        return 0;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIxy(double newXY)
{
    int index1;
    int index2;

    //initialize mass matrix
    initMassMat();

    try
    {
        index1 = findIndex(Rx);  //Index of rotation X-axis
        index2 = findIndex(Ry);  //Index of rotation Y-axis

        pMassMat(index1, index2) = newXY;
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMomIxy()\n" +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMomIxy()");
        logStd.Notify();
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIxy()
{
    int index1;
    int index2;

    //initialize mass matrix
    initMassMat();

    try
    {
        index1 = findIndex(Rx);  //Index of rotation X-axis
        index2 = findIndex(Ry);  //Index of rotation Y-axis

        return pMassMat(index1, index2);
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  getMomIxy()\n" +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
        return 0;
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  getMomIxy()");
        logStd.Notify();
        return 0;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIxz(double newXZ)
{
    int index1;
    int index2;

    //initialize mass matrix
    initMassMat();

    try
    {
        index1 = findIndex(Rx);  //Index of rotation X-axis
        index2 = findIndex(Rz);  //Index of rotation Z-axis

        pMassMat(index1, index2) = newXZ;
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMomIxz()\n" +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMomIxz()");
        logStd.Notify();
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIxz()
{
    int index1;
    int index2;

    //initialize mass matrix
    initMassMat();

    try
    {
        index1 = findIndex(Rx);  //Index of rotation X-axis
        index2 = findIndex(Rz);  //Index of rotation Z-axis

        return pMassMat(index1, index2);
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  gettMomIxz()\n" +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
        return 0;
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  getMomIxz()");
        logStd.Notify();
        return 0;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMomIyz(double newYZ)
{
    int index1;
    int index2;

    //initialize mass matrix
    initMassMat();

    try
    {
        index1 = findIndex(Ry);  //Index of rotation Y-axis
        index2 = findIndex(Rz);  //Index of rotation Z-axis

        pMassMat(index1, index2) = newYZ;
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMomIyz()\n" +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  setMomIyz()");
        logStd.Notify();
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getMomIyz()
{
    int index1;
    int index2;

    //initialize mass matrix
    initMassMat();

    try
    {
        index1 = findIndex(Ry);  //Index of rotation Y-axis
        index2 = findIndex(Rz);  //Index of rotation Z-axis

        return pMassMat(index1, index2);
    }
    catch(std::exception &err)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  getMomIyz()\n" +
                     string("Error Message:  ") + string(err.what()));
        logStd.Notify();
        return 0;
    }
    catch(...)
    {
        //Error handler.
        logErr.Write("Error occurred.  Object:  Body, Function:  getMomIyz()");
        logStd.Notify();
        return 0;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
Mat<double> Body::getMassMatrix()
{
    //initialize mass matrix
    initMassMat();

    return pMassMat;
}

//------------------------------------------Function Separator --------------------------------------------------------
Mat<double> &Body::MassMatrix()
{
    //initialize mass matrix
    initMassMat();

    return pMassMat;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMassMatrix(Mat<double> MassMatIn)
{
    //initialize mass matrix
    initMassMat();

    pMassMat = MassMatIn;

    //Add mass coupling
    //setMassCouple();
    //Not doing mass coupling because if the program specified the exact mass matrix, there is probably a reason.
    //The mass matrix should _not_ get changed by mass coupling.
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setCenX(double newCenX)
{
    pCentroid(Tx,0) = newCenX;

    //Add mass coupling
    setMassCouple();
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getCenX()
{
    return pCentroid(Tx,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setCenY(double newCenY)
{
    pCentroid(Ty,0) = newCenY;

    //Add mass coupling
    setMassCouple();
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getCenY()
{
    return pCentroid(Ty,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setCenZ(double newCenZ)
{
    pCentroid(Tz,0) = newCenZ;

    //Add mass coupling
    setMassCouple();
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getCenZ()
{
    return pCentroid(Tz,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
Mat<double> Body::getCen()
{
    return pCentroid;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setPosnX(double input)
{
    pPosn(Tx,0) = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getPosnX()
{
    return pPosn(Tx,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setPosnY(double input)
{
    pPosn(Ty,0) = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getPosnY()
{
    return pPosn(Ty,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setPosnZ(double input)
{
    pPosn(Tz,0) = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
double Body::getPosnZ()
{
    return pPosn(Tz,0);
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
cx_mat &Body::refDataSolution()
{
    return pSoln;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> &Body::refDataSolution(int varIndexIn)
{
    //Search through the data variables to find the index of the solution that matches the correct variable
    int i = findIndex(varIndexIn);

    //Return value
    return pSoln.at(i,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
Body Body::Copy()
{
    return *this;
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::initMassMat()
{
    //Initialize the mass matrix

    //Check if the motion model is assigned
    if (motModel)
    {
        //Resize the mass matrix.
        int size = motModel->listDataEquation().size();

        if (pMassMat.n_rows != size)
        {
            if (pMassMat.n_rows == 0)
            {
                //Resize and fill matrix with zeros.
                pMassMat.zeros(size, size);
            }
            else
            {
                //Just resize
                pMassMat.resize(size, size);
            }
        }
    }
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
void Body::setMotionModel(ofreq::MotionModel &modelIn)
{
    motModel = &modelIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
MotionModel &Body::getMotionModel()
{
    return *motModel;
}

//------------------------------------------Function Separator --------------------------------------------------------
int Body::getEquationCount()
{
    return motModel->listEquation().size();
}

//------------------------------------------Function Separator --------------------------------------------------------
void Body::setMassCouple()
{
    //Sets the coupling between mass and moment terms.
    //Should be run any time the mass is set or the centroid is set.

    int index0;     //index for X-axis translation
    int index1;     //index for Y-axis translation
    int index2;     //index for Z-axis translation
    int index3;     //index for X-axis rotation
    int index4;     //index for Y-axis rotation
    int index5;     //index for Z-axis rotation


    try
    {
        //Get indices for each equation of motion
        index0 = findIndex(Tx);
        index1 = findIndex(Ty);
        index2 = findIndex(Tz);
        index3 = findIndex(Rx);
        index4 = findIndex(Ry);
        index5 = findIndex(Rz);

        //Mass coupling for force X
        pMassMat(index0, index3) = 0;
        pMassMat(index0, index4) = pMassMat(index0, index0) * pCentroid(Tz, 0);
        pMassMat(index0, index5) = -1 * pMassMat(index0, index0) * pCentroid(Ty, 0);
    }
    catch(...)
    {
        //Do nothing
    }

    try
    {
        //Get indices for each equation of motion
        index0 = findIndex(Tx);
        index1 = findIndex(Ty);
        index2 = findIndex(Tz);
        index3 = findIndex(Rx);
        index4 = findIndex(Ry);
        index5 = findIndex(Rz);

        //Mass coupling for force Y
        pMassMat(index1, index3) = -1 * pMassMat(index1, index1) * pCentroid(Tz, 0);
        pMassMat(index1, index4) = 0;
        pMassMat(index1, index5) = pMassMat(index1, index1) * pCentroid(Tx, 0);
    }
    catch(...)
    {
        //Do nothing
    }

    try
    {
        //Get indices for each equation of motion
        index0 = findIndex(Tx);
        index1 = findIndex(Ty);
        index2 = findIndex(Tz);
        index3 = findIndex(Rx);
        index4 = findIndex(Ry);
        index5 = findIndex(Rz);

        //Mass coupling for force Z
        pMassMat(index2, index3) = pMassMat(index2, index2) * pCentroid(Ty, 0);
        pMassMat(index2, index4) = -1 * pMassMat(index2, index2) * pCentroid(Tx, 0);
        pMassMat(index2, index5) = 0;
    }
    catch(...)
    {
        //Do nothing
    }

    try
    {
        //Get indices for each equation of motion
        index0 = findIndex(Tx);
        index1 = findIndex(Ty);
        index2 = findIndex(Tz);
        index3 = findIndex(Rx);

        //Mass coupling for moment X
        pMassMat(index3, index0) = 0;
        pMassMat(index3, index1) = -1 * pMassMat(index0, index0) * pCentroid(Tz, 0);
        pMassMat(index3, index2) = pMassMat(index0, index0) * pCentroid(Ty, 0);
    }
    catch(...)
    {
        //Do nothing
    }

    try
    {
        //Get indices for each equation of motion
        index0 = findIndex(Tx);
        index1 = findIndex(Ty);
        index2 = findIndex(Tz);
        index4 = findIndex(Ry);

        //Mass coupling for moment Y
        pMassMat(index4, index0) = +1 * pMassMat(index1, index1) * pCentroid(Tz, 0);
        pMassMat(index4, index1) = 0;
        pMassMat(index4, index2) = -1 * pMassMat(index1, index1) * pCentroid(Tx, 0);
    }
    catch(...)
    {
        //Do nothing
    }

    try
    {
        //Get indices for each equation of motion
        index0 = findIndex(Tx);
        index1 = findIndex(Ty);
        index2 = findIndex(Tz);
        index5 = findIndex(Rz);

        //Mass coupling for moment Z
        pMassMat(index5, index0) = -1 * pMassMat(index2, index2) * pCentroid(Ty, 0);
        pMassMat(index5, index1) = pMassMat(index2, index2) * pCentroid(Tx, 0);
        pMassMat(index5, index2) = 0;
    }
    catch(...)
    {
        //Do nothing
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int Body::findIndex(int DataIndexIn)
{
    int output;         //integer to write as output.
    int check;          //The integer to check against.
    bool test = false;  //Check if a match was found

    //Finds the integer of the equation object by data index.
    for (unsigned int i = 0 ; i < motModel->listEquation().size(); i++)
    {
        //Check the data index of the object.
        if (motModel->listEquation(i).getDataIndex() < 0)
        {
            //No data index set.  Use the position in the list.
            check = i;
        }
        else
        {
            //Data index is used.  Use the position in the list.
            check = motModel->listEquation(i).getDataIndex();
        }

        //Check for match
        if (check == DataIndexIn)
        {
            output = i;
            test = true;
            break;
        }
    }

    //Check for a match
    if (!test)
    {
        //No match.  Throw an exception
        throw 1;
    }

    return output;
}
