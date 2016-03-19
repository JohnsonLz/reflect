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

#ifndef REFECTION_CONTEXT_ERRNO_H_
#define REFECTION_CONTEXT_ERRNO_H_

#include <iostream>

namespace reflection {


class __ENSURE {

public:
    ~__ENSURE() {}

    __ENSURE&  __PRINT_EXPR(const char* expr) {
        std::cout<<"Assert failed: "<<expr<<std::endl;
        std::cout<<"File: "<<__FILE__<<" Line: "<<__LINE__<<std::endl;
        std::cout<<"Context variables:"<<std::endl;
        return *this;
    }

    template<typename Type>
    __ENSURE& __PRINT_CONTEXT(const char* context, Type item) {
        std::cout<<"    "<<context<<": "<<item<<std::endl;
        return * this;
    }

    static __ENSURE& __GEN_ENSURE();

public:
    char __CONTEXT_A_;
    char __CONTEXT_B_;


private:
    explicit __ENSURE(){}
    __ENSURE(const __ENSURE&){}
    void operator =(__ENSURE&){}
};

__ENSURE& __ENSURE::__GEN_ENSURE() {

    static __ENSURE E;
    return E;
}


#define ENSURE(expr)  \
    if(expr){}  \
    else __ENSURE::__GEN_ENSURE().__PRINT_EXPR(#expr).__CONTEXT_A_  \


#define __CONTEXT_A_(context) __CONTEXT_PRINT_(context, B)
#define __CONTEXT_B_(context) __CONTEXT_PRINT_(context, A)
#define __CONTEXT_PRINT_(context, next) \
        __PRINT_CONTEXT(#context, context).__CONTEXT_##next##_  \


} //namespace reflection

#endif