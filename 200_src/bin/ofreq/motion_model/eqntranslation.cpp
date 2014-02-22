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

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnTranslation::Sum(std::complex<double> (EqnTranslation::*force)(void),
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
complex<double> EqnTranslation::Func1()
{
    return ForceReact_hydro(ord(),var()) * Ddt(var(),ord());
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EqnTranslation::Func2()
{
    return Sum(
               &EqnTranslation::Func1, "var", 0, 5
               );
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EqnTranslation::Func3()
{
    return ForceReact_user(ord(),var()) * Ddt(var(),ord());
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EqnTranslation::Func4()
{
    return Sum(
               &EqnTranslation::Func3, "var"
               );
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EqnTranslation::Func5()
{
    return ForceCross_hydro(body(),ord(),var()) * Ddt(var(),ord(),body());
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EqnTranslation::Func6()
{
    return Kronecker(curbody(),body(),true) *
            Sum(
                &EqnTranslation::Func9,
                "ord", 0, 2
                );
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EqnTranslation::Func7()
{
    return ForceCross_user(body(),ord(),var()) * Ddt(var(),ord(),body());
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EqnTranslation::Func8()
{
    return Kronecker(curbody(),body(),true) *
            Sum(
                &EqnTranslation::Func10,
                "ord"
                );
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EqnTranslation::Func9()
{
    return Sum(
                &EqnTranslation::Func5, "var", 0, 5
                );
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EqnTranslation::Func10(){
    return Sum(
                &EqnTranslation::Func7, "var"
                );
}


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

    //Add in reactive force objects
    //Sum hydrodynamic reactive forces for derivative orders 0 - 2
    valOut += Sum(
                  &EqnTranslation::Func2, "ord", 0, 2
                  );

    //Sum user reactive forces for however high the derivative order goes.
    valOut += Sum(
                  &EqnTranslation::Func4, "ord"
                  );


    //Add in cross-body force objects
    //Add cross-body forces for hydrodynamic forces.  Sum for order from 0 to 2.
    valOut += Sum(
                  &EqnTranslation::Func6, "body"
                  );

    valOut += Sum(
                  &EqnTranslation::Func8, "body"
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
