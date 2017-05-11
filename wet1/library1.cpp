#include "library1.h" 
#include "School.h"

void* Init() {
	School * DS = new School();
	DS->add_team(0);
	return (void*)DS;
}

StatusType AddStudent(void *DS, int StudentID, int Grade, int Power) {
	if (DS == NULL) return INVALID_INPUT;
	return ((School*)DS)->add_student(StudentID, Grade, Power);
}

StatusType AddTeam(void *DS, int TeamID) {
	if (DS == NULL) return INVALID_INPUT;
	return ((School*)DS)->add_team(TeamID);
}

StatusType MoveStudentToTeam(void *DS, int StudentID, int TeamID) {
	if (DS == NULL) return INVALID_INPUT;
	return ((School*)DS)->move_student_to_team(StudentID, TeamID);
}

StatusType GetMostPowerful(void *DS, int TeamID, int *StudentID) {
	if (DS == NULL) return INVALID_INPUT;
	return ((School*)DS)->get_most_powerful(TeamID, StudentID);
}

StatusType RemoveStudent(void *DS, int StudentID) {
	if (DS == NULL) return INVALID_INPUT;
	return ((School*)DS)->remove_student(StudentID);
}


StatusType GetAllStudentsByPower(void *DS, int TeamID, int **Students, int *numOfStudents) {
	if ((DS == NULL)||(Students==NULL)||(numOfStudents==NULL)) return INVALID_INPUT;
	return ((School*)DS)->get_all_students_by_power(TeamID, Students, numOfStudents);
}

StatusType IncreaseLevel(void *DS, int Grade, int PowerIncrease) {
	if (DS == NULL) return INVALID_INPUT;
	return ((School*)DS)->increase_level(Grade, PowerIncrease);
}

void Quit(void** DS) {
	if (DS == NULL) return;
	delete *((School**)DS);
	*DS = NULL;
}