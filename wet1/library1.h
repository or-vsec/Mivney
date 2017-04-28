/****************************************************************************/
/*                                                                          */
/* This file contains the interface functions                               */
/* you should use for the wet ex 1                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library1.h                                                   */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_
#define _234218_WET1_

#ifdef __cplusplus
extern "C" {
#endif

/* Return Values
 * ----------------------------------- */
typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    ALLOCATION_ERROR = -2,
    INVALID_INPUT = -3
} StatusType;

/* Required Interface for the Data Structure
 * -----------------------------------------*/

/* Description:   Initiates the data structure.
 * Input:         DS - A pointer to the data structure.
 * Output:        None.
 * Return Values: A pointer to a new instance of the data structure - as a void* pointer.
 */
void* Init();

/* Description:   Adds a new Mutant.
 * Input:         DS - A pointer to the data structure.
 *                StudentID - The ID of the Mutant to add.
 *				  Grade - The grade of the mutant
 *				  Power - The mutant's power
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, StudentID <= 0, Power <= 0 or Grade < 0 .
 *                FAILURE - If StudentID is already in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType AddStudent(void *DS, int StudentID, int Grade, int Power);

/* Description:   Adds a new empty team to the system.
 * Input:         DS - A pointer to the data structure.
 *                TeamID - The new team's ID
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, or TeamID <=0
 *                FAILURE - If TeamID is already in the DS
 *                SUCCESS - Otherwise.
 */
StatusType AddTeam(void *DS, int TeamID);

/* Description:   Removes an existing creature.
 * Input:         DS - A pointer to the data structure.
 *                StudentID - The ID of the mutant.
 *				  TeamID - The ID of the team to assign the mutant to
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, StudentID <= 0 or TeamID <= 0.
 *                FAILURE - If StudentID or TeamID aren't in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType MoveStudentToTeam(void *DS, int StudentID, int TeamID);

/* Description:   Returns the most powerful mutant assigned to teamID
 * 			      If teamID < 0, returns the most powerful mutant in the entire DS.
 * Input:         DS - A pointer to the data structure.
 *                teamID - The team that we would like to get the data for.
 * Output:        StudentID - A pointer to a variable that should be updated to the ID of the most powerful mutant.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, or if creatureID == NULL, or if TeamID == 0.
 *                FAILURE - If teamID > 0 and teamID doesn't exist in the DS
 *                SUCCESS - Otherwise.
 */
StatusType GetMostPowerful(void *DS, int TeamID, int *StudentID);

/* Description:   Removes an existing mutant from the DS
 * Input:         DS - A pointer to the data structure.
 *                StudentID - The ID of the mutant to remove.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL or if StudentID <= 0
 *                FAILURE - If StudentID isn't in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType RemoveStudent(void *DS, int StudentID);

/* Description:   Returns all the creatures assigned to magiID sorted by their danger level.
 *           			If magiID < 0, returns all the creatures in the entire DS sorted by their danger level.
 * Input:         DS - A pointer to the data structure.
 *                TeamID - The team that we'd like to get the data for.
 * Output:        Students - A pointer to a to an array that you should update with the mutants' IDs sorted by their power,
 *			                  in case two mutants have same power, they should be sorted by their ID.
 *                numOfCreatures - A pointer to a variable that should be updated to the number of returned mutants.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If any of the arguments is NULL or if TeamID == 0.
 *                FAILURE - if teamID > 0 and teamID doesn't exist in the DS
 *                SUCCESS - Otherwise.
 */
StatusType GetAllStudentsByPower(void *DS, int TeamID, int **Students, int *numOfStudents);

/* Description:   Increases the power of all the mutants in a certain grade.
 * Input:         DS - A pointer to the data structure.
 *                Grade - The grade of the mutants.
 *		          PowerIncrease - The increase in the mutants' power.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, or if Grade<0, or if PowerIncrease<=0
 *                FAILURE - If creatureID isn't in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType IncreaseLevel(void *DS, int Grade, int PowerIncrease);

/* Description:   Quits and deletes the database.
 *                DS should be set to NULL.
 * Input:         DS - A pointer to the data structure.
 * Output:        None.
 * Return Values: None.
 */
void Quit(void** DS);

#ifdef __cplusplus
}
#endif

#endif    /*_234218_WET1_ */
