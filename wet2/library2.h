/****************************************************************************/
/*                                                                          */
/* This file contains the interface functions                               */
/* you should use for the wet ex 2                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library2.h                                                   */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET2_
#define _234218_WET2_

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
 *                n - The number of teams in the DS.
 * Output:        None.
 * Return Values: A pointer to a new instance of the data structure - as a void* pointer.
 */
void* Init(int n);

/* Description:   Adds a new Mutant.
 * Input:         DS - A pointer to the data structure.
 *                StudentID - The ID of the Mutant to add.
 *				  Team - The team of the mutant
 *				  Power - The mutant's power
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, StudentID <= 0, Power <= 0 or Team <= 0 or Team > n.
 *                FAILURE - If StudentID is already in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType AddStudent(void *DS, int StudentID, int Team ,int Power);

/* Description:   Removes an existing student.
 * Input:         DS - A pointer to the data structure.
 *                StudentID - The ID of the mutant.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, StudentID <= 0.
 *                FAILURE - If StudentID isn't in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType RemoveStudent(void *DS, int StudentID);

/* Description:   Join two teams into one.
 * Input:         DS - A pointer to the data structure.
 *                Team1 - The first team to unite.
 * 				  Team2 - The second team to unite.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, or if Team1/2 <= 0, or if Team1/2 > n.
 *                SUCCESS - Otherwise.
 */
StatusType JoinTeams(void *DS, int Team1, int Team2);

/* Description:   Conduct a (practice) fight between two teams.
 * Input:         DS - A pointer to the data structure.
 *                Team1 - The first team in the fight.
 * 				  Team2 - The second team in the fight.
 * 				  NumOfFighters - The numbers of mutatnts each team is allowed to use.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, or if Team1/2 <= 0, or if Team1/2 > n or NumOfFighters <= 0.
 *                SUCCESS - Otherwise.
 */
StatusType TeamFight(void *DS, int Team1, int Team2, int NumOfFighters);

/* Description:   Returns the number of fight a certain team won.
 * Input:         DS - A pointer to the data structure.
 *                Team - The team that we'd like to get the data for.
 * Output:        Wins - A pointer to a to a variable that you should contain the number of wins.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If any of the pointers is NULL or if Team <= 0 or if Team > n.
 *                SUCCESS - Otherwise.
 */
StatusType GetNumOfWins(void *DS, int Team, int* Wins);

/* Description:   Returns the team leader of a certain student.
 * Input:         DS - A pointer to the data structure.
 *                StudentID - The student that we'd like to get the data for.
 * Output:        Leader - A pointer to a to a variable that you should contain the team leader.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If any of the pointers is NULL or if StudentID <= 0.
 *                FAILURE - If StudentID doesn't exist in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType GetStudentTeamLeader(void *DS, int StudentID, int* Leader);

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

#endif    /*_234218_WET2_ */