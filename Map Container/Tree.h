#ifndef AVLTREE_H
#define AVLTREE_H
									
#include <iostream>
#include <algorithm>
#include "Node.h"

							/*
										UGUR BUYUKDURAK
							*/

template<class Key_T, class Mapped_T>
class Tree;

template<class Key_T, class Mapped_T>
using TreePtr = Tree<Key_T, Mapped_T> *;

template<class Key_T, class Mapped_T>
class Tree {
public:

	struct Iterator;
	struct ConstIterator;
	struct ReverseIterator;

	struct Iterator {

		NodePtr<Key_T, Mapped_T> inode;
		TreePtr<Key_T, Mapped_T> ptr;    //This is newly added.

		Iterator() {
			inode = NULL;
		}

		//Modified for holding a pointer to the Tree class.
		Iterator(NodePtr<Key_T, Mapped_T> node, const TreePtr<Key_T, Mapped_T> tree) : ptr(tree) {
			inode = NULL;
			if (node != NULL)
				inode = node;
		}

		Iterator(const Iterator & copy) {
			inode = copy.inode;
			ptr = copy.ptr;   //Modified.
		}

		bool operator==(const Iterator & itTwo) {
			return inode == itTwo.inode;
		}

		bool operator==(const ConstIterator & itTwo) {
			return inode == itTwo.inode;
		}

		bool operator!=(const Iterator & itTwo) {
			return !(inode == itTwo.inode);
		}

		bool operator!=(const ConstIterator & itTwo) {
			return !(inode == itTwo.inode);
		}

		//Operators
		Iterator& operator++() {
			increment();
			return *this;
		}
		Iterator operator++(int) {
			Iterator temp(*this);
			increment();
			return temp;
		}
		Iterator& operator--() {
			decrement();
			return *this;
		}
		Iterator operator--(int) {
			Iterator temp(*this);
			decrement();
			return temp;
		}
		ValueType<const Key_T, Mapped_T> & operator *() const {
			return inode->pair;
		}
		ValueType<const Key_T, Mapped_T> * operator->() const {
			return &(inode->pair);
		}

		void increment();
		void decrement();
	};

	struct ConstIterator {

		NodePtr <Key_T, Mapped_T> inode;
		const Tree<Key_T, Mapped_T> *ptr;

		ConstIterator() {
			inode = NULL;
		}

		//Modified for holding a pointer to the Tree class.
		ConstIterator(const NodePtr<Key_T, Mapped_T> & node, const Tree<Key_T, Mapped_T> *tree) : ptr(tree) {
			inode = NULL;
			if (node != NULL)
				inode = node;
		}
		//Conversion Constructor.
		//ConstIterator(const Iterator & copy) {}

		ConstIterator(const ConstIterator & iter) {
			inode = iter.inode;
			ptr = iter.ptr;  // Modified.
		}

		bool operator==(const Iterator & itTwo) {
			return inode == itTwo.inode;
		}

		bool operator==(const ConstIterator & itTwo) {
			return inode == itTwo.inode;
		}

		bool operator!=(const Iterator & itTwo) {
			return !(inode == itTwo.inode);
		}

		bool operator!=(const ConstIterator & itTwo) {
			return !(inode == itTwo.inode);
		}

		//Operators
		ConstIterator& operator++() {
			increment();
			return *this;
		}
		ConstIterator operator++(int) {
			ConstIterator temp(*this);
			increment();
			return temp;
		}
		ConstIterator& operator--() {
			decrement();
			return *this;
		}
		ConstIterator operator--(int) {
			ConstIterator temp(*this);
			decrement();
			return temp;
		}
		const ValueType<const Key_T, Mapped_T> & operator *() const {
			return inode->pair;
		}
		const ValueType<const Key_T, Mapped_T> * operator-> () const {
			return &(inode->pair);
		}

		void increment();
		void decrement();
	};

	struct ReverseIterator {

		NodePtr <Key_T, Mapped_T> inode;
		TreePtr<Key_T, Mapped_T> ptr;

		ReverseIterator() {
			inode = NULL;
		}

		//Modified for holding a pointer to the Tree class.
		ReverseIterator(NodePtr<Key_T, Mapped_T> node, const TreePtr<Key_T, Mapped_T> tree) : ptr(tree) {
			inode = NULL;
			if (node != NULL)
				inode = node;
		}
		ReverseIterator(const ReverseIterator & copy) {
			inode = copy.inode;
			ptr = copy.ptr;  //Modified.
		}

		bool operator==(const ReverseIterator & itTwo) {
			return inode == itTwo.inode;
		}

		bool operator!=(const ReverseIterator & itTwo) {
			return !(inode == itTwo.inode);
		}

		ReverseIterator& operator++() {
			increment();
			return *this;
		}
		ReverseIterator operator++(int) {
			ReverseIterator temp(*this);
			increment();
			return *this;
		}
		ReverseIterator& operator--() {
			decrement();
			return *this;
		}
		ReverseIterator operator--(int) {
			ReverseIterator temp(*this);
			decrement();
			return temp;
		}
		ValueType<const Key_T, Mapped_T> & operator *() const {
			return inode->pair;
		}
		ValueType<const Key_T, Mapped_T> * operator->() const {
			return &(inode->pair);
		}

		void increment();
		void decrement();
	};

	Iterator begin();
	Iterator end();
	const ConstIterator begin() const;
	const ConstIterator end() const;
	ReverseIterator rbegin();
	ReverseIterator rend();

	//Throw out of range exception.
	Mapped_T & at(const Key_T & key);
	const Mapped_T & at(const Key_T &) const;

	Tree<Key_T, Mapped_T>();
	Tree<Key_T, Mapped_T>(const Tree<Key_T, Mapped_T> & otherTree);

	bool empty() const {
		return treeRoot == NULL;
	}

	size_t sizeR() const {
		return size;
	}

	//Changed to use key.
	bool search(const Key_T & key) const;
	NodePtr<Key_T, Mapped_T> searchAuxPublic(const Key_T & key) {
		return searchAux(key);
	}

	NodePtr<Key_T, Mapped_T> searchAuxPublic(const Key_T & key) const {
		return searchAux(key);
	}

	NodePtr <Key_T, Mapped_T> insert(const Key_T &, const Mapped_T &);

	void remove(const Key_T & key) {
		removeNode(key);
	}

	//Traversal of tree just to see trees structure.
	//void inorder(std::ostream & out) const;

	void showbigesstSmallest() {
		std::cout << minValue(treeRoot)->data << std::endl;
		std::cout << maxValue(treeRoot)->data << std::endl;
	}

	void clear() {
		destructorAux();
		treeRoot = NULL;
		size = 0;
	}

	Tree<Key_T, Mapped_T> & operator = (const Tree & original);

	~Tree<Key_T, Mapped_T>();

private:

	//Root Node of the tree.
	NodePtr<Key_T, Mapped_T> treeRoot;

	//Size of the AVL Tree.
	size_t size;

	//Auxilary Functions
	//void inorderAux(std::ostream & out, NodePtr<Key_T, Mapped_T> subTreePtr) const;
	void updateHeightsForInsert(NodePtr<Key_T, Mapped_T> & insertedPtr);
	void updateHeightsForRemove(NodePtr<Key_T, Mapped_T> & insertedPtr);
	void copyConsAux(const Tree<Key_T, Mapped_T> & original);
	void destructorAux();
	void postOrderTraversalForDest(NodePtr<Key_T, Mapped_T> root);

	//Node Funcs
	short int bFactor(const NodePtr<Key_T, Mapped_T> & p) const;
	short int height(const NodePtr<Key_T, Mapped_T> & p) const;

	//Calculate balance factor for every node along the way.
	void calculateBalanceFactor(NodePtr<Key_T, Mapped_T> & leave);

	//Calculate height of each node.
	void calculateHeight(const NodePtr<Key_T, Mapped_T> & node) const;

	//Performs when right is heavy.
	void leftRotation(NodePtr<Key_T, Mapped_T> node);

	//Performs when left is heavy.
	void rightRotation(NodePtr<Key_T, Mapped_T> node);

	//Double Rotations
	void left_rightRotation(NodePtr<Key_T, Mapped_T> & node);
	void right_leftRotation(NodePtr<Key_T, Mapped_T> & node);

	//If a nodes balance factor is either -2 or +2 then this node is the pivot.
	void decideRotationForInsert(NodePtr<Key_T, Mapped_T> & node);
	void decideRotationForRemove(NodePtr<Key_T, Mapped_T> & node);

	//Retuns a pointer to the min or max value in the given subTree.
	NodePtr<Key_T, Mapped_T> minValue(const NodePtr<Key_T, Mapped_T> & subTreeRoot) const;
	NodePtr<Key_T, Mapped_T> maxValue(const NodePtr<Key_T, Mapped_T> & subTreeRoot) const;

	//Returns a pointer to successor or predecessor of a provided node in the parameters.
	NodePtr<Key_T, Mapped_T> inorderSuccessor(NodePtr<Key_T, Mapped_T> node);
	NodePtr<Key_T, Mapped_T> inorderPredecessor(NodePtr<Key_T, Mapped_T> node);

	//SearchAux Function For Remove.
	NodePtr<Key_T, Mapped_T> searchAux(const Key_T & item) const;
	NodePtr<Key_T, Mapped_T> searchAux(const Key_T & item);

	//Remove a node.
	void removeNode(const Key_T & data);
};

template<class Key_T, class Mapped_T>
Tree<Key_T, Mapped_T>::Tree() : treeRoot(0), size(0) { //Constructor
}

//Copy Constructors.
template<class Key_T, class Mapped_T>
Tree<Key_T, Mapped_T>::Tree(const Tree<Key_T, Mapped_T> & original) {
	copyConsAux(original);
}

template<class Key_T, class Mapped_T>
Tree<Key_T, Mapped_T>::~Tree() {
	destructorAux();
}

template<class Key_T, class Mapped_T>
Tree<Key_T, Mapped_T> & Tree<Key_T, Mapped_T>::operator = (const Tree<Key_T, Mapped_T> & original) {
	destructorAux();
	copyConsAux(original);
	return *this;
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::copyConsAux(const Tree<Key_T, Mapped_T> & original) {
	Tree<Key_T, Mapped_T>::ConstIterator x = original.begin();
	for (; x != original.end(); ++x)
		insert(x->first, x->second);

}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::destructorAux() {
	postOrderTraversalForDest(treeRoot);
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::postOrderTraversalForDest(NodePtr<Key_T, Mapped_T> root) {
	if (root == NULL) {
		return;
	}
	else {
		postOrderTraversalForDest(root->left);
		postOrderTraversalForDest(root->right);
		delete root;
	}
}

template<class Key_T, class Mapped_T>
bool Tree<Key_T, Mapped_T>::search(const Key_T & key) const {
	NodePtr<Key_T, Mapped_T> nodePtr = treeRoot;
	bool found = false;
	while (!found && nodePtr != 0) {
		if (key < nodePtr->key)
			nodePtr = nodePtr->left;
		else if (nodePtr->key < key)
			nodePtr = nodePtr->right;
		else
			found = true;
	}
	return found;
}

template<class Key_T, class Mapped_T>
Mapped_T & Tree<Key_T, Mapped_T>::at(const Key_T & key) {
	NodePtr<Key_T, Mapped_T> temp = searchAux(key);
	if (temp == NULL)
		throw std::out_of_range("out of range exception");
	return temp->data;
}

template<class Key_T, class Mapped_T>
const Mapped_T & Tree<Key_T, Mapped_T>::at(const Key_T & key) const {
	NodePtr<Key_T, Mapped_T> temp = searchAux(key);
	if (temp == NULL)
		throw std::out_of_range("out of range exception");
	return temp->data;
}

//Normal tree Insert operation.
//Function needs to let know avl algorithm if the node is placed.
template<class Key_T, class Mapped_T>
NodePtr<Key_T, Mapped_T> Tree<Key_T, Mapped_T>::insert(const Key_T & key, const Mapped_T & item) {
	NodePtr<Key_T, Mapped_T> locationPtr = treeRoot, parent = 0;
	bool found = false;

	while (!found && locationPtr != 0) {
		parent = locationPtr;
		if (key < locationPtr->key)
			locationPtr = locationPtr->left;
		else if (locationPtr->key < key)
			locationPtr = locationPtr->right;
		else
			found = true;
	}

	//If found replace the item with the new one.
	//This one removes node completely and reinserts it because pair needs to be updated to avoid data conflict.
	//During insertion, node is created once again so that pair is updated through Node Class' Constructor.
	if (found) {
		//locationPtr->data = item;
		removeNode(locationPtr->key);
		insert(key, item);
	}

	//If not found the value, and will be inserted.
	if (!found) {
		locationPtr = new Node<Key_T, Mapped_T>(key, item);
		if (parent == 0) { // empty tree
			treeRoot = locationPtr;
		}
		else if (key < parent->key) // insert left of parent
			parent->left = locationPtr;
		else
			parent->right = locationPtr;

		locationPtr->parent = parent;

		updateHeightsForInsert(locationPtr);

		if (NodePtr<Key_T, Mapped_T> successor = inorderSuccessor(locationPtr)) {
			locationPtr->listNext = successor;
			successor->listPrevious = locationPtr;
		}
		if (NodePtr<Key_T, Mapped_T> predecessor = inorderPredecessor(locationPtr)) {
			locationPtr->listPrevious = predecessor;
			predecessor->listNext = locationPtr;
		}

		//Inserted Size Increases one.
		++size;
		return locationPtr;
	}
	//else
		//std::cout << "Item is already in the tree" << std::endl;
	return NULL;
}

//Maintains nodes height property.
//This function is dependent on a node's parent property.
template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::updateHeightsForInsert(NodePtr<Key_T, Mapped_T> & insertedPtr) {
	NodePtr<Key_T, Mapped_T> child = insertedPtr;
	while (child->parent != 0) {

		calculateHeight(child);
		calculateBalanceFactor(child);
		this->decideRotationForInsert(child);

		child = child->parent;
	}

	if (child->parent == 0) {
		calculateHeight(child);
		calculateBalanceFactor(child);
		decideRotationForInsert(child);
	}
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::updateHeightsForRemove(NodePtr<Key_T, Mapped_T> & insertedPtr) {
	NodePtr<Key_T, Mapped_T> child = insertedPtr;
	if (child != NULL) {
		while (child->parent != 0) {

			calculateHeight(child);
			calculateBalanceFactor(child);
			decideRotationForRemove(child);

			child = child->parent;
		}

		if (child->parent == 0) {
			calculateHeight(child);
			calculateBalanceFactor(child);
			decideRotationForRemove(child);
		}
	}
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::calculateBalanceFactor(NodePtr<Key_T, Mapped_T> & node) {
	node->balanceFactor = bFactor(node);
}

template<class Key_T, class Mapped_T>
short int Tree<Key_T, Mapped_T>::bFactor(const NodePtr<Key_T, Mapped_T> & p) const {
	return height(p->left) - height(p->right);
}

template<class Key_T, class Mapped_T>
short int Tree<Key_T, Mapped_T>::height(const NodePtr<Key_T, Mapped_T> & p) const {
	return p ? p->height : -1;
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::calculateHeight(const NodePtr<Key_T, Mapped_T> & node) const {
	node->height = std::max(height(node->right), height(node->left)) + 1;
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::leftRotation(NodePtr<Key_T, Mapped_T> node) {
	//if (node->balanceFactor == -2) {
	NodePtr<Key_T, Mapped_T> tempNode = node->right;
	tempNode->parent = node->parent;
	if (node->parent != NULL) {
		if (node == node->parent->left)
			node->parent->left = tempNode;
		else if (node == node->parent->right)
			node->parent->right = tempNode;
	}

	node->parent = tempNode;
	node->right = tempNode->left;

	//Set left child's rightsubtre's left wing's parent to node.
	if (tempNode->left != NULL)
		tempNode->left->parent = node;

	tempNode->left = node;

	calculateHeight(node);
	calculateBalanceFactor(node);

	calculateHeight(tempNode);
	calculateBalanceFactor(tempNode);

	if (tempNode->parent == NULL)
		this->treeRoot = tempNode;
	//}
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::rightRotation(NodePtr<Key_T, Mapped_T> node) {
	//if (node->balanceFactor == 2) {
	NodePtr<Key_T, Mapped_T> tempNode = node->left;
	tempNode->parent = node->parent;
	if (node->parent != NULL) {
		if (node == node->parent->left)
			node->parent->left = tempNode;
		else if (node == node->parent->right)
			node->parent->right = tempNode;
	}
	node->parent = tempNode;
	node->left = tempNode->right;

	//Change is made here.
	if (tempNode->right != NULL)
		tempNode->right->parent = node;

	tempNode->right = node;

	//Update heights and balance factors. Because tree changes.
	//Check if right and left subtrees are null or not.
	calculateHeight(node);
	calculateBalanceFactor(node);

	calculateHeight(tempNode);
	calculateBalanceFactor(tempNode);

	if (tempNode->parent == NULL)
		this->treeRoot = tempNode;
	//}
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::left_rightRotation(NodePtr<Key_T, Mapped_T> & node) {
	leftRotation(node->left);
	rightRotation(node);
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::right_leftRotation(NodePtr<Key_T, Mapped_T> & node) {
	rightRotation(node->right);
	leftRotation(node);
}

//If a nodes balance factor is either -2 or +2 then this node is the pivot.
template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::decideRotationForInsert(NodePtr<Key_T, Mapped_T> & node) {
	NodePtr<Key_T, Mapped_T> temp = NULL;
	//Left subtree of pivot is longer in height.
	if (node->balanceFactor == 2) {
		temp = node->left;
		//Node added to left subtree of left child of pivot.
		if (temp->balanceFactor == 1) {
			//Call right rotation
			rightRotation(node);
		}
		//Node added to right subtree of left child of pivot.
		if (temp->balanceFactor == -1) {
			//Call double rotation.
			left_rightRotation(node);
		}
	}
	//Right subtree of pivot is longer in height.
	if (node->balanceFactor == -2) {
		temp = node->right;
		//Node added to left subtree of right child of pivot.
		if (temp->balanceFactor == 1) {
			//Call dobule rotation
			right_leftRotation(node);
		}
		//Node added to right subtree of right child of pivot.
		if (temp->balanceFactor == -1) {
			//Call left left rotation.
			leftRotation(node);
		}
	}
}
template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::decideRotationForRemove(NodePtr<Key_T, Mapped_T> & node) {
	NodePtr<Key_T, Mapped_T> temp = NULL;

	//Left subtree is longer
	if (node->balanceFactor == 2) {
		temp = node->left;

		//Node added to left subtree of left child of pivot.
		if (temp->balanceFactor == 1 || temp->balanceFactor == 0) {
			//Call right rotation
			rightRotation(node);
		}

		//Node added to right subtree of left child of pivot.
		else if (temp->balanceFactor == -1) {
			//Call double rotation.
			left_rightRotation(node);
		}

	}
	//Right subtree is longer.
	if (node->balanceFactor == -2) {
		temp = node->right;

		if (temp->balanceFactor == 1) {
			//Call dobule rotation
			right_leftRotation(node);
		}

		//Node added to right subtree of right child of pivot.
		else if (temp->balanceFactor == -1 || temp->balanceFactor == 0) {
			//Call left left rotation.
			leftRotation(node);
		}

	}
}

template<class Key_T, class Mapped_T>
NodePtr<Key_T, Mapped_T> Tree<Key_T, Mapped_T>::minValue(const NodePtr<Key_T, Mapped_T> & subTreeRoot) const {
	if (subTreeRoot == NULL)
		return NULL;

	NodePtr<Key_T, Mapped_T> current = subTreeRoot;
	while (current->left != NULL) {
		current = current->left;
	}
	return current;
}

template<class Key_T, class Mapped_T>
NodePtr<Key_T, Mapped_T> Tree<Key_T, Mapped_T>::maxValue(const NodePtr<Key_T, Mapped_T> & subTreeRoot) const {
	if (subTreeRoot == NULL)
		return NULL;

	NodePtr<Key_T, Mapped_T> current = subTreeRoot;
	while (current->right != NULL) {
		current = current->right;
	}
	return current;
}

template<class Key_T, class Mapped_T>
NodePtr<Key_T, Mapped_T> Tree<Key_T, Mapped_T>::inorderSuccessor(NodePtr<Key_T, Mapped_T> node) {  //node is the node whose successor to be found.
																								   //If the rightSubTree of node is not NULL successor resides in that subtree.
	if (node->right != NULL)
		return minValue(node->right);

	//If the rightSubTree is NULL, then successor is one of its ancestors.
	NodePtr<Key_T, Mapped_T> p = node->parent;
	while (p != NULL && node == p->right) {
		node = p;
		p = node->parent;
	}
	return p;
}

template<class Key_T, class Mapped_T>
NodePtr<Key_T, Mapped_T> Tree<Key_T, Mapped_T>::inorderPredecessor(NodePtr<Key_T, Mapped_T> node) {
	if (node->left != NULL)
		return maxValue(node->left);

	NodePtr<Key_T, Mapped_T> p = node->parent;
	while (p != NULL && node == p->left) {
		node = p;
		p = node->parent;
	}

	return p;
}

template<class Key_T, class Mapped_T>
NodePtr<Key_T, Mapped_T> Tree<Key_T, Mapped_T>::searchAux(const Key_T & key) {
	NodePtr<Key_T, Mapped_T> nodePtr = treeRoot;
	bool found = false;
	while (!found && nodePtr != 0) {
		if (key < nodePtr->key)
			nodePtr = nodePtr->left;
		else if (nodePtr->key < key)
			nodePtr = nodePtr->right;
		else
			found = true;
	}
	if (found)
		return nodePtr;

	return NULL;
}


template<class Key_T, class Mapped_T>
NodePtr<Key_T, Mapped_T> Tree<Key_T, Mapped_T>::searchAux(const Key_T & key) const {
	NodePtr<Key_T, Mapped_T> nodePtr = treeRoot;
	bool found = false;
	while (!found && nodePtr != 0) {
		if (key < nodePtr->key)
			nodePtr = nodePtr->left;
		else if (nodePtr->key < key)
			nodePtr = nodePtr->right;
		else
			found = true;
	}
	if (found)
		return nodePtr;

	return NULL;
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::removeNode(const Key_T & key) {

	NodePtr<Key_T, Mapped_T> subTree = NULL;
	NodePtr<Key_T, Mapped_T> nodeToDelete = NULL;

	if (NodePtr<Key_T, Mapped_T> node = searchAux(key)) {

		nodeToDelete = node;

		if (node->left != NULL && node->right != NULL) {

			//SuccessorNode
			NodePtr<Key_T, Mapped_T> nodeSucc = inorderSuccessor(node);

			//New Node with successor's value,
			NodePtr<Key_T, Mapped_T> newNode = new Node<Key_T, Mapped_T>(nodeSucc->pair);

			//Copy old node's contents into new one.
			newNode->height = node->height;
			newNode->balanceFactor = node->balanceFactor;
			newNode->parent = node->parent;
			newNode->right = node->right;
			newNode->left = node->left;
			newNode->listPrevious = node->listPrevious;
			newNode->listNext = node->listNext;

			//Set Predecessor's and Successor's pointer to newly created Node.
			node->listPrevious->listNext = newNode;
			node->listNext->listPrevious = newNode;

			//Set children's parent
			node->right->parent = newNode;   //NewNode is the new parent.
			node->left->parent = newNode;

			if (node->parent == NULL) // Node is the root node of the tree.
				treeRoot = newNode;

			//Check if the node to be deleted is not root meaning has a parent that it not NULL.
			if (node->parent != NULL) {   // If node to be deleted is not root,
										  //Update parent of node to be deleted.
				if (node == node->parent->right) {
					node->parent->right = newNode;
				}
				if (node == node->parent->left) {
					node->parent->left = newNode;
				}
			}

			//After copying is done, delete the node to be deleted.
			delete node;

			//Set successor node.
			nodeToDelete = nodeSucc;
		}
		//The node to be deleted either has 1 or 0 nodes.
		//Choose left or right subtrees in which right can also be empty subtree.
		//Subtree->parent is the node of actionPosition.

		//First set node to be deleted's left side as a subtree.
		subTree = nodeToDelete->left;
		//subTree = tmp->left;
		//If node's left subtree is empty then set the node's right as subtree. (right subtree can be empty as well)
		if (subTree == NULL) {
			subTree = nodeToDelete->right;
			//subTree = tmp->right;
		}
		//Check if the node to be deleted is root.
		if (nodeToDelete->parent == NULL) {
			//if(tmp->parent == NULL){
			treeRoot = subTree;
		}
		//Check the node to be deleted is left or right node.
		else if (nodeToDelete->parent->left == nodeToDelete) {
			//else if(tmp->parent->left == tmp){
			nodeToDelete->parent->left = subTree;
			//tmp->parent->left = subTree;
		}
		else {
			nodeToDelete->parent->right = subTree;
			//tmp->parent->right = subTree;
		}
		//If subtree is not null, set its parent pointer to node's parent pointer.
		if (subTree != NULL) {
			subTree->parent = nodeToDelete->parent;
			//subTree->parent = tmp->parent;
		}
		//Update heights of newly created tree. (updates entire tree)
		//updateHeightsForRemove(node->parent);
		updateHeightsForRemove(nodeToDelete->parent);

		{
			if (nodeToDelete->listPrevious != NULL)
				(nodeToDelete->listPrevious)->listNext = (nodeToDelete->listNext);
			if (nodeToDelete->listNext != NULL)
				(nodeToDelete->listNext)->listPrevious = (nodeToDelete->listPrevious);
		}

		//On removed decrease size.
		--size;
		//delete node;
		delete nodeToDelete;
	}
	//else {
	//	std::cout << "The node does not exist" << std::endl;
	//}
}

//Iterators start here.
template<class Key_T, class Mapped_T>
typename Tree<Key_T, Mapped_T>::Iterator Tree<Key_T, Mapped_T>::begin() {
	//Modified.
	return Iterator(minValue(treeRoot), this);
}

template<class Key_T, class Mapped_T>
typename Tree<Key_T, Mapped_T>::Iterator Tree<Key_T, Mapped_T>::end() {
	//Modified.
	return Iterator(NULL, this);
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::Iterator::increment() {
	if (inode != NULL) {
		inode = inode->listNext;
	}
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::Iterator::decrement() {
	if (inode != NULL) {
		inode = inode->listPrevious;
	}
	//This if statement is added later,
	if (inode == NULL) {
		//If decrement operator is called (decrement method is called within the operator) inode gets initialized to Maximum(last) element in the tree.
		//Ptr is the pointer to points to actual tree class.
		inode = ptr->maxValue(ptr->treeRoot);
	}
}

template<class Key_T, class Mapped_T>
const typename Tree<Key_T, Mapped_T>::ConstIterator Tree<Key_T, Mapped_T>::begin() const {
	return ConstIterator(minValue(treeRoot), this);
}

template<class Key_T, class Mapped_T>
const typename Tree<Key_T, Mapped_T>::ConstIterator Tree<Key_T, Mapped_T>::end() const {
	return ConstIterator(NULL, this);
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::ConstIterator::increment() {
	if (inode != NULL) {
		inode = inode->listNext;
	}
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::ConstIterator::decrement() {
	if (inode != NULL) {
		inode = inode->listPrevious;
	}
	//This if statement is added later.
	if (inode == NULL) {
		//If decrement operator is called (decrement method is called within the operator) inode gets initialized to Maximum(last) element in the tree.
		//Ptr is the pointer to points to actual tree class.
		inode = ptr->maxValue(ptr->treeRoot);
	}
}

template<class Key_T, class Mapped_T>
typename Tree<Key_T, Mapped_T>::ReverseIterator Tree<Key_T, Mapped_T>::rbegin() {
	//Modified.
	return ReverseIterator(maxValue(treeRoot), this);
}

template<class Key_T, class Mapped_T>
typename Tree<Key_T, Mapped_T>::ReverseIterator Tree<Key_T, Mapped_T>::rend() {
	//Modified.
	return ReverseIterator(NULL, this);
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::ReverseIterator::increment() {
	if (inode != NULL) {
		inode = inode->listPrevious;
	}
}

template<class Key_T, class Mapped_T>
void Tree<Key_T, Mapped_T>::ReverseIterator::decrement() {
	if (inode != NULL) {
		inode = inode->listNext;
	}
	//This if statement is added later,
	if (inode == NULL) {
		//If decrement operator is called (decrement method is called within the operator) inode gets initialized to Maximum(last) element in the tree.
		//Ptr is the pointer to point to actual tree class.
		inode = ptr->minValue(ptr->treeRoot);
	}
}

//Tree Comparison Functions
template <class Key_T, class Mapped_T>
bool operator==(const Tree<Key_T, Mapped_T> & treeOne, const Tree<Key_T, Mapped_T> & treeTwo) {
	typename Tree<Key_T, Mapped_T>::ConstIterator first = treeOne.begin();
	typename Tree<Key_T, Mapped_T>::ConstIterator second = treeTwo.begin();
	if (treeOne.sizeR() != treeTwo.sizeR())
		return false;
	for (; first != treeOne.end() || second != treeTwo.end(); ++first, ++second) {
		if (!(first->first == second->first) || !(first->second == second->second))
			return false;
	}
	return true;
}

template <class Key_T, class Mapped_T>
bool operator!=(const Tree<Key_T, Mapped_T> & treeOne, const Tree<Key_T, Mapped_T> & treeTwo) {
	return !(treeOne == treeTwo);
}
//Should I check size?
template <class Key_T, class Mapped_T>
bool operator<(const Tree<Key_T, Mapped_T> & treeOne, const Tree<Key_T, Mapped_T> & treeTwo) {
	return treeOne.sizeR() < treeTwo.sizeR();
}

#endif