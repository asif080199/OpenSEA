/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date          Author				Description
 *---------------------------------------------------------------------------------------------------------------------
 *May 15 2013   Shane Honanie       Initially created
 *Aug 01 2013   Nicholas Barczak    Added comments to header file.
 *Aug 03 2013   Nicholas Barczak    Reactored file for shorter property names and put a few public variables as
 *                                  private.
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
#ifndef BODY_H
#define BODY_H
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <QtGlobal>
#ifdef Q_OS_WIN
    #include "armadillo.h"  //References the armadillo library in lib folder.
#elif defined Q_OS_LINUX
    #include <armadillo>    //Armadillo library included with standard system libraries.
#endif
#include "forceactive.h"
#include "forcecross.h"
#include "forcereact.h"

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
/*Create forward declarations of the force objects to include them in this class definition without including the
 *files.
 */
//class ForceActive;
//class ForceReact;
//class ForceCross;
//class MotionModel;

//######################################### Class Separator ###########################################################
/**
 * This class holds all of the data for the Body Input File.
 */

class Body
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
	Body(); /**< The default constructor */

    //------------------------------------------Function Separator ----------------------------------------------------
	~Body(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Overload for operator == to compare two Body objects.  Comparison is based on body names.
     * @param bodIn The other body to compare to.
     * @return Returns true if the body names are equal.
     * Returned variable is passed by value.
     */
    bool operator ==(Body &bodIn);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the bodyName.
	 * @param newName The std::string passed in sets bodyName.
	 */
	void setBodyName(std::string);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Exposes the body name property for operation.
     * @return Pointer to the body name property.
     */
    std::string &refBodyName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Exposes the hydro body name property for operation.
     * @return Pointer to the hydro body name property.
     */
    std::string &refHydroBodName();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the hydroBody.
	 * @param newName The std::string passed in sets the hydroBody.
	 */
    void setHydroBodName(std::string);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Gets the name of the hydro body.
     * @return Returns std::string.  The name of the hydrobody object associated with the body.  Variable passed by value.
     */
    std::string getHydroBodName();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the heading.
	 * @param newHeading The double passed in sets the heading.
	 */
	void setHeading(double);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the heading for the body.  Heading is measured in radians.  Zero heading is True North, proceeding
     * counter clockwise around the compass rose.
     * @return Returns double variable.  Heading of the Body object.  Variable passed by value.
     */
    double getHeading();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Exposes the heading property for operations.
     * @return Pointer to the heading property.
     */
    double &refHeading();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the mass.
	 * @param newMass The double passed in sets the mass.
	 */
	void setMass(double);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the mass of the body.
     * @return Returns the mass of the body.  Output is in units of kilograms.
     */
    double getMass();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the Moment of Inertia XX (Ixx)
	 * @param newXX The double passed in sets momentOfInertiaXX.
	 */
    void setMomIxx(double);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the mass moment of inertia on the XX axis for the body.
     * @return Returns the mass moment of inertia on the XX axis for the body.  Output is in units of kilogram-m^2.
     */
    double getMomIxx();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the Moment of Inertia YY (Iyy)
	 * @param newYY The double passed in sets momentOfInertiaYY.
	 */
    void setMomIyy(double);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the mass moment of inertia on the YY axis for the body.
     * @return Returns the mass moment of inertia on the YY axis for the body.  Output is in units of kilogram-m^2.
     */
    double getMomIyy();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the Moment of Inertia ZZ (Izz)
	 * @param newZZ The double passed in sets momentOfInertiaZZ.
	 */
    void setMomIzz(double);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the mass moment of inertia on the ZZ axis for the body.
     * @return Returns the mass moment of inertia on the ZZ axis for the body.  Output is in units of kilogram-m^2.
     */
    double getMomIzz();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the Product of Inertia XY (Ixy)
	 * @param newXY The double passed in sets setCrossMomentXY.
	 */
    void setMomIxy(double);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the cross product of intertia on the XY axis for the body.
     * @return Returns the cross product of intertia on the XY axis for the body. OUtput is in units of kilogram-m^2.
     */
    double getMomIxy();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the Product of Inertia XZ (Ixz)
	 * @param newXZ The double passed in sets setCrossMomentXZ.
	 */
    void setMomIxz(double);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the cross product of intertia on the XZ axis for the body.
     * @return Returns the cross product of intertia on the XZ axis for the body. OUtput is in units of kilogram-m^2.
     */
    double getMomIxz();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the Product of Inertia YZ (Iyz)
	 * @param newYZ The double passed in sets setCrossMomentYZ.
	 */
    void setMomIyz(double);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the cross product of intertia on the YZ axis for the body.
     * @return Returns the cross product of intertia on the YZ axis for the body. OUtput is in units of kilogram-m^2.
     */
    double getMomIyz();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the Centroid X.
	 * @param newCenX The double passed in sets centroidX.
	 */
    void setCenX(double);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the mass matrix for the body.
     * @return Returns the mass matrix for the body, as a single matrix.  Returned by value.
     */
    arma::Mat<double> getMassMatrix();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Implements the method getMassMatrix(), just under a different name.
     * @return Returns the mass matrix for the body, as a single matrix.
     */
    arma::Mat<double> &MassMatrix();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Set the mass matrix for the body.
     * @param MassMatIn The input mass matrix for the body.  A 6x6 matrix.
     */
    void setMassMatrix(arma::Mat<double> MassMatIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the centroid of the body mass, X-axis.
     *
     * Returns the centroid of the body mass, X-axis.  Centroid is relative to body coordinates.  This includes
     * body rotation and translation.
     * @return Returns the centroid of the body mass, X-axis.  Centroid is relative to body coordinates.  This includes
     * body rotation and translation.
     */
    double getCenX();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the Centroid Y.
	 * @param newCenY The double passed in sets centroidY.
	 */
    void setCenY(double);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the centroid of the body mass, Y-axis.
     *
     * Returns the centroid of the body mass, Y-axis.  Centroid is relative to body coordinates.  This includes
     * body rotation and translation.
     * @return Returns the centroid of the body mass, Y-axis.  Centroid is relative to body coordinates.  This includes
     * body rotation and translation.
     */
    double getCenY();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the Centroid Z.
	 * @param newCenZ The double passed in sets centroidZ.
	 */
    void setCenZ(double);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the centroid of the body mass, Z-axis.
     *
     * Returns the centroid of the body mass, Z-axis.  Centroid is relative to body coordinates.  This includes
     * body rotation and translation.
     * @return Returns the centroid of the body mass, Z-axis.  Centroid is relative to body coordinates.  This includes
     * body rotation and translation.
     */
    double getCenZ();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the entire mass centroid matrix.
     * @return Returns the entire mass centroid matrix.  Output is a 3x1 matrix of the body centroid, relative to
     * body coordinate system.
     */
    arma::Mat<double> getCen();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the body position in the X-axis.
     *
     * Sets the body position in the X-axis.  Position is set relative to the world coordinate system.  Units are in
     * meters.
     * @param input Double input specifying the position on the X-axis, units of meters.
     */
    void setPosnX(double input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the body position in the X-axis.
     *
     * Gets the body position in the X-axis.  Position is set relative to the world coordinate system.  Units are in
     * meters.
     * @return Double precision floating number specifying the position on the X-axis, units of meters.
     */
    double getPosnX();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the body position in the Y-axis.
     *
     * Sets the body position in the Y-axis.  Position is set relative to the world coordinate system.  Units are in
     * meters.
     * @param input Double input specifying the position on the Y-axis, units of meters.
     */
    void setPosnY(double input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the body position in the Y-axis.
     *
     * Gets the body position in the Y-axis.  Position is set relative to the world coordinate system.  Units are in
     * meters.
     * @return Double precision floating number specifying the position on the Y-axis, units of meters.
     */
    double getPosnY();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the body position in the Z-axis.
     *
     * Sets the body position in the Z-axis.  Position is set relative to the world coordinate system.  Units are in
     * meters.
     * @param input Double input specifying the position on the Z-axis, units of meters.
     */
    void setPosnZ(double input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the body position in the Z-axis.
     *
     * Gets the body position in the Z-axis.  Position is set relative to the world coordinate system.  Units are in
     * meters.
     * @return Double precision floating number specifying the position on the Z-axis, units of meters.
     */
    double getPosnZ();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the entire matrix for position of the body.
     *
     * Returns the entire matrix for the position of the body.  Output is a 3x1 matrix with double point precision.
     * First entry (1,1) = Position in X-axis.  Second entry (2,1) = Position in Y-axis.  Third entry (3,1) = Position
     * in Z-axis.  Units are in meters.  Position is relative to the orientation of the world coordinate system.
     * @return Returns the entire matrix for the position of the body.  Output is a 3x1 matrix with double point precision.
     * First entry (1,1) = Position in X-axis.  Second entry (2,1) = Position in Y-axis.  Third entry (3,1) = Position
     * in Z-axis.  Units are in meters.  Position is relative to the orientation of the world coordinate system.
     */
    arma::Mat<double> getPosn();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Exposes the position property for operation.  The entire matrix for position of the body.
     *
     * Returns the entire matrix for the position of the body.  Output is a 3x1 matrix with double point precision.
     * First entry (1,1) = Position in X-axis.  Second entry (2,1) = Position in Y-axis.  Third entry (3,1) = Position
     * in Z-axis.  Units are in meters.  Position is relative to the orientation of the world coordinate system.
     * @return Returns a pointer to the the entire matrix for the position of the body.
     * Output is a 3x1 matrix with double point precision.
     * First entry (1,1) = Position in X-axis.  Second entry (2,1) = Position in Y-axis.  Third entry (3,1) = Position
     * in Z-axis.  Units are in meters.  Position is relative to the orientation of the world coordinate system.
     */
    arma::Mat<double> &refPosn();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Get the name of the body.
	 * @return The name of the body.
	 */    
	std::string getBodyName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Set the solution matrix for the body.
     *
     * Sets the solution matrix for the body.  Used to store the solution from the motion solver.  This variable is
     * initially empty on body creation.  It gets filled with the output from the motion solver.  Output is a column
     * matrix (n by 1) of complex numbers.  Output is in units of meters.
     * @param input Column matrix of complex numbers.  Matrix size is not hard coded.  Number of rows in matrix must match
     * number of equations for body property.
     */
    void setSolnMat(arma::cx_mat input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Get the solution matrix for the body.
     *
     * Gets the solution matrix for the body.  Used to store the solution from the motion solver.  This variable is
     * initially empty on body creation.  It gets filled with the output from the motion solver.  Output is a column
     * matrix (n by 1) of complex numbers.  Output is in units of meters.
     * @return Column matrix of complex numbers.  Matrix size is not hard coded.  Number of rows in matrix must match
     * number of equations for body property.
     */
    arma::cx_mat getSolution();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Get the solution matrix for the body.
     *
     * Gets the solution matrix for the body.  Used to store the solution from the motion solver.  This variable is
     * initially empty on body creation.  It gets filled with the output from the motion solver.  Output is a column
     * matrix (n by 1) of complex numbers.  Output is in units of meters.
     * @return Reference to column matrix of complex numbers.  Value returned by reference.  Matrix size is not
     * hard coded.  Number of rows in matrix must match number of equations for body property.
     */
    arma::cx_mat &refSolution();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Copies the body object, complete with all current data.
     * @return Returns a copy of the body object, complete with all current data.  Passed by value, not reference.
     */
    Body Copy();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of active user forces.
     *
     * The list of active user forces.  A vector of pointers directing to the active user forces.  Warning that
     * these forces may be linked to other bodies as well and should not be changed.
     * @return A vector of pointers to various user active forces.
     */
    std::vector<ForceActive* > &listForceActive_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A single active user force.
     *
     * A single active user force.  A pointer directing to the active user force.  Warning that
     * these forces may be linked to other bodies as well and should not be changed.
     * @param forceIn Integer.  Index of the ForceActive object requested.
     * @return A single pointer to the user active forces requested by parameter forceIn.  Pointer passed by value.
     */
    ForceActive* listForceActive_user(int forceIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of active hydrodynamic forces.
     *
     * The list of active hydrodynamic forces.  A vector of pointers directing to the active hydrodynamic forces.
     * Warning that these forces may be linked to other bodies as well and should not be changed.
     * @return A vector of pointes to various hydrodynamic active forces.
     */
    std::vector<ForceActive* > &listForceActive_hydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A single active hydrodynamic force.
     *
     * A single active hydrodynamic force.  A pointer directing to the active hydrodynamic force.
     * Warning that these forces may be linked to other bodies as well and should not be changed.
     * @param forceIn Integer.  Index of the ForceActive object requested.
     * @return A single pointer to the user active forces requested by parameter forceIn.  Pointer passed by value.
     */
    ForceActive* listForceActive_hydro(int forceIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of reactive user forces.
     *
     * The list of reactive user forces.  A vector of pointers directing to the reactive user forces.  Warning that
     * these forces may be linked to other bodies as well and should not be changed.
     * @return A vector of pointers to various user reactive forces.
     */
    std::vector<ForceReact* > &listForceReact_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A single reactive user force.
     *
     * A single reactive user force.  A pointer directing to the reactive user force.  Warning that
     * these forces may be linked to other bodies as well and should not be changed.
     * @param forceIn Integer.  Index of the ForceReact object requested.
     * @return A single pointer to the user reactive forces requested by parameter forceIn.  Pointer passed by value.
     */
    ForceReact *listForceReact_user(int forceIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of reactive hydrodynamic forces.
     *
     * The list of reactive hydrodynamic forces.  A vector of pointers directing to the reactive hydrodynamic
     * forces.  Warning that these forces may be linked to other bodies as well and should not be changed.
     * @return A vector of pointers to various hydrodynamic reactive forces.
     */
    std::vector<ForceReact* > &listForceReact_hydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A single reactive hydrodynamic force.
     *
     * A single reactive hydrodynamic force.  A pointer directing to the reactive hydrodynamic force.
     * Warning that these forces may be linked to other bodies as well and should not be changed.
     * @param forceIn Integer.  Index of the ForceReact object requested.
     * @return A single pointer to the user reactive forces requested by parameter forceIn.  Pointer passed by value.
     */
    ForceReact* listForceReact_hydro(int forceIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of user cross-body forces.
     *
     * The list of user cross-body forces.  A vector of pointers directing to the user cross-body forces.  Warning
     * that these forces may be linked to other bodies as well and should not be changed.  There is another vector:
     * the listLinkedBody_usr.  That determines which body each cross-body force links to.  The indices of the two
     * lists should match.  So that when a force gets added at index 5 in the listForceCross_usr, it should have a
     * matching entry at index 5 in the listLinkedBody_usr.
     * @return A list of pointers to various user cross-body forces.
     */
    std::vector<ForceCross* > &listForceCross_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A single cross-body user force.
     *
     * A single cross-body user force.  A pointer directing to the cross-body user force.  Warning that
     * these forces may be linked to other bodies as well and should not be changed.
     * @param forceIn Integer.  Index of the ForceCross object requested.
     * @return A single pointer to the user cross-body forces requested by parameter forceIn.  Pointer passed by value.
     */
    ForceCross* listForceCross_user(int forceIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of hydrodynamic cross-body forces.
     *
     * The list of hydrodynamic cross-body forces.  A vector of pointers directing to the hydrodynamic cross-body
     * forces.  Warning that these forces may be linked to other bodies as well and should not be changed.  There is
     * another vector: the listLinkedBody_usr.  That determines which body each cross-body force links to.  The indices
     * of the two lists should match.  So that when a force gets added at index 5 in the listForceCross_hydro, it should
     * have a matching entry at index 5 in the listLinkedBody_hydro.
     * @return A list of pointers to various hydrodynamic cross-body forces.
     */
    std::vector<ForceCross* > &listForceCross_hydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief A single cross-body hydrodynamic force.
     *
     * A single cross-body hydrodynamic force.  A pointer directing to the cross-body hydrodynamic force.
     * Warning that these forces may be linked to other bodies as well and should not be changed.
     * @param forceIn Integer.  Index of the ForceCross object requested.
     * @return A single pointer to the user cross-body forces requested by parameter forceIn.  Pointer passed by value.
     */
    ForceCross* listForceCross_hydro(int forceIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of linked bodies for user cross-body forces.
     *
     * The list of linked bodies for user cross-body forces.  This is a list of pointers to the other bodies. This
     * corresponds with the vector listForceCross_usr.  The indices of the two vectors should match.  The indices
     * of the two lists should match.  So that when a force gets added at index 5 in the listForceCross_usr, it should
     * have a matching entry at index 5 in the listLinkedBody_usr.
     * @return A list of pointers to various linked bodies for user cross-body forces.
     */
    std::vector<Body* > &listCrossBody_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns reference to individual linked Body for the user cross-body force.
     *
     * Returns reference for linked Body specified by the index.  The index corresponds to the index of the cross-body
     * force.  So that when a cross-body force is stored in its list at index 5, the linked Body can be retrieved
     * from this method with index 5.  Body stored internally as a pointer to the Body object.
     * @param index Integer.  The index of the linked Body to return.
     * @return Returns reference to a Body object.  The reference points to the linked Body object that corresponds to
     * the ForceCross object at the same index.
     */
    Body &listCrossBody_user(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of linked bodies for hydrodynamic cross-body forces.
     *
     * The list of linked bodies for hydrodynamic cross-body forces.  This is a list of pointers to the other bodies.
     * This corresponds with the vector listForceCross_usr.  The indices of the two vectors should match.  The indices
     * of the two lists should match.  So that when a force gets added at index 5 in the listForceCross_hydro, it should
     * have a matching entry at index 5 in the listLinkedBody_hydro.
     * @return A list of pointers to various linked bodies for hydro cross-body forces.
     */
    std::vector<Body* > &listCrossBody_hydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns reference to individual linked Body for the hydro cross-body force.
     *
     * Returns reference for linked Body specified by the index.  The index corresponds to the index of the cross-body
     * force.  So that when a cross-body force is stored in its list at index 5, the linked Body can be retrieved
     * from this method with index 5.  Body stored internally as a pointer to the Body object.
     * @param index Integer.  The index of the linked Body to return.
     * @return Returns reference to a Body object.  The reference points to the linked Body object that corresponds to
     * the ForceCross object at the same index.
     */
    Body &listCrossBody_hydro(int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of names of linked bodies for user cross-body forces.  This is a list of names of other bodies
     * that a cross-body force references.  This corresponds to the vector listForceCross_usr.  The indices of the two
     * vectors should match.  So that when a force gets added at index 5 in the listForceCross_user, it should have a
     * matching entry at index 5 in listNamedLink_usr.  The list of names only is a temporary list used during the
     * input stage of bodies.  This is required because the linked body may name a body which is not yet read from
     * the input file.  Thus, the body is not currently defined.  Once all Bodies are defined, the System object calls
     * a function to read through each name in the list and assign corresponding pointers in the listLinkedBody_usr.
     * @return Returns the list of named bodies linked to the Cross-Body forces.  Returned object is a vector of
     * std::string objects.  Returned variable passed by reference.
     * @sa listLinkedBody_user()
     * @sa System
     */
    std::vector<std::string> &listNamedLink_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of names of linked bodies for user cross-body forces.  This is a list of names of other bodies
     * that a cross-body force references.  This corresponds to the vector listForceCross_usr.  The indices of the two
     * vectors should match.  So that when a force gets added at index 5 in the listForceCross_user, it should have a
     * matching entry at index 5 in listNamedLink_usr.  The list of names only is a temporary list used during the
     * input stage of bodies.  This is required because the linked body may name a body which is not yet read from
     * the input file.  Thus, the body is not currently defined.  Once all Bodies are defined, the System object calls
     * a function to read through each name in the list and assign corresponding pointers in the listLinkedBody_usr.
     * @param varIn Integer input specifying exactly which item in the list to return.
     * @return Returns the named body linked to the Cross-Body forces.  Returned object is a std::string object.
     * Returned variable passed by reference.
     * @sa listLinkedBody_user()
     * @sa System
     */
    std::string &listNamedLink_user(unsigned int varIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of names of linked bodies for hydro cross-body forces.  This is a list of names of other bodies
     * that a cross-body force references.  This corresponds to the vector listForceCross_hydro.  The indices of the two
     * vectors should match.  So that when a force gets added at index 5 in the listForceCross_hydro, it should have a
     * matching entry at index 5 in listNamedLink_hydro.  The list of names only is a temporary list used during the
     * input stage of bodies.  This is required because the linked body may name a body which is not yet read from
     * the input file.  Thus, the body is not currently defined.  Once all Bodies are defined, the System object calls
     * a function to read through each name in the list and assign corresponding pointers in the listLinkedBody_hydro.
     * @return Returns the list of named bodies linked to the Cross-Body forces.  Returned object is a vector of
     * std::string objects.  Returned variable passed by reference.
     * @sa listLinkedBody_hydro()
     * @sa System
     */
    std::vector<std::string> &listNamedLink_hydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of names of linked bodies for hydro cross-body forces.  This is a list of names of other bodies
     * that a cross-body force references.  This corresponds to the vector listForceCross_hydro.  The indices of the two
     * vectors should match.  So that when a force gets added at index 5 in the listForceCross_hydro, it should have a
     * matching entry at index 5 in listNamedLink_hydro.  The list of names only is a temporary list used during the
     * input stage of bodies.  This is required because the linked body may name a body which is not yet read from
     * the input file.  Thus, the body is not currently defined.  Once all Bodies are defined, the System object calls
     * a function to read through each name in the list and assign corresponding pointers in the listLinkedBody_hydro.
     * @param varIn Integer input specifying exactly which item in the list to return.
     * @return Returns the named body linked to the Cross-Body forces.  Returned object is a std::string object.
     * Returned variable passed by reference.
     * @sa listLinkedBody_hydro()
     * @sa System
     */
    std::string &listNamedLink_hydro(unsigned int varIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the motion model for lookup later.
     *
     * @param modelIn Variable input that is the motion model object.  Variable passed by reference.  Stored
     * internally as a pointer.
     */
//    void setMotionModel(ofreq::MotionModel &modelIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the motion model.
     *
     * Returns the motion model object used by this body object.
     * @return Returns MotinModel object.  Variable passed by reference.
     */
//    ofreq::MotionModel &getMotionModel();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the number of equations used in the body.
     *
     * Gets the number of equations used in the body.
     * @return Integer number representing the number of equations used in the body.
     */
    int getEquationCount();

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of active user forces.
     *
     * The list of active user forces.  A vector of pointers directing to the active user forces.  Warning that
     * these forces may be linked to other bodies as well and should not be changed.
     */
    std::vector<ForceActive*> plistForceActive_usr;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of active hydrodynamic forces.
     *
     * The list of active hydrodynamic forces.  A vector of pointers directing to the active hydrodynamic forces.
     * Warning that these forces may be linked to other bodies as well and should not be changed.
     */
    std::vector<ForceActive*> plistForceActive_hydro;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of reactive user forces.
     *
     * The list of reactive user forces.  A vector of pointers directing to the reactive user forces.  Warning that
     * these forces may be linked to other bodies as well and should not be changed.
     */
    std::vector<ForceReact*> plistForceReact_usr;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of reactive hydrodynamic forces.
     *
     * The list of reactive hydrodynamic forces.  A vector of pointers directing to the reactive hydrodynamic
     * forces.  Warning that these forces may be linked to other bodies as well and should not be changed.
     */
    std::vector<ForceReact*> plistForceReact_hydro;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of user cross-body forces.
     *
     * The list of user cross-body forces.  A vector of pointers directing to the user cross-body forces.  Warning
     * that these forces may be linked to other bodies as well and should not be changed.  There is another vector:
     * the listLinkedBody_usr.  That determines which body each cross-body force links to.  The indices of the two
     * lists should match.  So that when a force gets added at index 5 in the listForceCross_usr, it should have a
     * matching entry at index 5 in the listLinkedBody_usr.
     */
    std::vector<ForceCross*> plistForceCross_usr;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of hydrodynamic cross-body forces.
     *
     * The list of hydrodynamic cross-body forces.  A vector of pointers directing to the hydrodynamic cross-body
     * forces.  Warning that these forces may be linked to other bodies as well and should not be changed.  There is
     * another vector: the listLinkedBody_usr.  That determines which body each cross-body force links to.  The indices
     * of the two lists should match.  So that when a force gets added at index 5 in the listForceCross_hydro, it should
     * have a matching entry at index 5 in the listLinkedBody_hydro.
     */
    std::vector<ForceCross*> plistForceCross_hydro;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of linked bodies for user cross-body forces.
     *
     * The list of linked bodies for user cross-body forces.  This is a list of pointers to the other bodies. This
     * corresponds with the vector listForceCross_usr.  The indices of the two vectors should match.  The indices
     * of the two lists should match.  So that when a force gets added at index 5 in the listForceCross_usr, it should
     * have a matching entry at index 5 in the listLinkedBody_usr.
     */
    std::vector<Body*> plistLinkedBody_usr;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of linked bodies for hydrodynamic cross-body forces.
     *
     * The list of linked bodies for hydrodynamic cross-body forces.  This is a list of pointers to the other bodies.
     * This corresponds with the vector listForceCross_usr.  The indices of the two vectors should match.  The indices
     * of the two lists should match.  So that when a force gets added at index 5 in the listForceCross_hydro, it should
     * have a matching entry at index 5 in the listLinkedBody_hydro.
     */
    std::vector<Body*> plistLinkedBody_hydro;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of names of linked bodies for user cross-body forces.  This is a list of names of other bodies
     * that a cross-body force references.  This corresponds to the vector listForceCross_usr.  The indices of the two
     * vectors should match.  So that when a force gets added at index 5 in the listForceCross_user, it should have a
     * matching entry at index 5 in listNamedLink_usr.  The list of names only is a temporary list used during the
     * input stage of bodies.  This is required because the linked body may name a body which is not yet read from
     * the input file.  Thus, the body is not currently defined.  Once all Bodies are defined, the System object calls
     * a function to read through each name in the list and assign corresponding pointers in the listLinkedBody_usr.
     * @sa listLinkedBody_user()
     * @sa System
     */
    std::vector<std::string> plistNamedLink_usr;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of names of linked bodies for hydro cross-body forces.  This is a list of names of other bodies
     * that a cross-body force references.  This corresponds to the vector listForceCross_hydro.  The indices of the two
     * vectors should match.  So that when a force gets added at index 5 in the listForceCross_hydro, it should have a
     * matching entry at index 5 in listNamedLink_hydro.  The list of names only is a temporary list used during the
     * input stage of bodies.  This is required because the linked body may name a body which is not yet read from
     * the input file.  Thus, the body is not currently defined.  Once all Bodies are defined, the System object calls
     * a function to read through each name in the list and assign corresponding pointers in the listLinkedBody_hydro.
     * @sa listLinkedBody_hydro()
     * @sa System
     */
    std::vector<std::string> plistNamedLink_hydro;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Private variable for the mass matrix.
     *
     * Private variable for the mass matrix.  This configuration assumes a standard 6dof motion model.
     * However, this is acceptable because the transition to the motion model object decides which elements
     * to pick out and put into equations.  Mass is units of kilograms.  Moments of inertia are in kg-m^2.
     */
    arma::Mat<double> pMassMat;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The name for this body object.
     *
     * The name for this body object.  An identifier that the user creates.  Also used to locate the body in a list.
     */
    std::string pBodyName;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Private variable for the centroid matrix.
     *
     * Private variable for the centroid matrix.  Used to store the centroid matrix.  This configuration assumes
     * a standard 6dof motion model.  However, this is acceptable because the transition to the motion model object
     * decides which elements to pick out and put into equations.  Centroid is measured in units of meters.
     */
    arma::Mat<double> pCentroid;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Private variable for the solution matrix.
     *
     * Private variable for the solution matrix.  Used to store the solution from the motion solver.  This variable is
     * initially empty on body creation.  It gets filled with the output from the motion solver.  Output is a column
     * matrix (n by 1) of complex numbers.  Output is in units of meters.
     */
    arma::cx_mat pSoln;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Private variable for the position of the body relative to the world coordinate system.
     *
     * Private variable for the position of the body relative to the world coordinate system origin.  The body may
     * be at a position relative to the world coordinate system.  Any coordintes entered for the body are relative to
     * this body position value.  Position is measured in units of meters.
     */
    arma::Mat<double> pPosn;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Private variable for the heading of the body relative to the world coordinate system.
     *
     * Private variable for the heading of the body relative to the world coordinate system.  All wave angles are
     * specified in the world coordinate system and then motions are translated relative to the body heading.  Angle
     * is measured in units of radians.
     */
    double pHead;

    //------------------------------------------Function Separator ----------------------------------------------------
    std::string hydroBody; /**< The name of this hydro body object. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The number of equations used by the body.  This sets the size of the matrices.
     *
     * The number of equations used by the body.  This sets the size of the matrices.
     */
    int eqnCount;

    //------------------------------------------Function Separator ----------------------------------------------------
//    ofreq::MotionModel* motModel; /**< Pointer to the motion model used by the body. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the coupling terms for the mass matrix.
     *
     * When a body has a center of gravity not located exactly at the origin of a coordinate system, the mathematics
     * for force balance create a coupling between momentum in linear motion and momentum in angular motion.  This
     * coupling induces additional equations in the mass matrix.  The user does not need to explicitely define these
     * coupling terms.  They can be automatically derived from just the mass and center of gravity for the Body.
     *
     * This functions calculates those coupling terms and adds them into the mass matrix.
     */
    void setMassCouple();
};

}   //Namespace ofreq
}   //Namespace osea

#endif
