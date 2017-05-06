#ifndef _SCHOOL_
#define _SCHOOL_

#include "AVLTree.h"
#include "library1.h"
#include <iostream>

class Team;

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
	Mutant(int id, int grade, int power) : id(id), grade(grade), power(power) {
		update_power_id();
	}
};

class Team {
public:
	int id;
	AVLTree<long long, Mutant*> mutants; //Sorted by power;
};

class School {
	AVLTree<int, Mutant*> mutants_by_id;
	AVLTree<long long, Mutant*> mutants_by_power;
	AVLTree<int, Team> teams;
public:
	StatusType add_student(int student_id, int grade, int power);
	StatusType IncreaseLevel(int grade, int power_increase);
};

#endif