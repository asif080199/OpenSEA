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

//######################################### Class Separator ###########################################################
#include "model6dof.h"

using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Public Members

//------------------------------------------Function Separator --------------------------------------------------------
Model6DOF::Model6DOF() :
    MotionModel()
{
    //Define the motion model, with all equations
    DefineEquations();
}

//------------------------------------------Function Separator --------------------------------------------------------
Model6DOF::Model6DOF(std::vector<Body> &listBodIn) :
    MotionModel(listBodIn)
{
    //Define the motion model, with all equations
    DefineEquations();
}

//------------------------------------------Function Separator --------------------------------------------------------
Model6DOF::~Model6DOF()
{

}

//*********************************************************************************************************************
//*********************************************************************************************************************
//                              DO NOT EDIT ANYTHING ABOVE THIS COMMENT BLOCK
//                              DO NOT EDIT ANYTHING ABOVE THIS COMMENT BLOCK
//*********************************************************************************************************************
//
// Seriously, unless you know the consequences of your actions, don't change anything above this comment block.
// This is part of the system to keep your customizations isolated so that you don't need to learn all the details
// of how the rest of the program works.
//
//*********************************************************************************************************************
//*********************************************************************************************************************

//Start editting from here on down.
//*********************************************************************************************************************

//==========================================Section Separator =========================================================
//Protected Members

//------------------------------------------Function Separator --------------------------------------------------------
void Model6DOF::DefineEquations()
{
    //Defines all equations for a six-degree of freedom (6DOF) rigid body dynamics motion model.

    //First create an equation object for translation.
    EqnTranslation *MotTrans1 = new EqnTranslation(this);     //Creation statement
    MotTrans1->setDataIndex(1);   //Set data index.  Data index for X-axis translation.
    MotTrans1->setName("TransX");     //Set short name for equation.
    MotTrans1->setDescription("Linear translation, X-axis");  //Set long description for equation.
    AddEquation(MotTrans1);  //Add the Equation to the list of equations.

    EqnTranslation *MotTrans2 = new EqnTranslation(this);    //Create next equation.
    MotTrans2->setDataIndex(2);  //Set data index.  Data index for Y-axis translation.
    MotTrans2->setName("TransY");    //Set short name for equation.
    MotTrans2->setDescription("Linear translation, Y-axis"); //Set long description for equation.
    AddEquation(MotTrans2); //Add the equation to the list of equations.

    EqnTranslation *MotTrans3 = new EqnTranslation(this);    //Create next equation.
    MotTrans3->setDataIndex(3);  //Set data index.  Data index for Z-axis translation.
    MotTrans3->setName("TransZ");    //Set short name for equation.
    MotTrans3->setDescription("Linear translation, Z-axis"); //Set long description for equation.
    AddEquation(MotTrans3); //Add the equation to the list of equations.


    EqnRotation *MotRot1 = new EqnRotation(this);    //Create next equation.
    MotRot1->setDataIndex(4);    //Set data index.  Data index for X-axis rotation.
    MotRot1->setName("RotX");    //Set short name for equation.
    MotRot1->setDescription("Angular rotation, about X-axis");   //Set description for equation.
    AddEquation(MotRot1);   //Add the equation ot the list of equations.

    EqnRotation *MotRot2 = new EqnRotation(this);    //Create next equation.
    MotRot2->setDataIndex(5);    //Set data index.  Data index for Y-axis rotation.
    MotRot2->setName("RotY");    //Set short name for equation.
    MotRot2->setDescription("Angular rotation, about Y-axis");   //Set description for equation.
    AddEquation(MotRot2);   //Add the equation ot the list of equations.

    EqnRotation *MotRot3 = new EqnRotation(this);    //Create next equation.
    MotRot3->setDataIndex(6);    //Set data index.  Data index for Z-axis rotation.
    MotRot3->setName("RotZ");    //Set short name for equation.
    MotRot3->setDescription("Angular rotation, about Z-axis");   //Set description for equation.
    AddEquation(MotRot3);   //Add the equation ot the list of equations.

    //All equations added.

    //Define name for motion model.  Used to select the motion model in the input files.
    setName("6DOF");
    //Define description for motion model.  Just for user reference in output.
    setDescription("Six degree of freedom (6DOF) rigid body dynamics model");

    //All done.  Motion model fully defined.
}

//==========================================Section Separator =========================================================
//Private Members
