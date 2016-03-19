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

#ifndef REFLECTION_CONTEXT_ALLOC_H_
#define REFLECTION_CONTEXT_ALLOC_H_

namespace reflection {

// Class Alloc for client to operator heap memoey. Alloc can not be
// instantiation, all the methods are static.
class Alloc {

	private:
		Alloc(){}
		Alloc(Alloc&) {}

	public:
		
		// Return void* pointer pointed to the memory. client can 
		// cast the pointer to the type they wanted. It is the
		// client's responsibility to dellocate the pointer when
		// it is done with it. 
		static void* allocate(unsigned size);

		// Dellocate the pointer pointed to the headp memory that
		// has been allocate befor. Require the pointer must be 
		// void* type.
		static void dellocate(void* p);

};

}//namespace reflection

#endif

