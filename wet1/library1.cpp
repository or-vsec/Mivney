#include "library1.h" 
#include "School.h"

void* Init() {
	School * DS = new School();
	return (void*)DS;
}

StatusType AddStudent(void *DS, int StudentID, int Grade, int Power) {
	if (DS == NULL) return INVALID_INPUT;
	return ((School*)DS)->add_student(StudentID, Grade, Power);
}