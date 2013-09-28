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

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictBodies::dictBodies()
{
    //------------------------------------------Function Separator ----------------------------------------------------
    //Class Name constants
    OBJECT_BODY = "body";
    OBJECT_MASSPROP = "massprop";
    OBJECT_CENTROID = "centroid"; /**< Object designator for new centroid object. */
    OBJECT_FORCE_ACTIVE = "force_active"; /**< Object designator for new ForceActive object. */
    OBJECT_FORCE_REACT = "force_reactive"; /**< Object designator for new ForceReact object. */
    OBJECT_FORCE_CROSS = "force_crossbody"; /**< Object designator for new ForceCross object. */
    OBJECT_MODEL = "model"; /**< Object designator for model of ForceCross object. */

    //------------------------------------------Function Separator ----------------------------------------------------
    //Keyword name static constants
    KEY_NAME = "name"; /**< Key for body name */
    KEY_HYDROBODY = "hydrobody"; /**< Key for hydrobody name. */
    KEY_MASS = "mass"; /**< Key for mass of body */
    KEY_IXX = "ixx"; /**< Key for moment of inertia, X-X axis */
    KEY_IYY = "iyy"; /**< Key for moment of inertia, Y-Y axis */
    KEY_IZZ = "izz"; /**< Key for moment of inertia, Z-Z axis */
    KEY_IXY = "ixy"; /**< Key for cross-product of inertia, X-Y coupling */
    KEY_IXZ = "ixz"; /**< Key for cross-product of inertia, X-Z coupling */
    KEY_IYZ = "iyz"; /**< Key for cross-product of inertia, Y-Z coupling */
    KEY_COGX = "cogx"; /**< Key for center of gravity, X-axis coordinate */
    KEY_COGY = "cogy"; /**< Key for center of gravity, Y-axis coordinate */
    KEY_COGZ = "cogz"; /**< Key for center of gravity, Z-axis coordinate */
    KEY_HEADING = "heading"; /**< Key for heading of body, Z-axis rotation */
    KEY_MOTION = "motion"; /**< Key to specify motion model.*/
    KEY_LINKEDBODY = "linkedbody"; /**< Key for linked body */
    KEY_MODEL = "model"; /**< Key for model to use for force specification. */
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
    switch(keyIn)
    {
    case KEY_NAME:
        //Set body name
        ptSystem->refBody(pBody).setBodyName(valIn[0]);
        //Return success
        return 0;
        break;

    case KEY_HYDROBODY:
        //Set hydrobody name
        ptSystem->refBody(pBody).setHydroBodName(valIn[0]);
        //Return success
        return 0;
        break;

    case KEY_MASS:
        //Set the mass
        ptSystem->refBody(pBody).setMass(valIn[0]);
        //Update the mass definitions
        pMassProp[0] = true;
        //Return success
        return 0;
        break;

    case KEY_IXX:
        //Set IXX
        ptSystem->refBody(pBody).setMomIxx(valIn[0]);
        //Update the mass definitions
        pMassProp[1] = true;
        //Return success
        return 0;
        break;

    case KEY_IYY:
        //Set IYY
        ptSystem->refBody(pBody).setMomIyy(valIn[0]);
        //Update the mass definitions
        pMassProp[2] = true;
        //Return success
        return 0;
        break;

    case KEY_IZZ:
        //Set IZZ
        ptSystem->refBody(pBody).setMomIzz(valIn[0]);
        //Update the mass definitions
        pMassProp[3] = true;
        //Return success
        return 0;
        break;

    case KEY_IXY:
        //Set IXY
        ptSystem->refBody(pBody).setMomIxy(valIn[0]);
        //Update the mass definitions
        pMassProp[4] = true;
        //Return success
        return 0;
        break;

    case KEY_IXZ:
        //Set IXZ
        ptSystem->refBody(pBody).setMomIxz(valIn[0]);
        //Update the mass definitions
        pMassProp[5] = true;
        //Return success
        return 0;
        break;

    case KEY_IYZ:
        //Set IYZ
        ptSystem->refBody(pBody).setMomIxz(valIn[0]);
        //Update the mass definitions
        pMassProp[6] = true;
        //Return success
        return 0;
        break;

    case KEY_COGX:
        //Set COGX
        ptSystem->refBody(pBody).setCenX(valIn[0]);
        //Update the COG definitions
        pCOG[0] = true;
        //Return success
        return 0;
        break;

    case KEY_COGY:
        //Set COGY
        ptSystem->refBody(pBody).setCenY(valIn[0]);
        //Update the COG definitions
        pCOG[1] = true;
        //Return success
        return 0;
        break;

    case KEY_COGZ:
        //Set COGZ
        ptSystem->refBody(pBody).setCenZ(valIn[0]);
        //Update the COG definitions
        pCOG[2] = true;
        //Return success
        return 0;
        break;

    case KEY_HEADING:
        //Set the heading for the body
        ptSystem->refBody(pBody).setHeading(valIn[0]);
        //Return success
        return 0;
        break;

    case KEY_MOTION:
        //Set the motion model name for the body
        ptSystem->refBody(pBody).setMotionModel(valIn[0]);
        //Return success
        return 0;
        break;

    case KEY_LINKEDBODY:
        //First check that the cross-body object is active
        if (pForceType == 3)
        {
            //Set the linked body name
            ptSystem->refBody(pBody).listCrossBody_user()[pForceBodyIndex] = valIn[0];

            //Return success
            return 0;
        }
        else
        {
            //Invalid use of key.
            //Return error
            return 2;
        }
        break;

    case KEY_MODEL:
        if (pForceType == 1)
        {
            //Active Force model
            ptSystem->refBody(pBody).listForceActive_user().push_back(
                        ptSystem->refForceActive_user(valIn[0])
                    );
            //Set body index
            pForceBodyIndex = ptSystem->refBody(pBody).listForceActive_user().size() - 1;
            //Set model index
            pForceIndex = ptSystem->refBody(pBody).listForceActive_user()[pForceBodyIndex].getSystemIndex();
            //Return success
            return 0;
        }
        else if (pForceType == 2)
        {
            //Reactive force model
            ptSystem->refBody(pBody).listForceReact_user().push_back(
                        ptSystem->refForceReact_user(valIn[0])
                    );
            //Set body index
            pForceBodyIndex = ptSystem->refBody(pBody).listForceReact_user().size() - 1;
            //Set model index
            pForceIndex = ptSystem->refBody(pBody).listForceReact_user()[pForceBodyIndex].getSystemIndex();
            //Return success
            return 0;
        }
        else if (pForceType == 3)
        {
            //Cross-body force type
            ptSystem->refBody(pBody).listForceCross_user().push_back(
                        ptSystem->refForceCross_user(valIn[0])
                    );
            //Set body index
            pForceBodyIndex = ptSystem->refBody(pBody).listForceCross_user().size() - 1;
            //Set model index
            pForceIndex = ptSystem->refBody(pBody).listForceCross_user()[pForceBodyIndex].getSystemIndex();
            //Return success
            return 0;
        }
        else
        {
            //Invalid use of the key.
            //Return error.
            return 2;
        }
        break;

    default:
        //Return error
        return 1;
        break;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int dictBodies::defineClass(string nameIn)
{
    switch (nameIn)
    {
    case OBJECT_BODY:
        //Create new body object in System
        ptSystem->addBody();

        //Set index of current body object.
        pBody = ptSystem->listBody().size() - 1;

        //Return success
        return 0;
        break;

    case OBJECT_MASSPROP:
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
        break;

    case OBJECT_CENTROID:
        // No action required.  Center of gravity properties created in body by default.

        //Reset all properties of COG keys.
        for (int i = 0; i <= 2; i++)
        {
            pCOG[i] = false;
        }

        //Return success
        return 0;
        break;

    case OBJECT_FORCE_ACTIVE:
        //No action required.  Force object already created.
        //Set internal tracker of force type.
        pForceType = 1;

        //Set the latest force index
        setForceIndex();

        //Return success
        return 0;
        break;

    case OBJECT_FORCE_REACT:
        //No action required.  Force object already created.
        //Set internal tracker of force type.
        pForceType = 2;

        //Set the latest force index
        setForceIndex();

        //Return success
        return 0;
        break;

    case OBJECT_FORCE_CROSS:
        //No action required.  Force object already created.
        //Set internal tracker of force type.
        pForceType = 3;

        //Set the latest force index
        setForceIndex();

        //Return success
        return 0;
        break;

    case OBJECT_MODEL:
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
        break;

    default:
        //Return error
        return 1;
        break;
    }
}

//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
 void dictBodies::setForceIndex(int forceIn = -1)
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
