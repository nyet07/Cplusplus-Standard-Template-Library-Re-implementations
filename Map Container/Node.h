#ifndef NODE_H
#define NODE_H

#include <string>

template<class Key_T, class Mapped_T>
using ValueType = std::pair <const Key_T, Mapped_T>;
			
							/*
											UGUR BUYUKDURAK
							*/

template <class Key_T, class Mapped_T>
class Node {
public:

	//A pointer to parent. This is used for meantaining height property.
	Node * parent;
	//Recently added pair object for Iterator's returning ValueType. 
	ValueType<Key_T, Mapped_T> pair;
	//Key and the Data for dictionary
	Mapped_T & data;
	const Key_T & key;
	//Nodes left and right children.
	Node * right;
	Node * left;
	//Pointers for constant iteration. 
	Node * listNext;
	Node * listPrevious;
	//Height Property of the Tree.
	unsigned char height;
	//Balance Factor
	short int balanceFactor;

	//Functions
	Node() :height(0), left(0), right(0), parent(0), key(pair.first), data(pair.second) {}
	
	Node(Key_T key, Mapped_T item) : parent(0), pair(key, item), data(pair.second), key(pair.first),
		right(0), left(0), listNext(0), listPrevious(0), height(0), balanceFactor(0) {}

	Node(ValueType <Key_T, Mapped_T> x):parent(0), pair(x), data(pair.second), key(pair.first),
		right(0), left(0), listNext(0), listPrevious(0), height(0), balanceFactor(0) {}
};


template<class Key_T, class Mapped_T>
using NodePtr = Node <Key_T, Mapped_T> *;

#endif