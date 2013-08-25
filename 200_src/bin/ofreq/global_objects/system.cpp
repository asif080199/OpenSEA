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

#include "system.h"

//==========================================Section Separator =========================================================
//Public methods

//------------------------------------------Function Separator --------------------------------------------------------
System::System() : pWaveDirections(), pWaveFrequencies()
{
	/*waveDirections = new WaveDirections();
	waveFrequencies = new WaveFrequencies();*/
}

//------------------------------------------Function Separator --------------------------------------------------------
System::~System()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::testPrint()
{
	cout << "Analysis Type: " << analysisType << endl;
	pWaveDirections.testPrint();
	pWaveFrequencies.testPrint();
	cout << endl;
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::setWaveFrequencies(vector<double> vecIn)
{
    pWaveFrequencies = vecIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::setWaveDirections(vector<double> vecIn)
{
    pWaveDirections = vecIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::setSpreadModel(string spreadIn)
{
	pWaveDirections.setSpreadModel(spreadIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> System::getWaveFrequencies()
{
    return pWaveFrequencies;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &System::refWaveFrequencies()
{
    return pWaveFrequencies;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> System::getWaveDirections()
{
    return pWaveDirections;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<double> &System::refWaveDirections()
{
    return pWaveDirections;
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::setCurWaveDirInd(int input)
{
    pCurWaveDir = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::setCurFreqInd(int input)
{
    pCurWaveFreq = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
int System::getCurWaveDirInd()
{
    return pCurWaveDir;
}

//------------------------------------------Function Separator --------------------------------------------------------
int System::getCurFreqInd()
{
    return pCurWaveFreq;
}

//------------------------------------------Function Separator --------------------------------------------------------
double System::getCurWaveDir()
{
    return pWaveDirections[pCurWaveDir];
}

//------------------------------------------Function Separator --------------------------------------------------------
double System::getCurFreq()
{
    return pWaveFrequencies[pCurWaveFreq];
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<Body>& System::reflistBody()
{
    return plistBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<OutputsBody>& System::reflistOutputs()
{
    return plistOutputs;
}

//------------------------------------------Function Separator --------------------------------------------------------
Body& System::refBody(int input = 0)
{
    return plistBody.at(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
OutputsBody& System::refOutput(int input = 0)
{
    return plistOutputs.at(input);
}

//==========================================Section Separator =========================================================
//Public Slots

//------------------------------------------Function Separator --------------------------------------------------------
void System::setAnalysisType(string analysisTypeIn)
{
    analysisType = analysisTypeIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addBody(Body input)
{
    plistBody.push_back(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addBody()
{
    plistBody.push_back(Body());
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addOutput(OutputsBody input)
{
    plistOutputs.push_back(input);
}

//------------------------------------------Function Separator --------------------------------------------------------
void System::addOutput()
{
    plistOutputs.push_back(OutputsBody());
}


//==========================================Section Separator =========================================================
//Protected methods

//==========================================Section Separator =========================================================
//Private methods
