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
#include <cstdlib>
#include <iostream>

#include "context/mempool.h"

namespace reflection {

size_t MemPool::round_up(size_t bytes) {

	return (((bytes) + align - 1) & ~(align -1));
}

size_t MemPool::freeListIndex(size_t bytes) {

	return (((bytes) + align - 1) / align - 1);
}

void* MemPool::allocate(size_t n) {
	
	if(n > MaxBytes - 1) {
		char* tmp = static_cast<char*>(malloc(n+1));
		*tmp = static_cast<unsigned int>(MaxBytes+1) & 0xff;
		tmp++;
		return static_cast<void*>(tmp);
	}

	obj** myFreeList;
	obj* result;

	myFreeList = freeList_ + freeListIndex(n+1);
	if(*myFreeList == 0) {
		char* result = static_cast<char*>(injectPool(round_up(n+1)));
		*result = static_cast<unsigned int>(n+1) & 0xff;
		result++;
		return static_cast<void*>(result);
	}

	result = *myFreeList;
	*myFreeList = result->free_list_link;
	char* tmp = reinterpret_cast<char*>(result);
	*tmp = static_cast<unsigned int>(n+1) & 0xff;
	tmp++;
	return static_cast<void*>(tmp);
}

void* MemPool::injectPool(size_t bytes) {

	int nobjs = 20;
	char* chunk = chunkAlloc(bytes, nobjs);

	if(nobjs == 1)
		return static_cast<void*>(chunk);

	obj** myFreeList;
	obj* next;
	obj* result;
	
	myFreeList = freeList_ + freeListIndex(bytes);
	result = reinterpret_cast<obj*>(chunk);
	next = reinterpret_cast<obj*>(chunk + bytes);
	*myFreeList = next;

	for(int i=1;; i++) {
		obj* current = next;
		next = reinterpret_cast<obj*>(reinterpret_cast<char*>(next) + bytes);
		if(nobjs - 1 == i) {
			current->free_list_link = 0;
			return static_cast<void*>(result);
		}
		current->free_list_link = next;
	}
	return static_cast<void*>(result);
}

char* MemPool::chunkAlloc(size_t size, int& nobjs) {

	char* result;
	size_t total = size * nobjs;
	size_t left = poolEnd_ - poolStart_;

	if(total < left) {
		result = poolStart_;
		poolStart_ += total;
		return result;
	} else if(left >= size) {
		nobjs = left/size;
		result = poolStart_;
		poolStart_ += nobjs* size;
		return result;
	}
	else {
		if(left > 0) {
			obj** myFreeList;
			myFreeList = freeList_ + freeListIndex(left);
			obj* current = reinterpret_cast<obj*>(poolStart_);
			current->free_list_link = *myFreeList;
			*myFreeList = current;
		}

		size_t mallocSize = total*2;
		char* result = static_cast<char*>(malloc(mallocSize));
		//TODO: handle the situation that memory is poor
		node* head = static_cast<node*>(malloc(sizeof(node)));
		head->address = result;
		head->next = mallocList_;
		mallocList_ = head;

		poolStart_ = result + total;
		poolEnd_ = result + mallocSize;
		return result;
	}
}

void MemPool::clean() {

	while(mallocList_ != 0) {
		node* tmp = mallocList_;
		mallocList_ = mallocList_->next;
		free(tmp->address);
		free(tmp);
	}
}

void MemPool::dellocate(void* p) {

	char* ptr = static_cast<char*>(p);
	ptr--;
	int size= static_cast<int>(*ptr);
	if(size > MaxBytes)
		free(ptr);
	obj** myFreeList = freeList_ + freeListIndex(size);
	obj* current = reinterpret_cast<obj*>(ptr);
	current->free_list_link = *myFreeList;
	*myFreeList = current;

}


}//namespace reflection

