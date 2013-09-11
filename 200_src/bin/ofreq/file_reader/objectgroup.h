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
 *Aug 25 2013       Nicholas Barczak    Initially created
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
//Put header files
#ifndef OBJECTGROUP_H
#define OBJECTGROUP_H
#include <vector>
#include <string>
using namespace std;

//######################################### Class Separator ###########################################################
//Prototype class declarations
class ObjectGroup;      //Forward declaration necessary to allow class recursion.

//######################################### Class Separator ###########################################################
//Constant variables
/**
 * @brief Type definition used to store key values.  Must be a vector of vectors because a value may also be a list
 * of values.
 */
typedef vector< vector<string> > vecValue;

/**
 * @brief Type defintion used to store keywords.
 */
typedef vector<string> vecKeyword;

typedef vector<ObjectGroup*> vecObject;

//######################################### Class Separator ###########################################################
/**
 * @brief The ObjectGroup class contains groupings of object definitions captured from an input file.  It is a data
 * container to hold the segmented input file for interpretation.  The container contains three things:
 * 1.)  Object class name (as specified by input file)
 * 2.)  Vector of keyword names
 * 3.)  Vector of keyword values.  Each entry in the vector of values is also a vector.  This allows the definition of
 *      lists.  A list will be as long as it needs to be for specification of all values in the list.  The index of
 *      the value is specified by its position in the vector list.  The value is the entry.
 */
class ObjectGroup
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.
     */
    ObjectGroup();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.
     */
    ~ObjectGroup();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the class name, as specified by the input file.
     * @param input String.  The name of the class, as specified by the input file.  Variable passed by value.
     */
    void setClassName(string input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the class name, as specified by the input file.
     * @return Returns string that represents the name of the class, as specified by the input file.  Variable passed
     * by value.
     */
    string getClassName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to thelist of key words.
     * @return Returns a reference to the list of key words.  Variable passed by reference.
     */
    vecKeyword& refListKey();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of key values.
     * @return Returns a reference to the list of key values.  Variable passed by reference.
     */
    vecValue& refListVal();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of sub object definitions.
     * @return Returns a reference to the list of objects.  Variable passed by reference.  Returned variable is a
     * vector of pointers to ObjectGroup objects.
     */
    vecObject& refListObject();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds an object to the list of ObjectGroup objects defined under this existing ObjectGroup.
     * @param objIn An ObjectGroup object.  Variable passed by value.
     */
    void addSubObject(ObjectGroup objIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds an object to the list of ObjectGroup objects defined under this existing ObjectGroup.
     */
    void addSubObject();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a new keyword-value set to the ObjectGroup definition.
     * @param key The key word to input.  Variable passed by value.
     * @param val The key value to input.  Can only be a single value.  Variable passed by value.
     */
    void addKeySet(string key, string val);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Overloaded function that adds a new keyword-value set to the ObjectGroup definition.  This version of the
     * method is used for passing lists in the keyword-value set definition.
     * @param key The key word to input.  Variable passed by value.
     * @param val The list of key values to input.  Vector list.  The index of each entry in the vector represents
     * its index in the input file.  The value of the entry represents the value in the input file.  Blank entries
     * are allowed.
     */
    void addKeySet(string key, vector<string> val);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a key word to the list of key words.  Only adds the word.  Not the value.
     * @param word String input.  The key word to add to the set.  Variable passed by value.
     * @param index The index of where to add the key word to the set.  If left at the default setting, the word
     * is automatically added to the end of the current list.  Variable passed by value.
     */
    void addKeyWord(string word, int index = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief addKeyVal Adds a key value to the list of key values.  Only adds the value.  Not the word.
     * @param val String input.  The key value to add to the set.  Variable passed by value.  Assumes a single key
     * value.
     * @param index The index of where to add the key value to the set.  If left at the default setting, the value
     * is automatically added to the end of the current list.  Variable passed by value.
     */
    void addKeyVal(string val, int index = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief addKeyVal Adds a key value to the list of key values.  Only adds the value.  Not the word.  This
     * is a function overload that allows for adding vector lists of key values.
     * @param val Vector of string inputs.  The vector of values to add to the key.  Variable passed by value.
     * Assumes a vector of key values.
     * @param index The index of where to add the key value to the set.  If left at the default setting, the value
     * is automatically added to the end of the current list.  Variable passed by value.
     */
    void addKeyVal(vector<string> val, int index = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets a key word as specified by the index.
     * @param index Integer.  Specifies the index of which key word to grab.
     * @return Returns a string object that represents the key word.  Variable passed by value.
     */
    string getKey(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the key value as specified by the index.
     * @param index Integer.  Specifies the index of which key word to grab.
     * @return Returns a vector of string objects that represent the key value.  For cases of a single key value,
     * the vector will only be one entry long.  For cases of lists, the vector has an unlimited length.  Variable
     * passed by value.
     */
    vector<string> getVal(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the version property of the object.
     * @param input String input.  The version property of the object.  Variable passed by value.
     */
    void setVersion(string input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the version property of the object.
     * @return Returns string output.  The version property of the object.  Variable passed by value.
     */
    string getVersion();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the format property of the object.
     * @param input String input.  The format property of the object.  Variable passed by value.
     */
    void setFormat(string input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the format property of the object.
     * @return Returns string output.  The format property of the object.  Variable passed by value.
     */
    string getFormat();

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name of the class object.
     */
    string pClassName;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Vector of keywords.
     */
    vecKeyword plistKey;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Vector of vectors.  Each entry in the outer vector is a key value and corresponds to a keyword in the
     * plistKey vector.  The inner vector represents situations of lists.  Each entry in the inner vector is an
     * entry in the list of key values.
     */
    vecValue plistVal;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Vector of objects defined within this object group.
     */
    vecObject plistObject;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Records the version of the class definition.  Currently only used to define the seafile class.  Not used
     * yet, but will be useful for future revisions to track any formatting differences between versions.
     */
    string pVersion;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Records the format of the class definition.  Right now, all formats are ascii.  That may change in the
     * future.
     * ascii:  Normal text format.  Parse as normal.
     */
    string pFormat;
};

#endif // OBJECTGROUP_H
