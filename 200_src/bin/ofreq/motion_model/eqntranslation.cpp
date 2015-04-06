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
#include "eqntranslation.h"

//######################################### Class Separator ###########################################################
using namespace osea::ofreq;
using namespace std;

//######################################### Class Separator ###########################################################

//==========================================Section Separator =========================================================
//Public Members

//------------------------------------------Function Separator --------------------------------------------------------
EqnTranslation::EqnTranslation(MotionModel *modelIn) :
    EquationofMotion(modelIn)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
EqnTranslation::EqnTranslation(MotionModel *modelIn, std::string NameIn) :
    EquationofMotion(modelIn, NameIn)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
EqnTranslation::EqnTranslation(MotionModel *modelIn, std::string NameIn, int IndexIn) :
    EquationofMotion(modelIn, NameIn, IndexIn)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
EqnTranslation::~EqnTranslation()
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
complex<double> EqnTranslation::setFormula()
{
    complex<double> valOut;

    //I only want to define a single equation to linear translation.  Add some if statements to change equation
    //definition, depending on equation index.
    if (this->getDataIndex() == 0) //Data index for X-translation, in computer numbering
    {
        //Add in mass objects.
        valOut = ForceMass(1) * Ddt("Func21()",2) +  //Direct mass object
                 ForceMass(5) * Ddt("Func25()",2) +
                 ForceMass(6) * Ddt("Func26()",2);
    }
    else if (this->getDataIndex() == 1) //Data index for Y-translation, in computer numbering
    {
        //Add in mass objects.
        valOut = ForceMass(2) * Ddt("Func22()",2) +
                 ForceMass(4) * Ddt("Func24()",2) +
                 ForceMass(6) * Ddt("Func25()",2);
    }
    else if (this->getDataIndex() == 2) //Data index for Z-translation, in computer numbering
    {
        //Add in mass objects.
        valOut = ForceMass(3) * Ddt("Func23()",2) +
                 ForceMass(4) * Ddt("Func24()",2) +
                 ForceMass(5) * Ddt("Func25()",2);
    }


    //Add in reactive force objects
    //Sum hydrodynamic reactive forces for derivative orders 0 - 2
    valOut += Sum("Func2()", "ord", 0, 2);              //ForceReact_hydro

    //Sum user reactive forces for however high the derivative order goes.
    valOut += Sum("Func4()", "ord");                    //ForceReact_user


    //Add in cross-body force objects
    //Add cross-body forces for hydrodynamic forces.  Sum for order from 0 to 2.
    valOut += Sum("Func6()", "body");                   //ForceCross_hydro
    valOut += Sum("Func8()", "body");                   //ForceCross_user

    //Add in active force objects.
    //Active forces must be entered negative to account for the rearranged equation.
    valOut -= ForceActive_user();
    valOut -= ForceActive_hydro();

    //Write out results
    return valOut;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::setVarGlobtoBod()
{
    //I only want to define a single class for all of the translation equations.
    //So I will use if-then statements to create three separate resposnes.

    if (this->getDataIndex() == 0)  //Data index for x-translation, in computer numbering
    {
        return Func21();    //Return variable in body coordinate system.
    }

    else if (this->getDataIndex() == 1) //Data index for y-translation, in computer numbering
    {
        return Func22();    //Return variable in body coordinate system.
    }

    else if (this->getDataIndex() == 2) //Data index for z-translation, in computer numbering
    {
        return Func23();    //Return variable in body coordinate system.
    }
}

//==========================================Section Separator =========================================================
//Custom function definitions

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func1()
{
    //ForceReact_hydro
    return ForceReact_hydro(ord(),var()) * Func20();
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func2()
{
    //ForceReact_hydro
    return Sum("Func1()", "var", 1, 6);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func3()
{
    //ForceReact_user
    return ForceReact_user(ord(),var()) * Func20();
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func4()
{
    //ForceReact_user
    return Sum("Func3()", "var");
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func5()
{
    //ForceCross_hydro
    return ForceCross_hydro(body(),ord(),var()) * Func20();
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func6()
{
    //ForceCross_hydro
    return Kronecker(curbody(),body(),true) *
            Sum("Func9()", "ord", 0, 2);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func7()
{
    //ForceCross_user
    return ForceCross_user(body(),ord(),var()) * Func20();
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func8()
{
    //ForceCross_user
    return Kronecker(curbody(),body(),true) *
            Sum("Func10()", "ord");
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func9()
{
    //ForceCross_hydro
    return Sum("Func5()", "var", 1, 6);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func10()
{
    //ForceCross_user
    return Sum("Func7()", "var");
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func11()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func12()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func13()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func14()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func15()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func16()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func17()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func18()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func19()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func20()
{
    //Returning derivative of motion variables, in body coordinate system.

    std::complex<double> out;

    switch(var())
    {
    case 1:
        out = Ddt("Func21()", ord(), bod());
    case 2:
        out = Ddt("Func22()", ord(), bod());
    case 3:
        out = Ddt("Func23()", ord(), bod());
    case 4:
        out = Ddt("Func24()", ord(), bod());
    case 5:
        out = Ddt("Func25()", ord(), bod());
    case 6:
        out = Ddt("Func26()", ord(), bod());
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func21()
{
    //Translation X in body coordinates
    //X_1a * cos(R_z) + X_2a * sin(R_z)
    Func27() * cos(R_z()) + Func28() * sin(R_z());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func22()
{
    //Translation Y in body coordinates
    //-X_1a * sin(R_z) + X_2a * cos(R_z)
    return -Func27() * sin(R_z()) + Func28() * cos(R_z());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func23()
{
    return T_x() * Ddt(6,0) - T_y() * Ddt(4,0) + Ddt(3,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func24()
{
    return Func29() * cos(R_z()) + Func30() * sin(R_z());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func25()
{
    return -Func29() * sin(R_z()) + Func30() * cos(R_z());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func26()
{
    return T_x() * Ddt(3,0) - T_y() * Ddt(2,0) + Ddt(5,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func27()
{
    //Partial conversion of body coordinates.
    //X1a;
    return T_y() * Ddt(6,0) - T_z() * Ddt(5,0) + Ddt(1,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func28()
{
    //Partial conversion of body coordinates.
    //X2a;
    return -T_x() * Ddt(6,0) + T_z() * Ddt(4,0) + Ddt(2,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func29()
{
    //Partial conversion of body coordinates.
    //X4a;
    return T_y() * Ddt(3,0) - T_z() * Ddt(2,0) + Ddt(4,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func30()
{
    //Partial conversion of body coordinates.
    //X5a;
    return -T_x() * Ddt(3,0) + T_z() * Ddt(1,0) + Ddt(5,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func31()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func32()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func33()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func34()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func35()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func36()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func37()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func38()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func39()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func40()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func41()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func42()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func43()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func44()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func45()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func46()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func47()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func48()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func49()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func50()
{
    return complex<double>(0,0);
}

//==========================================Section Separator =========================================================
//Private Members
