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

#include "context/alloc.h"
#include "context/mempool.h"

namespace reflection {

void* Alloc::allocate(unsigned size) {

	return MemPool::sharedMemPool().allocate(size);
}

void Alloc::dellocate(void* p) {

	MemPool::sharedMemPool().dellocate(p);
}

}// namespace reflection

