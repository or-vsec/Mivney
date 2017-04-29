#include "AVLTree.h"

int main() {
	AVLTree<int, int> tree;

	for (int i = 0; i <= 1000; i++) {
		tree.Insert(i, i);
		for (int j = 0; j <= i; j++) {
			tree.Find(j);
		}
	}
	for (int i = 0; i <= 1000; i++) {
		tree.Erase(i);
		for (int j = 1000; j > i; j--) {
			tree.Find(j);
		}
	}

	for (int i = 1000; i >= 0; i--) {
		tree.Insert(i, i);
		for (int j = 1000; j >= i; j--) {
			tree.Find(j);
		}
	}
	for (int i = 1000; i >= 0; i--) {
		tree.Erase(i);
		for (int j = 1000; j >i; j--) {
			tree.Find(j);
		}
	}


	return 0;
}