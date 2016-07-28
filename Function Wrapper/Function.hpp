#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <iostream>
#include <cstddef>
                    /* 
                     *      UGUR BUYUKDURAK 
                     */
namespace cs540 {

	struct BadFunctionCall {};

	template <typename T>
	class Function;

	//Template Specilization.
	template <typename Ret, typename ... Args>
	class Function <Ret(Args...)> {

	public:

		struct BaseWrapper {
			virtual Ret operator()(Args...) = 0;
			virtual BaseWrapper *  clone() = 0;
			virtual ~BaseWrapper() {}
		};

		template <typename T>
		struct Wrapper: public BaseWrapper {

			Wrapper(T parameter) : object(parameter) {
			}

			Ret operator()(Args... args) override {
				return object(args...);
			}

			Wrapper * clone() override {
				return new Wrapper(*this);	
			}

			~Wrapper() {}

			T object;
		};

	public:

		Function();
		template <typename FunctionType> Function(FunctionType);
		Function(const Function &);
		Function &operator=(const Function &);
		~Function();
		Ret operator()(Args...);
		explicit operator bool() const;

	private:
		BaseWrapper * wrapper;

		template <typename ResultType, typename ... ArgumentTypes>
		friend bool operator==(const Function<ResultType(ArgumentTypes...)> & f, std::nullptr_t);

		template <typename ResultType, typename ... ArgumentTypes>
		friend bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> & f);

		template <typename ResultType, typename ... ArgumentTypes>
		friend bool operator!=(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t);

		template <typename ResultType, typename ... ArgumentTypes>
		friend bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> & f);
	};

	template <typename Ret, typename ... Args>
	Function <Ret(Args...)>::Function() : wrapper(nullptr) {}
	
	template <typename Ret, typename ... Args>
	template <typename FunctionType>
	Function <Ret(Args...)>::Function(FunctionType func){
		wrapper = new Wrapper<FunctionType>(func);
	}
	
	template <typename Ret, typename ... Args>
	Function <Ret(Args...)>::Function(const Function & otherFunc) {
		wrapper = otherFunc.wrapper->clone();
	}

	template <typename Ret, typename ... Args>
	Function<Ret(Args...)> & Function <Ret(Args...)>::operator=(const Function & otherFunc) {
		delete wrapper;
		wrapper = otherFunc.wrapper->clone();
		return *this;
	}

	template <typename Ret, typename ... Args>
	Function <Ret(Args...)>::~Function() {
		delete wrapper;
	}

	template <typename Ret, typename ... Args>
	Ret Function<Ret(Args...)>::operator()(Args ... args) {
		if(wrapper != nullptr)
			return (*wrapper)(args...);
		throw BadFunctionCall();	
	}

	template <typename Ret, typename ... Args>
	Function <Ret(Args...)>::operator bool() const {
		if (wrapper == nullptr)
			return false;
		return true;
	}

	template <typename ResultType, typename ... ArgumentTypes>
	bool operator==(const Function<ResultType(ArgumentTypes...)> & f, std::nullptr_t) {
		return f.wrapper == nullptr;
	}

	template <typename ResultType, typename ... ArgumentTypes>
	bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> & f) {
		return f.wrapper == nullptr;
	}

	template <typename ResultType, typename ... ArgumentTypes>
	bool operator!=(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t) {
		return f.wrapper != nullptr;
	}

	template <typename ResultType, typename ... ArgumentTypes>
	bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> & f) {
		return f.wrapper != nullptr;
	}
}
#endif
