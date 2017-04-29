#ifndef _AVLTREE_
#define _AVLTREE_


class AVLTreeException {};
class AVLTreeKeyNotFoundException : public AVLTreeException {};
class AVLTreeKeyAlreadyExistsExpection : public AVLTreeException {};
class AVLTreeAllocationFailedException : public AVLTreeException {};


// Requirements: KeyType::opertor<,
//               KeyType::operator==,
//               KeyType and ValueType copy c'tor
template<typename KeyType, typename ValueType>
class AVLTree {

protected:

	class Node {
	public:
		KeyType key;
		ValueType value;
		Node* father;
		Node* left_son;
		Node* right_son;
		int bf;
		Node(KeyType const & key, ValueType const & value) : key(key), value(value), father(nullptr), left_son(nullptr), right_son(nullptr), bf(0) {}
		Node() = delete;
	};

	Node* root;
	Node* last_searched_node;
	int size;


	typename AVLTree<KeyType, ValueType>::Node* Find_Recursive(KeyType const & key, Node* node);
	typename AVLTree<KeyType, ValueType>::Node** GetFatherToSonLinkPointer(Node* son);
	void SwapNodes(Node& a, Node& b);

public:

	AVLTree() : root(nullptr), size(0) {};
	~AVLTree() {};
	void Insert(KeyType const & key, ValueType const & value);
	void Erase(KeyType const & key);
	ValueType& Find(KeyType const & key);
	int Size() const { return size; }
};

#endif    /*_AVLTREE_ */


template<typename KeyType, typename ValueType>
typename AVLTree<KeyType, ValueType>::Node* AVLTree<KeyType, ValueType>::Find_Recursive(KeyType const & key, Node * node)
{
	if (node == nullptr) throw AVLTreeKeyNotFoundException();
	last_searched_node = node;
	if (node->key == key) return node;
	else if (node->key < key) return Find_Recursive(key, node->right_son);
	return Find_Recursive(key, node->left_son);
}

template<typename KeyType, typename ValueType>
typename AVLTree<KeyType, ValueType>::Node ** AVLTree<KeyType, ValueType>::GetFatherToSonLinkPointer(Node * son)
{
	if (son->father == nullptr) return &root;
	if (son->father->left_son == son) {
		return &(son->father->left_son);
	}
	return &(son->father->right_son);
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::SwapNodes(Node & a, Node & b)
{
	Node *left_son_t = a.left_son, *right_son_t = a.right_son, *father_t = a.father;
	int bf_t = a.bf;
	*(GetFatherToSonLinkPointer(&b)) = &a;
	*(GetFatherToSonLinkPointer(&a)) = &b;
	a.left_son = b.left_son;
	if (a.left_son != nullptr) a.left_son->father = &a;
	b.left_son = left_son_t;
	if (b.left_son != nullptr) b.left_son->father = &b;
	a.right_son = b.right_son;
	if (a.right_son != nullptr) a.right_son->father = &a;
	b.right_son = right_son_t;
	if (b.right_son != nullptr) b.right_son->father = &b;
	a.father = b.father;
	b.father = father_t;
	a.bf = b.bf;
	b.bf = bf_t;
	if (a.father == &a) a.father = &b;
	if (a.right_son == &a) a.right_son = &b;
	if (a.left_son == &a) a.left_son = &b;
	if (b.father == &b) b.father = &a;
	if (b.right_son == &b) b.right_son = &a;
	if (b.left_son == &b) b.left_son = &a;
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::Insert(KeyType const & key, ValueType const & value)
{
	if (root == nullptr) {
		root = new Node(key, value);
		return;
	}
	try {
		Find_Recursive(key, root);
		throw AVLTreeKeyAlreadyExistsExpection();
	}
	catch (AVLTreeKeyNotFoundException e) {
		Node* new_node = new Node(key, value);
		new_node->father = last_searched_node;
		if (last_searched_node->key < key) {
			last_searched_node->right_son = new_node;
		}
		else {
			last_searched_node->left_son = new_node;
		}
	}
}

template<typename KeyType, typename ValueType>
inline void AVLTree<KeyType, ValueType>::Erase(KeyType const & key)
{
	Node* node_to_delete = Find_Recursive(key, root);
	Node** father_to_son_pointer = GetFatherToSonLinkPointer(node_to_delete);
	if (node_to_delete->left_son != nullptr && node_to_delete->right_son != nullptr) {
		Node* next_minimal_node = node_to_delete->right_son;
		while (next_minimal_node->left_son != nullptr) {
			next_minimal_node = next_minimal_node->left_son;
		}
		SwapNodes(*node_to_delete, *next_minimal_node);
		Erase(key);
	}
	else {
		if (node_to_delete->left_son == nullptr) {
			*father_to_son_pointer = node_to_delete->right_son;
			node_to_delete->right_son->father = node_to_delete->father;
		}
		else if (node_to_delete->right_son == nullptr) {
			*father_to_son_pointer = node_to_delete->left_son;
			node_to_delete->left_son->father = node_to_delete->father;
		}
		else {
			*father_to_son_pointer = nullptr;
		}
		delete node_to_delete;
	}
}

template<typename KeyType, typename ValueType>
ValueType& AVLTree<KeyType, ValueType>::Find(KeyType const & key)
{
	return Find_Recursive(key, root)->value;
}
