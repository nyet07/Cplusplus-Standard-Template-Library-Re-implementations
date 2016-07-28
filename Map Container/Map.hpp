#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include "Tree.h"
								/*
										UGUR BUYUKDURAK
								*/		
namespace cs540 {

	template<class Key_T, class Mapped_T>
	class Map;

	template<class Key_T, class Mapped_T>
	bool operator== (const Map<Key_T, Mapped_T> &, const Map<Key_T, Mapped_T> &);

	template<class Key_T, class Mapped_T>
	bool operator!= (const Map<Key_T, Mapped_T> &, const Map<Key_T, Mapped_T> &);

	template<class Key_T, class Mapped_T>
	bool operator< (const Map<Key_T, Mapped_T> &, const Map<Key_T, Mapped_T> &);

	template<class Key_T, class Mapped_T>
	class Map {
	public:

		using Iterator = typename Tree<Key_T, Mapped_T>::Iterator;
		using ConstIterator = typename Tree<Key_T, Mapped_T>::ConstIterator;
		using ReverseIterator = typename Tree<Key_T, Mapped_T>::ReverseIterator;

		Map <Key_T, Mapped_T>() {}
		Map <Key_T, Mapped_T>(const Map<Key_T, Mapped_T> & original) {
			tree = original.tree;
		}

		Map<Key_T, Mapped_T> & operator = (const Map & original) {
			clear();
			tree = original.tree;
			return *this;
		}

		Map <Key_T, Mapped_T>(std::initializer_list<std::pair<const Key_T, Mapped_T>> list) {
			for (auto x : list) {
				insert(x);
			}
		}

		size_t size() const {
			return tree.sizeR();
		}

		bool empty() const {
			return tree.empty();
		}

		//Iterator Functions	
		Iterator begin();
		Iterator end();
		ConstIterator begin() const;
		ConstIterator end() const;
		ReverseIterator rbegin();
		ReverseIterator rend();

		//Element Access.
		Iterator find(const Key_T &);
		ConstIterator find(const Key_T &) const;
		Mapped_T &at(const Key_T &);
		const Mapped_T &at(const Key_T &) const;
		Mapped_T & operator[] (const Key_T &);

		//Modifiers
		std::pair<typename Map <Key_T, Mapped_T>::Iterator, bool> insert(const ValueType<const Key_T, Mapped_T> & pair);

		template <typename IT_T>
		void insert(IT_T range_beg, IT_T range_end);

		void erase(Iterator pos);
		void erase(const Key_T &);

		void clear();

	private:

		Tree<Key_T, Mapped_T> tree;

		friend bool operator== <>(const Map<Key_T, Mapped_T> &, const Map<Key_T, Mapped_T> &);
		friend bool operator!= <>(const Map<Key_T, Mapped_T> &, const Map<Key_T, Mapped_T> &);
		friend bool operator< <>(const Map<Key_T, Mapped_T> &, const Map<Key_T, Mapped_T> &);
	};

	//Modifiers

	template<class Key_T, class Mapped_T>
	std::pair<typename Map <Key_T, Mapped_T>::Iterator, bool> Map<Key_T, Mapped_T>::insert(const ValueType<const Key_T, Mapped_T> & pair) {
		//std::pair< Map<Key_T, Mapped_T>::Iterator, bool> obj;
		NodePtr <Key_T, Mapped_T> tmp = tree.searchAuxPublic(pair.first);
		if (tmp) //If there exits a node with given key,
			return std::pair<Iterator, bool>(Iterator(tmp, &tree), false);			//Modified.
		tmp = tree.insert(pair.first, pair.second);
		return std::pair<Iterator, bool>(Iterator(tmp, &tree), true);				//Modified.
	}

	template<class Key_T, class Mapped_T>
	template <typename IT_T>
	void Map<Key_T, Mapped_T>::insert(IT_T range_beg, IT_T range_end) {
		for (; range_beg != range_end; ++range_beg) {
			tree.insert(range_beg->first, range_beg->second);	//First is key, second is data.
		}
	}

	template<class Key_T, class Mapped_T>
	void Map<Key_T, Mapped_T>::erase(const Key_T & key) {
		tree.remove(key);
	}

	template<class Key_T, class Mapped_T>
	void Map<Key_T, Mapped_T>::erase(const Iterator pos) {
		Key_T temp = pos.inode->pair.first;
		tree.remove(temp);
	}

	template<class Key_T, class Mapped_T>
	void Map<Key_T, Mapped_T>::clear() {
		tree.clear();
	}

	//Element Access.

	template<class Key_T, class Mapped_T>
	Mapped_T & Map<Key_T, Mapped_T>::at(const Key_T & key) {
		return tree.at(key);
	}

	template<class Key_T, class Mapped_T>
	const Mapped_T & Map<Key_T, Mapped_T>::at(const Key_T & key) const {
		return tree.at(key);
	}

	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::find(const Key_T & key) {
		return Map<Key_T, Mapped_T>::Iterator(tree.searchAuxPublic(key), &tree);     //Modified. to pass tree's pointer.
	}

	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::find(const Key_T & key) const {
		return Map<Key_T, Mapped_T>::ConstIterator(tree.searchAuxPublic(key), &tree);			//Modified.
	}

	template<class Key_T, class Mapped_T>
	Mapped_T & Map<Key_T, Mapped_T>::operator[] (const Key_T & key) {
		//If key is in the map object return reference to it.
		NodePtr<Key_T, Mapped_T> temp;
		if ((temp = tree.searchAuxPublic(key))) {
			return temp->data;
		}
		//If no object ever exist for the given key, construct a default object for the key.
		temp = tree.insert(key, Mapped_T());
		return temp->data;
	}

	//Iterator implementations
	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::begin() {
		return tree.begin();
	}

	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::end() {
		return tree.end();
	}

	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::begin() const {
		return tree.begin();
	}

	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::end() const {
		return tree.end();
	}

	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rbegin() {
		return tree.rbegin();
	}

	template<class Key_T, class Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rend() {
		return tree.rend();
	}

	//Global comparison functions for Map class.
	template<class Key_T, class Mapped_T>
	bool operator==(const Map<Key_T, Mapped_T> & mapF, const Map<Key_T, Mapped_T> & mapS) {
		return mapF.tree == mapS.tree;
	}

	template<class Key_T, class Mapped_T>
	bool operator!=(const Map<Key_T, Mapped_T> & mapF, const Map<Key_T, Mapped_T> & mapS) {
		return mapF.tree != mapS.tree;
	}

	template <class Key_T, class Mapped_T>
	bool operator < (const Map<Key_T, Mapped_T> & mapF, const Map<Key_T, Mapped_T> & mapS) {
		return mapF.tree < mapS.tree;
	}
}
#endif