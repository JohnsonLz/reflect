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

#include <cstdio>
#include <new>
#include <string>
#include <algorithm>

#include "tinyxml.h"
#include "tinystr.h"

#include "context/context.h"
#include "context/info.h"
#include "context/errno.h"
#include "context/alloc.h"
#include "context/scope.h"
#include "context/statue.h"

namespace reflection {

Context::Context() {

    earse_ = false;
}

Context::~Context() {

    if(!earse_) {
        releaseResource();
    }
}

void Context::releaseResource() {

    map_.freeValueType([](void* item) {

        Info* it = static_cast<Info*>(item);
        Alloc::dellocate(const_cast<char*>(it->Scale));
        Alloc::dellocate(const_cast<char*>(it->Name));
        Alloc::dellocate(it);
    });

    std::map<const char*, Container*>::iterator iter;
    for(iter = classMap_.begin(); iter != classMap_.end(); iter++) {

        Alloc::dellocate(const_cast<char*>(iter->first));
        Alloc::dellocate(iter->second);
    }  
    earse_ = true;    
}

void Context::setContextEnv(const char* fileName) {

    loadContextFile_(fileName);
}

const char* Context::copyStr_(const char* str) {

    int length = strlen(str);
    char* res = static_cast<char*>(Alloc::allocate(length+1));
    strncpy(res, str, length);
    res[length] = '\0';
    return res;
}

void Context::loadContextFile_(const char* fileName) {

    TiXmlDocument* Doc;
    Doc = new TiXmlDocument(fileName);
    ON_SCOPE_EXIT([&] { delete Doc;})

    if(Doc->LoadFile()) {

        TiXmlElement* root = 0;
        root = Doc->FirstChildElement("context");
        if(root) {

            TiXmlElement* func = root->FirstChildElement("func");
            if(func) {
                const char* name = copyStr_(func->Attribute("name"));
                const char* scole = copyStr_(func->Attribute("scole"));
                unsigned long long pc = static_cast<unsigned long long>(atoi(func->Attribute("pc")));

                Info* inf = static_cast<Info*>(Alloc::allocate(sizeof(Info)));
                new (inf) Info(name, scole, pc);
                map_.insert(inf);
            }
            TiXmlElement* next;
            while(true) {
                next = func->NextSiblingElement("func");
                if(next) {
                    func = next;
                    const char* name = copyStr_(func->Attribute("name"));
                    const char* scole = copyStr_(func->Attribute("scole"));
                    unsigned long long pc = static_cast<unsigned long long>(atoi(func->Attribute("pc")));

                    Info* inf = static_cast<Info*>(Alloc::allocate(sizeof(Info)));
                    new (inf) Info(name, scole, pc);
                    map_.insert(inf);                   
                }
                else {
                    break;
                }
            }

        }
        else {
            throw Statue::XML_ROOT_ERROR;
        }
    }
    else {
        throw Statue::XML_NOT_FOUND;
    }
}


void Context::registClass(const char* name, Container* cont) {

    std::map<const char*, Container*>::iterator iter;
    iter = std::find_if(classMap_.begin(), classMap_.end(), [&](std::map<const char*, Container*>::value_type& pair) {
        int length = strlen(name);
        if(length != strlen(pair.first)) {
            return false;
        }
        int res = strncmp(name, pair.first, length);
        return res == 0?true:false;
    });
    if(iter == classMap_.end()) {

        const char* key = copyStr_(name);
        classMap_.insert(std::make_pair(key, cont));
    }
    else {
        throw Statue::ITEM_COLLISION;
    }
}

void* Context::instanceOfClass(const char* name) {

    std::map<const char*, Container*>::iterator iter;
    iter = std::find_if(classMap_.begin(), classMap_.end(), [&](std::map<const char*, Container*>::value_type& pair) {
        int length = strlen(name);
        if(length != strlen(pair.first)) {
            return false;
        }
        int res = strncmp(name, pair.first, length);
        return res == 0?true:false;
    });
    if(iter == classMap_.end()) {

        throw Statue::ITEM_NOT_FOUND;
    }
    else {
        return iter->second->instanceOfClass();
    }
}

void Context::invoke(const char* name, const char* scole, const std::vector<const char*>& args) {

    Info inf(name, scole, 0);
    Info* res = map_.search(&inf);
    if(res == NULL) {
        throw Statue::ITEM_NOT_FOUND;
    }

    std::map<const char*, Container*>::iterator iter;
    iter = std::find_if(classMap_.begin(), classMap_.end(), [&](std::map<const char*, Container*>::value_type& pair) {
        int length = strlen(scole);
        if(length != strlen(pair.first)) {
            return false;
        }
        int res = strncmp(scole, pair.first, length);
        return res == 0?true:false;
    });

    if(iter == classMap_.end()) {
        throw Statue::ITEM_NOT_FOUND;
    }
    else {
        
        iter->second->invoke(res->PC, args);
    }

}


}// namespace reflection