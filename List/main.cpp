#include <iostream>
#include "list.h"

using std::cout;
using std::endl;
using std::ostream;

struct point {
	int x;
	int y;
};

ostream & operator << (ostream & out, const point & x) {
	out << "x: " << x.x << " " << "y: " << x.y << endl;
	return out;
}

int main() {

	point x = { 2,3 };
	point y = { 10,11 };
	point z = { 100, 150 };

	//
	List<point> three_d{ x, y, z };
	
	//Constructor with default parameter is called
	List<int>obj_1;

	//Constructor is given a value
	List<int>obj_2(100);

	//List is provided with a initialization list
	List<int>obj_3{ 1,2,3,4,5 };

	//Same as before
	List<int>obj_4 = { 1,2,3,4,5,6,7,8,9 };

	//Copy Constructor is called
	List<int>obj_5 = obj_4;

	//Copy Assignment operator is called
	obj_1 = obj_5;

	//Move Constructor is called
	List<int>obj_6 = List<int>{ 1,2,3,4,5,6,7,8,9,10 };

	//Move Assignment Operator is called
	obj_4 = List<int>{12,12,12,12,12};

	//Iterator dereference test
	for (auto iter = obj_4.begin(); iter != obj_4.end(); ++iter) {
		std::cout << *iter << std::endl;
	}

	//Iterator arrow operator test
	for (auto iter = three_d.begin(); iter != three_d.end(); ++iter) {
		std::cout << iter->x << std::endl;
	}

	for (auto i = 0; i < 1000; ++i)
		obj_4.append(i);
	obj_4.insert(0, 123456);

	std::cout << obj_4 << std::endl;

	List<int> other {12, 12, 12, 13, 15};
	obj_4 = other;
	std::cout << "obj4 : " << obj_4 << std::endl;

	while (1);
	exit(0);
}