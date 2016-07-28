#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Written by Ugur Buyukdurak */

#define ARRSIZE 10
#define INITIALCAPACITY (ARRSIZE-1)
#define TYPENAME "Deque_"

#define Deque_DEFINE(T) 						 \
struct Deque_##T;								 \
												 \
struct Deque_##T##_Iterator {					 \
	Deque_##T * dequePtr;						 \
	int index;									 \
    void(*inc)(Deque_##T##_Iterator *);  		 \
	void(*dec)(Deque_##T##_Iterator *);  		 \
	T & (*deref)(Deque_##T##_Iterator *); 	     \
};																	   \
																	   \
struct Deque_##T {													   \
																	   \
	/* DATA MEMBERS START HERE */									   \
																	   \
	/*Allocate 10 integer space from heap for start*/				   \
	T * deque;													       \
																	   \
	/* Hold size of the deque so that when it becomes full, 		   \
	realocated bigger space. 2X more */								   \
	int dequeSize;													   \
																	   \
	/*Capacity of the deque */										   \
	int capacity;													   \
																	   \
	/* Actual Array Size */											   \
	int arraySize;													   \
																	   \
	/* Head */														   \
	Deque_##T##_Iterator head;										   \
																	   \
	/* Tail */														   \
	Deque_##T##_Iterator tail;										   \
																	   \
	/* Typename */													   \
	char type_name [sizeof(TYPENAME#T)];							   \
																	   \
	/* FUNCTIONS START HERE */										   \
																	   \
	void(*push_front)(Deque_##T *, const T & data);				       \
																	   \
	void(*push_back)(Deque_##T *, const T & data);				       \
																	   \
	void(*pop_back)(Deque_##T *);									   \
																	   \
	void(*pop_front)(Deque_##T *);									   \
																	   \
	T & (*back)(Deque_##T *);										   \
																	   \
	T & (*front)(Deque_##T *);									       \
																	   \
	T & (*at)(Deque_##T *, int index);							       \
																	   \
	bool(*empty)(Deque_##T *);										   \
																	   \
	const int &(*size)(Deque_##T *);								   \
																	   \
	bool(*lessby)(const T &, const T &);						       \
																	   \
	void(*clear)(Deque_##T *);										   \
																	   \
	void(*dtor)(Deque_##T *);										   \
																	   \
	Deque_##T##_Iterator(*begin)(Deque_##T *);						   \
																	   \
	Deque_##T##_Iterator (*end)(Deque_##T *);						   \
																	   \
};																		\
																		\
T * doubleArraySize(Deque_##T * &);									    \
bool empty(Deque_##T *);												\
																		\
void push_front(Deque_##T * ptr, const T & data) {					    \
	/* Check size */													\
	if (ptr->capacity > ptr->dequeSize) {								\
		/*Add an item at head */										\
		if (ptr->dequeSize == 0) {										\
			(ptr->deque)[ptr->head.index] = data;						\
			(ptr->dequeSize)++;											\
			return;														\
		}																\
		/* Decrement head */											\
		/* To Decrement call Iterator's dec method */					\
		ptr->head.dec(&ptr->head);										\
		(ptr->deque)[ptr->head.index] = data;							\
		(ptr->dequeSize)++;												\
	}																	\
	/* No enough size, expand the collection */							\
	else {																\
		/* Call the function which doubles the size */					\
		ptr->deque = doubleArraySize(ptr);								\
		ptr->head.dec(&ptr->head);										\
		(ptr->deque)[ptr->head.index] = data;							\
		(ptr->dequeSize)++;												\
	}																	\
}																		\
																		\
/* Add an element to Back(Tail) */										\
void push_back(Deque_##T * ptr, const T & data) {						\
	/* Check size */													\
	if (ptr->capacity > ptr->dequeSize) {								\
		/* Add an item at tail */										\
		if (ptr->dequeSize == 0) {										\
			(ptr->deque)[ptr->tail.index] = data;						\
			(ptr->dequeSize)++;											\
			return;														\
		}																\
		/* Increment tail and store value at incremented tail */		\
		/*To Increment call Iterator's inc method */					\
		ptr->tail.inc(&ptr->tail);										\
		(ptr->deque)[ptr->tail.index] = data;							\
		(ptr->dequeSize)++;												\
	}																	\
	/* No enough size, expand the collection. */						\
	else {																\
		/* Call the function which doubles the size */					\
		ptr->deque = doubleArraySize(ptr);								\
		ptr->tail.inc(&ptr->tail);										\
		(ptr->deque)[ptr->tail.index] = data;							\
		(ptr->dequeSize)++;												\
	}																	\
}																		\
																		\
/* Remove last element (at tail) */										\
void pop_back(Deque_##T * ptr){											\
	/* Check if deque is empty first */									\
	if(!empty(ptr)){													\
		ptr->tail.dec(&ptr->tail);										\
		(ptr->dequeSize)--;												\
	}																	\
}																		\
																		\
/* Remove first element (at head) */									\
void pop_front(Deque_##T * ptr) {										\
	/* Check if deque is empty first */									\
	if(!empty(ptr)){													\
		ptr->head.inc(&ptr->head);										\
		(ptr->dequeSize)--;												\
	}																	\
}																		\
																		\
/* Examine last element (tail) */										\
T & back(Deque_##T * ptr) {											    \
	return (ptr->deque)[ptr->tail.index];								\
}																		\
																		\
/* Examine first element (head) */										\
T & front(Deque_##T * ptr) {											\
	return (ptr->deque)[ptr->head.index];								\
}																		\
																		\
T& at(Deque_##T * ptr, int index) {									    \
	Deque_##T##_Iterator head = ptr->head;								\
	for (int i = 0; i < index; i++) {									\
		head.inc(&head);												\
	}																	\
	return (ptr->deque)[head.index];									\
}																		\
																		\
const int & size(Deque_##T * ptr) {										\
	return ptr->dequeSize;												\
}																		\
																		\
bool empty(Deque_##T * ptr) {											\
	if (ptr->dequeSize == 0)											\
		return true;													\
	return false;														\
}																		\
																		\
void clear(Deque_##T * ptr) {											\
	free(ptr->deque);													\
																		\
	/* Set Deque's Iterator Properties */								\
	ptr->deque = (T *)malloc(sizeof(T) * ARRSIZE);					    \
	ptr->head.index = ARRSIZE;											\
	ptr->tail.index = ARRSIZE;											\
																		\
	/* Set Deque properties */											\
	ptr->dequeSize = 0;													\
	ptr->capacity = INITIALCAPACITY;									\
}																		\
																		\
void dtor(Deque_##T * ptr) {											\
	free(ptr->deque);													\
}																		\
																		\
Deque_##T##_Iterator begin(Deque_##T * ptr) {							\
	return ptr->head;													\
}																		\
																		\
Deque_##T##_Iterator end(Deque_##T * ptr){								\
	/* Incremented here */												\
	Deque_##T##_Iterator tempIter = ptr->tail;							\
	tempIter.inc(&tempIter);											\
	return tempIter;													\
}																		\
																		\
/* Iterator Functions */												\
void inc(Deque_##T##_Iterator * ptr) {									\
	/* int index = (index + 1) % size */								\
	int arraySize = (ptr->dequePtr->capacity) + 1;						\
	int & iteratorIndex = ptr->index;									\
	iteratorIndex = (iteratorIndex + 1) % arraySize;					\
}																		\
																		\
void dec(Deque_##T##_Iterator * ptr) {									\
	/* int index = (index + size -1) % size */							\
	int arraySize = (ptr->dequePtr->capacity) + 1;						\
	int & iteratorIndex = ptr->index;									\
	iteratorIndex = (iteratorIndex + arraySize - 1) % arraySize;		\
}																		\
																		\
T & deref(Deque_##T##_Iterator * ptr) {								    \
	/* This should return an item in the array */						\
	const int & index = ptr->index;										\
	return (ptr->dequePtr->deque)[index];								\
}																		\
																		\
/* Global Functions */													\
void Deque_##T##_ctor(Deque_##T * ap, 									\
	bool(*func)(const T &objOne, const T &objTwo)) {				    \
																		\
	/* Initialize Data Members */										\
	ap->deque = (T*)malloc(sizeof(T) * ARRSIZE);					    \
	ap->dequeSize = 0;													\
	ap->capacity = INITIALCAPACITY;										\
	ap->arraySize = ARRSIZE;										    \
	memcpy(ap->type_name, TYPENAME#T, strlen(TYPENAME#T)+1);		    \
																		\
	/* Initalize Functions */											\
	ap->push_front = &push_front;										\
	ap->push_back = &push_back;											\
	ap->pop_front = &pop_front;											\
	ap->pop_back = &pop_back;											\
	ap->front = &front;													\
	ap->back = &back;													\
																		\
	ap->at = &at;														\
	ap->empty = &empty;													\
	ap->size = &size;													\
																		\
	/* This will be used for comparing two different classes */			\
	ap->lessby = func;													\
																		\
	ap->clear = &clear;													\
	ap->dtor = &dtor;													\
																		\
	ap->begin = &begin;													\
	ap->end = &end;														\
																		\
	/* Initalize Iterators of deque class */ 							\
	ap->head.index = ARRSIZE / 2;										\
	ap->head.inc = &inc;												\
	ap->head.dec = &dec;												\
	ap->head.deref = &deref;											\
	ap->head.dequePtr = ap;												\
																		\
	ap->tail.index = ARRSIZE / 2;										\
	ap->tail.inc = &inc;												\
	ap->tail.dec = &dec;												\
	ap->tail.deref = &deref;											\
	ap->tail.dequePtr = ap;												\
																		\
}																		\
																		\
bool Deque_##T##_Iterator_equal(const Deque_##T##_Iterator & ptrBegin, 	\
	const Deque_##T##_Iterator & ptrEnd) {								\
	if (ptrBegin.index == ptrEnd.index)									\
		return true;													\
	return false;														\
}																		\
																		\
bool Deque_##T##_equal(Deque_##T & objOne, Deque_##T & objTwo) {			\
	Deque_##T##_Iterator objOneBegin = objOne.begin(&objOne);				\
	Deque_##T##_Iterator objTwoBegin = objTwo.begin(&objTwo);				\
																		\
	if (objOne.dequeSize != objTwo.dequeSize) 							\
		return false;													\
																		\
	for (; !Deque_##T##_Iterator_equal(objOneBegin, objOne.end(&objOne)); \
	objOneBegin.inc(&objOneBegin), objTwoBegin.inc(&objTwoBegin)) {		\
		if (objOne.lessby(objOneBegin.deref(&objOneBegin), 				\
			objTwoBegin.deref(&objTwoBegin)) || 						\
			objOne.lessby(objTwoBegin.deref(&objTwoBegin), 				\
				objOneBegin.deref(&objOneBegin))) 						\
			return false;												\
	}																	\
	return true;														\
}																		\
																		\
T * doubleArraySize(Deque_##T * & ptr) {								\
	assert(ptr->dequeSize == ptr->capacity);								 \
																			 \
	/*Capacity + 1 is the actual arraysize.								     \
	  Capacity is number of objects allowed to be stored.*/					 \
																			 \
	T * biggerDeque = (T*)malloc(sizeof(T) * (ptr->arraySize * 2));			 \
																			 \
	int tempIndex = 0;														 \
	for (Deque_##T##_Iterator it = ptr->begin(ptr); 						 \
	!Deque_##T##_Iterator_equal(it, ptr->end(ptr)); it.inc(&it)) {	         \
		biggerDeque[tempIndex] = it.deref(&it);				                 \
		tempIndex++;														 \
	}																		 \
	/* Reset head and tail's indexes */							             \
																			 \
	ptr->arraySize *= 2;													 \
	ptr->capacity = ptr->arraySize - 1;										 \
																			 \
	ptr->head.index = 0;													 \
	ptr->tail.index = tempIndex;											 \
	free(ptr->deque);														 \
	return biggerDeque;														 \
}				
#endif