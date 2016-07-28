#ifndef LIST_H
#define LIST_H

//Template List ADT implemented with linear array.

#include <iostream>
#include <cstring>
#include <new>
#include <cassert>
#include <initializer_list>

using std::ostream;

template<typename T>
class List {
public:	
	struct Iterator {
		Iterator(T * tmp = nullptr) {
			ptr = tmp;
		}
		Iterator(const Iterator & other) {
			ptr = other.ptr;
		}
		bool operator==(const Iterator & other) {
			return ptr == other.ptr;
		}
		bool operator!=(const Iterator & other) {
			return ptr != other.ptr;
		}
		Iterator & operator++() {
			ptr++;
			return *this;
		}
		Iterator & operator(int) {
			Iterator temp(*this);
			ptr++;
			return temp;
		}

		Iterator& operator--() {
			ptr--;
			return *this;
		}
		Iterator operator--(int) {
			Iterator temp(*this);
			ptr--;
			return temp;
		}
		T & operator *() const {
			return (*ptr);
		}
	 	T * operator->() const {
			return ptr;
		}
	private:
		T * ptr;
	};

	//Iterator Functions
	Iterator begin();
	Iterator end();
	
	//Constructor
	List(int = 16);

	//Initilazer List
	List(std::initializer_list<T>);

	//Copy Constructor
	List(const List<T> &);

	//Destructor
	~List();

	//Assignment Operator Overload
	const List<T> & operator=(const List &);

	//Move Constructor
	List(List<T> &&);

	//Move Assignment Operator
	List<T> & operator=(List<T> &&);

	//Check if empty
	bool empty() const;

	//Return size
	int size() const;

	//Display
	void display(ostream &) const;

	//Insert an element
	bool insert(int, T);

	//Append an element to end
	void append(T);

	//Delete an element
	bool remove(int);

	//Square bracket overload operator
	T & operator[](int);

private:
	//Number of elements in the List
	unsigned int listSize;
	//Allocated space for the array
	unsigned int listCapacity;
	//Private function to increase existing array sapce
	void doubleSize();
	//Pointer to dynamically created array
	T * myArray;

};

template <typename T> 
ostream & operator<< (ostream & out, const List<T> & list);

template <typename T>
List<T>::List(int initialCapacity) : listCapacity(initialCapacity), listSize(0) {
	//Do not throw exception on failure, return null pointer instead
	myArray = new (std::nothrow) T[listCapacity];
	assert(myArray != NULL);
}

template <typename T>
List<T>::List(std::initializer_list<T> l) : listCapacity(l.size() + 16), listSize(l.size()) {
	myArray = new (std::nothrow) T[listCapacity];
	assert(myArray != NULL);
	int i = 0;
	for (auto x : l) {
		myArray[i++] = x;
	}
}

template <typename T>
List<T>::List(const List<T> & other) : listCapacity(other.listCapacity), listSize(other.listSize) {
	this->myArray = new (std::nothrow) T[listCapacity];
	if (NULL != myArray)
		std::memcpy(this->myArray, other.myArray, other.listSize * sizeof(T));
	else {
		std::cerr << "Not enough space for memory allocation, program exiting..." << std::endl;
		exit(EXIT_FAILURE);
	}
}

template <typename T>
const List<T> & List<T>::operator=(const List<T> & other) {
	//Make sure whether both are the same.
	if (this != &other) {
		delete[] this->myArray;
		this->listCapacity = other.listCapacity;
		this->myArray = new (std::nothrow) T[this->listCapacity];
		if (NULL == this->myArray) { //Is memory available?
			std::cerr << "Inadequate memory to allocate stack, program exiting..." << std::endl;
			exit(EXIT_FAILURE);
		}
		this->listSize = other.listSize;
		std::memcpy(this->myArray, other.myArray, listSize * sizeof(T));
	}
	return *this;
}

template <typename T>
List<T>::List(List<T> && other) : listCapacity(other.listCapacity), listSize(other.listSize) {
	myArray = other.myArray;
	other.myArray = nullptr;
}

template <typename T>
List<T> & List<T>::operator=(List<T> && other) {
	if (this != &other) {
		delete[] this->myArray;
		this->listCapacity = other.listCapacity;
		this->listSize = other.listSize;
		this->myArray = other.myArray;
		other.myArray = nullptr;
	}
	return *this;
}

template <typename T>
List<T>::~List() {
	delete[] myArray;
	myArray = nullptr;
}

template <typename T>
bool List<T>::empty() const {
	return 0 == listSize;
}

template <typename T>
int List<T>::size() const {
	return this->listSize;
}

template <typename T>
void List<T>::display(ostream & out) const {
	for (auto i = 0; i < listSize; ++i)
		std::cout << myArray[i] << " ";
	std::cout << std::endl;
}

template <typename T>
bool List<T>::insert(int pos, T element) {
	//Check whether there is enough space for insertion
	if (listSize == listCapacity) {
		doubleSize();
	}
	//Check if position provided is valid
	if (pos < 0 || pos > listSize) {
		std::cerr << "Cannot be inserted in the provided position" << std::endl;
		return false;
	}
	//Else
	for (auto i = listSize; i > pos; --i)
		myArray[i] = myArray[i - 1];
	myArray[pos] = element;
	++listSize;

	return true;
}

template <typename T>
void List<T>::append(T element) {
	//Check whether there is enough space for append
	if (listSize == listCapacity)
		doubleSize();
	//Below statement both appends the value and increments listSize by one.
	myArray[listSize++] = element;
}

template <typename T>
bool List<T>::remove(int pos) {
	if (0 == listSize) return false;
	if (pos < 0 || pos = > listSize) {
		std::cerr << "The item in provided position does not exists" << std::endl;
		return false;
	}
	//else
	for (auto i = pos; i < listSize; ++i)
		myArray[i] = myArray[i + 1];
	--listSize;
	return true;
}

//Private function to double array size.
template <typename T>
void List<T>::doubleSize() {
	T * tmp = new (std::nothrow) T[listCapacity *= 2];
	if (NULL == tmp) {
		std::cerr << "Inadequate memory to allocate stack, program exiting..." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::memcpy(tmp, myArray, listSize * sizeof(myArray));
	delete[] myArray;
	myArray = tmp;
	tmp = nullptr;
}

template <typename T>
T & List<T>::operator[](int pos) {
	if (pos < 0 || pos >= this->listSize) {
		std::cerr << "Provided index is invalid, program exiting..." << std::endl;
		exit(EXIT_FAILURE);
	}
	return (this->myArray)[pos];
}

template <typename T>
ostream & operator << (ostream & out, const List<T> & list) {
	list.display(out);
	return out;
}

//Iterators
template <typename T>
typename List<T>::Iterator List<T>::begin() {
	return Iterator(myArray);
}

template <typename T>
typename List<T>::Iterator List<T>::end() {
	return Iterator(myArray + listSize);
}
#endif