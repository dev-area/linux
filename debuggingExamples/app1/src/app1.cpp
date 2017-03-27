//============================================================================
// Name        : app1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

template<typename T>
T add(T a,T b)
{
	return a+b;
}

int main() {
	cout << "!!!Hello World!!!" << add(10,20) << endl; // prints !!!Hello World!!!

	return 0;
}
