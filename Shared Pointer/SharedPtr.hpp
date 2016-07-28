#ifndef SHAREDPTR_HPP
#define SHAREDPTR_HPP

#include <iostream>
#include <atomic>
                        /*
                         *  Ugur Buyukdurak
                         */
namespace cs540 {

	struct Proxy {
		virtual ~Proxy() {};
	};

	template <typename U>
	struct ProxyDerived : public Proxy {

		ProxyDerived(U * pointer) :ptr(pointer) {}

		U * ptr;

		~ProxyDerived() {
			delete ptr;
		}
	};

	template<typename T>
	class SharedPtr {

	public:

		template <typename X> friend class SharedPtr;

		SharedPtr<T>();

		template <typename U> explicit SharedPtr<T>(U *);

		SharedPtr<T>(const SharedPtr<T> &);
		template <typename U> SharedPtr<T>(const SharedPtr<U> &);

		SharedPtr<T>(SharedPtr &&p);
		template <typename U> SharedPtr<T>(SharedPtr<U> &&p);

		SharedPtr<T> & operator=(const SharedPtr<T> &);
		template <typename U> SharedPtr<T> & operator=(const SharedPtr<U> &);

		SharedPtr<T> &operator=(SharedPtr &&p);
		template <typename U> SharedPtr<T> & operator=(SharedPtr<U> &&p);

		~SharedPtr();

		/*Modifiers*/

		void reset();

		template <typename U> void reset(U * p);

		/* Observers */

		T *get() const;
		T &operator*() const;
		T * operator -> () const;
		explicit operator bool() const;

	private:

		T * ptr;
		Proxy * proxyPtr;
		std::atomic_size_t * count;

		//This private constructor is used inside cast operators.
		SharedPtr(T *, Proxy *, std::atomic_size_t *);

		template <typename T1, typename T2>
		friend SharedPtr<T1> static_pointer_cast(const SharedPtr<T2> &) noexcept;

		template <typename T1, typename T2>
		friend SharedPtr<T1> dynamic_pointer_cast(const SharedPtr<T2> &) noexcept;

	};

	/*   Member function definitions  */
	template<typename T>
	SharedPtr<T>::SharedPtr() : ptr(nullptr), proxyPtr(nullptr), count(nullptr) {}

	template<typename T>
	SharedPtr<T>::SharedPtr(T * p, Proxy * proxyp, std::atomic_size_t * cp) {
		this->ptr = p;
		this->proxyPtr = proxyp;
		this->count = cp;
		
		if(count != nullptr)
			++*count;
	}

	template<typename T>
	template<typename U>
	SharedPtr<T>::SharedPtr(U * obj) : ptr(obj), proxyPtr(new ProxyDerived<U>(obj)), count(new std::atomic_size_t(1)) {}

	template<typename T>
	SharedPtr<T>::SharedPtr(const SharedPtr<T> & obj) {
		ptr = obj.ptr;
		count = obj.count;
		proxyPtr = obj.proxyPtr;
		
		if(count != nullptr)
			++*count;
	}

	template<typename T>
	template<typename U>
	SharedPtr<T>::SharedPtr(const SharedPtr<U> & obj) {
		ptr = obj.ptr;
		count = obj.count;
		proxyPtr = obj.proxyPtr;
		
		if(count != nullptr)
			++*count;
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(SharedPtr && other) {
		ptr = other.ptr;
		count = other.count;
		proxyPtr = other.proxyPtr;

		other.ptr = nullptr;
		other.count = nullptr;
		other.proxyPtr = nullptr;
	}

	template <typename T>
	template <typename U>
	SharedPtr<T>::SharedPtr(SharedPtr<U> && other) {
		ptr = other.ptr;
		count = other.count;
		proxyPtr = other.proxyPtr;

		other.ptr = nullptr;
		other.count = nullptr;
		other.proxyPtr = nullptr;
	}

	//Copy Assignment Operator
	template<typename T>
	SharedPtr<T> & SharedPtr<T>::operator=(const SharedPtr<T> & other) {
		//Shoud I check if count exists and not set to NULL 
		if (other.count != nullptr)
			++(*other.count);
		if (count != nullptr) {
			if (--*count == 0) {
				delete proxyPtr;
				delete count;
				count = nullptr;
			}
		}

		ptr = other.ptr;
		proxyPtr = other.proxyPtr;
		count = other.count;

		return *this;
	}

	template <typename T>
	template <typename U>
	SharedPtr<T> & SharedPtr<T>::operator=(const SharedPtr<U> & other) {
		//Shoud I check if count exists and not set to NULL 
		
		if (other.count != nullptr)
			++(*other.count);
		if (count != nullptr) {
			if (--*count == 0) {
				//proxyPtr->del();
				delete proxyPtr;
				delete count;
				count = nullptr;
			}
		}

		ptr = other.ptr;
		proxyPtr = other.proxyPtr;
		count = other.count;

		return *this;
	}

	template<typename T>
	SharedPtr<T> & SharedPtr<T>::operator=(SharedPtr<T> && other) {
		//Shoud I check if count exists and not set to NULL 
		if (count != nullptr) {
			if (--*count == 0) {
				//proxyPtr->del();
				delete proxyPtr;
				delete count;
				//delete proxyPtr;
				count = nullptr;
			}
		}
		
		ptr = other.ptr;
		proxyPtr = other.proxyPtr;
		count = other.count;

		other.ptr = nullptr;
		other.count = nullptr;
		other.proxyPtr = nullptr;

		return *this;
	}

	template <typename T>
	template <typename U>
	SharedPtr<T> & SharedPtr<T>::operator=(SharedPtr<U> && other) {

		//Shoud I check if count exists and not set to NULL 
		if (count != nullptr) {
			if (--*count == 0) {
				//proxyPtr->del();
				delete proxyPtr;
				delete count;
				count = nullptr;
			}
		}

		ptr = other.ptr;
		proxyPtr = other.proxyPtr;
		count = other.count;

		other.ptr = nullptr;
		other.count = nullptr;
		other.proxyPtr = nullptr;

		return *this;
	}

	template<typename T>
	SharedPtr<T>::~SharedPtr() {
		if (count != nullptr) {
			if (--*count == 0) {
				delete proxyPtr;
				delete count;
			}
		}
	}

	//Modifiers

	template<typename T>
	void SharedPtr<T>::reset() {
		if (count != nullptr) {
			if (--*count == 0) {
				//proxyPtr->del();
				delete proxyPtr;
				delete count;
			}
		}
		count = nullptr;
		ptr = nullptr;
		proxyPtr = nullptr;
	}

	//Replace owned source with another pointer.
	template <typename T>
	template <typename U>
	void SharedPtr<T>::reset(U * pointer) {
		if (count != nullptr) {
			if (--*count == 0) {
				delete proxyPtr;
				delete count;
				count = nullptr;
			}
		}
		count = new std::atomic_size_t(1);
		ptr = pointer;
		proxyPtr = new ProxyDerived<U>(pointer);
	}

	//Observers
	template <typename T>
	T * SharedPtr<T>::get() const {
		return ptr;
	}

	template <typename T>
	T & SharedPtr<T>::operator*() const {
		return *ptr;
	}

	template<typename T>
	T * SharedPtr<T>::operator -> () const {
		return ptr;
	}

	template<typename T>
	SharedPtr<T>::operator bool() const {
		return ptr != nullptr;
	}

	/*Non-member (Free) Functions*/

	template<typename T1, typename T2>
	bool operator==(const SharedPtr<T1> & objOne, const SharedPtr<T2> & objTwo) {
		return objOne.get() == objTwo.get();
	}

	template<typename T>
	bool operator==(const SharedPtr<T> & objOne, std::nullptr_t) {
		return objOne.get() == nullptr;
	}

	template<typename T>
	bool operator==(std::nullptr_t, const SharedPtr<T> & objOne) {
		return nullptr == objOne.get();
	}

	template<typename T1, typename T2>
	bool operator!=(const SharedPtr<T1> & objOne, const SharedPtr<T2> & objTwo) {
		return objOne.get() != objTwo.get();
	}

	template<typename T>
	bool operator !=(const SharedPtr<T> & objOne, std::nullptr_t) {
		return objOne.get() != nullptr;
	}

	template<typename T>
	bool operator !=(std::nullptr_t, const SharedPtr<T> & objOne) {
		return objOne.get() != nullptr;
	}

	template<typename T, typename U>
	SharedPtr<T> static_pointer_cast(const SharedPtr<U> & sp) noexcept {
		auto tempPtr = static_cast<T*>(sp.get());
		return SharedPtr<T>(tempPtr, sp.proxyPtr, sp.count);
	}

	template <typename T, typename U>
	SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp) noexcept {
		T * object = dynamic_cast<T *>(sp.get());
		if (object) {
			return SharedPtr<T>(object, sp.proxyPtr, sp.count);
		}
		else {
			return SharedPtr<T>();
		}
	}

}
#endif
