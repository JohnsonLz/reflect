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

#ifndef REFLECTION_CONTEXT_LIST_H_
#define REFLECTION_CONTEXT_LIST_H_

#include <stdlib.h>
#include <functional>
#include "context/alloc.h"

namespace reflection {

template<typename ValueType>
class List {

	private:

	struct innerNode_ {
		struct innerNode_* next;
		struct innerNode_* down;
		ValueType* data;
	};

	class Compare {

		public:

		bool greater(ValueType* left, ValueType* right) {
			return *left > *right;
		}
		bool equal(ValueType* left, ValueType* right) {
			return *left == *right;
		}
	};


	struct innerNode_* head_[5];
	Compare comp_;

	int randomLevel_() {
		int level = 1;
		while((rand() % 2))
			level++;

		return level < 5? level:5;
	}

	void clean_() {

		innerNode_* curson;
		for(int i=4; i>=0; i--) {
			curson = head_[i];
			if(head_[i] != NULL) {
				curson = head_[i]->next;
			}
			else {
				continue;
			}
			while(curson != NULL) {
				innerNode_* tmp = curson;
				curson = curson->next;
				Alloc::dellocate(tmp);
			}
		}
	}


	public:
	List() {
		for(int i=0; i<5; i++) {
			head_[i] = static_cast<innerNode_*>(Alloc::allocate(sizeof(innerNode_)));
			head_[i]->data = NULL;
			head_[i]->down = NULL;
			head_[i]->next = NULL;
		}
	}
	~List() {
		clean_();
	}

	typedef const innerNode_* const_iterator;
	const_iterator start() const {
		return head_[0]->next;
	}
	const_iterator end() const {
		return NULL;
	}

	void reset() {

		clean_();
		for(int i=0; i<5; i++) {
			head_[i] ->data = NULL;
			head_[i] ->down = NULL;
			head_[i] ->next = NULL;
		}
	}

	void insert(ValueType* item) {
		
		int level = randomLevel_();
		innerNode_* tmp = NULL;
		for(int i=0; i<level; i++) {
			innerNode_* nodeToInsert = static_cast<innerNode_*>(Alloc::allocate(sizeof(innerNode_)));
			nodeToInsert->data = item;
			nodeToInsert->down = tmp;
			tmp = nodeToInsert;
			innerNode_* cursonHead = head_[i];
			innerNode_* cursonTail = head_[i]->next;
			
			int hasInsert = 0;
			while(cursonTail != NULL) {
				if(comp_.greater(cursonTail->data, item)) {
					nodeToInsert->next = cursonTail;
					cursonHead->next = nodeToInsert;
					hasInsert = 1;
					break;
				}
				cursonHead = cursonTail;
				cursonTail = cursonTail->next;
			}
			if(!hasInsert) {
				nodeToInsert->next = NULL;
				cursonHead->next = nodeToInsert;
			}
		}
	}

	ValueType* search(ValueType* item) {

		int level = 4;
		innerNode_* curson = head_[level];
		while(curson->next == NULL) {
			if(level == 0)
				return NULL;
			curson = head_[--level];
		}
		while(comp_.greater(curson->next->data,item)) {
			if(level == 0)
				return NULL;
			curson = head_[--level];
		}

		curson = curson->next;
		while(true) {
			if(comp_.equal(curson->data, item))
				return curson->data;
			if(curson->next == NULL) {
				curson = curson->down;
				if(curson == NULL)
					return NULL;
			} 
			else if(comp_.greater(curson->next->data, item)) {
				curson = curson->down;
				if(curson == NULL)
					return NULL;
			}
			else
				curson = curson->next;
		}
	}

	void freeValueType(std::function<void(void*)> fm) {

		innerNode_* curson = head_[0]->next;
		while(curson != NULL) {
			fm(static_cast<void*>(curson->data));
			curson = curson->next;
		}
	}



};

} // namespace reflection

#endif

