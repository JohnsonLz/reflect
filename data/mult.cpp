#include <iostream>
#include "header.h"
using namespace std;

int main() {

	test();
	A a;
	B b;
	head* h = &a;
	h->run();
	h = &b;
	h->run();
}
