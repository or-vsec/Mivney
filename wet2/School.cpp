#include "School.h"

//TODO: What about n = 0 ???
School::School(int n) : teams(n), n(n)
{}

School::~School()
{}

StatusType School::add_student(int student_id, int team_id, int power)
{
	if (student_id <= 0 || power <= 0 || team_id <= 0 || team_id > n) return INVALID_INPUT;
	try {
		mutants.insert(student_id, Mutant(student_id, power, team_id));
		Mutant* mutant = &mutants.get(student_id);
		teams.Find(team_id).mutants.insert(mutant->power, mutant);
	}
	catch (std::bad_alloc) {
		try {
			mutants.erase(student_id);
		}
		catch (...) {}
		return ALLOCATION_ERROR;
	}
	catch (UnionFindKeyNotFoundException) {
		return INVALID_INPUT;
	}
	catch (HashTableAlreadyExistsException) {
		return FAILURE;
	 }
	return SUCCESS;
}

StatusType School::remove_student(int student_id)
{
	if (student_id <= 0) return INVALID_INPUT;
	try {
		Mutant* mutant = &mutants.get(student_id);
		teams.Find(mutant->team).mutants.erase(mutant->power);
		mutants.erase(student_id);
	}
	catch (HashTableKeyNotFoundException) {
		return FAILURE;
	}
	return SUCCESS;
}

StatusType School::join_teams(int team_1, int team_2)
{
	if (team_1 <= 0 || team_2 <= 0 || team_1 > n || team_2 > n) return INVALID_INPUT;
	Team& team1 = teams.Find(team_1);
	Team& team2 = teams.Find(team_2);	
	try {
		Team& new_team = teams.Union(team_1, team_2);
		AVLTree<PowerID, Mutant*> merged_tree = AVLTree<PowerID, Mutant*>::merge(team1.mutants, team2.mutants);
		new_team.mutants = merged_tree;
		new_team.num_of_wins = team1.num_of_wins + team2.num_of_wins;
	}
	catch (UnionFindSameGroupException) {}
	return SUCCESS;
}

StatusType School::team_fight(int team_1, int team_2, int num_of_fighters)
{
	if (num_of_fighters <= 0 || team_1 <= 0 || team_2 <= 0 || team_1 > n || team_2 > n) return INVALID_INPUT;
	int team_1_power = teams.Find(team_1).mutants.get_fighters_total_power(num_of_fighters).power;
	int team_2_power = teams.Find(team_2).mutants.get_fighters_total_power(num_of_fighters).power;

	if (team_1_power < team_2_power) {
		teams.Find(team_2).num_of_wins++;
	}
	else if (team_2_power < team_1_power) {
		teams.Find(team_1).num_of_wins++;
	}
	return SUCCESS;
}

StatusType School::get_num_of_wins(int team_id, int * wins)
{
	if (team_id <= 0 || team_id <= 0 || wins == NULL) return INVALID_INPUT;
	*wins = teams.Find(team_id).num_of_wins;
	return SUCCESS;
}

StatusType School::get_student_team_leader(int student_id, int * leader)
{
	if (student_id <= 0 || leader == NULL) return INVALID_INPUT;
	try {
		int team_id = mutants.get(student_id).team;
		*leader = teams.Find(team_id).mutants.biggest()->id;
	}
	catch (HashTableKeyNotFoundException) {
		return FAILURE;
	}
	return SUCCESS;
}


