#include "AVLTree.h"

int main() {
	AVLTree<int, int> tree;
	tree.Insert(1,1);
	tree.Insert(4,4);
	tree.Insert(3,3);
	tree.Find(4);
	tree.Find(3);
	tree.Erase(4);
	tree.Find(3);
	tree.Find(4);

	return 0;
}