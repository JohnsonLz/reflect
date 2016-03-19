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

#ifndef REFLECTION_CONTEXT_MEMPOOL_H_
#define REFLECTION_CONTEXT_MEMPOOL_H_

#include <sys/types.h>

namespace reflection {

class MemPool {

	private:

	enum {align = 8};
	enum {MaxBytes = 128};
	enum {MemListSize = MaxBytes/align};

	union obj {
		union obj* free_list_link;
		char clientData[1];
	};

	struct node {
		struct node* next ;
		char* address;
	};

	node*  mallocList_;
	obj* freeList_[MemListSize] ;
	char* poolStart_;
	char* poolEnd_;
	size_t poolSize_;
	
	size_t round_up(size_t bytes);
	size_t freeListIndex(size_t bytes);
	void* injectPool(size_t bytes);
	char* chunkAlloc(size_t size, int& nobjs);
	void clean();

	MemPool():poolStart_(0),poolEnd_(0),poolSize_(0) {
		mallocList_ = 0;
		for(int i=0;i<MemListSize; i++)
			freeList_[i] = 0;
	}
	MemPool(MemPool&){}

	public:
	~MemPool(){
		clean();
	}
	void* allocate(size_t n);
	void dellocate(void* p);

	static MemPool& sharedMemPool() {
		static MemPool mp;
		return mp;
	}
};



}// namespace reflection

#endif

