#pragma once

#include <iostream>
#include <math.h>

class HashTableException {};
class HashTableKeyNotFoundException : public HashTableException {};
class HashTableAlreadyExistsException : public HashTableException {};

#define MIN_SIZE		16
#define USED_SLOT		-1

template<typename ValueType>
class HashTable
{

public:

	HashTable(int size = MIN_SIZE);
	~HashTable();

	void insert(int key, const ValueType& value);
	void erase(int key);
	ValueType& get(int key);
	int size() { return _size; }

protected:

	class Node {
	public:
		int _key;
		ValueType _value;
		Node(int const & key, ValueType const & value) : _key(key), _value(value) {}
	};

	int _capacity;
	int _size;
	int _rehash_counter;
	Node** _array;

	void resize(int new_capacity);
	int hash(int key, int step);
};

template<typename ValueType>
HashTable<ValueType>::HashTable(int size) : _capacity(size), _size(0), _rehash_counter(0)
{
	_array = new Node*[_capacity];
	for (int i = 0; i < _capacity; i++) _array[i] = NULL;
}

template<typename ValueType>
HashTable<ValueType>::~HashTable()
{
	for (int i = 0; i < _capacity; i++) {
		if (_array[i] != NULL && _array[i] != (Node*)USED_SLOT) {
			delete _array[i];
		}
	}
	delete[] _array;
}

template<typename ValueType>
void HashTable<ValueType>::insert(int key, const ValueType & value)
{
	if (_size >= _capacity / 2) resize(_capacity * 2);
	int step = 0;
	while (_array[hash(key, step)] != NULL) {
		if (_array[hash(key, step)] != (Node*)USED_SLOT &&
			_array[hash(key, step)]->_key == key) {
			throw HashTableAlreadyExistsException();
		}
		step++;
	}
	_array[hash(key, step)] = new Node(key, value);
	_size++;
}

template<typename ValueType>
void HashTable<ValueType>::erase(int key)
{
	int step = 0;
	while (_array[hash(key, step)] != NULL) {
		if (_array[hash(key, step)] != (Node*)USED_SLOT &&
			_array[hash(key, step)]->_key == key) {
			delete _array[hash(key, step)];
			_array[hash(key, step)] = (Node*)USED_SLOT;
			_rehash_counter++;
			_size--;
			if (_size <= _capacity / 4) resize(_capacity / 2);
			if (_rehash_counter >= _capacity / 4) resize(_capacity);
			return;
		}
		step++;
	}
	throw HashTableKeyNotFoundException();
}

template<typename ValueType>
ValueType & HashTable<ValueType>::get(int key)
{
	int step = 0;
	while (_array[hash(key, step)] != NULL) {
		if (_array[hash(key, step)] != (Node*)USED_SLOT &&
			_array[hash(key, step)]->_key == key) {
			return _array[hash(key, step)]->_value;
		}
		step++;
	}
	throw HashTableKeyNotFoundException();
}

template<typename ValueType>
void HashTable<ValueType>::resize(int new_capacity)
{
	int old_capacity = _capacity;
	_capacity = new_capacity;
	_rehash_counter = 0;
	Node** new_array = new Node*[_capacity];
	for (int i = 0; i < _capacity; i++) new_array[i] = NULL;
	for (int i = 0; i < old_capacity; i++) {
		if (_array[i] != NULL && _array[i] != (Node*)USED_SLOT)
		{
			int step = 0;
			while (new_array[hash(_array[i]->_key, step)] != NULL) step++;
			new_array[hash(_array[i]->_key, step)] = _array[i];
		}
	}
	delete[] _array;
	_array = new_array;
}

template<typename ValueType>
int HashTable<ValueType>::hash(int key, int step)
{
	unsigned int knuth = 2654435769;
	return ((key * knuth) + step * 3) % _capacity;
}
