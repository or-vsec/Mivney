#pragma once

#include <iostream>

class UnionFindException {};
class UnionFindKeyNotFoundException : public UnionFindException {};
class UnionFindSameGroupExpcetion : public UnionFindException {};

class UnionFind {

public:
	int Find(const int key);
	int Union(const int group_1, const int group_2);

	UnionFind(int size);
	~UnionFind();

protected:

	class Node {
	public:
		int _key;
		Node* _parent;
		Node(int key) : _key(key), _parent(NULL) {}
		Node() : _parent(NULL) {}
	};

	class GroupNode {
	public:
		Node* _node;
		int _size;
		GroupNode(Node* node) : _node(node), _size(1) {}
	};


	// Members
	Node** _nodes;
	GroupNode** _groups;
	GroupNode** _all_groups;
	int _size;
};

int UnionFind::Find(const int key)
{
	if (key > _size || key <= 0) throw UnionFindKeyNotFoundException();
	Node* node = _nodes[key-1];
	Node* current_node = node;
	while (current_node->_parent != NULL) {
		current_node = current_node->_parent;
	}
	Node* root = current_node;
	current_node = node;
	while (current_node->_parent != NULL) {
		Node* parent = current_node->_parent;
		current_node->_parent = root;
		current_node = parent;
	}
	return root->_key;
}

int UnionFind::Union(const int group_1, const int group_2)
{
	if (group_1 > _size || group_2 > _size || group_1 <= 0 || group_2 <= 0) throw UnionFindKeyNotFoundException();
	GroupNode* group1 = _groups[group_1-1];
	GroupNode* group2 = _groups[group_2-1];
	if (group1 == group2) throw UnionFindSameGroupExpcetion();
	if (group1->_size >= group2->_size) {
		group1->_size += group2->_size;
		group2->_node->_parent = group1->_node;
		_groups[group_2-1] = group1;
		return group1->_node->_key;
	}
	else {
		group2->_size += group1->_size;
		group1->_node->_parent = group2->_node;
		_groups[group_1-1] = group2;
		return group2->_node->_key;
	}
}

UnionFind::UnionFind(int size)
{
	_size = size;
	_nodes = new Node*[size];
	_groups = new GroupNode*[size];
	_all_groups = new GroupNode*[size];
	for (int i = 0; i < size; i++) {
		_nodes[i] = new Node(i+1);
		_all_groups[i] = new GroupNode(_nodes[i]);
		_groups[i] = _all_groups[i];
	}
}

UnionFind::~UnionFind()
{
	for (int i = 0; i < _size; i++) {
		delete _nodes[i];
		delete _all_groups[i];
	}
	delete[] _nodes;
	delete[] _groups;
	delete[] _all_groups;
}

