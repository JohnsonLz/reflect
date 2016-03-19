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

#ifndef REFLECTION_CONTEXT_CONTEXT_H_
#define REFLECTION_CONTEXT_CONTEXT_H_

#include <map>
#include <vector>

#include "context/list.h"
#include "context/container.h"

namespace reflection {

class Info;

class Context {

private:

    List<Info> map_;
    std::map<const char*, Container*> classMap_;
    bool earse_;

    void loadContextFile_(const char* fileName);
    const char* copyStr_(const char* str);

    Context();
    Context(const Context&){}
    void operator =(const Context&){}

public:

    ~Context();

    static Context& sharedContext() {
        static Context c;
        return c;
    }

    void releaseResource();

    void setContextEnv(const char* fileName);
  //  void configure(const char* fileName);
    void registClass(const char* name, Container* cont);

    // return void pointer pointed to the class
    // client can transform type 
    // pointer pointed to the heap memory
    // client must release the pointer
    void* instanceOfClass(const char* className);
    void invoke(const char* name, const char* scole, const std::vector<const char*>& args);

};

}// namespace reflection

#endif