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

#ifndef REFLECTION_CONTEXT_INFO_H_
#define REFLECTION_CONTEXT_INFO_H_

namespace reflection {

class Info {

public:
    Info(const char* name, const char* scale, unsigned long long pc):Name(name), Scale(scale), PC(pc){};

    bool operator==(const Info& item);
    bool operator > (const Info& item);

    const char* Name;
    const char* Scale;
    unsigned long long PC;

};

}// namespace reflection

#endif
