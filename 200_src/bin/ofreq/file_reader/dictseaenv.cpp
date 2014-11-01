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

//==========================================Section Separator =========================================================
//Inclusions and namespaces
#include "dictseaenv.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Class Name constants
string dictSeaEnv::OBJECT_WAVE_SPEC = "wave_spec"; /**< Class designation for a wave spectra. */
string dictSeaEnv::OBJECT_SEA_MODEL = "sea_model"; /**< Class designation for a sea model. */
string dictSeaEnv::OBJECT_WAVE_DEF = "wave_def"; /**< Class designation for a wave direction definition. */

//------------------------------------------Function Separator ----------------------------------------------------
//Keyword name static constants
string dictSeaEnv::KEY_CUSTOM = "custom";   /**< Class designation for either custom wave spectra, or custom sea model.*/
string dictSeaEnv::KEY_BRETSCHNEIDER = "bretschneider"; /**< Class designation for Bretschneider type of wave spectrum. */
string dictSeaEnv::KEY_ITTC = "ittc"; /**< Class designation for Bretschneider type of wave spectrum, just different name.*/
string dictSeaEnv::KEY_JONSWAP = "jonswap"; /**< Class designation for JONSWAP type of wave spectrum.*/
string dictSeaEnv::KEY_PM = "pm"; /**< Class designation for Pierson Moskowitz type of wave spectrum. */
string dictSeaEnv::KEY_1D = "single_direction"; /**< Class designation for a single direction sea model. */
string dictSeaEnv::KEY_2D = "dual_direction"; /**< Class designation for a dual direction sea model. */
string dictSeaEnv::KEY_LONG = "long_crested"; /**< Class designation for a long crested wave sea model. */
string dictSeaEnv::KEY_UNIFORM = "uniform"; /**< Class designatino for a uniform wave sea model. */

string dictSeaEnv::KEY_CLASS = "class";   /**< Keyword to specify class type for sea model or wave spectrum. */
string dictSeaEnv::KEY_NAME = "name";    /**< Keyword to specify the user name designation for sea model or wave spectrum. */
string dictSeaEnv::KEY_FREQUENCY = "frequency"; /**< Keyword to specify the frequencies of a custom wave spectrum. */
string dictSeaEnv::KEY_WAVEENERGY = "wave_energy"; /**< Keyword to specity wave energy of a custom wave spectrum. */
string dictSeaEnv::KEY_HSIG = "h_sig";    /**< Keyword to specify the significant wave height for definition of a wave spectrum. */
string dictSeaEnv::KEY_TMEAN = "t_mean";   /**< Keyword to specify the mean period for definition of a wave spectrum. */
string dictSeaEnv::KEY_TPEAK = "t_peak";   /**< Keyword to specify the peak period for definition of a wave spectrum. */
string dictSeaEnv::KEY_TZERO = "t_zero";   /**< Keyword to specify the zero crossing period for definition of a wave spectrum. */
string dictSeaEnv::KEY_BANDPARAM = "bandwidth_param";   /**< Keyword to specify gamma value of JONSWAP spectrum. */
string dictSeaEnv::KEY_GAMMA = "gamma";   /**< Keyword to specify gamma value of JONSWAP spectrum, under a different name. */
string dictSeaEnv::KEY_WIND = "wind";    /**< Keyword to specify the wind speed of the Pierson Moskowitz spectrum. */
string dictSeaEnv::KEY_DIRECTION = "direction"; /**< Keyword to specify the wave direction for a sea model component. */
string dictSeaEnv::KEY_SPECTRUM = "spectrum";  /**< Keyword to specify the wave spectrum for a sea model component. */
string dictSeaEnv::KEY_SCALE = "scale";   /**< Keyword to specify the scale factor for a sea model component. */


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictSeaEnv::dictSeaEnv() : Dictionary()
{
    pClassSet = false;
    pMakeNewWaveDef = false;
    pListIndex = 0;
}

//------------------------------------------Function Separator --------------------------------------------------------
dictSeaEnv::~dictSeaEnv()
{

}

//==========================================Section Separator =========================================================
//Signal Functions


//==========================================Section Separator =========================================================
//Public Slots Functions


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
int dictSeaEnv::defineKey(string keyIn, vector<string> valIn)
{
    //Start with keywords that define a class type
    if (keyIn == KEY_CLASS)
    {
        //Define a class type.
        return buildClass(valIn.at(0));
    }
    else
    {
        //Case for any actual key words.  Must first check if the class type was properly set.
        if (pClassSet)
        {
            //Class set.  Proceed with definitions.

            //------------------------------------------
            if (keyIn == KEY_NAME)
            {
                //Set the name for the object.
                if (pClassType == OBJECT_SEA_MODEL)
                {
                    //Sea model name
                    ptSystem->listSeaModel(pObjIndex).setName(valIn.at(0));
                    return 0;
                }
                else if (pClassType == OBJECT_WAVE_SPEC)
                {
                    //Wave spectra name
                    ptSystem->listWaveSpec(pObjIndex).setName(valIn.at(0));
                    return 0;
                }
            }

            //------------------------------------------
            else if (keyIn == KEY_FREQUENCY)
            {
                //Set the list of frequencies for the wave spectrum.
                if (pClassType == OBJECT_WAVE_SPEC)
                {
                    std::vector<double> waveFreq;       //List of wave frequencies.
                    for (int i = 0; i < valIn.size(); i++)
                    {
                        waveFreq.push_back(atof(valIn.at(i).c_str()));
                    }

                    //Get the object.
                    osea::WaveSpec* Spec = dynamic_cast<osea::WaveSpec*>(ptSystem->listWaveSpecPt(pObjIndex));

                    //Add the list of wave frequencies to the object.
                    Spec->setWaveFreq(waveFreq);
                    return 0;
                }
                else
                {
                    logStd.Notify();
                    logErr.Write("Attempted to specify frequency list on an invalid class type.");
                    return 2;
                }
            }

            //------------------------------------------
            else if (keyIn == KEY_WAVEENERGY)
            {
                //Set the list of frequencies for the wave spectrum.
                if (pClassType == OBJECT_WAVE_SPEC)
                {
                    std::vector<double> waveEnerg;       //List of wave frequencies.
                    for (int i = 0; i < valIn.size(); i++)
                    {
                        waveEnerg.push_back(atof(valIn.at(i).c_str()));
                    }

                    //Get the object.
                    osea::WaveSpec* Spec = dynamic_cast<osea::WaveSpec*>(ptSystem->listWaveSpecPt(pObjIndex));

                    //Add the list of wave frequencies to the object.
                    Spec->setWaveEnergy(waveEnerg);
                    return 0;
                }
                else
                {
                    logStd.Notify();
                    logErr.Write("Attempted to specify wave energy list on an invalid class type.");
                    return 2;
                }
            }

            //------------------------------------------
            else if (keyIn == KEY_HSIG)
            {
                //Set the significant wave height.
                if ((pSubClassType == KEY_BRETSCHNEIDER) ||
                        (pSubClassType == KEY_ITTC) ||
                        (pSubClassType == KEY_JONSWAP) ||
                        (pSubClassType == KEY_PM))
                {
                    //Get the object, depending on object type needed.
                    if (pSubClassType == KEY_BRETSCHNEIDER)
                    {
                        //Bretschneider class
                        osea::SpecBretschneider* Spec = dynamic_cast<osea::SpecBretschneider*>
                                                        (ptSystem->listWaveSpecPt(pObjIndex));
                        //Set the wave height
                        Spec->setSigWaveHeight(
                                    atof(valIn.at(0).c_str()));
                        return 0;
                    }
                    else if (pSubClassType == KEY_ITTC)
                    {
                        //Bretschneider class
                        osea::SpecBretschneider* Spec = dynamic_cast<osea::SpecBretschneider*>
                                                        (ptSystem->listWaveSpecPt(pObjIndex));
                        //Set the wave height
                        Spec->setSigWaveHeight(
                                    atof(valIn.at(0).c_str()));
                        return 0;
                    }
                    else if (pSubClassType == KEY_JONSWAP)
                    {
                        //JONSWAP class
                        osea::SpecJONSWAP* Spec = dynamic_cast<osea::SpecJONSWAP*>
                                                  (ptSystem->listWaveSpecPt(pObjIndex));
                        //Set the wave height
                        Spec->setSigWaveHeight(
                                    atof(valIn.at(0).c_str()));
                        return 0;
                    }
                    else if (pSubClassType == KEY_PM)
                    {
                        //PM Class
                        osea::SpecPM* Spec = dynamic_cast<osea::SpecPM*>
                                             (ptSystem->listWaveSpecPt(pObjIndex));
                        //Set the wave height
                        Spec->setSigWaveHeight(
                                    atof(valIn.at(0).c_str()));
                        return 0;
                    }
                    else
                    {
                        //Throw an error.
                        logStd.Notify();
                        logErr.Write(keyIn + string(" parameter does not apply to the specified wave spectrum:  ")
                                     + pSubClassType);
                        return 2;
                    }
                }
                else
                {
                    //Throw an error.
                    logStd.Notify();
                    logErr.Write(keyIn + string(" parameter does not apply to the specified wave spectrum:  ")
                                 + pSubClassType);
                    return 2;
                }
            }

            //------------------------------------------
            else if (keyIn == KEY_TMEAN)
            {
                //Set the Mean period for the spectrum.
                if ((pSubClassType == KEY_BRETSCHNEIDER) ||
                        (pSubClassType == KEY_ITTC) ||
                        (pSubClassType == KEY_JONSWAP))
                {
                    //Get the object, depending on object type needed.
                    if (pSubClassType == KEY_BRETSCHNEIDER)
                    {
                        //Bretschneider class
                        osea::SpecBretschneider* Spec = dynamic_cast<osea::SpecBretschneider*>
                                                        (ptSystem->listWaveSpecPt(pObjIndex));
                        //Set the wave height
                        Spec->setTMean(
                                    atof(valIn.at(0).c_str()));
                        return 0;
                    }
                    else if (pSubClassType == KEY_ITTC)
                    {
                        //Bretschneider class
                        osea::SpecBretschneider* Spec = dynamic_cast<osea::SpecBretschneider*>
                                                        (ptSystem->listWaveSpecPt(pObjIndex));
                        //Set the wave height
                        Spec->setTMean(
                                    atof(valIn.at(0).c_str()));
                        return 0;
                    }
                    else if (pSubClassType == KEY_JONSWAP)
                    {
                        //JONSWAP class
                        osea::SpecJONSWAP* Spec = dynamic_cast<osea::SpecJONSWAP*>
                                                  (ptSystem->listWaveSpecPt(pObjIndex));
                        //Set the wave height
                        Spec->setTMean(
                                    atof(valIn.at(0).c_str()));
                        return 0;
                    }
                    else
                    {
                        //Throw an error.
                        logStd.Notify();
                        logErr.Write(keyIn + string(" parameter does not apply to the specified wave spectrum:  ")
                                     + pSubClassType);
                        return 2;
                    }
                }
                else
                {
                    //Throw an error.
                    logStd.Notify();
                    logErr.Write(keyIn + string(" parameter does not apply to the specified wave spectrum:  ")
                                 + pSubClassType);
                    return 2;
                }
            }

            //------------------------------------------
            else if (keyIn == KEY_TPEAK)
            {
                //Set the peak period for the spectrum.
                if ((pSubClassType == KEY_BRETSCHNEIDER) ||
                        (pSubClassType == KEY_ITTC) ||
                        (pSubClassType == KEY_JONSWAP))
                {
                    //Get the object, depending on object type needed.
                    if (pSubClassType == KEY_BRETSCHNEIDER)
                    {
                        //Bretschneider class
                        osea::SpecBretschneider* Spec = dynamic_cast<osea::SpecBretschneider*>
                                                        (ptSystem->listWaveSpecPt(pObjIndex));
                        //Set the wave height
                        Spec->setTPeak(
                                    atof(valIn.at(0).c_str()));
                        return 0;
                    }
                    else if (pSubClassType == KEY_ITTC)
                    {
                        //Bretschneider class
                        osea::SpecBretschneider* Spec = dynamic_cast<osea::SpecBretschneider*>
                                                        (ptSystem->listWaveSpecPt(pObjIndex));
                        //Set the wave height
                        Spec->setTPeak(
                                    atof(valIn.at(0).c_str()));
                        return 0;
                    }
                    else if (pSubClassType == KEY_JONSWAP)
                    {
                        //JONSWAP class
                        osea::SpecJONSWAP* Spec = dynamic_cast<osea::SpecJONSWAP*>
                                                  (ptSystem->listWaveSpecPt(pObjIndex));
                        //Set the wave height
                        Spec->setTPeak(
                                    atof(valIn.at(0).c_str()));
                        return 0;
                    }
                    else
                    {
                        //Throw an error.
                        logStd.Notify();
                        logErr.Write(keyIn + string(" parameter does not apply to the specified wave spectrum:  ")
                                     + pSubClassType);
                        return 2;
                    }
                }
                else
                {
                    //Throw an error.
                    logStd.Notify();
                    logErr.Write(keyIn + string(" parameter does not apply to the specified wave spectrum:  ")
                                 + pSubClassType);
                    return 2;
                }
            }

            //------------------------------------------
            else if (keyIn == KEY_TZERO)
            {
                //Set the zero period for the spectrum.
                if ((pSubClassType == KEY_BRETSCHNEIDER) ||
                        (pSubClassType == KEY_ITTC) ||
                        (pSubClassType == KEY_JONSWAP))
                {
                    //Get the object, depending on object type needed.
                    if (pSubClassType == KEY_BRETSCHNEIDER)
                    {
                        //Bretschneider class
                        osea::SpecBretschneider* Spec = dynamic_cast<osea::SpecBretschneider*>
                                                        (ptSystem->listWaveSpecPt(pObjIndex));
                        //Set the wave height
                        Spec->setTZero(
                                    atof(valIn.at(0).c_str()));
                        return 0;
                    }
                    else if (pSubClassType == KEY_ITTC)
                    {
                        //Bretschneider class
                        osea::SpecBretschneider* Spec = dynamic_cast<osea::SpecBretschneider*>
                                                        (ptSystem->listWaveSpecPt(pObjIndex));
                        //Set the wave height
                        Spec->setTZero(
                                    atof(valIn.at(0).c_str()));
                        return 0;
                    }
                    else if (pSubClassType == KEY_JONSWAP)
                    {
                        //JONSWAP class
                        osea::SpecJONSWAP* Spec = dynamic_cast<osea::SpecJONSWAP*>
                                                  (ptSystem->listWaveSpecPt(pObjIndex));
                        //Set the wave height
                        Spec->setTZero(
                                    atof(valIn.at(0).c_str()));
                        return 0;
                    }
                    else
                    {
                        //Throw an error.
                        logStd.Notify();
                        logErr.Write(keyIn + string(" parameter does not apply to the specified wave spectrum:  ")
                                     + pSubClassType);
                        return 2;
                    }
                }
                else
                {
                    //Throw an error.
                    logStd.Notify();
                    logErr.Write(keyIn + string(" parameter does not apply to the specified wave spectrum:  ")
                                 + pSubClassType);
                    return 2;
                }
            }

            //------------------------------------------
            else if (keyIn == KEY_BANDPARAM)
            {
                //Set the bandwidth parameter for the JONSWAP spectrum.
                if (pSubClassType == KEY_JONSWAP)
                {
                    //Get the object
                    //JONSWAP class
                    osea::SpecJONSWAP* Spec = dynamic_cast<osea::SpecJONSWAP*>
                                              (ptSystem->listWaveSpecPt(pObjIndex));

                    //Set the band parameter.
                    Spec->setSpecBandParam(
                                atof(valIn.at(0).c_str()));
                    return 0;
                }
                else
                {
                    //Throw an error.
                    logStd.Notify();
                    logErr.Write(keyIn + string(" parameter does not apply to the specified wave spectrum:  ")
                                 + pSubClassType);
                    return 2;
                }
            }

            //------------------------------------------
            else if (keyIn == KEY_GAMMA)
            {
                //Set the bandwidth parameter for the JONSWAP spectrum.
                if (pSubClassType == KEY_JONSWAP)
                {
                    //Get the object
                    //JONSWAP class
                    osea::SpecJONSWAP* Spec = dynamic_cast<osea::SpecJONSWAP*>
                                              (ptSystem->listWaveSpecPt(pObjIndex));

                    //Set the band parameter.
                    Spec->setSpecBandParam(
                                    atof(valIn.at(0).c_str()));
                    return 0;
                }
                else
                {
                    //Throw an error.
                    logStd.Notify();
                    logErr.Write(keyIn + string(" parameter does not apply to the specified wave spectrum:  ")
                                 + pSubClassType);
                    return 2;
                }
            }

            //------------------------------------------
            else if (keyIn == KEY_WIND)
            {
                //Set the wind speed for the Pierson Moskowitz spectrum.
                if (pSubClassType == KEY_PM)
                {
                    //Get the object.
                    //PM Class
                    osea::SpecPM* Spec = dynamic_cast<osea::SpecPM*>
                                         (ptSystem->listWaveSpecPt(pObjIndex));

                    //Set the wind speed.
                    Spec->setWindSpeed(
                                atof(valIn.at(0).c_str()));
                    return 0;
                }
                else
                {
                    //Throw an error.
                    logStd.Notify();
                    logErr.Write(keyIn + string(" parameter does not apply to the specified wave spectrum:  ")
                                 + pSubClassType);
                    return 2;
                }
            }

            //------------------------------------------
            else if (keyIn == KEY_DIRECTION)
            {
                //Set the wave direction for a sea model component.
                if (pClassType == OBJECT_SEA_MODEL)
                {
                    ptSystem->listSeaModel(pObjIndex).addWaveDir(
                                atof(valIn.at(0).c_str()));
                    return 0;
                }
                else
                {
                    //Throw an error.
                    logStd.Notify();
                    logErr.Write(keyIn + string(" parameter does not apply to the specified object type:  ")
                                 + pClassType + string(":") + pSubClassType);
                    return 2;
                }
            }

            //------------------------------------------
            else if (keyIn == KEY_SPECTRUM)
            {
                //Add a wave spectrum for the sea model component
                if (pClassType == OBJECT_SEA_MODEL)
                {
                    WaveSpecBase *ptSpec;       //Pointer to the wave spectrum.

                    //First get the pointer to the wave spectrum from the system.
                    ptSpec = &(ptSystem->listWaveSpec(valIn.at(0)));

                    //First check if list sizes match for the sea model.
                    if ((ptSystem->listSeaModel(pObjIndex).listWaveSpec().size() - 1 == pListIndex) &&
                        (pListIndex != 0))
                    {
                        //Sizes match.  Just change the last entry.
                        ptSystem->listSeaModel(pObjIndex).listWaveSpec(pListIndex) = ptSpec;
                        //Reset the index
                        pListIndex = 0;
                    }
                    else
                    {
                        //Add the wave spectrum to the list of wave spectra in the object.
                        ptSystem->listSeaModel(pObjIndex).addWaveSpec(ptSpec);
                        //Get the index of the last entry in the wave spectrum.
                        pListIndex = ptSystem->listSeaModel(pObjIndex).listWaveSpec().size() - 1;
                    }
                    return 0;
                }
                else
                {
                    //Throw an error.
                    logStd.Notify();
                    logErr.Write(keyIn + string(" parameter does not apply to the specified object type:  ")
                                 + pClassType + string(":") + pSubClassType);
                    return 2;
                }
            }

            //------------------------------------------
            else if (keyIn == KEY_SCALE)
            {
                //Add a wave scaling parameter to the sea model component.
                if (pClassType == OBJECT_SEA_MODEL)
                {
                    //First check if list sizes match for the sea model.
                    if ((ptSystem->listSeaModel(pObjIndex).listWaveScale().size() - 1 == pListIndex) &&
                        (pListIndex != 0))
                    {
                        //Sizes match.  Just change the last entry.
                        ptSystem->listSeaModel(pObjIndex).listWaveScale(pListIndex) =
                                atof(valIn.at(0).c_str());
                        //Reset the index
                        pListIndex = 0;
                    }
                    else
                    {
                        //Add the wave scaling parameter to the list of wave scaling.
                        ptSystem->listSeaModel(pObjIndex).addWaveScale(
                                    atof(valIn.at(0).c_str()));
                        //Get the index of the last entry in the wave spectrum.
                        pListIndex = ptSystem->listSeaModel(pObjIndex).listWaveScale().size() - 1;
                    }
                    return 0;
                }
                else
                {
                    //Throw an error.
                    logStd.Notify();
                    logErr.Write(keyIn + string(" parameter does not apply to the specified object type:  ")
                                 + pClassType + string(":") + pSubClassType);
                    return 2;
                }
            }

            //------------------------------------------
            else
            {
                //Error handler if no match found.
                logStd.Notify();
                logErr.Write(string("No keyword found to match the input.  Keyword specified:  ") + keyIn);
                return 1;
            }
        }
        else
        {
            //Class not set.  Send an error messssage.
            logStd.Notify();
            logErr.Write("Object properties specified before ""class"" keyword set.  Set the ""class"" keyword value first.");
            return 2;
        }

    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int dictSeaEnv::defineClass(string nameIn)
{
    if (nameIn == OBJECT_WAVE_SPEC)
    {
        //Start creation of a wave spectrum object
        pClassType = nameIn;
        pClassSet = false;
        return 0;
    }

    else if (nameIn == OBJECT_SEA_MODEL)
    {
        //Start creation of a sea model object
        pClassType = nameIn;
        pClassSet = false;
        return 0;
    }

    else if (nameIn == OBJECT_WAVE_DEF)
    {
        //Start definition of a wave definition within a sea model.
        //Notify that a new object needs to be built.
        pMakeNewWaveDef = true;
        return 0;
    }

    else
    {
        //Case if no match found.
        logStd.Notify();
        logErr.Write(string("No class name found to match the input.  Class specified:  ") + nameIn);
        return 1;
    }
}

//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
int dictSeaEnv::buildClass(std::string nameIn)
{
    if (nameIn == KEY_CUSTOM)
    {
        //Custom specification of either wave spectra or sea model.
        if (pClassType == OBJECT_WAVE_SPEC)
        {
            //Class definition of a custom wave spectra
            ptSystem->addWaveSpec(osea::WaveSpec());

            //Set the specific class type
            pSubClassType = nameIn;

            //Get the index
            pObjIndex = ptSystem->listWaveSpec().size() - 1;

            //Notify of properly set class.
            pClassSet = true;
            return 0;
        }

        else if (pClassType == OBJECT_SEA_MODEL)
        {
            //Class definition of a custom sea model
            ptSystem->addSeaModel(osea::SeaModel());

            //Set the specific class type
            pSubClassType = nameIn;

            //Get the index
            pObjIndex = ptSystem->listSeaModel().size() - 1;

            //Notify the properly set class.
            pClassSet = true;
            return 0;
        }
    }

    else if (nameIn == KEY_BRETSCHNEIDER)
    {
        //Specification of a bretschneider wave spectrum.
        ptSystem->addWaveSpec(SpecBretschneider());

        //Set the specific class type
        pSubClassType = nameIn;

        //Get the index
        pObjIndex = ptSystem->listWaveSpec().size() - 1;

        //Notify the properly set class.
        pClassSet = true;
        return 0;
    }

    else if (nameIn == KEY_ITTC)
    {
        //Specification of a bretschneider wave spectrum.
        ptSystem->addWaveSpec(osea::SpecBretschneider());

        //Set the specific class type
        pSubClassType = nameIn;

        //Get the index
        pObjIndex = ptSystem->listWaveSpec().size() - 1;

        //Notify the properly set class.
        pClassSet = true;
        return 0;
    }

    else if (nameIn == KEY_JONSWAP)
    {
        //Specification of the JONSWAP wave spectrum.
        ptSystem->addWaveSpec(osea::SpecJONSWAP());

        //Set the specific class type
        pSubClassType = nameIn;

        //Get the index
        pObjIndex = ptSystem->listWaveSpec().size() - 1;

        //Notify the properly set class.
        pClassSet = true;
        return 0;
    }

    else if (nameIn == KEY_PM)
    {
        //Specification of a Pierson Moskowitz wave spectrum.
        ptSystem->addWaveSpec(osea::SpecPM());

        //Set the specific class type
        pSubClassType = nameIn;

        //Get the index
        pObjIndex = ptSystem->listWaveSpec().size() - 1;

        //Notify the properly set class.
        pClassSet = true;
        return 0;
    }

    else if (nameIn == KEY_1D)
    {
        //Specification of a 1 direction sea model.
        ptSystem->addSeaModel(osea::SeaModel_SingleDirection());

        //Set the specific class type
        pSubClassType = nameIn;

        //Get the index
        pObjIndex = ptSystem->listSeaModel().size() - 1;

        //Notify the properly set class.
        pClassSet = true;
        return 0;
    }

    else if (nameIn == KEY_2D)
    {
        //Specification of a 2 direction sea model.
        ptSystem->addSeaModel(osea::SeaModel_DualDirection());

        //Set the specific class type
        pSubClassType = nameIn;

        //Get the index
        pObjIndex = ptSystem->listSeaModel().size() - 1;

        //Notify the properly set class.
        pClassSet = true;
        return 0;
    }

    else if (nameIn == KEY_LONG)
    {
        //Specification of a long crested wave sea model.
        ptSystem->addSeaModel(osea::SeaModel_LongCrest());

        //Set the specific class type
        pSubClassType = nameIn;

        //Get the index
        pObjIndex = ptSystem->listSeaModel().size() - 1;

        //Notify the properly set class.
        pClassSet = true;
        return 0;
    }

    else if (nameIn == KEY_UNIFORM)
    {
        //Specification of a uniform sea model.
        ptSystem->addSeaModel(osea::SeaModel_Uniform());

        //Set the specific class type
        pSubClassType = nameIn;

        //Get the index
        pObjIndex = ptSystem->listSeaModel().size() - 1;

        //Notify the properly set class.
        pClassSet = true;
        return 0;
    }
}
