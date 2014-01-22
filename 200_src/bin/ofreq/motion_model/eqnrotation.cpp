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
//Custom function definitions


//==========================================Section Separator =========================================================
//Protected Members

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EqnRotation::setFormula()
{
    std::complex<double> valOut;

    //Add in mass objects
    valOut = Sum(
                 [&]() -> std::complex<double> {return ForceMass(var()) * Ddt(var(),2);},
                 "var"
                 );

    //Add in reactive force objects
    valOut += Sum(
                  Sum(
                       [&]() -> std::complex<double> {return ForceReact_hydro(ord(),var()) * Ddt(var(),ord());},
                       "var",
                       0,
                       5),
                  "ord",
                   0,
                   2);
    valOut += Sum(
                  Sum(
                       [&]() -> std::complex<double> {return ForceReact_user(ord(),var()) * Ddt(var(),ord());},
                       "var"),
                  "ord");

    //Add in cross-body force objects
    valOut += Sum(
                  [&]() -> std::complex<double> {return Kronecker(curbody(),body(),true) *
                  Sum(
                      Sum(
                           [&]() -> std::complex<double> {return ForceCross_hydro(body(),ord(),var()) *
                                                         Ddt(var(),ord(),body());},
                           "var",
                           0,
                           5),
                      "ord");},
                  "body");

    valOut += Sum(
                  [&]() -> std::complex<double> {return Kronecker(curbody(),body(),true) *
                  Sum(
                      Sum(
                          [&]() -> std::complex<double> {return ForceCross_user(body(),ord(),var()) *
                                                        Ddt(var(),ord(),body());},
                          "var"),
                      "ord");},
                  "body");

    //Add in active force objects.
    //Active forces must be entered negative to account for the rearranged equation.
    valOut -= ForceActive_user();
    valOut -= ForceActive_hydro();

    //Write out results
    return valOut;
}


//==========================================Section Separator =========================================================
//Private Members
