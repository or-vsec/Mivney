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
		delete new_mutant;
		return ALLOCATION_ERROR;
	}
	catch (AVLTreeKeyAlreadyExistsExpection) {
		delete new_mutant;
		return FAILURE;
	}
	try {
		mutants_by_power.insert(new_mutant->power, new_mutant);
	}
	catch (std::bad_alloc) {
		mutants_by_id.erase(new_mutant->id);
		delete new_mutant;
		return ALLOCATION_ERROR;
	}
	return SUCCESS;
}

StatusType School::add_team(int TeamID) {
	if (TeamID <= 0) return INVALID_INPUT;
	try {
		teams.insert(TeamID, Team(TeamID));
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
		Team* toteam = &teams.find(TeamID);
		Team* fromteam = mut->team;
		toteam->mutants->insert(mut->power, mut);
		if (fromteam != NULL) {
			fromteam->mutants->erase(mut->power);
		}
		mut->team = toteam;
	}
	catch (AVLTreeKeyNotFoundException) { //find failed
		return FAILURE;
	}
	catch (std::bad_alloc) { // insert failed
		return ALLOCATION_ERROR;
	}
	return SUCCESS;
}

StatusType School::get_most_powerful(int TeamID, int *StudentID) {
	if ((StudentID == NULL) || (TeamID == 0)) return INVALID_INPUT;
	try {
		if (TeamID < 0) {
			*StudentID = mutants_by_power.biggest()->id;
		}
		else {
			*StudentID = teams.find(TeamID).mutants->biggest()->id;
		}
	}
	catch (AVLTreeKeyNotFoundException) {
		return FAILURE;
	}
	catch (AVLTreeIsEmpty) {
		*StudentID = -1;
	}
	return SUCCESS;
}

StatusType School::remove_student(int StudentID) {
	if (StudentID <= 0) return INVALID_INPUT;
	try {
		Mutant* mut = mutants_by_id.find(StudentID);
		Team* team = mut->team;
		team->mutants->erase(mut->power);
		mutants_by_power.erase(mut->power);
		mutants_by_id.erase(StudentID);
		delete mut;
	}
	catch (AVLTreeKeyNotFoundException) {
		return FAILURE;
	}
	return SUCCESS;
}

StatusType School::get_all_students_by_power(int TeamID, int **Students, int *numOfStudents) {
	if (TeamID == 0 || Students == NULL || numOfStudents == NULL) return INVALID_INPUT;
	try {
		AVLTree<PowerID, Mutant*>* mutants_tree;
		if (TeamID < 0) {
			mutants_tree = &mutants_by_power;
		}
		else {
			mutants_tree = teams.find(TeamID).mutants;
		}
		*numOfStudents = mutants_tree->size();
		if (*numOfStudents == 0) {
			**Students = NULL;
			return SUCCESS;
		}
		else {
			AVLTree<PowerID, Mutant*>::ArrayNode* mutants_array = AVLTree<PowerID, Mutant*>::tree_to_array(*mutants_tree);
			*Students = (int*)malloc(*numOfStudents * sizeof(int));
			if (*Students == NULL) {
				delete[] mutants_array;
				return ALLOCATION_ERROR;
			}
			for (int i = 0; i < (*numOfStudents); i++) {
				(*Students)[*numOfStudents - 1 - i] = mutants_array[i]._value->id; // added reversed because tree is built from weakest to strongest
			}
			delete[] mutants_array;
		}
	}
	catch (std::bad_alloc) {
		return ALLOCATION_ERROR;
	}
	catch (AVLTreeKeyNotFoundException) {
		return FAILURE;
	}
	return SUCCESS;
}

static StatusType merge(AVLTree<PowerID, Mutant*>& mutants, int Grade, int PowerIncrease) {
	try {
		AVLTree<PowerID, Mutant*>::ArrayNode* mutants_array = AVLTree<PowerID, Mutant*>::tree_to_array(mutants);
		Mutant** same_grade_mutants = new Mutant*[mutants.size()];
		Mutant** diff_grade_mutants = new Mutant*[mutants.size()];
		int cnt_same = 0;
		int cnt_diff = 0;
		for (int i = 0; i < mutants.size(); i++) {
			if (mutants_array[i]._value->grade == Grade) {
				same_grade_mutants[cnt_same] = mutants_array[i]._value;
				same_grade_mutants[cnt_same]->power += PowerIncrease;
				cnt_same++;
			}
			else {
				diff_grade_mutants[cnt_diff++] = mutants_array[i]._value;
			}
		}
		int ind_same = 0;
		int ind_diff = 0;
		int ind_comb = 0;
		while (ind_same < cnt_same && ind_diff < cnt_diff) {
			if (same_grade_mutants[ind_same]->power < diff_grade_mutants[ind_diff]->power) {
				mutants_array[ind_comb++]._value = same_grade_mutants[ind_same++];
			}
			else {
				mutants_array[ind_comb++]._value = diff_grade_mutants[ind_diff++];
			}
		}
		while (ind_same < cnt_same) {
			mutants_array[ind_comb++]._value = same_grade_mutants[ind_same++];
		}
		while (ind_diff < cnt_diff) {
			mutants_array[ind_comb++]._value = same_grade_mutants[ind_same++];
		}
		for (int i = 0; i < ind_comb; i++) {
			mutants_array[i]._key = mutants_array[i]._value->power;
		}
		delete[] same_grade_mutants;
		delete[] diff_grade_mutants;
		mutants = AVLTree<PowerID, Mutant*>(mutants_array, ind_comb);
	}
	catch (std::bad_alloc) {
		return ALLOCATION_ERROR;
	}
	return SUCCESS;
}

StatusType School::increase_level(int Grade, int PowerIncrease) {
	if (Grade < 0 || PowerIncrease <= 0) return INVALID_INPUT;
	try {
		if (merge(mutants_by_power, Grade, PowerIncrease) == ALLOCATION_ERROR)
			return ALLOCATION_ERROR;
		AVLTree<int, Team>::ArrayNode* team_array = AVLTree<int, Team>::tree_to_array(teams);
		for (int i = 0; i < teams.size(); i++) {
			if (merge(*team_array[i]._value.mutants, Grade, 0) == ALLOCATION_ERROR)
				return ALLOCATION_ERROR;
		}
		delete[] team_array;
	}
	catch (std::bad_alloc) {
		return ALLOCATION_ERROR;
	}
	return SUCCESS;
}

School::~School() {
	AVLTree<int, Team>::ArrayNode* team_array = AVLTree<int, Team>::tree_to_array(teams);
	for (int i = 0; i < teams.size(); i++) {
		delete team_array[i]._value.mutants;
	}
	delete[] team_array;
	AVLTree<int, Mutant*>::ArrayNode* mutants_array = AVLTree<int, Mutant*>::tree_to_array(mutants_by_id);
	for (int i = 0; i < mutants_by_id.size(); i++) {
		delete mutants_array[i]._value;
	}
	delete[] mutants_array;
}