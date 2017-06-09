#ifndef _SCHOOL_
#define _SCHOOL_

#include "AVLTree.h"
#include "UnionFind.h"
#include "HashTable.h"
#include "library2.h"
#include <iostream>

class Team;

class PowerID {
public:
	int id;
	int power;
	PowerID() : id(0), power(0) {};
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
	PowerID& operator+=(int power)
	{
		this->power += power;
		return *this;
	}
	PowerID& operator+=(const PowerID& rhs)
	{
		this->power += rhs.power;
		return *this;
	}
	PowerID operator+(const PowerID& rhs) const
	{
		PowerID result;
		result.id = this->id;
		result.power = this->power + rhs.power;
		return result;
	}
	PowerID operator-(const PowerID& rhs) const
	{
		PowerID result;
		result.id = this->id;
		result.power = this->power - rhs.power;
		return result;
	}

};

class Mutant {
public:
	int id;
	PowerID power;
	int team;
	Mutant(int id, int power, int team) : id(id), power(id, power), team(team) {}
};

class Team {
public:
	int num_of_wins;
	AVLTree<PowerID, Mutant*> mutants; //Sorted by power;
	Team() : num_of_wins(0) {}
};

class School {
	
	HashTable<Mutant> mutants;
	UnionFind<Team> teams;
	int n;
	
public:

	School(int n);
	~School();
	StatusType add_student(int student_id, int team_id, int power);
	StatusType remove_student(int student_id);
	StatusType join_teams(int team_1, int team_2);
	StatusType team_fight(int team_1, int team_2, int num_of_fighters);
	StatusType get_num_of_wins(int team_id, int * wins);
	StatusType get_student_team_leader(int student_id, int * leader);
	
};

#endif