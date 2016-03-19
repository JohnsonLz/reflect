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

#ifndef REFLECTION_CONTEXT_SCOPE_H_
#define REFLECTION_CONTEXT_SCOPE_H_

#include <functional>

namespace reflection {

class Scope {

private:
    bool dismiss_;
    std::function<void()> exitScope_;

public:
    explicit Scope(std::function<void()> func):exitScope_(func), dismiss_(false) {}
    ~Scope() {
        if(!dismiss_) {
            exitScope_();
        }
    }

    void Dismiss() { 
        dismiss_ = true;
    }

private:
    Scope(const Scope&) {}
    void operator==(Scope&){}

};


#define SCOPE_LINENAME_CAT(name, line) name##line
#define SCOPE_LINENAME(name, line) SCOPE_LINENAME_CAT(name, line)
#define ON_SCOPE_EXIT(callback) Scope SCOPE_LINENAME(EXIT, __LINE__)(callback);


} // namespace reflection

#endif