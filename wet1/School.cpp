#include "School.h"


StatusType School::add_student(int student_id, int grade, int power) {
	if (grade < 0 || power <= 0 || student_id <= 0) return INVALID_INPUT;
	Mutant* new_mutant;
	try {
		new_mutant = new Mutant(student_id, grade, power);
	}
	catch (std::bad_alloc) {
		return ALLOCATION_ERROR;
	}
	try {
		mutants_by_id.insert(new_mutant->id, new_mutant);
	}
	catch (std::bad_alloc) {
		return ALLOCATION_ERROR;
	}
	try {
		mutants_by_power.insert(new_mutant->power_id, new_mutant);
	}
	catch (std::bad_alloc) {
		return ALLOCATION_ERROR;
	}
	try{
		Team team0find = teams.find(0);
		team0find.mutants.insert(new_mutant->power_id, new_mutant);
	}
	catch (AVLTreeKeyAlreadyExistsExpection) {
		return FAILURE;
	}
	catch (std::bad_alloc) {
		return ALLOCATION_ERROR;
	}
	return SUCCESS;
}

StatusType School::add_team(int TeamID) {
	if (TeamID <= 0) return INVALID_INPUT;
	try {
		Team* new_team = new Team (TeamID);
		teams.insert(TeamID, *new_team);
		}
	catch (std::bad_alloc) {
		return ALLOCATION_ERROR;
	}
	catch (AVLTreeKeyAlreadyExistsExpection) {
		return FAILURE;
	}
	return SUCCESS;
}

StatusType School::move_student_to_team(int StudentID, int TeamID) {
	if (StudentID <= 0 || TeamID <= 0) return INVALID_INPUT;
	try {
		Mutant* mut = mutants_by_id.find(StudentID);
		Team* fromteam = mut->team;
		Team toteam = teams.find(TeamID);
	}
	catch (AVLTreeKeyNotFoundException) {
		return FAILURE;
	}
	Mutant* mut = mutants_by_id.find(StudentID);
	Team* fromteam = mut->team;
	Team toteam = teams.find(TeamID);
	try {
		toteam.mutants.insert(mut->power_id, mut);
	}
	catch (std::bad_alloc) {
		return ALLOCATION_ERROR;
	}
	fromteam->mutants.erase(mut->power_id); //what if insert fail? can i do try in try?
	mut->team = &toteam;
	return SUCCESS;
}

StatusType School::get_most_powerful(int TeamID, int *StudentID) {
	if ((StudentID == NULL)||(TeamID==0)) return INVALID_INPUT;
	try {
		if (TeamID > 0)
			Team team = teams.find(TeamID);
	}
	catch (AVLTreeKeyNotFoundException) {
		return FAILURE;
	}
	try {
		if (TeamID < 0)
			*StudentID = (mutants_by_power.biggest())->id;
		else 
			*StudentID = ((teams.find(TeamID)).mutants.biggest())->id;
	}
	catch (AVLTreeKeyNotFoundException) {
		*StudentID = -1;
	}
	return SUCCESS;
}

StatusType School::remove_student(int StudentID) {
	if (StudentID <= 0) return INVALID_INPUT;
	try {
		Mutant* mut = mutants_by_id.find(StudentID);
	}
		catch (AVLTreeKeyNotFoundException) {
			return FAILURE;
		}
		Mutant* mut = mutants_by_id.find(StudentID);
		Team* fromteam = mut->team;
		fromteam->mutants.erase(mut->power_id);
		mutants_by_power.erase(mut->power_id);
		mutants_by_id.erase(StudentID);
		return SUCCESS;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StatusType School::get_all_students_by_power(int TeamID, int **Students, int *numOfStudents) {
	if (TeamID == 0) return INVALID_INPUT;
	try {
		if (TeamID > 0)
			Team team = teams.find(TeamID);
	}
	catch (AVLTreeKeyNotFoundException) {
		return FAILURE;
	}
	if (TeamID < 0) {
		*numOfStudents = mutants_by_power.size();
		if (*numOfStudents == 0) {
			**Students = NULL;
			return SUCCESS;
		}
		else {
			try {
				AVLTree<long long, Mutant*>::ArrayNode* mutants_array = AVLTree<long long, Mutant*>::tree_to_array(mutants_by_power);
				**Students = malloc(*numOfStudents*(sizeof(int*))); /////////////////////???? 'what type do I return?
			}
			catch (std::bad_alloc) {
				return ALLOCATION_ERROR;
			}
			return SUCCESS;
		}
	}
	else {
		Team team = teams.find(TeamID);
		*numOfStudents = team.mutants.size();
		if (*numOfStudents == 0) {
			**Students = NULL;
			return SUCCESS;
		}
		else {
			try {
				AVLTree<long long, Mutant*>::ArrayNode* mutants_array = AVLTree<long long, Mutant*>::tree_to_array(team.mutants);
				*Students = malloc(*numOfStudents*(sizeof(int*))); /////////////////////???? 'what type do I return?
			}
			catch (std::bad_alloc) {
				return ALLOCATION_ERROR;
			}
			return SUCCESS;
		}
	}
	return SUCCESS;
}

StatusType School::increase_level(int Grade, int PowerIncrease){
	if (Grade < 0 || PowerIncrease <= 0) return INVALID_INPUT;
	AVLTree<long long, Mutant*>::ArrayNode* mutants_array = AVLTree<long long, Mutant*>::tree_to_array(mutants_by_power);
	try {
		Mutant** same_grade_mutants = new Mutant*[mutants_by_power.size()];
		Mutant** diff_grade_mutants = new Mutant*[mutants_by_power.size()];
		for (int i = 0; i < mutants_by_power.size(); i++) {
			if (mutants_array[i]._value->grade == Grade) {
				same_grade_mutants[i] = mutants_array[i]._value;
				same_grade_mutants[i]->power += PowerIncrease;
				same_grade_mutants[i]->update_power_id();
			}
			else {
				diff_grade_mutants[i] = mutants_array[i]._value;
			}
		}
		/////whats the balance function name?
	}
	catch (std::bad_alloc) {
			return ALLOCATION_ERROR;
	}
	return SUCCESS;
}

///////tbd
void School::quit(){
	delete this*.mutants_by_id;
	delete 

}