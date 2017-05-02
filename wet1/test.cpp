#include "AVLTree.h"

int main() {
	AVLTree<int, int> tree;

	for (int i = 0; i <= 1000; i++) {
		tree.insert(i, i);
		for (int j = 0; j <= i; j++) {
			tree.find(j);
		}
	}
	tree.size();
	for (int i = 0; i <= 1000; i++) {
		tree.erase(i);
		for (int j = 1000; j > i; j--) {
			tree.find(j);
		}
	}

	for (int i = 1000; i >= 0; i--) {
		tree.insert(i, i);
		for (int j = 1000; j >= i; j--) {
			tree.find(j);
		}
	}
	for (int i = 1000; i >= 0; i--) {
		tree.erase(i);
		for (int j = 0; j <i; j++) {
			tree.find(j);
		}
	}


	return 0;
}