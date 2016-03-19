/**
    @file
    Provide xxx

    @author Youjie Xie<lz283447@gmail.com>

    Copyright &copy; 2016.
    This software is distributed in the hope that it will be useful, but
    without WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Permission is granted to anyone to make or distribute verbatim
    copies of this software provided that the copyright notice and
    this permission notice are preserved, and that the distributor
    grants the recipient permission for further distribution as permitted
    by this notice.

    Comments and suggestions are always welcome.

*/

#ifndef REFLECTION_CONTEXT_REFLECT_H_
#define REFLECTION_CONTEXT_REFLECT_H_

#include <vector>
#include <new>

#include "context/alloc.h"
#include "context/container.h"

namespace reflection {

class Reflect {

private:
    Reflect(){}
    Reflect(const Reflect&){}
    void operator = (const Reflect&) {}

    bool regist_(const char* name, Container* cont);

public:

    void setContextEnv(const char* fileName);
    void* instanceOfClass(const char* name);

    // args are passes by pointer, which do not pointer to the head
    // memory, take care of the valid scope
    void invoke(const char* name, const char* scole, const std::vector<const char*>& args);

    static Reflect& sharedContext() {
        static Reflect f;
        return f;
    }

    template<typename ValueType>
    bool regist(const char* name, ValueType* ptr);
};

template<typename ValueType>
bool Reflect::regist(const char* name, ValueType* ptr) {

    Container* c = static_cast<Container*>(Alloc::allocate(sizeof(Container)));
    new (c) Container(ptr);
    return regist_(name, c);
}

#define REGIST_CAT(file, line) file##line
#define REGIST(Type)    \
    Type*  REGIST_CAT(Type, __LINE__);  \
    bool REGIST_CAT(__LINE__, Type) = Reflect::sharedContext().regist(#Type, REGIST_CAT(Type, __LINE__));  \

} // namespace reflection

#endif