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
#include "eqnrotation.h"

//######################################### Class Separator ###########################################################
using namespace osea::ofreq;

//######################################### Class Separator ###########################################################

//==========================================Section Separator =========================================================
//Public Members

//------------------------------------------Function Separator --------------------------------------------------------
EqnRotation::EqnRotation(MotionModel *modelIn) :
    EquationofMotion(modelIn)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
EqnRotation::EqnRotation(MotionModel *modelIn, std::string NameIn) :
    EquationofMotion(modelIn, NameIn)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
EqnRotation::EqnRotation(MotionModel *modelIn, std::string NameIn, int IndexIn) :
    EquationofMotion(modelIn, NameIn, IndexIn)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
EqnRotation::~EqnRotation()
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
std::complex<double> EqnRotation::setFormula()
{
    std::complex<double> valOut;

    //Add in mass objects
    valOut = Sum("Func1()", "var");                 //ForceMass

    //Add in reactive force objects
    valOut += Sum("Func10()", "ord", 0, 2);         //ForceReact_hydro
    valOut += Sum("Func11()", "ord");               //ForceReact_user

    //Add in cross-body force objects
    valOut += Sum("Func6()", "body");               //ForceCross_hydro
    valOut += Sum("Func9()", "body");               //ForceCross_user

    //Add in active force objects.
    //Active forces must be entered negative to account for the rearranged equation.
    valOut -= ForceActive_user();                   //ForceActive_user
    valOut -= ForceActive_hydro();                  //ForceActive_hydro

    //Write out results
    return valOut;
}

//==========================================Section Separator =========================================================
//Custom function definitions

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func1()
{
    //ForceMass
    return ForceMass(var()) * Ddt(var(),2);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func2()
{
    //ForceReact_hydro
    return ForceReact_hydro(ord(),var()) * Ddt(var(),ord());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func3()
{
    //ForceReact_user
    return ForceReact_user(ord(),var()) * Ddt(var(),ord());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func4()
{
    //ForceCross_hydro
    return ForceCross_hydro(body(),ord(),var()) * Ddt(var(),ord(),body());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func5()
{
    //ForceCross_hydro
    return Sum("Func4()", "var", 1, 6);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func6()
{
    //ForceCross_hydro
    return Kronecker(curbody(),body(),true) *
            Sum("Func5()", "ord");
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func7()
{
    //ForceCross-user
    return ForceCross_user(body(),ord(),var()) * Ddt(var(),ord(),body());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func8()
{
    //ForceCross-user
    return Sum("Func7()", "var");
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func9()
{
    //ForceCross-user
    return Kronecker(curbody(),body(),true) *
            Sum("Func8()", "ord");
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func10()
{
    //ForceReact_hydro
    return Sum("Func2()", "var", 1, 6);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func11()
{
    //ForceReact_user
    return Sum("Func3()", "var");
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func12()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func13()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func14()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func15()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func16()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func17()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func18()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func19()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func20()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func21()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func22()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func23()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func24()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func25()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func26()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func27()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func28()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func29()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func30()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func31()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func32()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func33()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func34()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func35()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func36()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func37()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func38()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func39()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func40()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func41()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func42()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func43()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func44()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func45()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func46()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func47()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func48()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func49()
{
    return std::complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func50()
{
    return std::complex<double>(0,0);
}


//==========================================Section Separator =========================================================
//Private Members
