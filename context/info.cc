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

#include <string.h>

#include "context/info.h"

namespace reflection {

bool Info::operator==(const Info& item) {

    int length = strlen(Scale);
    if(length != strlen(item.Scale)) {
        return false;
    }
    else {
        if(strncmp(Scale, item.Scale, length) != 0) {
            return false;
        }
    }
    length = strlen(Name);
    if(length != strlen(item.Name)) {
        return false;
    }
    else {
        if(strncmp(Name, item.Name, length) != 0) {
            return false;
        }
    }
    return true;
}

bool Info::operator>(const Info& item) {

    int length1 = strlen(Scale);
    int length2 = strlen(item.Scale);

    int length = length1>length2?length2:length1;
    int cmp = strncmp(Scale, item.Scale, length);
    if(cmp  > 0) {
        return true;
    }
    if(cmp < 0) {
        return false;
    }
    else {
        if(length1 > length2) {
            return true;
        }
        else if(length1 < length2) {
            return false;
        }
        else {
            length1 = strlen(Name);
            length2 = strlen(item.Name);
            length = length1>length2?length2:length1;
            cmp = strncmp(Name, item.Name, length);
            if(cmp >0) {
                return true;
            }
            if(cmp < 0) {
                return false;
            }
            else if(length1 > length2) {
                return true;
            }
            return false;
        }
    }
}


} // namespace reflection
