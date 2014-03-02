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

#include "solutionset.h"

using namespace std;
using namespace osea::ofreq;

//------------------------------------------Function Separator --------------------------------------------------------
SolutionSet::SolutionSet()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
SolutionSet::SolutionSet(int dir, int freq)
{
    //Constructor with wave directions and frequencies specified.
    resize(dir, freq);
}

//------------------------------------------Function Separator --------------------------------------------------------
SolutionSet::~SolutionSet()
{
    //Delete all solution objects to clear memory
    for (unsigned int i = 0; i < plist.size(); i++)
    {
        for (unsigned int j = 0; j < plist[i].size(); j++)
        {
            delete plist[i][j];
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
Solution &SolutionSet::refSolution(int dir, int freq)
{
    //Return reference to specified solution object.
    return *(plist[dir][freq]);
}

//------------------------------------------Function Separator --------------------------------------------------------
void SolutionSet::setSolnMat(int dir, int freq, Solution soln)
{
    //Set the Solution object with the new value.
    //Create new solution object
    Solution* newSoln = new Solution();

    //Set data for new solution
    newSoln->setBody(&soln.refBody());
    newSoln->setSolnMat(soln.getSolnMat());

    //Check that the specified index is the correct size.  If not, resize.
    if ((dir > n_dirs()) || (freq > n_freqs()))
        this->resize(dir, freq);

    //Set the solution
    plist[dir][freq] = newSoln;
}

//------------------------------------------Function Separator --------------------------------------------------------
Solution SolutionSet::getSolution(int dir, int freq)
{
    //Return the solution
    return *(plist[dir][freq]);
}

//------------------------------------------Function Separator --------------------------------------------------------
void SolutionSet::resize(int dir, int freq)
{
    //Resize the vector
    plist.resize(dir);
    for (unsigned int i = 0; i < plist.size(); i++)
    {
        plist[i].resize(freq);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<int> SolutionSet::size()
{
    vector<int> output(2);

    output[0] = plist.size();
    output[1] = plist[0].size();

    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
int SolutionSet::n_dirs()
{
    return size()[0];
}

//------------------------------------------Function Separator --------------------------------------------------------
int SolutionSet::n_freqs()
{
    return size()[1];
}
