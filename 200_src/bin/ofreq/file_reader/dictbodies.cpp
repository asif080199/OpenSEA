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
#include "dictbodies.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Class Name constants
string dictBodies::OBJECT_BODY = "body";
string dictBodies::OBJECT_MASSPROP = "massprop";
string dictBodies::OBJECT_CENTROID = "centroid"; /**< Object designator for new centroid object. */
string dictBodies::OBJECT_FORCE_ACTIVE = "force_active"; /**< Object designator for new ForceActive object. */
string dictBodies::OBJECT_FORCE_REACT = "force_reactive"; /**< Object designator for new ForceReact object. */
string dictBodies::OBJECT_FORCE_CROSS = "force_crossbody"; /**< Object designator for new ForceCross object. */
string dictBodies::OBJECT_MODEL = "model"; /**< Object designator for model of ForceCross object. */

//------------------------------------------Function Separator ----------------------------------------------------
//Keyword name static constants
string dictBodies::KEY_NAME = "name"; /**< Key for body name */
string dictBodies::KEY_HYDROBODY = "hydrobody"; /**< Key for hydrobody name. */
string dictBodies::KEY_MASS = "mass"; /**< Key for mass of body */
string dictBodies::KEY_IXX = "ixx"; /**< Key for moment of inertia, X-X axis */
string dictBodies::KEY_IYY = "iyy"; /**< Key for moment of inertia, Y-Y axis */
string dictBodies::KEY_IZZ = "izz"; /**< Key for moment of inertia, Z-Z axis */
string dictBodies::KEY_IXY = "ixy"; /**< Key for cross-product of inertia, X-Y coupling */
string dictBodies::KEY_IXZ = "ixz"; /**< Key for cross-product of inertia, X-Z coupling */
string dictBodies::KEY_IYZ = "iyz"; /**< Key for cross-product of inertia, Y-Z coupling */
string dictBodies::KEY_COGX = "cogx"; /**< Key for center of gravity, X-axis coordinate */
string dictBodies::KEY_COGY = "cogy"; /**< Key for center of gravity, Y-axis coordinate */
string dictBodies::KEY_COGZ = "cogz"; /**< Key for center of gravity, Z-axis coordinate */
string dictBodies::KEY_HEADING = "heading"; /**< Key for heading of body, Z-axis rotation */
string dictBodies::KEY_MOTION = "motion"; /**< Key to specify motion model.*/
string dictBodies::KEY_LINKEDBODY = "linkedbody"; /**< Key for linked body */
string dictBodies::KEY_MODEL = "model"; /**< Key for model to use for force specification. */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictBodies::dictBodies()
{
}

//==========================================Section Separator =========================================================
//Signal Functions


//==========================================Section Separator =========================================================
//Public Slots Functions


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
int dictBodies::defineKey(string keyIn, vector<string> valIn)
{
    if (keyIn == KEY_NAME)
    {
        //Set body name
        ptSystem->listBody(pBody).setBodyName(valIn.at(0));
        //Return success
        return 0;
    }

    else if (keyIn == KEY_HYDROBODY)
    {
        //Set hydrobody name
        ptSystem->listBody(pBody).setHydroBodName(valIn.at(0));
        //Return success
        return 0;
    }

    else if (keyIn == KEY_MASS)
    {
        //Set the mass
        double var;
        var = atof(valIn.at(0).c_str());
        ptSystem->listBody(pBody).setMass(var);
        //Update the mass definitions
        pMassProp[0] = true;
        //Return success
        return 0;
    }

    else if (keyIn == KEY_IXX)
    {
        //Set IXX
        double var;
        var = atof(valIn.at(0).c_str());
        ptSystem->listBody(pBody).setMomIxx(var);
        //Update the mass definitions
        pMassProp[1] = true;
        //Return success
        return 0;
    }

    else if (keyIn == KEY_IYY)
    {
        //Set IYY
        double var;
        var = atof(valIn.at(0).c_str());
        ptSystem->listBody(pBody).setMomIyy(var);
        //Update the mass definitions
        pMassProp[2] = true;
        //Return success
        return 0;
    }

    else if (keyIn == KEY_IZZ)
    {
        //Set IZZ
        double var;
        var = atof(valIn.at(0).c_str());
        ptSystem->listBody(pBody).setMomIzz(var);
        //Update the mass definitions
        pMassProp[3] = true;
        //Return success
        return 0;
    }

    else if (keyIn == KEY_IXY)
    {
        //Set IXY
        double var;
        var = atof(valIn.at(0).c_str());
        ptSystem->listBody(pBody).setMomIxy(var);
        //Update the mass definitions
        pMassProp[4] = true;
        //Return success
        return 0;
    }

    else if (keyIn == KEY_IXZ)
    {
        //Set IXZ
        double var;
        var = atof(valIn.at(0).c_str());
        ptSystem->listBody(pBody).setMomIxz(var);
        //Update the mass definitions
        pMassProp[5] = true;
        //Return success
        return 0;
    }

    else if (keyIn == KEY_IYZ)
    {
        //Set IYZ
        double var;
        var = atof(valIn.at(0).c_str());
        ptSystem->listBody(pBody).setMomIxz(var);
        //Update the mass definitions
        pMassProp[6] = true;
        //Return success
        return 0;
    }

    else if (keyIn == KEY_COGX)
    {
        //Set COGX
        double var;
        var = atof(valIn.at(0).c_str());
        ptSystem->listBody(pBody).setCenX(var);
        //Update the COG definitions
        pCOG[0] = true;
        //Return success
        return 0;
    }

    else if (keyIn == KEY_COGY)
    {
        //Set COGY
        double var;
        var = atof(valIn.at(0).c_str());
        ptSystem->listBody(pBody).setCenY(var);
        //Update the COG definitions
        pCOG[1] = true;
        //Return success
        return 0;
    }

    else if (keyIn == KEY_COGZ)
    {
        //Set COGZ
        double var;
        var = atof(valIn.at(0).c_str());
        ptSystem->listBody(pBody).setCenZ(var);
        //Update the COG definitions
        pCOG[2] = true;
        //Return success
        return 0;
    }

    else if (keyIn == KEY_HEADING)
    {
        //Set the heading for the body
        double var;
        var = atof(valIn.at(0).c_str());
        ptSystem->listBody(pBody).setHeading(var);
        //Return success
        return 0;
    }

    else if (keyIn == KEY_MOTION)
    {
        //Set the motion model name for the body
        ptSystem->listBody(pBody).setMotionModel(
                    ptSystem->listModel(valIn.at(0))
                );
        //Return success
        return 0;
    }

    else if (keyIn == KEY_LINKEDBODY)
    {
        //First check that the cross-body object is active
        if (pForceType == 3)
        {
            //Set the linked body name
            ptSystem->
                    listBody(pBody).
                    listNamedLink_user(pForceBodyIndex)
                    = valIn.at(0);

            //Return success
            return 0;
        }
        else
        {
            //Invalid use of key.
            //Return error
            return 2;
        }
    }

    else if (keyIn == KEY_MODEL)
    {
        if (pForceType == 1)
        {
            //Active Force Model

            //Assign the pointer for the force object to this body object.
            //Force object located by name of the object.
            ptSystem->listBody(pBody).listForceActive_user().push_back(
                        ptSystem->refForceActive_user(valIn.at(0))
                        );

            //Set the body index
            pForceBodyIndex = ptSystem->listBody(pBody).listForceActive_user().size() - 1;
            //Return success
            return 0;
        }
        else if (pForceType == 2)
        {
            //Reactive force model

            //Assign the pointer for the force object to this body object.
            //Force object located by the name of the object.
            ptSystem->listBody(pBody).listForceReact_user().push_back(
                        ptSystem->refForceReact_user(valIn.at(0))
                        );
            //Set body index
            pForceBodyIndex = ptSystem->listBody(pBody).listForceReact_user().size() - 1;
            //Set model index
            pForceIndex = ptSystem->listBody(pBody).listForceReact_user(pForceBodyIndex)->getSystemIndex();
            //Return success
            return 0;
        }
        else if (pForceType == 3)
        {
            //Cross-body force type

            //Assign the pointer for the force object to this body object.
            //Force object located by the name of the object.
            ptSystem->listBody(pBody).listForceCross_user().push_back(
                        ptSystem->refForceCross_user(valIn.at(0))
                        );
            //Set body index
            pForceBodyIndex = ptSystem->listBody(pBody).listForceCross_user().size() - 1;
            //Set model index
            pForceIndex = ptSystem->listBody(pBody).listForceCross_user(pForceBodyIndex)->getSystemIndex();
            //Return success
            return 0;
        }
        else
        {
            //Invalid use of the key.
            //Return error.
            return 2;
        }
    }

    else
    {
        //Return error
        return 1;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int dictBodies::defineClass(string nameIn)
{
    if (nameIn == OBJECT_BODY)
    {
        //Create new body object in System
        ptSystem->addBody();

        //Set index of current body object.
        pBody = ptSystem->listBody().size() - 1;

        //Return success
        return 0;
    }

    else if (nameIn == OBJECT_MASSPROP)
    {
        // No action required.  Mass properties created in body
        // by default.

        //Reset all properties of the massprop object.
        //Used if any future functions need to track
        for (int i = 0; i <= 6; i++)
        {
            pMassProp[i] = false;
        }

        //Return success
        return 0;
    }

    else if (nameIn == OBJECT_CENTROID)
    {
        // No action required.  Center of gravity properties created in body by default.

        //Reset all properties of COG keys.
        for (int i = 0; i <= 2; i++)
        {
            pCOG[i] = false;
        }

        //Return success
        return 0;
    }

    else if (nameIn == OBJECT_FORCE_ACTIVE)
    {
        //No action required.  Force object already created.
        //Set internal tracker of force type.
        pForceType = 1;

        //Set the latest force index
        setForceIndex();

        //Return success
        return 0;
    }

    else if (nameIn == OBJECT_FORCE_REACT)
    {
        //No action required.  Force object already created.
        //Set internal tracker of force type.
        pForceType = 2;

        //Set the latest force index
        setForceIndex();

        //Return success
        return 0;
    }

    else if (nameIn == OBJECT_FORCE_CROSS)
    {
        //No action required.  Force object already created.
        //Set internal tracker of force type.
        pForceType = 3;

        //Set the latest force index
        setForceIndex();

        //Return success
        return 0;
    }

    else if (nameIn == OBJECT_MODEL)
    {
        //Model object used in cross-body forces.
        if(pForceType == 3)
        {
            //No action required.
            //Return success
            return 0;
        }
        else
        {
            //wrong object name.  Return an error.
            return 2;
        }
    }

    else
    {
        //Return error
        return 1;
    }
}

//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
 void dictBodies::setForceIndex(int forceIn)
 {
     if (forceIn < 0)
     {
         if (pForceType == 1)
         {
             //Active forces
             pForceIndex = ptSystem->listForceActive_user().size();
         }
         else if (pForceType == 2)
         {
             //Reactive Forces
             pForceIndex = ptSystem->listForceReact_user().size();

         }
         else if (pForceType == 3)
         {
             //Cross-body forces
            pForceIndex = ptSystem->listForceCross_user().size();
         }
     }
     else
     {
         pForceIndex = forceIn;
     }
 }
