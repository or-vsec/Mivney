#include "library2.h" 
#include "School.h"

void * Init(int n)
{
	School * DS = new School(n);
	return (void*)DS;
}

StatusType AddStudent(void *DS, int StudentID, int Team, int Power) {
	if (DS == NULL) return INVALID_INPUT;
	return ((School*)DS)->add_student(StudentID, Team, Power);
}

StatusType RemoveStudent(void *DS, int StudentID) {
	if (DS == NULL) return INVALID_INPUT;
	return ((School*)DS)->remove_student(StudentID);
}

StatusType JoinTeams(void * DS, int Team1, int Team2)
{
	if (DS == NULL) return INVALID_INPUT;
	return ((School*)DS)->join_teams(Team1, Team2);
}

StatusType TeamFight(void * DS, int Team1, int Team2, int NumOfFighters)
{
	if (DS == NULL) return INVALID_INPUT;
	return ((School*)DS)->team_fight(Team1, Team2, NumOfFighters);
}

StatusType GetNumOfWins(void * DS, int Team, int * Wins)
{
	if (DS == NULL) return INVALID_INPUT;
	return ((School*)DS)->get_num_of_wins(Team, Wins);
}


StatusType GetStudentTeamLeader(void * DS, int StudentID, int * Leader)
{
	if (DS == NULL) return INVALID_INPUT;
	return ((School*)DS)->get_student_team_leader(StudentID, Leader);
}

void Quit(void** DS) {
	if (DS == NULL) return;
	delete *((School**)DS);
	*DS = NULL;
}
