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

#include <new>
#include <string>
#include <cstdio>

#include "context/context.h"
#include "context/reflect.h"
#include "context/container.h"
#include "context/statue.h"
#include "context/alloc.h"

namespace reflection {

void Reflect::setContextEnv(const char* fileName) {

    try {
        Context::sharedContext().setContextEnv(fileName);
    }catch(Statue s) {
        if(s == Statue::XML_ROOT_ERROR) {
            printf("Parse context.xml root error");
        }
        if(s == Statue::XML_NOT_FOUND) {
            printf("context.xml not found\n");
        }
        Context::sharedContext().releaseResource();
        exit(1);
    }
}

void Reflect::invoke(const char* name, const char* scole, const std::vector<const char*>& v) {

    try {
        Context::sharedContext().invoke(name, scole, v);
    }catch(Statue s) {
        printf("Scope %s has no function named %s\n", scole, name);
        Context::sharedContext().releaseResource();
        exit(1);
    }
}

void* Reflect::instanceOfClass(const char* name) {

    try {
        return Context::sharedContext().instanceOfClass(name);
    }catch(Statue s) {
        printf("Class %s does not existed\n", name);
        Context::sharedContext().releaseResource();
        exit(1);
    }
}

bool Reflect::regist_(const char* name, Container* cont) {

    try {
        Context::sharedContext().registClass(name, cont);
    }catch (Statue s) {
        printf("Regist collison: %s has existed\n", name);
    }

    return true;
}

}// namespace reflection
