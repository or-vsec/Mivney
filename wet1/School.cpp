#include "School.h"


StatusType School::add_student(int student_id, int grade, int power) {
	if (grade < 0 || power <= 0 || student_id <= 0) return INVALID_INPUT;
	try {
		Mutant* new_mutant = new Mutant(student_id, grade, power);
		mutants_by_id.insert(new_mutant->id, new_mutant);
		mutants_by_power.insert(new_mutant->power_id, new_mutant);
	}
	catch (AVLTreeKeyAlreadyExistsExpection) {
		return FAILURE;
	}
	catch (std::bad_alloc) {
		return ALLOCATION_ERROR;
	}
	return SUCCESS;
}

StatusType School::IncreaseLevel(int grade, int power_increase)
{
	if (grade < 0 || power_increase <= 0) return INVALID_INPUT;
	AVLTree<long long, Mutant*>::ArrayNode* mutants_array = AVLTree<long long, Mutant*>::tree_to_array(mutants_by_power);
	Mutant** same_grade_mutants = new Mutant*[mutants_by_power.size()];
	Mutant** diff_grade_mutants = new Mutant*[mutants_by_power.size()];
	for (int i = 0; i < mutants_by_power.size(); i++) {
		if (mutants_array[i]._value->grade == grade) {
			same_grade_mutants[i] = mutants_array[i]._value;
		}
		else {
			diff_grade_mutants[i] = mutants_array[i]._value;
		}
	}

	return StatusType();
}
