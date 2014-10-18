/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date              Author				Description
 *---------------------------------------------------------------------------------------------------------------------
 *Aug 02 2014       Nicholas Barczak    Initially created
 *
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
#ifndef MATHINTERP_H
#define MATHINTERP_H
#include <vector>
#include "../system_objects/ofreqcore.h"
#include "../motion_solver/matforceactive.h"
#include "../motion_solver/matforcecross.h"
#include "../motion_solver/matforcereact.h"

//######################################### Class Separator ###########################################################
//Namespace declarations

//==========================================Section Separator =========================================================
/**
 * The namespace for all code created under the OpenSEA project.  There are also several sub-namespaces, one
 * associated with each primary program under osea.
 * 1.)  ohydro:  Code associated with the program ohydro.
 * 2.)  ofreq:   Code associated with the program ofreq.
 * 3.)  otime:   Code associated with the program otime.
 * 4.)  ofourier:  Code associated with the program ofourier.
 * 5.)  obatch:    Code associated with the program obatch.
 * 6.)  guisea:    Code assocaited with the GUI that interacts with all OpenSEA programs.
 * Any code that may have common utility amongst all programs, such as file reading objects, goes under the generic
 * osea namespace.  Any code that is only useful within the specific program it serves, goes under the specific
 * namespace.  When in doubt, default to just the osea namespace.
 *
 * The namespaces are not intended to create an organizational structure.  They are only intended to prevent
 * name conflicts.
 */
namespace osea
{

//==========================================Section Separator =========================================================
/**
 * The namespace of all code specifically associated with ofreq.
 */
namespace ofreq
{

//######################################### Class Separator ###########################################################
/**
 * @brief The mathInterp class contains all math functions used for interpolation and extrapolation of data
 *
 * The class is designed to be used for internal functions in other classes.  Many of the functions are designed
 * as generic templates to allow large compatibility with other object types.
 */
class mathInterp : public osea::ofreq::oFreqCore
{
//==========================================Section Separator =========================================================
public:

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Nothing happens here.
     */
    mathInterp();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.  Nothing happens here.
     */
    ~mathInterp();

//==========================================Section Separator =========================================================
protected:

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A linear interpolation function
     *
     * This function will perform linear interpolation or extrapolation, based on two data points.  It is a template
     * function and can take almost any data type.  The independent variables (the x variables) in the function must
     * be double data type, but the dependent variables (the y variables) can be almost any data types, assuming
     * the data type supports certain operators.
     *
     * This function was defined within the header file because it is a template function.  For inheritence to work
     * properly, it must be defined in the header file.
     * @param x Double, variable passed by value.  This is the x value for the output that you want.  The result of the
     * interpolation will be based on this x value.
     * @param x1 Double, variable passed by value.  This is the first x value for the data that you have.
     * @param x2 Double, variable passed by value.  This is the second x value for the data that you have.
     * @param y1 Template variable, passed by value.  This is the first y value for the data that you have.  This y
     * value corresponds to the x1 value given earlier.  The object type passed to this parameter must support the
     * following operations:  (+, -, *)
     * @param y2 Template variable, passed by value.  This is the second y value for the data that you have.  This y
     * value corresponds to the x2 value given earlier.  The object type passed to this parameter must support the
     * following operations:  (+, -, *)
     * @return Returned type is the template type.  Variable is passed by value.
     */
    //template <class T>
    //T iePolate(double x, double x1, double x2, T y1, T y2);
    template <class T>
    T iePolate(double x, double x1, double x2, T y1, T y2)
    {
        //Calculate ratio
        try {
            //Check for division by zero
            if (x2 == x1)
                throw std::overflow_error("Divide by zero");

            double p = (x - x1) / (x2 - x1);

            //Check for a few cases
            if (p == 0.0)
                return y1;
            else if (p == 1.0)
                return y2;
            else
                return (y2 - y1) * p + y1;
        }
        catch(const std::exception &err)
        {
            logStd.Notify();
            logErr.Write(std::string(ID) + std::string(">>  ") + err.what());

        }
        catch(...)
        {
            logStd.Notify();
            logErr.Write(std::string(ID) + std::string(">>  Unknown error occurred."));
        }
    }


    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A linear interpolation function
     *
     * This function will perform linear interpolation, based on two data points.  It will only interpolate within
     * the data points.  If the requested value is outside the data points, it will return the nearest bound
     * to the requested value.  THis is a template
     * function and can take almost any data type.  The independent variables (the x variables) in the function must
     * be double data type, but the dependent variables (the y variables) can be almost any data types, assuming
     * the data type supports certain operators.
     *
     * This function was defined within the header file because it is a template function.  For inheritence to work
     * properly, it must be defined in the header file.
     * @param x Double, variable passed by value.  This is the x value for the output that you want.  The result of the
     * interpolation will be based on this x value.
     * @param x1 Double, variable passed by value.  This is the first x value for the data that you have.
     * @param x2 Double, variable passed by value.  This is the second x value for the data that you have.
     * @param y1 Template variable, passed by value.  This is the first y value for the data that you have.  This y
     * value corresponds to the x1 value given earlier.  The object type passed to this parameter must support the
     * following operations:  (+, -, *)
     * @param y2 Template variable, passed by value.  This is the second y value for the data that you have.  This y
     * value corresponds to the x2 value given earlier.  The object type passed to this parameter must support the
     * following operations:  (+, -, *)
     * @return Returned type is the template type.  Variable is passed by value.
     */
    //template <class T>
    //T iPolate(double x, double x1, double x2, T y1, T y2);
    template <class T>
    T iPolate(double x, double x1, double x2, T y1, T y2)
    {
        //Calculate ratio
        try {
            //Check for division by zero
            if (x2 == x1)
                throw std::overflow_error("Divide by zero");

            double p = (x - x1) / (x2 - x1);

            //Check for a few cases
            if (p < -1.0)
            {
                //Under lower bound.  Return y1
                return y1;
            }
            else if(p > 1.0)
            {
                //Over upper bound.  Return y2
                return y2;
            }
            else
            {
                //Somewhere in the middle, as it should be.  Perform interpolation.

                //Calculate output and write out
                return ((y2 - y1) * p) + y1;
            }
        }
        catch(const std::exception &err)
        {
            logStd.Notify();
            logErr.Write(std::string(ID) + std::string(">>  ") + err.what());
        }
        catch(...)
        {
            logStd.Notify();
            logErr.Write(std::string(ID) + std::string(">>  Unknown error occurred."));
        }
    }

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Finds the indices of the two items that are closest to the specified search term.
     * @param valSearch Double, passed by value.  The value that you are trying to find the closest match to.
     * @param listVal Vector of doubles, passed by reference.  The list of possible values to search through.
     * @return Returns a vector of only two entries.  These are the indices of the two closest matches.
     *      FindMatch[0] = The closest match
     *      FindMatch[1] = The second closest match
     */
    std::vector<int> FindMatch(double valSearch, std::vector<double> &listVal);


    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Checks an angle to ensure that it falls within the required range of 0 - 2*pi rad.
     *
     * If the angle is less than zero, 2pi rad will be added.  If the direction is over 2*pi rad, 2*pi rad
     * will be subtracted.  If neither of those operations brings the value within the specified range, the
     * function throws an error.
     * @param angIn Double, variable passed by value.  The input, unchecked angle, in units of radians (rad).
     * @return Returns a double, variable passed by value.  The output, checked angle, in units of radians.
     */
    double checkAngle(double angIn);

//==========================================Section Separator =========================================================
private:


};

}   //Namespace ofreq

}   //Namespace osea

#endif // MATHINTERP_H
