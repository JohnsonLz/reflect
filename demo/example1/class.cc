#include <iostream>
#include "class.h"
#include "context/reflect.h"
#include "context/alloc.h"
#include "context/container.h"

using namespace reflection;
void Point::say() {

	std::cout<<"Point\n"<<std::endl;
}
REGIST(Point)

void Line::say() {

	std::cout<<"Line\n"<<std::endl;
}
REGIST(Line)

void Init::declation(const char* arg) {

	std::cout<<"demo"<<std::endl;
	std::cout<<"Argument: "<<arg<<std::endl;
}
REGIST(Init)
