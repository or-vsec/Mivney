#pragma once
class hash
{
	int maxSize;
	int size;
	Node* table;
	int hashFunction(int id);
	void rehash();
public:

	hash(int tableSize = 11);
	void insert(int id, int teamId);
	int get(int id);
	~hash();
};


class Node {
	int teamId;
	int id;
	Node* next;
	Node* previous;
	void insert(int id, int teamId);
};