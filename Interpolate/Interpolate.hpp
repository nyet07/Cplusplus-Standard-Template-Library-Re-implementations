#ifndef INTERPOLATE_HPP
#define INTERPOLATE_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <tuple>
#include <iterator>
#include <type_traits>
#include <typeinfo>
#include <functional>

using std::ostream;
using std::cout;
using std::endl;
using std::string;
using std::tuple;
using std::get;
using std::declval;

namespace cs540{

	struct WrongNumberOfArgs {

	};   //Declared here for exception purposes.
		
	template <typename ... ArgsTypes>
	class InterpolateHelper{
	public:
		InterpolateHelper(const string & str, ArgsTypes ... params):originalStr(str), Arguments(params...)
		{

		}

	private:
		const string originalStr;
		static const size_t size = sizeof...(ArgsTypes);
		tuple<ArgsTypes...> Arguments;

		template<size_t index, size_t limit>
		struct Loop{
			
			template<typename T>
			static void output(ostream & os, const string::const_iterator & iteratorBegin, const string::const_iterator & iteratorEnd, const T & value, const tuple<ArgsTypes...> & tup)     //const tuple<ArgsTypes...> & tup)
			{
				auto iter = iteratorBegin;
				while (iter != iteratorEnd)
				{
					if (iter != iteratorEnd) {
						if (*iter == '%') {
							os << value;
							Loop<index + 1, limit>::output(os, ++iter, iteratorEnd, get<index + 1>(tup), tup);
							break;
						}

						else if (*iter == '\\') {
							if (*(iter + 1) == '%') {
								os << '%';
								++iter;
							}
						}
						else {
							//If manipulator then add it here.
							os << *iter;
						}
						++iter;
					}
				}
			}

			//All test functions have same return value with boolalpha.
			static void output(ostream & os, 
				const string::const_iterator & iteratorBegin, 
				const string::const_iterator & iteratorEnd, 
				decltype(std::boolalpha) & value, 
				const tuple<ArgsTypes...> & tup)
			{
				os << value;
				Loop<index + 1, limit>::output(os, iteratorBegin, iteratorEnd, get<index + 1>(tup), tup);
			}

			static void output(ostream & os,
				const string::const_iterator & iteratorBegin,
				const string::const_iterator & iteratorEnd,
				decltype(std::setprecision(5)) value,
				const tuple<ArgsTypes...> & tup)
			{
				os << value;
				Loop<index + 1, limit>::output(os, iteratorBegin, iteratorEnd, get<index + 1>(tup), tup);
			}

			static void output(ostream & os,
				const string::const_iterator & iteratorBegin,
				const string::const_iterator & iteratorEnd,
				std::function<ostream&(ostream&)> value,
				const tuple<ArgsTypes...> & tup)
			{
				value(os);
				auto iter = iteratorBegin;
				Loop<index + 1, limit>::output(os, ++iter , iteratorEnd, get<index + 1>(tup), tup);
			}

		};

		

		template <size_t limit>
		struct Loop<limit, limit> {
			template <typename T>
			static void output(ostream & os, const string::const_iterator & iteratorBegin, const string::const_iterator & iteratorEnd, const T & value, const tuple<ArgsTypes...> &)
			{
				auto iter = iteratorBegin;
				bool flag = false;
				if (std::distance(iter, iteratorEnd) == 0)
					throw WrongNumberOfArgs();
				
				while (iter != iteratorEnd)
				{
					if (iter != iteratorEnd) {
						if (*iter == '%') {
							os << value;
							if (flag)
								throw WrongNumberOfArgs();
							flag = true;
						}

						else if (*iter == '\\') {
							if (std::distance(iter, iteratorEnd) > 1){
								if (*(iter + 1) == '%') {
									os << '%';
									++iter;
								}
							}
							else {
								os << '\\';
							}
						}
						else {
							os << *iter;
						}
						++iter;
					}
				}
			}

			static void output(ostream & os,
				const string::const_iterator & ,
				const string::const_iterator & ,
				decltype(std::boolalpha) & value,
				const tuple<ArgsTypes...> & )
			{
				os << value;
			}

			static void output(ostream & os,
				const string::const_iterator & ,
				const string::const_iterator & ,
				std::function<ostream&(ostream&)> value,
				const tuple<ArgsTypes...> &)
			{
				value(os);
			}


		};

		template<typename ... Params>
		friend ostream& operator<<(ostream&, const InterpolateHelper<Params...> &);
	};


	template <>
	class InterpolateHelper<> {
	public:
		InterpolateHelper(const string & str) :originalStr(str)
		{

		}

	private:

		//Artik parametresiz verileri geciyor.
		template<size_t N>
		struct Loop {
			static void output(ostream & os, const string & str) {
				if (str.empty()) 
				{
					os << str;
					return;
				}
				for (auto iter = str.begin(); iter != str.end(); ++iter) 
				{
					if (*iter == '\\') 
					{
						if (std::distance(iter, str.end()) > 1) 
						{
							if (*(iter + 1) == '%') {
								os << '%';
								++iter;
							}
							else {
								os << '\\';
							}
						}
						else 
						{
							os << '\\';
						}
					}
					else {
						os << *iter;
					}
				}
			}
		};

		const string originalStr;

		template<typename ... Params>
		friend ostream& operator<<(ostream&, const InterpolateHelper<Params...> &);
	};
	
	template <typename ... Params>
	inline InterpolateHelper <const Params & ...> Interpolate(const string & str, const Params & ... args){
		return InterpolateHelper<const Params &...>(str, args...);
	}

	template <typename ... ArgsTypes>
	ostream& operator<<(ostream& os, const InterpolateHelper<ArgsTypes...> & obj){
		InterpolateHelper<ArgsTypes...>::template Loop<0, sizeof...(ArgsTypes) - 1>::output(os, obj.originalStr.begin(), obj.originalStr.end(), get<0>(obj.Arguments), obj.Arguments);
		return os;
	}

	template <>
	ostream& operator<<(ostream& os, const InterpolateHelper<> & obj) {
		InterpolateHelper<>::template Loop<0>::output(os, obj.originalStr);
		return os;
	}

	std::function<ostream&(ostream&)> ffr(ostream & (*func)(ostream &)) {
		return func;
	}

}

#endif


	