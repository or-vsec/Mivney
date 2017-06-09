#include "hash.h"



hash::hash(int tableSize)
{
		maxSize = tableSize;
		size = 0;
		table = new Node[tableSize];
	}
}


hash::~hash()
{
}

hash::Member() {

}