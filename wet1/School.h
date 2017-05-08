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
	Team(int id) : id(id) {}
	
};

class School {
	AVLTree<int, Mutant*> mutants_by_id;
	AVLTree<long long, Mutant*> mutants_by_power;
	AVLTree<int, Team> teams;
public:
	//School init();
	StatusType add_student(int student_id, int grade, int power);
	StatusType add_team(int TeamID);
	StatusType move_student_to_team(int StudentID, int TeamID);
	StatusType get_most_powerful(int TeamID, int *StudentID);
	StatusType remove_student(int StudentID);
	StatusType increase_level(int Grade, int PowerIncrease);
	StatusType get_all_students_by_power(int TeamID, int **Students, int *numOfStudents);
	void quit();

};

#endif