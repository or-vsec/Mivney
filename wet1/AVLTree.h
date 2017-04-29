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
		int height;
		Node(KeyType const & key, ValueType const & value) : key(key), value(value), father(nullptr), left_son(nullptr), right_son(nullptr), height(0) {}
		Node() = default;
		void UpdateHeight() {
			if (left_son == nullptr && right_son == nullptr) height = 0;
			else if (right_son == nullptr) height = left_son->height + 1;
			else if (left_son == nullptr) height = right_son->height + 1;
			else {
				if (left_son->height >= right_son->height) height = left_son->height + 1;
				else height = right_son->height + 1;
			}
		}
		int BF() const {
			if (left_son == nullptr && right_son == nullptr) return 0;
			else if (right_son == nullptr) return left_son->height + 1;
			else if (left_son == nullptr) return -(right_son->height + 1);
			return left_son->height - right_son->height;
		}
	};

	// Members
	Node* root;
	Node* last_searched_node;
	int size;

	// Protected methods
	Node* Find_Recursive(KeyType const & key, Node* node);

	Node** GetFatherToSonLinkPointer(Node* son);

	void SwapNodes(Node& a, Node& b);

	void Balance(Node* node);
	void BalanceBottomTop(Node* bottom);
	void LLRotation(Node * b);
	void LRRotation(Node * b);
	void RRRotation(Node * b);
	void RLRotation(Node * b);

	void DeleteRecursive(Node* node);

	void AddToArrayRecursive(Node* node, Node** array, int* offset);
	Node** TreeToArray();

	Node* CompleteTree(int height);
	void MinimizeCompleteTree(Node* root, int final_size, int *current_size);
	AVLTree(Node** array, int size, int height);

public:
	// Public Methods
	AVLTree() : root(nullptr), size(0) {};
	~AVLTree() { DeleteRecursive(root); };
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
	int height_t = a.height;
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
	a.height = b.height;
	b.height = height_t;
	if (a.father == &a) a.father = &b;
	if (a.right_son == &a) a.right_son = &b;
	if (a.left_son == &a) a.left_son = &b;
	if (b.father == &b) b.father = &a;
	if (b.right_son == &b) b.right_son = &a;
	if (b.left_son == &b) b.left_son = &a;
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::Balance(Node * node)
{
	if (node->BF() == 2) {
		if (node->left_son->BF() == 1) LLRotation(node);
		else if (node->left_son->BF() == -1) LRRotation(node);
		else throw AVLTreeException();
	}
	else if (node->BF() == -2) {
		if (node->right_son->BF() == -1) RRRotation(node);
		else if (node->right_son->BF() == 1) RLRotation(node);
		else throw AVLTreeException();
	}
	else throw AVLTreeException();
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::BalanceBottomTop(Node * bottom)
{
	Node *current = bottom;
	while (current != root) {
		Node *parent = current->father;
		if (parent->height >= current->height + 1) break;
		parent->height = current->height + 1;
		if (parent->BF() > 1 || parent->BF() < -1) {
			Balance(parent);
		}
		current = parent;
	}
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::LLRotation(Node * b)
{
	Node* a = b->left_son;
	*GetFatherToSonLinkPointer(b) = a;
	a->father = b->father;
	b->father = a;
	b->left_son = a->right_son;
	if (a->right_son != nullptr) a->right_son->father = b;
	a->right_son = b;
	b->UpdateHeight();
	a->UpdateHeight();
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::LRRotation(Node * b)
{
	RRRotation(b->left_son);
	LLRotation(b);
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::RRRotation(Node * b)
{
	Node* a = b->right_son;
	*GetFatherToSonLinkPointer(b) = a;
	a->father = b->father;
	b->father = a;
	b->right_son = a->left_son;
	if (a->left_son != nullptr) a->left_son->father = b;
	a->left_son = b;
	b->UpdateHeight();
	a->UpdateHeight();
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::RLRotation(Node * b)
{
	LLRotation(b->right_son);
	RRRotation(b);
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::DeleteRecursive(Node * node)
{
	if (node == nullptr) return;
	DeleteRecursive(node->left_son);
	DeleteRecursive(node->right_son);
	delete node;
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::AddToArrayRecursive(Node * node, Node ** array, int * offset)
{
	if (node == nullptr) return;
	AddToArrayRecursive(node->left_son, array, offset);
	*(array + *offset) = node;
	*offset = *offset + 1;
	AddToArrayRecursive(node->right_son, array, offset);
}

template<typename KeyType, typename ValueType>
typename AVLTree<KeyType, ValueType>::Node ** AVLTree<KeyType, ValueType>::TreeToArray()
{
	Node** array = new Node*[size];
	int offset = 0;
	AddToArrayRecursive(root, array, &offset);
	return array;
}

template<typename KeyType, typename ValueType>
typename AVLTree<KeyType, ValueType>::Node * AVLTree<KeyType, ValueType>::CompleteTree(int height)
{
	if (height == -1) return nullptr;
	Node* left_son = CompleteTree(height - 1);
	Node* right_son = CompleteTree(height - 1);
	Node* root = new Node();
	root->left_son = left_son;
	root->right_son = right_son;
	if (left_son != nullptr) left_son->father = root;
	if (right_son != nullptr) right_son->father = root;
	root->height = height;
	return root;
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::MinimizeCompleteTree(Node* root, int final_size, int *current_size)
{
	if (*current_size == final_size) return;
	MinimizeCompleteTree(root->right_son, final_size, current_size);
	if (root->height == 0) {
		*(GetFatherToSonLinkPointer(root)) = nullptr;
		delete root;
		*current_size--;
	}
	MinimizeCompleteTree(root->left_son, final_size, current_size);
}

template<typename KeyType, typename ValueType>
AVLTree<KeyType, ValueType>::AVLTree(Node ** array, int size, int height)
{
	Node* blank_tree = CompleteTree(height);
	int current_size = 2**height;
	MinimizeCompleteTree(blank_tree, size, &current_size);

}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::Insert(KeyType const & key, ValueType const & value)
{
	AVLTree tree(nullptr, 5, 2);
	if (root == nullptr) {
		root = new Node(key, value);
		size++;
		return;
	}
	try {
		Find_Recursive(key, root);
		throw AVLTreeKeyAlreadyExistsExpection();
	}
	catch (AVLTreeKeyNotFoundException) {}
	Node* new_node = new Node(key, value);
	new_node->father = last_searched_node;
	if (last_searched_node->key < key) {
		last_searched_node->right_son = new_node;
	}
	else {
		last_searched_node->left_son = new_node;
	}

	BalanceBottomTop(new_node);
	size++;
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::Erase(KeyType const & key)
{
	Node* node_to_delete = Find_Recursive(key, root);
	if (node_to_delete->left_son != nullptr && node_to_delete->right_son != nullptr) {
		Node* next_minimal_node = node_to_delete->right_son;
		while (next_minimal_node->left_son != nullptr) {
			next_minimal_node = next_minimal_node->left_son;
		}
		SwapNodes(*node_to_delete, *next_minimal_node);
	}

	BalanceBottomTop(node_to_delete);

	Node** father_to_son_pointer = GetFatherToSonLinkPointer(node_to_delete);
	if (node_to_delete->left_son == nullptr && node_to_delete->right_son == nullptr) {
		*father_to_son_pointer = nullptr;
	}
	else if (node_to_delete->left_son == nullptr) {
		*father_to_son_pointer = node_to_delete->right_son;
		node_to_delete->right_son->father = node_to_delete->father;
	}
	else {
		*father_to_son_pointer = node_to_delete->left_son;
		node_to_delete->left_son->father = node_to_delete->father;
	}

	delete node_to_delete;
	size--;
}

template<typename KeyType, typename ValueType>
ValueType& AVLTree<KeyType, ValueType>::Find(KeyType const & key)
{
	return Find_Recursive(key, root)->value;
}
