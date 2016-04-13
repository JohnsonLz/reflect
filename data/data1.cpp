// #include <iostream>
// #include "header.h"
// using namespace std;

// void test() {

// 	cout<<"global test"<<endl;
// }

// void A::run() {

// 	cout<<"A"<<endl;
// }

// void A::A_st() {
// 	cout<<"A static"<<endl;
// }

class A {

public:
	void run();
	static void sf();
};

void A::run(){}
void A::sf(){}

int main() {

}