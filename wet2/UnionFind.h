#pragma once

#include <iostream>

class UnionFindException {};
class UnionFindKeyNotFoundException : public UnionFindException {};
class UnionFindSameGroupException : public UnionFindException {};

template<typename ValueType>
class UnionFind {

public:
	ValueType& Find(const int key);
	ValueType& Union(int group_1, int group_2);

	UnionFind(int size);
	~UnionFind();

protected:

	class Node {
	public:
		ValueType _value;
		int _id;
		Node* _parent;
		Node(int id) : _id(id), _parent(NULL) {}
	};

	class GroupNode {
	public:
		Node* _node;
		int _size;
		GroupNode(Node* node) : _node(node), _size(1) {}
	};

	int get_group_root_id(const int key);
	Node* get_group_root(const int key);

	// Members
	Node** _nodes;
	GroupNode** _groups;
	GroupNode** _all_groups;
	int _size;
};

template<typename ValueType>
ValueType& UnionFind<ValueType>::Find(const int key)
{
	return get_group_root(key)->_value;
}


template<typename ValueType>
ValueType& UnionFind<ValueType>::Union(int group_1, int group_2)
{
	if (group_1 > _size || group_2 > _size || group_1 <= 0 || group_2 <= 0) throw UnionFindKeyNotFoundException();

	group_1 = get_group_root_id(group_1) - 1;
	group_2 = get_group_root_id(group_2) - 1;
	if (group_1 == group_2) throw UnionFindSameGroupException();

	GroupNode* group1 = _groups[group_1];
	GroupNode* group2 = _groups[group_2];

	if (group1->_size >= group2->_size) {
		group1->_size += group2->_size;
		group2->_node->_parent = group1->_node;
		_groups[group_2] = group1;
		return group1->_node->_value;
	}
	else {
		group2->_size += group1->_size;
		group1->_node->_parent = group2->_node;
		_groups[group_1] = group2;
		return group2->_node->_value;
	}
}

template<typename ValueType>
UnionFind<ValueType>::UnionFind(int size)
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

template<typename ValueType>
UnionFind<ValueType>::~UnionFind()
{
	for (int i = 0; i < _size; i++) {
		delete _nodes[i];
		delete _all_groups[i];
	}
	delete[] _nodes;
	delete[] _groups;
	delete[] _all_groups;
}

template<typename ValueType>
int UnionFind<ValueType>::get_group_root_id(const int key)
{
	return get_group_root(key)->_id;
}

template<typename ValueType>
typename UnionFind<ValueType>::Node* UnionFind<ValueType>::get_group_root(const int key)
{
	if (key > _size || key <= 0) throw UnionFindKeyNotFoundException();
	Node* node = _nodes[key - 1];
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
	return root;
}

