#ifndef _SCHOOL_
#define _SCHOOL_

#include "AVLTree.h"
#include "library1.h"
#include <iostream>

class Team;

class PowerID {
	int id;
	int power;
public:
	PowerID() = default;
	PowerID(int id, int power) : id(id), power(power) {}
	bool operator==(const PowerID& rhs) const {
		return id == rhs.id && power == rhs.power;
	}
	bool operator<(const PowerID& rhs) const {
		if (power == rhs.power) {
			return id > rhs.id;
		}
		return power < rhs.power;
	}
	PowerID& operator=(int power)
	{
		this->power = power;
		return *this;
	}
};

class Mutant {
public:
	int id;
	int grade;
	PowerID power;
	Team* team;
	Mutant(int id, int grade, int power) : id(id), grade(grade), power(id, power) {}
};

class Team {
public:
	int id;
	AVLTree<PowerID, Mutant*> mutants; //Sorted by power;
	Team(int id) : id(id) {}
};

class School {
	AVLTree<int, Mutant*> mutants_by_id;
	AVLTree<PowerID, Mutant*> mutants_by_power;
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