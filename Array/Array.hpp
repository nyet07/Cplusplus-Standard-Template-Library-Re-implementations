#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>
#include <cstring>
#include <type_traits>
#include <cstdlib>
				/*		Ugur Buyukdurak			*/
using std::cout;
using std::endl;

namespace cs540{

	//Exception.
	struct OutOfRange{};

	template <typename T>
	constexpr T product(const T &v) {
		return v;
	}

	template <typename T, typename... Ts>
	constexpr T product(const T &v, const Ts &... params) {
		return v * product(params...);
	}

	//Helper_Class

	template <typename T, size_t N>
	struct IndexHelper {
		IndexHelper(size_t sum, const T *arr, const size_t * dimenArr, const int arrIndex, const size_t arrSize, const size_t arraySize) : sum(sum), pointer(arr), dimensions(dimenArr), 
		starting_index(arrIndex), arrSize(arrSize), arraySize(arraySize){ }
		
		IndexHelper <T, N - 1> operator [] (size_t index){
			if (index >= dimensions[starting_index - 1])
				throw OutOfRange();
			
			for (unsigned int i = starting_index; i < arrSize; ++i)
				index *= dimensions[i];
			
			return IndexHelper<T, N - 1>(index + sum, pointer, dimensions, starting_index + 1, arrSize, arraySize);
		}

		const IndexHelper <T, N - 1> operator [] (size_t index) const{
			if (index >= dimensions[starting_index - 1])
				throw OutOfRange();
			
			for (unsigned int i = starting_index; i < arrSize; ++i)
				index *= dimensions[i];
			return IndexHelper<T, N - 1>(index + sum, pointer, dimensions, starting_index + 1, arrSize, arraySize);
		}

		size_t sum; //Sum coming from upper calculation.
		const T * pointer; //Actual Array
		const size_t * dimensions; //Dimension Array holding dimensions. 
		const int starting_index; // Index to start calculating.
		const size_t arrSize; //How many dimensions.
		const size_t arraySize; //Actual Array size.
	};

	//Helper_Class_Specilization

	template <typename T>
	struct IndexHelper<T, 0> {
		IndexHelper(size_t sum, const T *arr, const size_t * dimenArr, const int arrIndex, const size_t arrSize, const size_t arraySize) : sum(sum), pointer(arr), dimensions(dimenArr),
		starting_index(arrIndex), arrSize(arrSize), arraySize(arraySize){ }
		
		T & operator [] (size_t index){

			if (index >= dimensions[starting_index - 1])
				throw OutOfRange();

			const size_t i = sum + index;
			if (i >= arraySize){
				throw OutOfRange();
			}
			T * tempArr = const_cast<T *>(pointer);
			return tempArr[i];
		}

		const T & operator [] (size_t index) const{

			if (index >= dimensions[starting_index - 1])
				throw OutOfRange();

			const size_t i = sum + index;
			if (i >= arraySize){
				throw OutOfRange();
			}
			return pointer[i];
		}

		size_t sum;
		const T * pointer;
		const size_t * dimensions;
		const int starting_index;
		const size_t arrSize;
		const size_t arraySize;
	};


	template <typename T, size_t... Dims> 
	class Array {
	
	public:

		using ValueType = T;

		struct FirstDimensionMajorIterator{	
			FirstDimensionMajorIterator(){
				arr = nullptr;
			}
			FirstDimensionMajorIterator(T * ptr){
				this->arr = ptr;
			}

			bool operator==(const FirstDimensionMajorIterator & other){
				return arr == other.arr;
			}
			
			bool operator!=(const FirstDimensionMajorIterator & other){
				return arr != other.arr;
			}
			
			FirstDimensionMajorIterator &operator++(){
				++arr;
				return *this;
			}
			
			FirstDimensionMajorIterator operator++(int){
				//Post increment
				FirstDimensionMajorIterator temp(arr);
				++arr;
				return temp;
			}
			
			T &operator*() const{
				return *arr;
			}

			size_t size = product(Dims...);
			T * arr;
		};
		
		struct LastDimensionMajorIterator{

			LastDimensionMajorIterator(){
				Arr = nullptr;
				tmp = nullptr;
				flag = false;
				index = 0;
			}

			LastDimensionMajorIterator(T * ptr, T * ptr2): Arr(ptr), tmp(ptr2), flag(false), index(0) {}
			
			bool operator==(const LastDimensionMajorIterator & other){
				return tmp == other.tmp;
			}

			bool operator!=(const LastDimensionMajorIterator & other){
				return tmp != other.tmp;
			}

			LastDimensionMajorIterator &operator++(){
				SetRealIndex();
				tmp = this->index + Arr;

				if (flag){
					tmp = Arr - 1;
				}

				return *this;
			}

			LastDimensionMajorIterator operator++(int){
				LastDimensionMajorIterator temp(*this);
				SetRealIndex();
				tmp = this->index + Arr;

				if (flag){
					tmp = Arr - 1;
				}

				return temp;
			}

			T & operator*() const{
				return Arr[this->index];
			}

		private:

			//End situation is dimensions[dimensionSize - 1] --> return the right-most index. 
			//size_t lastDimension = dimensions[dimensionSize - 1];
			//indexes[dimensionSize - 1] => dimensions[dimensionSize - 1] then return a indicator value.

			void SetRealIndex(){

				modifyIndexes();  //Modify Indexes to point to the next element

				size_t real_index = 0;
				size_t product = 0;

				for (unsigned int k = 1, i2 = 0; k < dimensionSize; ++k, ++i2){
					product = dimensions[k] * indexes[i2];
					for (unsigned int i = k + 1; i < dimensionSize; ++i)
						product *= dimensions[i] ; //Dimensions cannot be zero, only indexes can be zeros.
					real_index += product;
				}

				this->index = real_index + indexes[dimensionSize - 1];
			}

			//Modify Indexes to point to the next element
			void modifyIndexes(){
				for (unsigned int i = 0; i < dimensionSize; ++i){
					if (indexes[i] < dimensions[i] - 1){
						++indexes[i]; break;
					}
					else{
						indexes[i] = 0;
					}
				}
				
				if (checkIfAllZero()){
					flag = true;
				}

			}

			bool checkIfAllZero(){
				for (auto x : indexes){
					if (x != 0)
						return false;
				}
				return true;
			}

			T * Arr;
			T * tmp;
			bool flag;
			size_t index;
			size_t dimensionSize = sizeof...(Dims);
			size_t dimensions[sizeof...(Dims)] = { Dims... };
			size_t indexes[sizeof...(Dims)] = { 0 };  //Keep track of indexes here. x = [0], y = [1], z = [2];
		};
	
	public:
		Array();
		
		template <typename U> Array(const Array<U, Dims...> &);
		
		Array &operator=(const Array &);
		template <typename U> Array &operator=(const Array<U, Dims...> &);

		inline IndexHelper<T, sizeof...(Dims) - 2> operator [] (size_t i);
		inline const IndexHelper<T, sizeof...(Dims)-2> operator [] (size_t i) const;

		FirstDimensionMajorIterator fmbegin();
		FirstDimensionMajorIterator fmend();

		LastDimensionMajorIterator lmbegin();
		LastDimensionMajorIterator lmend();

	private:
		//Calculate size at compile time with product function.		
		size_t size = product(Dims...);							//Array size
		size_t dimSize = sizeof...(Dims);						//How many dimensions
		size_t dimensionArr[sizeof...(Dims)] = { Dims... };		//Limits of dimensions.
		T Arr[product(Dims...)];								//Actual linear array.

		template <typename U, size_t ... Args>
		friend class Array;
	};

	template <typename T, size_t... Dims>  
	Array<T, Dims...>::Array(){}

	template <typename T, size_t... Dims>
	template <typename U>
	Array<T, Dims...>::Array(const Array <U, Dims...> & other){
		
		for (size_t i = 0; i < size; ++i){
			Arr[i] = other.Arr[i];
		}
		
	}

	template <typename T, size_t... Dims>
	Array<T, Dims...> & Array<T, Dims...>::operator=(const Array & other) {
		if (this != &other) {
			for (size_t i = 0; i < size; ++i) {
				Arr[i] = other.Arr[i];
			}
		}

		return *this;
	}

	template <typename T, size_t... Dims>
	template <typename U>
	Array<T, Dims...> & Array<T, Dims...>::operator=(const Array<U, Dims...> & other){
		for (size_t i = 0; i < size; ++i){
			Arr[i] = other.Arr[i];
		}

		return *this;
	}

	template <typename T, size_t... Dims>
	inline IndexHelper<T, sizeof...(Dims) - 2> Array<T, Dims...>::operator [] (size_t index){
		//Calculate first value here. 
		int starting_index = 1;
		for (unsigned int i = starting_index; i < dimSize; ++i)
			index *= dimensionArr[i];
		//Passing uncompleted index, data array, array holding dimension information, starting index, dimension array
		return IndexHelper<T, sizeof...(Dims) - 2>(index, Arr, dimensionArr, starting_index + 1, dimSize, size);
	}

	template <typename T, size_t... Dims>
	inline const IndexHelper<T, sizeof...(Dims)-2> Array<T, Dims...>::operator [] (size_t index) const{
		//Calculate first value here. 
		int starting_index = 1;
		for (unsigned int i = starting_index; i < dimSize; ++i)
			index *= dimensionArr[i];
		//Passing uncompleted index, data array, array holding dimension information, starting index, dimension array
		return IndexHelper<T, sizeof...(Dims) - 2>(index, Arr, dimensionArr, starting_index + 1, dimSize, size);
	}

	template <typename T, size_t... Dims>
	typename Array<T, Dims...>::FirstDimensionMajorIterator Array<T, Dims...>::fmbegin(){
		return FirstDimensionMajorIterator(this->Arr);
	}
	
	template <typename T, size_t... Dims>
	typename Array<T, Dims...>::FirstDimensionMajorIterator Array<T, Dims...>::fmend(){
		size_t tempSize = this->size - 1;
		T * obj = &(this->Arr[tempSize]); //Get address of last element. 
		return FirstDimensionMajorIterator(++obj);
	}
	
	template <typename T, size_t... Dims>
	typename Array<T, Dims...>::LastDimensionMajorIterator Array<T, Dims...>::lmbegin(){
		return LastDimensionMajorIterator(this->Arr, this->Arr);
	}
	
	template <typename T, size_t... Dims>
	typename Array<T, Dims...>::LastDimensionMajorIterator Array<T, Dims...>::lmend(){
		return LastDimensionMajorIterator(this->Arr, this->Arr - 1);
	}


	template <typename T, size_t N>
	class Array<T, N> {

	public:

		using ValueType = T;

		struct FirstDimensionMajorIterator{
			FirstDimensionMajorIterator(){
				ptr = nullptr;
			}
			FirstDimensionMajorIterator(T * ptr){
				this->ptr = ptr;
			}
			
			bool operator==(const FirstDimensionMajorIterator & other){
				return ptr == other.ptr;
			}

			bool operator!=(const FirstDimensionMajorIterator & other){
				return ptr != other.ptr;
			}

			FirstDimensionMajorIterator &operator++(){
				++ptr;
				return *this;
			}

			FirstDimensionMajorIterator operator++(int){
				FirstDimensionMajorIterator tmp(ptr);
				++ptr;
				return tmp;
			}

			T &operator*() const{
				return *ptr;
			}

			T * ptr;
		};

		struct LastDimensionMajorIterator{

			LastDimensionMajorIterator(){
				ptr = nullptr;
			}

			LastDimensionMajorIterator(T * ptr){
				this->ptr = ptr;
			}

			bool operator==(const LastDimensionMajorIterator & other){
				return ptr == other.ptr;
			}

			bool operator!=(const LastDimensionMajorIterator & other){
				return ptr != other.ptr;
			}

			LastDimensionMajorIterator &operator++(){
				++ptr;
				return *this;
			}

			LastDimensionMajorIterator operator++(int){
				LastDimensionMajorIterator tmp(ptr);
				++ptr;
				return tmp;
			}

			T &operator*() const{
				return *ptr;
			}

			T * ptr;
		};

	public:
		Array();

		template <typename U> Array(const Array<U, N> &);

		Array &operator=(const Array &);
		template <typename U> Array &operator=(const Array<U, N> &);

		inline T & operator [] (size_t i) {
			if (i >= size)
				throw OutOfRange();
			return Arr[i];
		}

		inline const T & operator [] (size_t i) const {
			if (i >= size)
				throw OutOfRange();
			return Arr[i];
		}

		FirstDimensionMajorIterator fmbegin();
		FirstDimensionMajorIterator fmend();

		LastDimensionMajorIterator lmbegin();
		LastDimensionMajorIterator lmend();

	private:
		size_t size = N;
		T Arr[N];  

		template <typename, size_t ...>
		friend class Array;
	};

	template <typename T, size_t N>
	Array<T, N>::Array(){
		//Empty.
	}

	template <typename T, size_t N>
	template <typename U>
	Array<T, N>::Array(const Array <U, N> & other){
		for (size_t i = 0; i < size; ++i){
			Arr[i] = other.Arr[i];
		}
	}

	template <typename T, size_t N>
	Array<T, N> & Array<T, N>::operator=(const Array & other) {
		if (this != &other) {
			for (size_t i = 0; i < size; ++i) {
				Arr[i] = other.Arr[i];
			}
		}

		return *this;
	}
	
	template <typename T, size_t N>
	template <typename U>
	Array<T, N> & Array<T, N>::operator=(const Array<U, N> & other){
		for (size_t i = 0; i < size; ++i){
			Arr[i] = other.Arr[i];
		}
		
		return *this;
	}

	template <typename T, size_t N>
	typename Array<T, N>::FirstDimensionMajorIterator Array<T, N>::fmbegin(){
		return FirstDimensionMajorIterator(this->Arr);
	}

	template <typename T, size_t N>
	typename Array<T, N>::FirstDimensionMajorIterator Array<T, N>::fmend(){
		size_t tempSize = this->size - 1;
		T * obj = &(this->Arr[tempSize]); //Get address of last element. 
		return FirstDimensionMajorIterator(++obj);
	}

	template <typename T, size_t N>
	typename Array<T, N>::LastDimensionMajorIterator Array<T, N>::lmbegin(){
		return LastDimensionMajorIterator(this->Arr);
	}

	template <typename T, size_t N>
	typename Array<T, N>::LastDimensionMajorIterator Array<T, N>::lmend(){
		size_t tempSize = this->size - 1;
		T * obj = &(this->Arr[tempSize]); //Get address of last element. 
		return LastDimensionMajorIterator(++obj);
	}

}

#endif
