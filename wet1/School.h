#ifndef _SCHOOL_
#define _SCHOOL_

#include "AVLTree.h"


class Mutant {
public:

	int id;
	int grade;
	int power;
	long long power_id;
	Team* team;
	void update_power_id() {
		power_id = id + power * sizeof(int);
	}
};

class Team {
public:
	int id;
	AVLTree<long long, Mutant*> mutants; //Sorted by power;
};

class School {
	AVLTree<int, Mutant> mutants_by_id;
	AVLTree<long long, Mutant*> mutants_by_power;
	AVLTree<int, Team> teams;
};

#endif