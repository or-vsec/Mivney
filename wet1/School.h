#ifndef _SCHOOL_
#define _SCHOOL_

#include "AVLTree.h"


class Mutant {
public:

	int id;
	int grade;
	int power;
	Team* team;
};

class Team {
public:
	int id;
	AVLTree<int, Mutant*> mutants; //Sorted by power;
	Mutant* most_powerful;
};

class School {
	AVLTree<int, Mutant&> mutants_by_id;
	AVLTree<int, Mutant*> mutants_by_power;
	AVLTree<int, Team&> teams;
};

#endif