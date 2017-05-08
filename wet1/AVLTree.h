#ifndef _AVLTREE_
#define _AVLTREE_

#include <math.h>
#include <iostream>

class AVLTreeException {};
class AVLTreeKeyNotFoundException : public AVLTreeException {};
class AVLTreeKeyAlreadyExistsExpection : public AVLTreeException {};
class AVLTreeIsEmpty : public AVLTreeException {};


// Requirements: KeyType::opertor<,
//               KeyType::operator==,
//               KeyType and ValueType copy c'tor
template<typename KeyType, typename ValueType>
class AVLTree {

public:

	class ArrayNode {
	public:
		KeyType _key;
		ValueType _value;
		ArrayNode(KeyType const & key, ValueType const & value) : _key(key), _value(value) {}
		ArrayNode() = default;
	};
	 
	// Public Methods
	// O(n)
	~AVLTree() { delete_recursive(_root); };
	AVLTree(ArrayNode* array, int size);
	static ArrayNode* tree_to_array(const AVLTree& tree);
	// O(1)
	AVLTree() : _root(NULL), _last_searched(NULL), _biggest(NULL), _size(0) {};

	// O(log n)
	void insert(KeyType const & key, ValueType const & value);
	void erase(KeyType const & key);
	ValueType& find(KeyType const & key);

	// O(1)
	ValueType& biggest() const;
	int size() const { return _size; }

protected:

	class Node {
	public:
		KeyType _key;
		ValueType _value;
		Node* _parent;
		Node* _left_son;
		Node* _right_son;
		int _height;
		Node(KeyType const & key, ValueType const & value) : _key(key), _value(value), _parent(NULL), _left_son(NULL), _right_son(NULL), _height(0) {}
		Node() = default;
		void update_height() {
			if (_left_son == NULL && _right_son == NULL) _height = 0;
			else if (_right_son == NULL) _height = _left_son->_height + 1;
			else if (_left_son == NULL) _height = _right_son->_height + 1;
			else {
				if (_left_son->_height >= _right_son->_height) _height = _left_son->_height + 1;
				else _height = _right_son->_height + 1;
			}
		}
		int bf() const {
			if (_left_son == NULL && _right_son == NULL) return 0;
			else if (_right_son == NULL) return _left_son->_height + 1;
			else if (_left_son == NULL) return -(_right_son->_height + 1);
			return _left_son->_height - _right_son->_height;
		}
	};

	// Members
	Node* _root;
	Node* _last_searched;
	Node* _biggest;
	int _size;

	// Protected methods
	// O(log n)
	Node* find_recursion(KeyType const & key, Node* node);
	void update_biggest();

	// O(1)
	Node** parent_to_son_link_pointer(Node const * const son);
	void swap_nodes(Node& a, Node& b);

	// O(log n)
	void balance_bottom_up(Node* bottom);
	// O(1)
	void balance_node(Node* node);
	void ll_rotation(Node * b);
	void lr_rotation(Node * b);
	void rr_rotation(Node * b);
	void rl_rotation(Node * b);

	// O(n)
	static void delete_recursive(Node* node);
	static void add_to_array_recursion(Node* node, ArrayNode* array, int* offset);
	static void add_from_array_recursion(Node* root, ArrayNode* array, int* offset);
	static Node* complete_tree(int height);
	static void minimize_complete_tree(Node* root, int final_size, int *current_size);
};



template<typename KeyType, typename ValueType>
typename AVLTree<KeyType, ValueType>::Node* AVLTree<KeyType, ValueType>::find_recursion(KeyType const & key, Node * node)
{
	if (node == NULL) throw AVLTreeKeyNotFoundException();
	_last_searched = node;
	if (node->_key == key) return node;
	else if (node->_key < key) return find_recursion(key, node->_right_son);
	return find_recursion(key, node->_left_son);
}

template<typename KeyType, typename ValueType>
typename AVLTree<KeyType, ValueType>::Node ** AVLTree<KeyType, ValueType>::parent_to_son_link_pointer(Node const * const son)
{
	if (son->_parent == NULL) return &_root;
	if (son->_parent->_left_son == son) {
		return &(son->_parent->_left_son);
	}
	return &(son->_parent->_right_son);
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::swap_nodes(Node & a, Node & b)
{
	Node *left_son_t = a._left_son, *right_son_t = a._right_son, *parent_t = a._parent;
	int height_t = a._height;
	*(parent_to_son_link_pointer(&b)) = &a;
	*(parent_to_son_link_pointer(&a)) = &b;
	a._left_son = b._left_son;
	if (a._left_son != NULL) a._left_son->_parent = &a;
	b._left_son = left_son_t;
	if (b._left_son != NULL) b._left_son->_parent = &b;
	a._right_son = b._right_son;
	if (a._right_son != NULL) a._right_son->_parent = &a;
	b._right_son = right_son_t;
	if (b._right_son != NULL) b._right_son->_parent = &b;
	a._parent = b._parent;
	b._parent = parent_t;
	a._height = b._height;
	b._height = height_t;
	if (a._parent == &a) a._parent = &b;
	if (a._right_son == &a) a._right_son = &b;
	if (a._left_son == &a) a._left_son = &b;
	if (b._parent == &b) b._parent = &a;
	if (b._right_son == &b) b._right_son = &a;
	if (b._left_son == &b) b._left_son = &a;
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::balance_node(Node * node)
{
	if (node->bf() == 2) {
		if (node->_left_son->bf() >= 0) ll_rotation(node);
		else if (node->_left_son->bf() == -1) lr_rotation(node);
		else throw AVLTreeException();
	}
	else if (node->bf() == -2) {
		if (node->_right_son->bf() <= 0) rr_rotation(node);
		else if (node->_right_son->bf() == 1) rl_rotation(node);
		else throw AVLTreeException();
	}
	else throw AVLTreeException();
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::balance_bottom_up(Node * bottom)
{
	Node *current = bottom;
	while (current != _root) {
		Node *parent = current->_parent;
		parent->update_height();
		if (parent->bf() > 1 || parent->bf() < -1) {
			balance_node(parent);
		}
		current = parent;
	}
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::ll_rotation(Node * b)
{
	Node* a = b->_left_son;
	*parent_to_son_link_pointer(b) = a;
	a->_parent = b->_parent;
	b->_parent = a;
	b->_left_son = a->_right_son;
	if (a->_right_son != NULL) a->_right_son->_parent = b;
	a->_right_son = b;
	b->update_height();
	a->update_height();
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::lr_rotation(Node * b)
{
	rr_rotation(b->_left_son);
	ll_rotation(b);
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::rr_rotation(Node * b)
{
	Node* a = b->_right_son;
	*parent_to_son_link_pointer(b) = a;
	a->_parent = b->_parent;
	b->_parent = a;
	b->_right_son = a->_left_son;
	if (a->_left_son != NULL) a->_left_son->_parent = b;
	a->_left_son = b;
	b->update_height();
	a->update_height();
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::rl_rotation(Node * b)
{
	ll_rotation(b->_right_son);
	rr_rotation(b);
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::update_biggest()
{
	_biggest = _root;
	if (_biggest != NULL) {
		while (_biggest->_right_son != NULL) _biggest = _biggest->_right_son;
	}
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::delete_recursive(Node * node)
{
	if (node == NULL) return;
	delete_recursive(node->_left_son);
	delete_recursive(node->_right_son);
	delete node;
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::add_to_array_recursion(Node * node, ArrayNode* array, int * offset)
{
	if (node == NULL) return;
	add_to_array_recursion(node->_left_son, array, offset);
	*(array + *offset) = ArrayNode(node->_key, node->_value);
	*offset = *offset + 1;
	add_to_array_recursion(node->_right_son, array, offset);
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::add_from_array_recursion(Node * node, ArrayNode* array, int * offset)
{
	if (node == NULL) return;
	add_from_array_recursion(node->_left_son, array, offset);
	ArrayNode* array_node = (array + *offset);
	node->_key = array_node->_key;
	node->_value = array_node->_value;
	*offset = *offset + 1;
	add_from_array_recursion(node->_right_son, array, offset);
}

template<typename KeyType, typename ValueType>
typename AVLTree<KeyType, ValueType>::ArrayNode* AVLTree<KeyType, ValueType>::tree_to_array(const AVLTree<KeyType, ValueType>& tree)
{
	ArrayNode* array = new ArrayNode[tree._size];
	int offset = 0;
	add_to_array_recursion(tree._root, array, &offset);
	return array;
}

template<typename KeyType, typename ValueType>
typename AVLTree<KeyType, ValueType>::Node * AVLTree<KeyType, ValueType>::complete_tree(int height)
{
	if (height == -1) return NULL;
	Node* left_son = complete_tree(height - 1);
	Node* right_son = complete_tree(height - 1);
	Node* node = new Node();
	node->_left_son = left_son;
	node->_right_son = right_son;
	if (left_son != NULL) left_son->_parent = node;
	if (right_son != NULL) right_son->_parent = node;
	node->_height = height;
	return node;
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::minimize_complete_tree(Node* node, int final_size, int *current_size)
{
	if (node == NULL) return;
	if (*current_size == final_size) return;
	if (node->_height == 0) {
		if (node->_parent->_left_son == node) {
			node->_parent->_left_son = NULL;
		}
		else {
			node->_parent->_right_son = NULL;
		}
		node->_parent->update_height();
		delete node;
		(*current_size)--;
		return;
	}
	minimize_complete_tree(node->_right_son, final_size, current_size);
	minimize_complete_tree(node->_left_son, final_size, current_size);
}

template<typename KeyType, typename ValueType>
AVLTree<KeyType, ValueType>::AVLTree(ArrayNode* array, int size)
{
	int height = (int)floor(log2(size));
	Node* blank_tree = complete_tree(height);
	int current_size = (int)pow(2, height + 1) - 1;
	minimize_complete_tree(blank_tree, size, &current_size);

	int array_offset = 0;
	add_from_array_recursion(blank_tree, array, &array_offset);
	delete array;
	_root = blank_tree;
	_size = size;
	update_biggest();
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::insert(KeyType const & key, ValueType const & value)
{
	if (_root == NULL) {
		_root = new Node(key, value);
		_biggest = _root;
		_size++;
		return;
	}
	try {
		find_recursion(key, _root);
		throw AVLTreeKeyAlreadyExistsExpection();
	}
	catch (AVLTreeKeyNotFoundException) {}
	Node* new_node = new Node(key, value);
	new_node->_parent = _last_searched;
	if (_last_searched->_key < key) {
		_last_searched->_right_son = new_node;
	}
	else {
		_last_searched->_left_son = new_node;
	}

	balance_bottom_up(new_node);
	if (_biggest->_key < new_node->_key) _biggest = new_node;
	_size++;
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::erase(KeyType const & key)
{
	Node* node_to_delete = find_recursion(key, _root);
	if (node_to_delete->_left_son != NULL && node_to_delete->_right_son != NULL) {
		Node* next_minimal_node = node_to_delete->_right_son;
		while (next_minimal_node->_left_son != NULL) {
			next_minimal_node = next_minimal_node->_left_son;
		}
		swap_nodes(*node_to_delete, *next_minimal_node);
	} 

	Node** parent_to_son_pointer = parent_to_son_link_pointer(node_to_delete);
	if (node_to_delete->_left_son == NULL && node_to_delete->_right_son == NULL) {
		*parent_to_son_pointer = NULL;
	}
	else if (node_to_delete->_left_son == NULL) {
		*parent_to_son_pointer = node_to_delete->_right_son;
		node_to_delete->_right_son->_parent = node_to_delete->_parent;
	}
	else {
		*parent_to_son_pointer = node_to_delete->_left_son;
		node_to_delete->_left_son->_parent = node_to_delete->_parent;
	}

	if (node_to_delete->_parent != NULL) {
		balance_bottom_up(node_to_delete);
	}
	if (_biggest == node_to_delete) {
		update_biggest();
	}
	delete node_to_delete;
	_size--;
}

template<typename KeyType, typename ValueType>
ValueType & AVLTree<KeyType, ValueType>::biggest() const
{
	if (_biggest == NULL) throw AVLTreeIsEmpty();
	return _biggest->_value;
}

template<typename KeyType, typename ValueType>
ValueType& AVLTree<KeyType, ValueType>::find(KeyType const & key)
{
	return find_recursion(key, _root)->_value;
}

#endif    /*_AVLTREE_ */
