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

#ifndef REFLECTION_CONTEXT_CONTAINER_H_
#define REFLECTION_CONTEXT_CONTAINER_H_


#include <new>
#include <vector>

#include "context/alloc.h"


namespace reflection {

class Container {

private:

    class bridge {

    public:
        bridge(){}
        virtual ~bridge(){}
        virtual void invoke(unsigned long long addr, const std::vector<const char*>& args) = 0;
        virtual void* instanceOfClass() = 0;
    };

    template<typename ValueType>
    class holder: public bridge {

    public:
        virtual void invoke(unsigned long long addr, const std::vector<const char*>& args) override{};
        virtual void* instanceOfClass() override{};
    };

    template<typename ValueType>
    class holder<ValueType*>: public bridge {

    private:
        ValueType* creator_() {
            ValueType* vt = static_cast<ValueType*>(Alloc::allocate(sizeof(ValueType)));
            new (vt) ValueType();
            return vt;
        }
        void* addrTrans_(unsigned long long addr) {

            return reinterpret_cast<void*>(addr);
        }

        void invoke_(void* addr) {
            typedef void(*func)(const ValueType*);
            func f = reinterpret_cast<func>(addr);
            ValueType* t = creator_();
            f(t);
            Alloc::dellocate(t);
        }

        void invoke_(void* addr, const char* arg1) {
            typedef void(*func)(const ValueType*, const char*);
            func f = reinterpret_cast<func>(addr);
            ValueType* t = creator_();
            f(t, arg1);
            Alloc::dellocate(t);
        }

        void invoke_(void* addr, const char* arg1, const char* arg2) {
            typedef void(*func)(const ValueType*, const char*, const char*);
            func f = reinterpret_cast<func>(addr);
            ValueType* t = creator_();
            f(t, arg1, arg2);
            Alloc::dellocate(t);
        }

        void invoke_(void* addr, const char* arg1, const char* arg2, const char* arg3) {
            typedef void(*func)(const ValueType*, const char*, const char*, const char*);
            func f = reinterpret_cast<func>(addr);
            ValueType* t = creator_();
            f(t, arg1, arg2, arg3);
            Alloc::dellocate(t);
        }

    public:
        holder(ValueType* item){}
        virtual ~holder() {}

        virtual void invoke(unsigned long long addr, const std::vector<const char*>& args) override{

            void* ptr = addrTrans_(addr);

            switch(args.size()) {
            case 0:
                invoke_(ptr);
                break;
            case 1:
                invoke_(ptr, args[0]);
                break;
            case 2:
                invoke_(ptr, args[0], args[1]);
                break;
            case 3:
                invoke_(ptr, args[0], args[1], args[2]);
                break;
            }
        }
        
        virtual void* instanceOfClass() override{

            ValueType* vt = creator_();
            return static_cast<void*>(vt);
        }
    };

    bridge* content_;
public:
    Container(){}
    ~Container(){
        Alloc::dellocate(content_);
    }

    template<typename ValueType>
    Container(ValueType* item) {

        holder<ValueType*>* h = static_cast<holder<ValueType*>*>(Alloc::allocate(sizeof(holder<ValueType*>)));
        new(h) holder<ValueType*>(item);
        content_ = h;
    }

    // three arguments most, and type are char*
    void invoke(unsigned long long addr, const std::vector<const char*>& args) {

        content_->invoke(addr, args);
    }

    void* instanceOfClass() {
        
        return content_->instanceOfClass();
    }

};


}// namespace reflection

#endif