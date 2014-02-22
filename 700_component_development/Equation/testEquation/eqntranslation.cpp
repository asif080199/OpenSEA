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
    if (this->refDataIndex() == 1) //Data index for X-translation
    {
        //Add in mass objects.
        valOut = ForceMass(1) * Ddt(1,2) +  //Direct mass object
                 ForceMass(5) * Ddt(5,2) +
                 ForceMass(6) * Ddt(6,2);
    }
    else if (this->refDataIndex() == 2) //Data index for Y-translation
    {
        //Add in mass objects.
        valOut = ForceMass(2) * Ddt(2,2) +
                 ForceMass(4) * Ddt(4,2) +
                 ForceMass(6) * Ddt(5,2);
    }
    else if (this->refDataIndex() == 3) //Data index for Z-translation
    {
        //Add in mass objects.
        valOut = ForceMass(3) * Ddt(3,2) +
                 ForceMass(4) * Ddt(4,2) +
                 ForceMass(5) * Ddt(5,2);
    }

//    //Add in reactive force objects
//    //Sum hydrodynamic reactive forces for derivative orders 0 - 2
//    valOut += Sum(
//                  &EqnTranslation::Func2, "ord", 0, 2
//                  );

//    //Sum user reactive forces for however high the derivative order goes.
//    valOut += Sum(
//                  &EqnTranslation::Func4, "ord"
//                  );


//    //Add in cross-body force objects
//    //Add cross-body forces for hydrodynamic forces.  Sum for order from 0 to 2.
//    valOut += Sum(
//                  &EqnTranslation::Func6, "body"
//                  );

//    valOut += Sum(
//                  &EqnTranslation::Func8, "body"
//                  );

//    //Add in active force objects.
//    //Active forces must be entered negative to account for the rearranged equation.
//    valOut -= ForceActive_user();
//    valOut -= ForceActive_hydro();

    cout << "Testing Sum of Member Function" << endl;
    cout << "Sum(0 to 2) = " << Sum("Func1()", "body",0,2) << endl;
    cout << "Sum(0 to 2), function = " << Func2() << endl;
    cout << "SSum(0 to 2) = " << Sum("Func2()", "body", 0, 2) << endl << endl;



    //Write out results
    return valOut;


}

//==========================================Section Separator =========================================================
//Custom function definitions

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func1()
{
    return ForceReact_hydro(ord(),var()) * Ddt(var(),ord());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func2()
{
    return Sum("Func1()", "var", 0, 5);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func3()
{
    return ForceReact_user(ord(),var()) * Ddt(var(),ord());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func4()
{
    return Sum("Func3()", "var");
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func5()
{
    return ForceCross_hydro(body(),ord(),var()) * Ddt(var(),ord(),body());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func6()
{
    return Kronecker(curbody(),body(),true) *
            Sum("Func9()", "ord", 0, 2);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func7()
{
    return ForceCross_user(body(),ord(),var()) * Ddt(var(),ord(),body());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func8()
{
    return Kronecker(curbody(),body(),true) *
            Sum("Func10()", "ord");
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func9()
{
    return Sum("Fun5()", "var", 0, 5);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func10()
{
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
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func21()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func22()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func23()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func24()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func25()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func26()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func27()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func28()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func29()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Func30()
{
    return complex<double>(0,0);
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
