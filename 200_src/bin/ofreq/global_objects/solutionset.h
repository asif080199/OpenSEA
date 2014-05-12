/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date          Author                  Description
 *---------------------------------------------------------------------------------------------------------------------
 *Mar 09 2013	Nicholas Barczak		Initially Created
 *May 7, 2014       Nicholas Barczak    Debugged and updated to Rev 1.0
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
#ifndef LISTSOLUTION_H
#define LISTSOLUTION_H
#include "solution.h"
#include <new>
#include <vector>
#include <QtGlobal>
#ifdef Q_OS_WIN
    //References for windows go in here.
#elif defined Q_OS_LINUX
    //References for linux go in here.
#endif
#include "../system_objects/ofreqcore.h"

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

typedef std::vector<Solution*> ptSoln; /**< Type definition for a vector of pointers to Solution object.*/

//######################################### Class Separator ###########################################################
/**
 * This class records the list of solutions obtained for a single Body object.  It is essentially a 2D version of the
 * std::vector<object> class.  But since vector can't handle 2-d storage solutions, this class was created.
 */
class SolutionSet : public osea::ofreq::oFreqCore
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.
     */
    SolutionSet();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Constructor with number of wave directions and wave frequencies specified.  Automatically allocates
     * dynamic memory for the specified number of wave directions and wave frequencies.
     * @param dir The new number of wave directions to resize the array to.
     * @param freq The new number of wave frequencies to resize the array to.
     */
    SolutionSet(int dir, int freq);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Default destructor.  Frees any memory dynamically assigned.
     */
    ~SolutionSet();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a pointer to the solution object at the specified wave direction and frequency.
     * @param dir Integer.  Index of the wave direction desired.
     * @param freq Integer.  Index of the wave frequency desired.
     * @return Returns a pointer to the Solution object at the specified wave direction and wave frequency.  Returned
     * variable is passed by reference.
     */
    Solution &refSolution(int dir, int freq);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the solution object at the specified wave direction and frequency.
     * @param dir Integer.  Index of the wave direction desired.
     * @param freq Integer.  Index of the wave frequency desired.
     * @param soln The solution object to pass in.
     */
    void setSolnMat(int dir, int freq, Solution soln);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the solution object at the specified wave direction and frequency.
     * @param dir Integer.  Index of the wave direction desired.
     * @param freq Integer.  Index of the wave frequency desired.
     * @return Returns the Solution object at the specified wave direction and wave frequency.  Returned variable is
     * passed by value.
     */
    Solution getSolution(int dir, int freq);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Resizes the 2-D array of Solution objects.  Any existing objects in the array are preserved.  If the
     * array is sized smaller than the existing number of Solution objects, any object beyond the new index range
     * are deleted.
     * @param dir The new number of wave directions to resize the array to.
     * @param freq The new number of wave frequencies to resize the array to.
     */
    void resize(int dir, int freq);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the size of the matrix as a vector of two elements.
     * @return Integer.  Returns the size of the matrix as a vector of two elements.
     */
    std::vector<int> size();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the number of rows as an integer.  This is the number of wave directions.
     * @return Integer.  Returns the number of rows as an integer.  This is the number of wave directions.
     */
    int n_dirs();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the number of columns as an integer.  This is the number of wave frequencies.
     * @return Integer.  Returns the number of columns as an integer.  This is the number of wave frequencies.
     */
    int n_freqs();

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Array of Solution objects.  Dynamically  allocated.  The first index is the wave direction. (The rows
     * direction.)  The second index is the wave frequency (The columns direction.)
     */
    std::vector< ptSoln > plist;

};

}   //Namespace ofreq
}   //Namespace osea

#endif // LISTSOLUTION_H
