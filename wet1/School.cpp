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
				*Students = new int[*numOfStudents]; 
				for (int i = 0; i < (*numOfStudents); i++) {
					*Students[i] = mutants_array[i]._value->id;
					}
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
				*Students = new int[*numOfStudents];
				for (int i = 0; i < (*numOfStudents); i++) {
					*Students[i] = mutants_array[i]._value->id;
				}
			}
			catch (std::bad_alloc) {
				return ALLOCATION_ERROR;
			}
			return SUCCESS;
		}
	}
	return SUCCESS;
}
static StatusType merge(AVLTree<long long, Mutant*> mutants, int Grade, int PowerIncrease) {
	AVLTree<long long, Mutant*>::ArrayNode* mutants_array = AVLTree<long long, Mutant*>::tree_to_array(mutants);
	try {
		Mutant** same_grade_mutants = new Mutant*[mutants.size()];
		Mutant** diff_grade_mutants = new Mutant*[mutants.size()];
		int cnt_same = 0;
		int cnt_diff = 0;
		for (int i = 0; i < mutants.size(); i++) {
			if (mutants_array[i]._value->grade == Grade) {
				same_grade_mutants[cnt_same] = mutants_array[i]._value;
				same_grade_mutants[cnt_same]->power += PowerIncrease;
				same_grade_mutants[cnt_same]->update_power_id();
				cnt_same++;
			}
			else {
				diff_grade_mutants[cnt_diff] = mutants_array[i]._value;
				cnt_diff++;
			}
		}
		int ind_same = 0;
		int ind_diff = 0;
		for (int i = 0; i < mutants.size(); i++) {
			if ((same_grade_mutants[ind_same]->power_id < diff_grade_mutants[ind_diff]->power_id) || (ind_diff == cnt_diff)) {
				mutants_array[i]._value = same_grade_mutants[ind_same];
				ind_same++;
			}
			else {
				mutants_array[i]._value = diff_grade_mutants[ind_diff];
				ind_diff++;
			}
		}
		AVLTree<long long, Mutant*> mutants_by_power(mutants_array, mutants.size());
	}
		catch (std::bad_alloc) {
			return ALLOCATION_ERROR;
		}
		return SUCCESS;///////////??????????
}

StatusType School::increase_level(int Grade, int PowerIncrease){
	if (Grade < 0 || PowerIncrease <= 0) return INVALID_INPUT;
	
	try {
		merge(mutants_by_power, Grade, PowerIncrease);
		}
	catch (std::bad_alloc) {
			return ALLOCATION_ERROR;
	}
	try {
		AVLTree<int, Team>::ArrayNode* team_array = AVLTree<int, Team>::tree_to_array(teams);
		for (int i = 0; i < teams.size(); i++)
			merge(team_array[i]._value.mutants, Grade, 0);
	}
	catch (std::bad_alloc) {
		return ALLOCATION_ERROR;
	}
	return SUCCESS;
}


void School::quit(){
		AVLTree<long long, Mutant*>::ArrayNode* mutants_array = AVLTree<long long, Mutant*>::tree_to_array(mutants_by_power);
		for (int i = 0; i < mutants_by_power.size(); i++) {
			delete mutants_array[i]._value;
		}
		AVLTree<int, Team>::ArrayNode* teams_array = AVLTree<int, Team>::tree_to_array(teams);
		for (int i = 0; i < teams.size(); i++) {
			delete (teams_array[i]._value);
		}
		delete this;
}