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

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Sum(std::complex<double> (EqnRotation::*force)(void),
                                           std::string index, int from, int to)
{
    //Create variable for output
    std::complex<double> output(0,0);        //Output variable

    //Select which index to Sum over.
    //Sum for variable
    //-----------------------------------
    if ((index.compare("var") == 0) ||
            (index.compare("v") == 0) ||
            (index.compare("V") == 0) ||
            (index.compare("Var") == 0) ||
            (index.compare("VAR") == 0))
    {
        //Check for summation limits
        if (from == undefArg)
        {
            //Get limit
            from = 0;
        }

        if (to == undefArg)
        {
            //Get limit
            to = maxvar();
        }

        //Sum for variable count.
        for (pCurVar = from ; pCurVar <= to; pCurVar++)
        {
            output = output + (this->*force)();
        }
        //Return counter to max limit
        pCurVar = to;
    }

    //Sum for Derivative Order
    //-----------------------------------
    else if ((index.compare("ord") == 0) ||
             (index.compare("o") == 0) ||
             (index.compare("O") == 0) ||
             (index.compare("Ord") == 0) ||
             (index.compare("ORD") == 0))
    {
        //Check for summation limits
        if (from == undefArg)
        {
            //Get limit
            from = 0;
        }

        if (to == undefArg)
        {
            //Get limit
            to = maxord();
        }

        //Sum for order of refDerivative.
        for (pCurOrd = from ; pCurOrd <= to; pCurOrd++)
        {
            output = output + (this->*force)();
        }
        //Return counter to max limit
        pCurOrd = to;
    }

    //Sum for Body
    //-----------------------------------
    else if ( (index.compare("bod") == 0) ||
              (index.compare("b") == 0) ||
              (index.compare("B") == 0) ||
              (index.compare("Bod") == 0) ||
              (index.compare("BOD") == 0) ||
              (index.compare("body") == 0) ||
              (index.compare("Body") == 0))
    {
        //Check for summation limits
        if (from == undefArg)
        {
            //Get limit
            from = 0;
        }

        if (to == undefArg)
        {
            //Get limit
            to = maxbody();
        }

        //Sum for bodies
        for (pBod = from ; pBod <= to; pBod++)
        {
            output = output + (this->*force)();
        }
        //Return counter to max limit
        pBod = to;
    }
    //write output
    return output;
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
//Custom function definitions

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func1()
{
    return ForceMass(var()) * Ddt(var(),2);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func2()
{
    return ForceReact_hydro(ord(),var()) * Ddt(var(),ord());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func3()
{
    return ForceReact_user(ord(),var()) * Ddt(var(),ord());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func4()
{
    return ForceCross_hydro(body(),ord(),var()) * Ddt(var(),ord(),body());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func5()
{
    return Kronecker(curbody(),body(),true) *
            Sum(
                &EqnRotation::Func9, "ord"
                );
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func6()
{
    return ForceCross_user(body(),ord(),var()) * Ddt(var(),ord(),body());
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func7()
{
    return Kronecker(curbody(),body(),true) *
            Sum(
                &EqnRotation::Func8, "ord"
                );
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func8()
{
    return Sum(
                &EqnRotation::Func6, "var"
                );
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func9()
{
    return Sum(
                &EqnRotation::Func4, "var", 0, 5
                );
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func10()
{
    return Sum(
                &EqnRotation::Func2, "var", 0, 5
                );
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::Func11()
{
    return Sum(
                &EqnRotation::Func3,"var"
                );
}

//==========================================Section Separator =========================================================
//Protected Members

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::setFormula()
{
    std::complex<double> valOut;

    //Add in mass objects
    valOut = Sum(
                 &EqnRotation::Func1, "var"
                 );

    //Add in reactive force objects
    valOut += Sum(
                  &EqnRotation::Func10, "ord", 0, 2
                  );

    valOut += Sum(
                  &EqnRotation::Func11, "ord"
                  );

    //Add in cross-body force objects
    valOut += Sum(
                  &EqnRotation::Func5, "body"
                  );

    valOut += Sum(
                  &EqnRotation::Func6, "body"
                  );

    //Add in active force objects.
    //Active forces must be entered negative to account for the rearranged equation.
    valOut -= ForceActive_user();
    valOut -= ForceActive_hydro();

    //Write out results
    return valOut;
}


//==========================================Section Separator =========================================================
//Private Members
