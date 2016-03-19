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

#ifndef REFLECTION_CONTEXT_STATUE_H_
#define REFLECTION_CONTEXT_STATUE_H_

namespace reflection {

enum class Statue{

    ELF_NOT_FOUND,
    DWARF_INIT_FAILED,
    DWARF_DESTORY_FAILED,
    DWARF_PARSE_DIE_FAILED,
    DWARF_PAESE_UNKNOWN_ERROR,
    DWARF_PARSE_CU_HEADER_FAILED,
    DWARF_PARSE_CU_DIE_FAILED,

    XML_ROOT_ERROR,
    XML_NOT_FOUND,

    ITEM_COLLISION,
    ITEM_NOT_FOUND
};

} // namespace reflection

#endif
